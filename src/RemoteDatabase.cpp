#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>
#include <QUrlQuery>

#include <iterator>

#include "FileDialog.h"
#include "RemoteDatabase.h"
#include "Settings.h"
#include "sqlite.h"
#include "version.h"

RemoteDatabase::RemoteDatabase() :
    m_dbLocal(nullptr)
{
}

RemoteDatabase::~RemoteDatabase()
{
    // Close local storage db - but only if it was created/opened in the meantime
    if(m_dbLocal)
        sqlite3_close(m_dbLocal);
}

void RemoteDatabase::localAssureOpened()
{
    // This function should be called first in each RemoteDatabase::local* function. It assures the database for storing
    // the local database information is opened and ready. If the database file doesn't exist yet it is created by this
    // function. If the database file is already created and opened this function does nothing. The reason to open the
    // database on first use instead of doing that in the constructor of this class is that this way no database file is
    // going to be created and no database handle is held when it's not actually needed. For people not interested in
    // the dbhub.io functionality this means no unnecessary files being created.

    // Check if database is already opened and return if it is
    if(m_dbLocal)
        return;

    // Make sure the directory exists
    QString database_directory = QStandardPaths::writableLocation(
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
                QStandardPaths::AppDataLocation
#else
                QStandardPaths::GenericDataLocation
#endif
                );
    QDir().mkpath(database_directory);

    // Open file
    QString database_file = database_directory + "/remotedbs.db";
    if(sqlite3_open_v2(database_file.toUtf8(), &m_dbLocal, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error opening local databases list.\n%1").arg(QString::fromUtf8(sqlite3_errmsg(m_dbLocal))));
        return;
    }

    // Create local local table if it doesn't exists yet
    char* errmsg;
    QString statement = QString("CREATE TABLE IF NOT EXISTS \"local\"("
                                "\"id\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                "\"identity\" TEXT NOT NULL,"
                                "\"name\" TEXT NOT NULL,"
                                "\"url\" TEXT NOT NULL,"
                                "\"commit_id\" TEXT NOT NULL,"
                                "\"file\" TEXT NOT NULL UNIQUE,"
                                "\"modified\" INTEGER DEFAULT 0,"
                                "\"branch\" TEXT NOT NULL DEFAULT \"master\""
                                ")");
    if(sqlite3_exec(m_dbLocal, statement.toUtf8(), nullptr, nullptr, &errmsg) != SQLITE_OK)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error creating local databases list.\n%1").arg(QString::fromUtf8(errmsg)));
        sqlite3_free(errmsg);
        sqlite3_close(m_dbLocal);
        m_dbLocal = nullptr;
        return;
    }
}

QString RemoteDatabase::localAdd(QString filename, QString identity, const QUrl& url, const std::string& new_commit_id, const std::string& branch)
{
    localAssureOpened();

    // Remove the path
    QFileInfo f(identity);
    identity = f.fileName();

    // Check if this file has already been checked in
    std::string last_commit_id = localLastCommitId(identity, url.toString(), branch);
    if(last_commit_id.empty())
    {
        // The file hasn't been checked in yet. So add a new record for it.

        // Generate a new file name to save the file under
        filename = QString("%2_%1.remotedb").arg(QDateTime::currentMSecsSinceEpoch()).arg(filename);

        // Insert database into local database list
        QString sql = QString("INSERT INTO local(identity, name, url, commit_id, file, branch) VALUES(?, ?, ?, ?, ?, ?)");
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
            return QString();

        if(sqlite3_bind_text(stmt, 1, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 2, url.fileName().toUtf8(), url.fileName().toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 3, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(),
                             url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 4, new_commit_id.c_str(), static_cast<int>(new_commit_id.size()), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 5, filename.toUtf8(), filename.size(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 6, branch.c_str(), static_cast<int>(branch.size()), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        sqlite3_finalize(stmt);

        // Return full path to the new file
        return Settings::getValue("remote", "clonedirectory").toString() + "/" + filename;
    }

    // If we get here, the file has been checked in before. Check next if it has been updated in the meantime.
    if(last_commit_id != new_commit_id)
    {
        // The file has already been checked in and the commit ids are different. If they weren't we wouldn't need to update anything

        QString sql = QString("UPDATE local SET commit_id=? WHERE identity=? AND url=? AND branch=?");
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
            return QString();

        if(sqlite3_bind_text(stmt, 1, new_commit_id.c_str(), static_cast<int>(new_commit_id.size()), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 2, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 3, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(),
                             url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 4, branch.c_str(), static_cast<int>(branch.size()), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        sqlite3_finalize(stmt);
    }

    // If we got here, the file was already checked in (and was either updated or not (obviously)). This mean we can just return the file name as
    // we know it.
    return localExists(url, identity, branch);
}

QString RemoteDatabase::localExists(const QUrl& url, QString identity, const std::string& branch)
{
    localAssureOpened();

    // Extract commit id from url and remove query part afterwards
    QString url_commit_id = QUrlQuery(url).queryItemValue("commit");

    // Query commit id and filename for the given combination of url and identity
    QString sql = QString("SELECT id, commit_id, file FROM local WHERE url=? AND identity=? AND branch=?");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return QString();

    if(sqlite3_bind_text(stmt, 1, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(), url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return QString();
    }

    QFileInfo f(identity);                  // Remove the path
    identity = f.fileName();
    if(sqlite3_bind_text(stmt, 2, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return QString();
    }

    if(sqlite3_bind_text(stmt, 3, branch.c_str(), static_cast<int>(branch.size()), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return QString();
    }

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        // If there was either an error or no record was found for this combination of url and
        // identity, stop here.
        sqlite3_finalize(stmt);
        return QString();
    }

    // Having come here we can assume that at least some local clone for the given combination of
    // url, identity and branch exists. So extract all the information we have on it.
    QString local_commit_id = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    QString local_file = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    sqlite3_finalize(stmt);

    // There are three possibilities now: either we didn't get any commit id in the URL in which case we just return the file we got, no matter what.
    // Or the requested commit id is the same as the local commit id in which case we return the file we got as well.
    // Or the requested commit id differ in which case we return no match.
    if(url_commit_id.isNull() || local_commit_id == url_commit_id)
    {
        // Both commit ids are the same. That's the perfect match, so we can open the local file if it still exists
        return localCheckFile(local_file);
    } else {
        // The commit ids differ. This means we have no match
        return QString();
    }
}

QString RemoteDatabase::localCheckFile(const QString& local_file)
{
    // This function takes the file name of a locally cloned database and checks if this file still exists. If it has been deleted in the meantime it returns
    // an empty string and deletes the file from the clone database. If the file still exists, it returns the full path to the file.

    localAssureOpened();

    // Build the full path to where the file should be
    QString full_path = Settings::getValue("remote", "clonedirectory").toString() + "/" + local_file;

    // Check if the database still exists. If so return its path, if not return an empty string to redownload it
    if(QFile::exists(full_path))
    {
        return full_path;
    } else {
        // Remove the apparently invalid entry from the local clones database to avoid future lookups and confusions. The file column should
        // be unique for the entire table because the files are all in the same directory and their names need to be unique because of this.
        localDeleteFile(local_file);

        // Return empty string to indicate a redownload request
        return QString();
    }
}

std::string RemoteDatabase::localLastCommitId(QString identity, const QUrl& url, const std::string& branch)
{
    localAssureOpened();

    // Query commit id for that file name
    QString sql = QString("SELECT commit_id FROM local WHERE identity=? AND url=? AND branch=?");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return std::string();

    QFileInfo f(identity);                  // Remove the path
    identity = f.fileName();
    if(sqlite3_bind_text(stmt, 1, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return std::string();
    }

    if(sqlite3_bind_text(stmt, 2, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(),
                         url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().size(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return std::string();
    }

    if(sqlite3_bind_text(stmt, 3, branch.c_str(), static_cast<int>(branch.size()), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return std::string();
    }

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        // If there was either an error or no record was found for this file name, stop here.
        sqlite3_finalize(stmt);
        return std::string();
    }

    // Having come here we can assume that at least some local clone with the given file name
    std::string local_commit_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    return local_commit_id;
}

std::vector<RemoteDatabase::LocalFileInfo> RemoteDatabase::localGetLocalFiles(QString identity)
{
    localAssureOpened();

    // Get all rows for this identity
    QString sql = QString("SELECT name, url, commit_id, file, branch FROM local WHERE identity=? ORDER BY url");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return {};

    QFileInfo f(identity);
    identity = f.fileName();
    if(sqlite3_bind_text(stmt, 1, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return {};
    }

    std::vector<RemoteDatabase::LocalFileInfo> result;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
                            identity.toStdString());
    }

    sqlite3_finalize(stmt);
    return result;
}

RemoteDatabase::LocalFileInfo RemoteDatabase::localGetLocalFileInfo(QString filename)
{
    localAssureOpened();

    // Find this file in our database
    QString sql = QString("SELECT name, url, commit_id, branch, identity FROM local WHERE file=?");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return {};

    // Remove the path for querying the file name
    filename = QFileInfo(filename).fileName();
    if(sqlite3_bind_text(stmt, 1, filename.toUtf8(), filename.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return {};
    }

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        // If there was either an error or no record was found for this file name, stop here.
        sqlite3_finalize(stmt);
        return {};
    }

    // Retrieve and return all the information we have
    RemoteDatabase::LocalFileInfo result(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                                         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                                         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                                         filename.toStdString(),
                                         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                                         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    sqlite3_finalize(stmt);
    return result;
}

void RemoteDatabase::localDeleteFile(QString filename)
{
    localAssureOpened();

    // Remove the file's entry in our database
    QString sql = QString("DELETE FROM local WHERE file=?");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return;
    if(sqlite3_bind_text(stmt, 1, filename.toUtf8(), filename.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    // Delete the actual file on disk
    QFile::remove(Settings::getValue("remote", "clonedirectory").toString() + "/" + filename);
}

QString RemoteDatabase::LocalFileInfo::user_name() const
{
    // Figure out the user name from the URL

    QString path = QUrl(QString::fromStdString(url)).path();

    if(path.count('/') < 2 || !path.startsWith('/'))
        return QString();
    else
        return path.mid(1, path.indexOf('/', 1) - 1);
}
