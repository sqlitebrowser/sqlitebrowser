#include "sqlitedb.h"
#include "sqlite.h"
#include "sqlitetablemodel.h"
#include "CipherDialog.h"
#include "CipherSettings.h"
#include "DotenvFormat.h"
#include "Settings.h"

#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <functional>
#include <atomic>
#include <algorithm>

QStringList DBBrowserDB::Datatypes = QStringList() << "INTEGER" << "TEXT" << "BLOB" << "REAL" << "NUMERIC";

// Helper template to allow turning member functions into a C-style function pointer
// See https://stackoverflow.com/questions/19808054/convert-c-function-pointer-to-c-function-pointer/19809787
template <typename T>
struct Callback;
template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};
template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

// collation callbacks
int collCompare(void* /*pArg*/, int sizeA, const void* sA, int sizeB, const void* sB)
{
    if(sizeA == sizeB)
        return memcmp(sA, sB, sizeA);
    return sizeA - sizeB;
}

static int sqlite_compare_utf16( void* /*arg*/,int size1, const void *str1, int size2, const void* str2)
{
    const QString string1 = QString::fromRawData(reinterpret_cast<const QChar*>(str1), size1 / sizeof(QChar));
    const QString string2 = QString::fromRawData(reinterpret_cast<const QChar*>(str2), size2 / sizeof(QChar));

    return QString::compare(string1, string2, Qt::CaseSensitive);
}

static int sqlite_compare_utf16ci( void* /*arg*/,int size1, const void *str1, int size2, const void* str2)
{
    const QString string1 = QString::fromRawData(reinterpret_cast<const QChar*>(str1), size1 / sizeof(QChar));
    const QString string2 = QString::fromRawData(reinterpret_cast<const QChar*>(str2), size2 / sizeof(QChar));

    return QString::compare(string1, string2, Qt::CaseInsensitive);
}

void DBBrowserDB::collationNeeded(void* /*pData*/, sqlite3* /*db*/, int eTextRep, const char* sCollationName)
{
    QString name(sCollationName);

    // Don't request built-in collations. SQLite requests these collations even though they are built into
    // the library. Since we have no need for overriding them, we just silently ignore these requests.
    if(name.compare("BINARY", Qt::CaseInsensitive) &&
            name.compare("NOCASE", Qt::CaseInsensitive) &&
            name.compare("RTRIM", Qt::CaseInsensitive))
    {
        emit requestCollation(name, eTextRep);
    }
}

static void regexp(sqlite3_context* ctx, int /*argc*/, sqlite3_value* argv[])
{
    // Get arguments and check their values
    QRegExp arg1(reinterpret_cast<const char*>(sqlite3_value_text(argv[0])));
    QString arg2(reinterpret_cast<const char*>(sqlite3_value_text(argv[1])));
    if(!arg1.isValid())
        return sqlite3_result_error(ctx, "invalid operand", -1);

    // Set the pattern matching syntax to a Perl-like one. This is the default in Qt 4.x but Qt 5
    // changes this to a greedy one (QRegExp::RegExp2). To make sure the behaviour of our application
    // doesn't change depending on the build environment, we make sure to always set the same pattern
    // matching syntax.
    arg1.setPatternSyntax(QRegExp::RegExp);

    // Perform the actual matching and return the result. Note that Qt's QRegExp returns -1 if the regex
    // doesn't match and the position in the string otherwise; SQLite expects a 0 for not found and a 1 for found.
    sqlite3_result_int(ctx, arg1.indexIn(arg2) >= 0);
}

bool DBBrowserDB::isOpen ( ) const
{
    return _db != nullptr;
}

bool DBBrowserDB::getDirty() const
{
    return !savepointList.empty();
}

bool DBBrowserDB::open(const QString& db, bool readOnly)
{
    if (isOpen()) close();

    isEncrypted = false;
    dontCheckForStructureUpdates = false;

    // Get encryption settings for database file
    CipherSettings* cipherSettings = nullptr;
    if(tryEncryptionSettings(db, &isEncrypted, cipherSettings) == false)
        return false;

    // Open database file
    if(sqlite3_open_v2(db.toUtf8(), &_db, readOnly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK)
    {
        lastErrorMessage = QString::fromUtf8(sqlite3_errmsg(_db));
        return false;
    }

    // Set encryption details if database is encrypted
#ifdef ENABLE_SQLCIPHER
    if(isEncrypted && cipherSettings)
    {
        executeSQL(QString("PRAGMA key = %1").arg(cipherSettings->getPassword()), false, false);
        executeSQL(QString("PRAGMA cipher_page_size = %1;").arg(cipherSettings->getPageSize()), false, false);
        executeSQL(QString("PRAGMA kdf_iter = %1;").arg(cipherSettings->getKdfIterations()), false, false);
        executeSQL(QString("PRAGMA cipher_hmac_algorithm = HMAC_%1;").arg(cipherSettings->getHmacAlgorithm()), false, false);
        executeSQL(QString("PRAGMA cipher_kdf_algorithm = PBKDF2_HMAC_%1;").arg(cipherSettings->getKdfAlgorithm()), false, false);
    }
#endif
    delete cipherSettings;

    if (_db)
    {
        // add UTF16 collation (comparison is performed by QString functions)
        sqlite3_create_collation(_db, "UTF16", SQLITE_UTF16, nullptr, sqlite_compare_utf16);
        // add UTF16CI (case insensitive) collation (comparison is performed by QString functions)
        sqlite3_create_collation(_db, "UTF16CI", SQLITE_UTF16, nullptr, sqlite_compare_utf16ci);

        // register collation callback
        Callback<void(void*, sqlite3*, int, const char*)>::func = std::bind(&DBBrowserDB::collationNeeded, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        void (*c_callback)(void*, sqlite3*, int, const char*) = static_cast<decltype(c_callback)>(Callback<void(void*, sqlite3*, int, const char*)>::callback);
        sqlite3_collation_needed(_db, nullptr, c_callback);

        // Set foreign key settings as requested in the preferences
        bool foreignkeys = Settings::getValue("db", "foreignkeys").toBool();
        setPragma("foreign_keys", foreignkeys ? "1" : "0");

        // Register REGEXP function
        if(Settings::getValue("extensions", "disableregex").toBool() == false)
            sqlite3_create_function(_db, "REGEXP", 2, SQLITE_UTF8, nullptr, regexp, nullptr, nullptr);

        // Check if file is read only
        QFileInfo fi(db);
        QFileInfo fid(fi.absoluteDir().absolutePath());
        isReadOnly = readOnly || !fi.isWritable() || !fid.isWritable();

        // Load extensions
        loadExtensionsFromSettings();

        // Execute default SQL
        if(!isReadOnly)
        {
            QString default_sql = Settings::getValue("db", "defaultsqltext").toString();
            if(!default_sql.isEmpty())
                executeMultiSQL(default_sql, false, true);
        }

        curDBFilename = db;

        updateSchema();

        return true;
    } else {
        return false;
    }
}

bool DBBrowserDB::attach(const QString& filePath, QString attach_as)
{
    if(!_db)
        return false;

    waitForDbRelease();

    // Check if this file has already been attached and abort if this is the case
    QString sql = "PRAGMA database_list;";
    logSQL(sql, kLogMsg_App);
    sqlite3_stmt* db_vm;
    if(sqlite3_prepare_v2(_db, sql.toUtf8(), sql.toUtf8().length(), &db_vm, nullptr) == SQLITE_OK)
    {
        // Loop through all the databases
        QFileInfo fi(filePath);
        while(sqlite3_step(db_vm) == SQLITE_ROW)
        {
            QFileInfo path(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(db_vm, 2))));
            if(fi == path)
            {
                QString schema = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(db_vm, 1)));
                QMessageBox::information(nullptr, qApp->applicationName(), tr("This database has already been attached. Its schema name is '%1'.").arg(schema));
                return false;
            }
        }
        sqlite3_finalize(db_vm);
    }

    // Ask for name to be given to the attached database if none was provided
    if(attach_as.isEmpty())
        attach_as = QInputDialog::getText(nullptr,
                                          qApp->applicationName(),
                                          tr("Please specify the database name under which you want to access the attached database"),
                                          QLineEdit::Normal,
                                          QFileInfo(filePath).baseName()
                                          ).trimmed();
    if(attach_as.isNull())
        return false;

#ifdef ENABLE_SQLCIPHER
    // Try encryption settings
    CipherSettings* cipherSettings = nullptr;
    bool is_encrypted;
    if(tryEncryptionSettings(filePath, &is_encrypted, cipherSettings) == false)
        return false;

    // Attach database
    QString key;
    if(cipherSettings && is_encrypted)
        key = "KEY " + cipherSettings->getPassword();
    if(!executeSQL(QString("ATTACH '%1' AS %2 %3").arg(filePath).arg(sqlb::escapeIdentifier(attach_as)).arg(key), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
    if(!executeSQL(QString("PRAGMA %1.cipher_page_size = %2").arg(sqlb::escapeIdentifier(attach_as)).arg(cipherSettings->getPageSize()), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
    if(!executeSQL(QString("PRAGMA %1.kdf_iter = %2").arg(sqlb::escapeIdentifier(attach_as)).arg(cipherSettings->getKdfIterations()), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
    if(!executeSQL(QString("PRAGMA %1.cipher_hmac_algorithm = HMAC_%2").arg(sqlb::escapeIdentifier(attach_as)).arg(cipherSettings->getHmacAlgorithm()), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
    if(!executeSQL(QString("PRAGMA %1.cipher_kdf_algorithm = PBKDF2_HMAC_%2").arg(sqlb::escapeIdentifier(attach_as)).arg(cipherSettings->getKdfAlgorithm()), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
    delete cipherSettings;
#else
    // Attach database
    if(!executeSQL(QString("ATTACH '%1' AS %2").arg(filePath).arg(sqlb::escapeIdentifier(attach_as)), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
#endif

    // Update schema to load database schema of the newly attached database
    updateSchema();

    return true;
}

bool DBBrowserDB::tryEncryptionSettings(const QString& filePath, bool* encrypted, CipherSettings*& cipherSettings)
{
    lastErrorMessage = tr("Invalid file format");

    // Open database file
    sqlite3* dbHandle;
    if(sqlite3_open_v2(filePath.toUtf8(), &dbHandle, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
        return false;

    // Try reading from database

#ifdef ENABLE_SQLCIPHER
    bool isDotenvChecked = false;

    // Determine default encryption settings depending on the SQLCipher version we use
    QString sqlite_version, sqlcipher_version;
    getSqliteVersion(sqlite_version, sqlcipher_version);
    int enc_default_page_size, enc_default_kdf_iter;
    QString enc_default_hmac_algorithm, enc_default_kdf_algorithm;
    if(sqlcipher_version.startsWith('4'))
    {
        enc_default_page_size = 4096;
        enc_default_kdf_iter = 256000;
        enc_default_hmac_algorithm = "SHA512";
        enc_default_kdf_algorithm = "SHA512";
    } else {
        enc_default_page_size = 1024;
        enc_default_kdf_iter = 64000;
        enc_default_hmac_algorithm = "SHA1";
        enc_default_kdf_algorithm = "SHA1";
    }
#endif

    *encrypted = false;
    cipherSettings = nullptr;
    while(true)
    {
        QString statement = "SELECT COUNT(*) FROM sqlite_master;";
        QByteArray utf8Statement = statement.toUtf8();
        sqlite3_stmt* vm;
        const char* tail;
        int err = sqlite3_prepare_v2(dbHandle, utf8Statement, utf8Statement.length(), &vm, &tail);
        if(err == SQLITE_BUSY || err == SQLITE_PERM || err == SQLITE_NOMEM || err == SQLITE_IOERR || err == SQLITE_CORRUPT || err == SQLITE_CANTOPEN)
        {
            lastErrorMessage = QString::fromUtf8(sqlite3_errmsg(dbHandle));
            sqlite3_close(dbHandle);
            return false;
        }

        if(sqlite3_step(vm) != SQLITE_ROW)
        {
            sqlite3_finalize(vm);
#ifdef ENABLE_SQLCIPHER
            bool foundDotenvPassword = false;

            // Being in a while loop, we don't want to check the same file multiple times
            if (!isDotenvChecked) {
                QFile databaseFile(filePath);
                QFileInfo databaseFileInfo(databaseFile);

                QString databaseDirectoryPath = databaseFileInfo.dir().path();
                QString databaseFileName(databaseFileInfo.fileName());

                QString dotenvFilePath = databaseDirectoryPath + "/.env";
                static const QSettings::Format dotenvFormat = QSettings::registerFormat("env", &DotenvFormat::readEnvFile, nullptr);
                QSettings dotenv(dotenvFilePath, dotenvFormat);

                QVariant passwordValue = dotenv.value(databaseFileName);

                foundDotenvPassword = !passwordValue.isNull();

                isDotenvChecked = true;

                if (foundDotenvPassword)
                {
                    QString password = passwordValue.toString();

                    QVariant keyFormatValue = dotenv.value(databaseFileName + "_keyFormat", QVariant(CipherSettings::KeyFormats::Passphrase));
                    CipherSettings::KeyFormats keyFormat = CipherSettings::getKeyFormat(keyFormatValue.toInt());

                    int pageSize = dotenv.value(databaseFileName + "_pageSize", enc_default_page_size).toInt();
                    int kdfIterations = dotenv.value(databaseFileName + "_kdfIter", enc_default_kdf_iter).toInt();
                    QString hmacAlgorithm = dotenv.value(databaseFileName + "_hmacAlgorithm", enc_default_hmac_algorithm).toString();
                    QString kdfAlgorithm = dotenv.value(databaseFileName + "_kdfAlgorithm", enc_default_kdf_algorithm).toString();

                    delete cipherSettings;
                    cipherSettings = new CipherSettings();

                    cipherSettings->setKeyFormat(keyFormat);
                    cipherSettings->setPassword(password);
                    cipherSettings->setPageSize(pageSize);
                    cipherSettings->setKdfIterations(kdfIterations);
                    cipherSettings->setHmacAlgorithm(hmacAlgorithm);
                    cipherSettings->setKdfAlgorithm(kdfAlgorithm);
                }
            }

            if(foundDotenvPassword)
            {
                // Skip the CipherDialog prompt for now to test if the dotenv password was correct
            } else {
	            CipherDialog *cipherDialog = new CipherDialog(nullptr, false);
	            if(cipherDialog->exec())
	            {
	                delete cipherSettings;
	                cipherSettings = new CipherSettings(cipherDialog->getCipherSettings());
	            } else {
	                sqlite3_close(dbHandle);
	                *encrypted = false;
	                delete cipherSettings;
	                cipherSettings = nullptr;
	                return false;
	            }
	        }

            // Close and reopen database first to be in a clean state after the failed read attempt from above
            sqlite3_close(dbHandle);
            if(sqlite3_open_v2(filePath.toUtf8(), &dbHandle, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
            {
                delete cipherSettings;
                cipherSettings = nullptr;
                return false;
            }

            // Set the key
            sqlite3_exec(dbHandle, QString("PRAGMA key = %1").arg(cipherSettings->getPassword()).toUtf8(), nullptr, nullptr, nullptr);

            // Set the page size if it differs from the default value
            if(cipherSettings->getPageSize() != enc_default_page_size)
                sqlite3_exec(dbHandle, QString("PRAGMA cipher_page_size = %1;").arg(cipherSettings->getPageSize()).toUtf8(), nullptr, nullptr, nullptr);
            if(cipherSettings->getKdfIterations() != enc_default_kdf_iter)
                sqlite3_exec(dbHandle, QString("PRAGMA kdf_iter = %1;").arg(cipherSettings->getKdfIterations()).toUtf8(), nullptr, nullptr, nullptr);
            if(cipherSettings->getHmacAlgorithm() != enc_default_hmac_algorithm)
                sqlite3_exec(dbHandle, QString("PRAGMA cipher_hmac_algorithm = HMAC_%1;").arg(cipherSettings->getHmacAlgorithm()).toUtf8(), nullptr, nullptr, nullptr);
            if(cipherSettings->getKdfAlgorithm() != enc_default_kdf_algorithm)
                sqlite3_exec(dbHandle, QString("PRAGMA cipher_kdf_algorithm = PBKDF2_HMAC_%1;").arg(cipherSettings->getKdfAlgorithm()).toUtf8(), nullptr, nullptr, nullptr);

            *encrypted = true;
#else
            lastErrorMessage = QString::fromUtf8(sqlite3_errmsg(dbHandle));
            sqlite3_close(dbHandle);
            return false;
#endif
        } else {
            sqlite3_finalize(vm);
            sqlite3_close(dbHandle);
            return true;
        }
    }
}

void DBBrowserDB::getSqliteVersion(QString& sqlite, QString& sqlcipher)
{
    sqlite = QString(SQLITE_VERSION);

    // The SQLCipher version must be queried via a pragma and for a pragma we need a database connection.
    // Because we want to be able to query the SQLCipher version without opening a database file first, we
    // open a separate connection to an in-memory database here.
    sqlcipher = QString();
#ifdef ENABLE_SQLCIPHER
    sqlite3* dummy;
    if(sqlite3_open(":memory:", &dummy) == SQLITE_OK)
    {
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(dummy, "PRAGMA cipher_version", -1, &stmt, nullptr) == SQLITE_OK)
        {
            if(sqlite3_step(stmt) == SQLITE_ROW)
                sqlcipher = QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, 0)), sqlite3_column_bytes(stmt, 0));

            sqlite3_finalize(stmt);
        }

        sqlite3_close(dummy);
    }
#endif
}

bool DBBrowserDB::setSavepoint(const QString& pointname)
{
    if(!isOpen())
        return false;
    if(savepointList.contains(pointname))
        return true;

    QString query = QString("SAVEPOINT %1;").arg(sqlb::escapeIdentifier(pointname));
    executeSQL(query, false, false);
    savepointList.append(pointname);
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::releaseSavepoint(const QString& pointname)
{
    if(!isOpen())
        return false;
    if(savepointList.contains(pointname) == false)
        // If there is no such savepoint in the list,
        // we have already released it, so in this case
        // the operation should be successfull
        return true;

    QString query = QString("RELEASE %1;").arg(sqlb::escapeIdentifier(pointname));
    if(!executeSQL(query, false, false))
        return false;
    // SQLite releases all savepoints that were created between
    // creation of given savepoint and releasing of it,
    // so we should too
    int point_index = savepointList.lastIndexOf(pointname);
    savepointList.erase(savepointList.begin()+point_index, savepointList.end());
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::revertToSavepoint(const QString& pointname)
{
    if(!isOpen() || savepointList.contains(pointname) == false)
        return false;

    QString query = QString("ROLLBACK TO SAVEPOINT %1;").arg(sqlb::escapeIdentifier(pointname));
    executeSQL(query, false, false);
    query = QString("RELEASE %1;").arg(sqlb::escapeIdentifier(pointname));
    executeSQL(query, false, false);
    // SQLite releases all savepoints that were created between
    // creation of given savepoint and releasing of it,
    // so we should too
    int point_index = savepointList.lastIndexOf(pointname);
    savepointList.erase(savepointList.begin()+point_index, savepointList.end());
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::releaseAllSavepoints()
{
    if(!_db)
        return false;

    waitForDbRelease();

    for(const QString& point : savepointList)
    {
        if(!releaseSavepoint(point))
            return false;
    }

    // When still in a transaction, commit that too
    if(sqlite3_get_autocommit(_db) == 0)
        executeSQL("COMMIT;", false, false);

    return true;
}

bool DBBrowserDB::revertAll()
{
    for(const QString& point : savepointList)
    {
        if(!revertToSavepoint(point))
            return false;
    }
    return true;
}

bool DBBrowserDB::create ( const QString & db)
{
    if (isOpen())
        close();

    // read encoding from settings and open with sqlite3_open for utf8 and sqlite3_open16 for utf16
    QString sEncoding = Settings::getValue("db", "defaultencoding").toString();

    int openresult = SQLITE_OK;

    if(sEncoding == "UTF-8" || sEncoding == "UTF8" || sEncoding == "Latin1")
        openresult = sqlite3_open(db.toUtf8(), &_db);
    else
        openresult = sqlite3_open16(db.utf16(), &_db);

    if( openresult != SQLITE_OK ){
        lastErrorMessage = QString::fromUtf8(sqlite3_errmsg(_db));
        sqlite3_close(_db);
        _db = nullptr;
        return false;
    }

    if (_db)
    {
        // Set foreign key settings as requested in the preferences
        bool foreignkeys = Settings::getValue("db", "foreignkeys").toBool();
        setPragma("foreign_keys", foreignkeys ? "1" : "0");

        // Register REGEXP function
        if(Settings::getValue("extensions", "disableregex").toBool() == false)
            sqlite3_create_function(_db, "REGEXP", 2, SQLITE_UTF8, nullptr, regexp, nullptr, nullptr);

        // force sqlite3 do write proper file header
        // if we don't create and drop the table we might end up
        // with a 0 byte file, if the user cancels the create table dialog
        {
            NoStructureUpdateChecks nup(*this);
            executeSQL("CREATE TABLE notempty (id integer primary key);", false, false);
            executeSQL("DROP TABLE notempty;", false, false);
        }

        // Load extensions
        loadExtensionsFromSettings();

        // Execute default SQL
        QString default_sql = Settings::getValue("db", "defaultsqltext").toString();
        if(!default_sql.isEmpty())
            executeMultiSQL(default_sql, false, true);

        curDBFilename = db;
        isEncrypted = false;
        isReadOnly = false;
        updateSchema();
        return true;
    } else {
        return false;
    }
}

bool DBBrowserDB::close()
{
    waitForDbRelease();

    if(_db)
    {
        if (getDirty())
        {
            // In-memory databases can't be saved to disk. So the need another text than regular databases.
            // Note that the QMessageBox::Yes option in the :memory: case and the QMessageBox::No option in the regular case are
            // doing the same job: proceeding but not saving anything.
            QMessageBox::StandardButton reply;
            if(curDBFilename == ":memory:")
            {
                reply = QMessageBox::question(nullptr,
                                              QApplication::applicationName(),
                                              tr("Do you really want to close this temporary database? All data will be lost."),
                                              QMessageBox::Yes | QMessageBox::Cancel);
            } else {
                reply = QMessageBox::question(nullptr,
                                              QApplication::applicationName(),
                                              tr("Do you want to save the changes made to the database file %1?").arg(curDBFilename),
                                              QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
            }

            // If the user clicked the cancel button stop here and return false
            if(reply == QMessageBox::Cancel)
                return false;

            // If he didn't it was either yes or no
            if(reply == QMessageBox::Save)
                releaseAllSavepoints();
            else
                revertAll(); //not really necessary, I think... but will not hurt.
        }
        if(sqlite3_close(_db) != SQLITE_OK)
            qWarning() << tr("Database didn't close correctly, probably still busy");
        _db = nullptr;
    }

    schemata.clear();
    savepointList.clear();
    emit dbChanged(getDirty());
    emit structureUpdated();

    // Return true to tell the calling function that the closing wasn't cancelled by the user
    return true;
}

DBBrowserDB::db_pointer_type DBBrowserDB::get(QString user, bool force_wait)
{
    if(!_db)
        return nullptr;

    waitForDbRelease(force_wait ? Wait : Ask);

    db_user = user;
    db_used = true;
    emit databaseInUseChanged(true, user);

    return db_pointer_type(_db, DatabaseReleaser(this));
}

void DBBrowserDB::waitForDbRelease(ChoiceOnUse choice)
{
    if(!_db)
        return;

    // We can't show a message box from another thread than the main thread. So instead of crashing we
    // just decide that we don't interrupt any running query in this case.
    if(choice == Ask && QThread::currentThread() != QApplication::instance()->thread())
        choice = Wait;

    std::unique_lock<std::mutex> lk(m);
    while(db_used) {
        // notify user, give him the opportunity to cancel that
        auto str = db_user;
        lk.unlock();

        bool cancel = choice == CancelOther;
        if(choice == Ask) {
            QMessageBox msgBox;
            msgBox.setText(tr("The database is currently busy: ") + str);
            msgBox.setInformativeText(tr("Do you want to abort that other operation?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();

            cancel = ret == QMessageBox::Yes;
        }
        if(cancel)
            sqlite3_interrupt(_db);

        lk.lock();
        cv.wait(lk, [this](){ return !db_used; });
    }
}

bool DBBrowserDB::dump(const QString& filePath,
    const QStringList& tablesToDump,
    bool insertColNames,
    bool insertNewSyntx,
    bool exportSchema,
    bool exportData,
    bool keepOldSchema)
{
    waitForDbRelease();

    // Open file
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        // Count the total number of all records in all tables for the progress dialog
        size_t numRecordsTotal = 0, numRecordsCurrent = 0;
        objectMap objMap = schemata["main"];            // We only always export the main database, not the attached databases
        QList<sqlb::ObjectPtr> tables = objMap.values("table");
        for(QMutableListIterator<sqlb::ObjectPtr> it(tables);it.hasNext();)
        {
            it.next();

            // Remove the sqlite_stat1 and the sqlite_sequence tables if they exist. Also remove any tables which are not selected for export.
            if(it.value()->name() == "sqlite_stat1" || it.value()->name() == "sqlite_sequence" || !tablesToDump.contains(it.value()->name()))
            {
                it.remove();
            } else {
                // Otherwise get the number of records in this table
                numRecordsTotal += querySingleValueFromDb(QString("SELECT COUNT(*) FROM %1;")
                                                         .arg(sqlb::ObjectIdentifier("main", it.value()->name()).toString())).toUInt();
            }
        }

        QProgressDialog progress(tr("Exporting database to SQL file..."),
                                 tr("Cancel"), 0, numRecordsTotal);
        progress.setWindowModality(Qt::ApplicationModal);
        progress.show();
        qApp->processEvents();

        // Open text stream to the file
        QTextStream stream(&file);

        // Put the SQL commands in a transaction block
        stream << "BEGIN TRANSACTION;\n";

        // First export the schema of all selected tables. We need to export the schema of all tables before we export the first INSERT statement to
        // make sure foreign keys are working properly.
        if(exportSchema)
        {
            for(auto it : tables)
            {
                // Write the SQL string used to create this table to the output file
                if(!keepOldSchema)
                    stream << QString("DROP TABLE IF EXISTS %1;\n").arg(sqlb::escapeIdentifier(it->name()));

                if(it->fullyParsed())
                    stream << it->sql("main", true) << "\n";
                else
                    stream << it->originalSql() << ";\n";
                }
        }

        // Now export the data as well
        if(exportData)
        {
            for(auto it : tables)
            {
                // get columns
                QStringList cols(std::dynamic_pointer_cast<sqlb::Table>(it)->fieldNames());

                QString sQuery = QString("SELECT * FROM %1;").arg(sqlb::escapeIdentifier(it->name()));
                QByteArray utf8Query = sQuery.toUtf8();
                sqlite3_stmt *stmt;
                QString lineSep(QString(")%1\n").arg(insertNewSyntx?',':';'));

                int status = sqlite3_prepare_v2(_db, utf8Query.data(), utf8Query.size(), &stmt, nullptr);
                if(SQLITE_OK == status)
                {
                    int columns = sqlite3_column_count(stmt);
                    size_t counter = 0;
                    qApp->processEvents();
                    while(sqlite3_step(stmt) == SQLITE_ROW)
                    {
                        if (counter) stream << lineSep;

                        if (!insertNewSyntx || !counter)
                        {
                            stream << "INSERT INTO " << sqlb::escapeIdentifier(it->name());
                            if (insertColNames)
                                stream << " (" << sqlb::escapeIdentifier(cols).join(",") << ")";
                            stream << " VALUES (";
                        }
                        else
                        {
                            stream << " (";
                        }

                        for (int i = 0; i < columns; ++i)
                        {
                            int fieldsize = sqlite3_column_bytes(stmt, i);
                            int fieldtype = sqlite3_column_type(stmt, i);
                            QByteArray bcontent(
                                        reinterpret_cast<const char*>(sqlite3_column_blob(stmt, i)),
                                        fieldsize);

                            if(bcontent.left(2048).contains('\0')) // binary check
                            {
                                stream << QString("X'%1'").arg(QString(bcontent.toHex()));
                            }
                            else
                            {
                                switch(fieldtype)
                                {
                                case SQLITE_TEXT:
                                case SQLITE_BLOB:
                                    stream << "'" << bcontent.replace("'", "''") << "'";
                                break;
                                case SQLITE_NULL:
                                    stream << "NULL";
                                break;
                                case SQLITE_FLOAT:
                                    if(bcontent.indexOf("Inf") != -1)
                                        stream << "'" << bcontent << "'";
                                    else
                                        stream << bcontent;
                                break;
                                default:
                                    stream << bcontent;
                                }
                            }
                            if(i != columns - 1)
                                stream << ',';
                        }

                        progress.setValue(++numRecordsCurrent);
                        if(counter % 5000 == 0)
                            qApp->processEvents();
                        counter++;

                        if(progress.wasCanceled())
                        {
                            sqlite3_finalize(stmt);
                            file.close();
                            file.remove();
                            QApplication::restoreOverrideCursor();
                            return false;
                        }
                    }
                    if (counter > 0) stream << ");\n";
                }
                sqlite3_finalize(stmt);
            }
        }

        // Finally export all objects other than tables
        if(exportSchema)
        {
            for(auto it : objMap)
            {
                // Make sure it's not a table again
                if(it->type() == sqlb::Object::Types::Table)
                    continue;

                // If this object is based on a table (e.g. is an index for that table) it depends on the existence of this table.
                // So if we didn't export the base table this depends on, don't export this object either.
                if(!it->baseTable().isEmpty() && !tablesToDump.contains(it->baseTable()))
                    continue;

                // Write the SQL string used to create this object to the output file
                if(!it->originalSql().isEmpty())
                {
                    if(!keepOldSchema)
                        stream << QString("DROP %1 IF EXISTS %2;\n")
                                  .arg(sqlb::Object::typeToString(it->type()).toUpper())
                                  .arg(sqlb::escapeIdentifier(it->name()));

                    if(it->fullyParsed())
                        stream << it->sql("main", true) << "\n";
                    else
                        stream << it->originalSql() << ";\n";
                }
            }
        }

        // Done
        stream << "COMMIT;\n";
        file.close();

        QApplication::restoreOverrideCursor();
        qApp->processEvents();
        return true;
    }
    return false;
}

bool DBBrowserDB::executeSQL(QString statement, bool dirtyDB, bool logsql)
{
    waitForDbRelease();
    if(!_db)
    {
        lastErrorMessage = tr("No database file opened");
        return false;
    }

    statement = statement.trimmed();

    if (logsql) logSQL(statement, kLogMsg_App);
    if (dirtyDB) setSavepoint();

    char* errmsg;
    if (SQLITE_OK == sqlite3_exec(_db, statement.toUtf8(), nullptr, nullptr, &errmsg))
    {
        // Update DB structure after executing an SQL statement. But try to avoid doing unnecessary updates.
        if(!dontCheckForStructureUpdates && (statement.startsWith("ALTER", Qt::CaseInsensitive) ||
                statement.startsWith("CREATE", Qt::CaseInsensitive) ||
                statement.startsWith("DROP", Qt::CaseInsensitive) ||
                statement.startsWith("ROLLBACK", Qt::CaseInsensitive)))
            updateSchema();

        return true;
    } else {
        lastErrorMessage = QString("%1 (%2)").arg(QString::fromUtf8(errmsg)).arg(statement);
        qWarning() << "executeSQL: " << statement << "->" << errmsg;
        sqlite3_free(errmsg);
        return false;
    }
}

bool DBBrowserDB::executeMultiSQL(const QString& statement, bool dirty, bool log)
{
    waitForDbRelease();
    if(!_db)
    {
        lastErrorMessage = tr("No database file opened");
        return false;
    }

    QString query = statement.trimmed();

    // Check if this SQL containts any transaction statements
    QRegExp transactionRegex("^\\s*BEGIN TRANSACTION;|COMMIT;\\s*$");
    if(query.contains(transactionRegex))
    {
        // If so remove them anc create a savepoint instead by overriding the dirty parameter
        query.remove(transactionRegex);
        dirty = true;
    }

    // Log the statement if needed
    if(log)
        logSQL(query, kLogMsg_App);

    // Set DB to dirty/create restore point if necessary
    QString savepoint_name;
    if(dirty)
    {
        savepoint_name = generateSavepointName("execmultisql");
        setSavepoint(savepoint_name);
    }

    // Show progress dialog
    int statement_size = query.size();
    QProgressDialog progress(tr("Executing SQL..."),
                             tr("Cancel"), 0, statement_size);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.show();

    // Execute the statement by looping until SQLite stops giving back a tail string
    sqlite3_stmt* vm;
    QByteArray utf8Query = query.toUtf8();
    const char *tail = utf8Query.data();
    int res = SQLITE_OK;
    unsigned int line = 0;
    bool structure_updated = false;
    while(tail && *tail != 0 && (res == SQLITE_OK || res == SQLITE_DONE))
    {
        line++;
        size_t tail_length = strlen(tail);

        // Update progress dialog, keep UI responsive
        progress.setValue(statement_size - tail_length);
        qApp->processEvents();
        if(progress.wasCanceled())
        {
            lastErrorMessage = tr("Action cancelled.");
            return false;
        }

        // Check whether the DB structure is changed by this statement
        QString qtail = QString(tail);
        if(!dontCheckForStructureUpdates && !structure_updated && (qtail.startsWith("ALTER", Qt::CaseInsensitive) ||
                qtail.startsWith("CREATE", Qt::CaseInsensitive) ||
                qtail.startsWith("DROP", Qt::CaseInsensitive) ||
                qtail.startsWith("ROLLBACK", Qt::CaseInsensitive)))
            structure_updated = true;

        // Execute next statement
        res = sqlite3_prepare_v2(_db, tail, tail_length, &vm, &tail);
        if(res == SQLITE_OK)
        {
            switch(sqlite3_step(vm))
            {
            case SQLITE_OK:
            case SQLITE_ROW:
            case SQLITE_DONE:
            case SQLITE_MISUSE:             // This is a workaround around problematic user scripts. If they lead to empty commands,
                                            // SQLite will return a misuse error which we hereby ignore.
                sqlite3_finalize(vm);
                break;
            default:
                // In case of *any* error abort the execution and roll back the transaction

                // Make sure to save the error message first before any other function can mess around with it
                lastErrorMessage = tr("Error in statement #%1: %2.\nAborting execution%3.")
                        .arg(line)
                        .arg(sqlite3_errmsg(_db))
                        .arg(dirty ? tr(" and rolling back") : "");
                qWarning() << lastErrorMessage;

                // Clean up
                sqlite3_finalize(vm);
                if(dirty)
                    revertToSavepoint(savepoint_name);
                return false;
            }
        } else {
            lastErrorMessage = tr("Error in statement #%1: %2.\nAborting execution%3.")
                    .arg(line)
                    .arg(sqlite3_errmsg(_db))
                    .arg(dirty ? tr(" and rolling back") : "");
            qWarning() << lastErrorMessage;
            if(dirty)
                revertToSavepoint(savepoint_name);
            return false;
        }
    }

    // If the DB structure was changed by some command in this SQL script, update our schema representations
    if(structure_updated)
        updateSchema();

    // Exit
    return true;
}

QByteArray DBBrowserDB::querySingleValueFromDb(const QString& sql, bool log, ChoiceOnUse choice)
{
    waitForDbRelease(choice);
    if(!_db)
        return QByteArray();

    if(log)
        logSQL(sql, kLogMsg_App);

    QByteArray retval;

    QByteArray utf8Query = sql.toUtf8();
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(_db, utf8Query, utf8Query.size(), &stmt, nullptr) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            if(sqlite3_column_count(stmt) > 0 && sqlite3_column_type(stmt, 0) != SQLITE_NULL)
            {
                int bytes = sqlite3_column_bytes(stmt, 0);
                if(bytes)
                    retval = QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, 0)), bytes);
                else
                    retval = "";
            }

            sqlite3_finalize(stmt);
        } else {
            lastErrorMessage = tr("didn't receive any output from %1").arg(sql);
            qWarning() << lastErrorMessage;
        }
    } else {
        lastErrorMessage = tr("could not execute command: %1").arg(sqlite3_errmsg(_db));
        qWarning() << lastErrorMessage;
    }

    return retval;
}

bool DBBrowserDB::getRow(const sqlb::ObjectIdentifier& table, const QString& rowid, QVector<QByteArray>& rowdata)
{
    waitForDbRelease();
    if(!_db)
        return false;

    QString sQuery = QString("SELECT * FROM %1 WHERE %2='%3';")
            .arg(table.toString())
            .arg(sqlb::escapeIdentifier(getObjectByName<sqlb::Table>(table)->rowidColumn()))
            .arg(rowid);

    QByteArray utf8Query = sQuery.toUtf8();
    sqlite3_stmt *stmt;
    bool ret = false;
    if(sqlite3_prepare_v2(_db, utf8Query, utf8Query.size(), &stmt, nullptr) == SQLITE_OK)
    {
        // even this is a while loop, the statement should always only return 1 row
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            for (int i = 0; i < sqlite3_column_count(stmt); ++i)
            {
                if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                    rowdata.append(QByteArray());
                } else {
                    int bytes = sqlite3_column_bytes(stmt, i);
                    if(bytes)
                        rowdata.append(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, i)), bytes));
                    else
                        rowdata.append(QByteArray(""));
                }
            }
            ret = true;
        }
    }
    sqlite3_finalize(stmt);

    return ret;
}

QString DBBrowserDB::max(const sqlb::ObjectIdentifier& tableName, const sqlb::Field& field) const
{
    QString sQuery = QString("SELECT MAX(CAST(%2 AS INTEGER)) FROM %1;").arg(tableName.toString()).arg(sqlb::escapeIdentifier(field.name()));
    QByteArray utf8Query = sQuery.toUtf8();
    sqlite3_stmt *stmt;
    QString ret = "0";

    if(sqlite3_prepare_v2(_db, utf8Query, utf8Query.size(), &stmt, nullptr) == SQLITE_OK)
    {
        // even this is a while loop, the statement should always only return 1 row
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            if(sqlite3_column_count(stmt) == 1)
                ret = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    }
    sqlite3_finalize(stmt);

    return ret;
}

QString DBBrowserDB::emptyInsertStmt(const QString& schemaName, const sqlb::Table& t, const QString& pk_value) const
{
    QString stmt = QString("INSERT INTO %1.%2").arg(sqlb::escapeIdentifier(schemaName)).arg(sqlb::escapeIdentifier(t.name()));

    QStringList vals;
    QStringList fields;
    for(const sqlb::Field& f : t.fields)
    {
        sqlb::ConstraintPtr pk = t.constraint({f.name()}, sqlb::Constraint::PrimaryKeyConstraintType);
        if(pk)
        {
            fields << f.name();

            if(!pk_value.isNull())
            {
                vals << (f.isText()? "'" + pk_value + "'" : pk_value);
            } else {
                if(f.notnull())
                {
                    QString maxval = this->max(sqlb::ObjectIdentifier(schemaName, t.name()), f);
                    QString newval = QString::number(maxval.toLongLong() + 1);
                    vals << (f.isText()? "'" + newval + "'" : newval);
                } else {
                    vals << "NULL";
                }
            }
        } else if(f.notnull() && f.defaultValue().length() == 0) {
            fields << f.name();

            if(f.isInteger())
                vals << "0";
            else
                vals << "''";
        } else {
            // don't insert into fields with a default value
            // or we will never see it.
            if(f.defaultValue().length() == 0)
            {
                fields << f.name();
                vals << "NULL";
            }
        }
    }

    if(fields.empty())
    {
        stmt.append(" DEFAULT VALUES;");
    } else {
        stmt.append("(");
        stmt.append(sqlb::escapeIdentifier(fields).join(","));
        stmt.append(") VALUES (");
        stmt.append(vals.join(","));
        stmt.append(");");
    }

    return stmt;
}

QString DBBrowserDB::addRecord(const sqlb::ObjectIdentifier& tablename)
{
    waitForDbRelease();
    if(!_db)
        return QString();

    sqlb::TablePtr table = getObjectByName<sqlb::Table>(tablename);
    if(!table)
        return QString();

    // For tables without rowid we have to set the primary key by ourselves. We do so by querying for the largest value in the PK column
    // and adding one to it.
    QString sInsertstmt;
    QString pk_value;
    if(table->isWithoutRowidTable())
    {
        pk_value = QString::number(max(tablename, *sqlb::findField(table, table->rowidColumn())).toLongLong() + 1);
        sInsertstmt = emptyInsertStmt(tablename.schema(), *table, pk_value);
    } else {
        sInsertstmt = emptyInsertStmt(tablename.schema(), *table);
    }

    if(!executeSQL(sInsertstmt))
    {
        qWarning() << "addRecord: " << lastErrorMessage;
        return QString();
    } else {
        if(table->isWithoutRowidTable())
            return pk_value;
        else
            return QString::number(sqlite3_last_insert_rowid(_db));
    }
}

bool DBBrowserDB::deleteRecords(const sqlb::ObjectIdentifier& table, const QStringList& rowids, const QString& pseudo_pk)
{
    if (!isOpen()) return false;

    // Get primary key of the object to edit.
    QString pk = primaryKeyForEditing(table, pseudo_pk);
    if(pk.isNull())
    {
        lastErrorMessage = tr("Cannot delete this object");
        return false;
    }

    QStringList quoted_rowids;
    for(QString rowid : rowids)
        quoted_rowids.append("'" + rowid.replace("'", "''") + "'");

    QString statement = QString("DELETE FROM %1 WHERE %2 IN (%3);")
            .arg(table.toString())
            .arg(pk)
            .arg(quoted_rowids.join(", "));
    if(executeSQL(statement))
    {
        return true;
    } else {
        qWarning() << "deleteRecord: " << lastErrorMessage;
        return false;
    }
}

bool DBBrowserDB::updateRecord(const sqlb::ObjectIdentifier& table, const QString& column,
                               const QString& rowid, const QByteArray& value, bool itsBlob, const QString& pseudo_pk)
{
    waitForDbRelease();
    if (!isOpen()) return false;

    // Get primary key of the object to edit.
    QString pk = primaryKeyForEditing(table, pseudo_pk);
    if(pk.isNull())
    {
        lastErrorMessage = tr("Cannot set data on this object");
        return false;
    }

    QString sql = QString("UPDATE %1 SET %2=? WHERE %3='%4';")
            .arg(table.toString())
            .arg(sqlb::escapeIdentifier(column))
            .arg(sqlb::escapeIdentifier(pk))
            .arg(QString(rowid).replace("'", "''"));

    logSQL(sql, kLogMsg_App);
    setSavepoint();

    // If we get a NULL QByteArray we insert a NULL value, and for that
    // we can pass NULL to sqlite3_bind_text() so that it behaves like sqlite3_bind_null()
    const char *rawValue = value.isNull() ? nullptr : value.constData();

    sqlite3_stmt* stmt;
    int success = 1;
    if(sqlite3_prepare_v2(_db, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        success = 0;
    if(success == 1) {
        if(itsBlob)
        {
            if(sqlite3_bind_blob(stmt, 1, rawValue, value.length(), SQLITE_STATIC))
                success = -1;
        }
        else
        {
            if(sqlite3_bind_text(stmt, 1, rawValue, value.length(), SQLITE_STATIC))
                success = -1;
        }
    }
    if(success == 1 && sqlite3_step(stmt) != SQLITE_DONE)
        success = -1;
    if(success != 0 && sqlite3_finalize(stmt) != SQLITE_OK)
        success = -1;

    if(success == 1)
    {
        return true;
    } else {
        lastErrorMessage = sqlite3_errmsg(_db);
        qWarning() << "updateRecord: " << lastErrorMessage;
        return false;
    }
}

QString DBBrowserDB::primaryKeyForEditing(const sqlb::ObjectIdentifier& table, const QString& pseudo_pk) const
{
    // This function returns the primary key of the object to edit. For views we support 'pseudo' primary keys which must be specified manually.
    // If no pseudo pk is specified we'll take the rowid column of the table instead. If this neither a table nor was a pseudo-PK specified,
    // it is most likely a view that hasn't been configured for editing yet. In this case we return a null string to abort.

    if(pseudo_pk.isEmpty())
    {
        sqlb::TablePtr tbl = getObjectByName<sqlb::Table>(table);
        if(tbl)
            return tbl->rowidColumn();
    } else {
        return pseudo_pk;
    }

    return QString();
}

bool DBBrowserDB::createTable(const sqlb::ObjectIdentifier& name, const sqlb::FieldVector& structure)
{
    // Build SQL statement
    sqlb::Table table(name.name());
    for(size_t i=0;i<structure.size();i++)
        table.fields.push_back(structure.at(i));

    // Execute it and update the schema
    return executeSQL(table.sql(name.schema()));
}

bool DBBrowserDB::addColumn(const sqlb::ObjectIdentifier& tablename, const sqlb::Field& field)
{
    QString sql = QString("ALTER TABLE %1 ADD COLUMN %2").arg(tablename.toString()).arg(field.toString());

    // Execute it and update the schema
    return executeSQL(sql);
}

bool DBBrowserDB::alterTable(const sqlb::ObjectIdentifier& tablename, const sqlb::Table& new_table, AlterTableTrackColumns track_columns, QString newSchemaName)
{
    // This function is split into three different parts:
    // Part 1 checks the arguments and prepares them for processing. It also prepares the transaction etc.
    // Part 2 uses the built-in abilities of SQLite to change as much of the table schema as possible.
    // Part 3 does the rest of the changes to the table schema. It also finishes the transaction etc.

    //
    // P A R T   1
    //

    // If no new schema name has been set, we just use the old schema name
    if(newSchemaName.isNull())
    {
        newSchemaName = tablename.schema();

        // When renaming the table in the current schema, check if it doesn't exist already in there
        if(tablename.name() != new_table.name() && getObjectByName(sqlb::ObjectIdentifier(newSchemaName, new_table.name())) != nullptr)
        {
            lastErrorMessage = tr("A table with the name '%1' already exists in schema '%2'.").arg(new_table.name()).arg(newSchemaName);
            return false;
        }
    } else {
        // We're moving the table to a different schema. So check first if it doesn't already exist in the new schema.
        if(newSchemaName != tablename.schema() && getObjectByName(sqlb::ObjectIdentifier(newSchemaName, new_table.name())) != nullptr)
        {
            lastErrorMessage = tr("A table with the name '%1' already exists in schema '%2'.").arg(new_table.name()).arg(newSchemaName);
            return false;
        }
    }

    // Get old table schema
    sqlb::TablePtr old_table_ptr = getObjectByName<sqlb::Table>(tablename);
    if(old_table_ptr == nullptr)
    {
        lastErrorMessage = tr("No table with name '%1' exists in schema '%2'.").arg(tablename.name()).arg(tablename.schema());
        return false;
    }
    sqlb::Table old_table = *old_table_ptr;

    // Check if tracked fields actually exist in the old table
    for(const auto& old_name : track_columns.keys())
    {
        if(!old_name.isNull() && sqlb::findField(old_table, old_name) == old_table.fields.end())
        {
            lastErrorMessage = tr("Cannot find column %1.").arg(old_name);
            return false;
        }
    }

    // Check if there are any columns in the old table which are not mentioned in the tracked columns list.
    // We do this before checking if all tracked fields are in the new table to make sure the following check includes them.
    for(const auto& field : old_table.fields)
    {
        if(!track_columns.keys().contains(field.name()))
        {
            // If a field isn't tracked, add it to the list and indicate explicitly that it has the same name in the new table
            track_columns[field.name()] = field.name();
        }
    }

    // Check if tracked fields actually exist in the new table
    for(const auto& new_name : track_columns.values())
    {
        if(!new_name.isNull() && sqlb::findField(new_table, new_name) == new_table.fields.end())
        {
            lastErrorMessage = tr("Cannot find column %1.").arg(new_name);
            return false;
        }
    }

    // Check if any changes were made to the table schema
    if(old_table == new_table)
        return true;

    // Create savepoint to be able to go back to it in case of any error
    QString savepointName = generateSavepointName("renamecolumn");
    if(!setSavepoint(savepointName))
    {
        lastErrorMessage = tr("Creating savepoint failed. DB says: %1").arg(lastErrorMessage);
        return false;
    }

    // No automatic schema updates from now on
    NoStructureUpdateChecks nup(*this);

    //
    // P A R T   2
    //

    // This variable is used to track whether something was changed by this part of the function
    bool changed_something = false;

    // Rename table if necessary
    if(newSchemaName == tablename.schema() && tablename.name() != new_table.name())
    {
        if(!renameTable(tablename.schema(), old_table.name(), new_table.name()))
        {
            revertToSavepoint(savepointName);
            return false;
        }

        changed_something = true;
    }

    // Add columns if necessary
    const auto new_fields = track_columns.values(QString());
    for(const auto& field : new_table.fields)
    {
        // We loop through all the fields of the new table schema and check for each of them if they are new.
        // If so, we add that field. The reason for looping through the new table schema instead of the track_columns
        // map is that this way we make sure to preserve their order which increases our chances that we are done after
        // this step.
        if(new_fields.contains(field.name()))
        {
            if(!addColumn(sqlb::ObjectIdentifier(tablename.schema(), new_table.name()), field))
            {
                revertToSavepoint(savepointName);
                return false;
            }
        }

        changed_something = true;
    }

    // Newer versions of SQLite add a better ALTER TABLE support which we can use
#if SQLITE_VERSION_NUMBER >= 3025000
    // If the name of a field should be changed do that by using SQLite's ALTER TABLE feature. We build a new
    // map for tracking column names here which uses the update column names as the old names too. This is to
    // make sure we are using the new table layout for later updates.
    AlterTableTrackColumns new_track_columns;
    for(const auto& old_name : track_columns.keys())
    {
        QString new_name = track_columns[old_name];
        if(!old_name.isNull() && !new_name.isNull() && new_name != old_name)
        {
            if(!executeSQL(QString("ALTER TABLE %1 RENAME COLUMN %2 TO %3;")
                           .arg(sqlb::ObjectIdentifier(tablename.schema(), new_table.name()).toString())
                           .arg(sqlb::escapeIdentifier(old_name))
                           .arg(sqlb::escapeIdentifier(new_name))))
            {
                QString error(tr("Renaming the column failed. DB says:\n%1").arg(lastErrorMessage));
                revertToSavepoint(savepointName);
                lastErrorMessage = error;
                return false;
            }

            changed_something = true;
            new_track_columns.insert(new_name, new_name);
        } else {
            new_track_columns.insert(old_name, new_name);
        }
    }
    track_columns.swap(new_track_columns);
#endif

    // Update our schema representation to get the new table and all the changed triggers, views and indices
    if(changed_something)
    {
        updateSchema();
        old_table = *getObjectByName<sqlb::Table>(sqlb::ObjectIdentifier(tablename.schema(), new_table.name()));
    }

    // Check if there's still more work to be done or if we are finished now
    if(tablename.schema() == newSchemaName && old_table == new_table)
    {
        // Release the savepoint - everything went fine
        if(!releaseSavepoint(savepointName))
        {
            lastErrorMessage = tr("Releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
            return false;
        }

        // Success, update the DB schema before returning
        updateSchema();
        return true;
    }

    //
    // P A R T   3
    //

    // Create a new table with the desired schema and a name that doesn't exist yet
    QString new_table_name = new_table.name();
    sqlb::Table new_table_with_random_name = new_table;
    new_table_with_random_name.setName(generateTemporaryTableName(newSchemaName));
    if(!executeSQL(new_table_with_random_name.sql(newSchemaName), true, true))
    {
        QString error(tr("Creating new table failed. DB says: %1").arg(lastErrorMessage));
        revertToSavepoint(savepointName);
        lastErrorMessage = error;
        return false;
    }

    // Assemble list of column names to copy from in the old table and list of column names to into into in the new table
    QStringList copy_values_from;
    QStringList copy_values_to;
    for(const auto& from : track_columns.keys())
    {
        // Ignore new fields
        if(from.isNull())
            continue;

        // Ignore deleted fields
        QString to = track_columns[from];
        if(to.isNull())
            continue;

        copy_values_from.push_back(from);
        copy_values_to.push_back(to);
    }

    // Copy the data from the old table to the new one
    if(!executeSQL(QString("INSERT INTO %1.%2 (%3) SELECT %4 FROM %5;")
                   .arg(sqlb::escapeIdentifier(newSchemaName))
                   .arg(sqlb::escapeIdentifier(new_table_with_random_name.name()))
                   .arg(sqlb::escapeIdentifier(copy_values_to).join(","))
                   .arg(sqlb::escapeIdentifier(copy_values_from).join(","))
                   .arg(tablename.toString())))
    {
        QString error(tr("Copying data to new table failed. DB says:\n%1").arg(lastErrorMessage));
        revertToSavepoint(savepointName);
        lastErrorMessage = error;
        return false;
    }

    // Save all indices, triggers and views associated with this table because SQLite deletes them when we drop the table in the next step
    QStringList otherObjectsSql;
    for(auto it : schemata[tablename.schema()])
    {
        // If this object references the table and it's not the table itself save it's SQL string
        if(it->baseTable() == tablename.name() && it->type() != sqlb::Object::Types::Table)
        {
            // If this is an index, update the fields first. This highly increases the chance that the SQL statement won't throw an
            // error later on when we try to recreate it.
            if(it->type() == sqlb::Object::Types::Index)
            {
                sqlb::IndexPtr idx = std::dynamic_pointer_cast<sqlb::Index>(it);

                // Loop through all changes to the table schema. For indices only the column names are relevant, so it suffices to look at the
                // list of tracked columns
                for(const auto& from : track_columns)
                {
                    QString to = track_columns[from];

                    // Are we updating the field name or are we removing the field entirely?
                    if(!to.isNull())
                    {
                        // We're updating the field name. So search for it in the index and replace it whereever it is found
                        for(size_t i=0;i<idx->fields.size();i++)
                        {
                            if(idx->fields[i].name() == from)
                                idx->fields[i].setName(to);
                        }
                    } else {
                        // We're removing a field. So remove it from any indices, too.
                        while(sqlb::removeField(idx, from))
                            ;
                    }
                }

                // Only try to add the index later if it has any columns remaining. Also use the new schema name here, too, to basically move
                // any index that references the table to the same new schema as the table.
                if(idx->fields.size())
                    otherObjectsSql << idx->sql(newSchemaName);
            } else {
                // If it's a view or a trigger we don't have any chance to corrections yet. Just store the statement as is and
                // hope for the best.
                otherObjectsSql << it->originalSql().trimmed() + ";";
            }
        }
    }

    // We need to disable foreign keys here. The reason is that in the next step the entire table will be dropped and there might be foreign keys
    // in other tables that reference this table. These foreign keys would then cause the drop command in the next step to fail. However, we can't
    // simply disable foreign keys here since that is not allowed from inside a transaction and we definitely are inside a transaction at that point.
    // So what we do instead is defer foreign key enforcement until the end of the transaction which effectively disables foreign keys for us here.
    // But because we don't really want to defer foreign keys, the former value of that pragma is saved here in order to restore the old value later.
    QString foreignKeysOldSettings = getPragma("defer_foreign_keys");
    setPragma("defer_foreign_keys", "1");

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE %1;").arg(tablename.toString()), true, true))
    {
        QString error(tr("Deleting old table failed. DB says: %1").arg(lastErrorMessage));
        revertToSavepoint(savepointName);
        lastErrorMessage = error;
        return false;
    }

    // Rename the temporary table
    if(!renameTable(newSchemaName, new_table_with_random_name.name(), tablename.name()))
    {
        revertToSavepoint(savepointName);
        return false;
    }

    // Restore the former foreign key settings
    setPragma("defer_foreign_keys", foreignKeysOldSettings);

    // Restore the saved triggers, views and indices
    QString errored_sqls;
    for(const QString& sql : otherObjectsSql)
    {
        if(!executeSQL(sql, true, true))
            errored_sqls += sql + "\n";
    }
    if(!errored_sqls.isEmpty())
    {
        QMessageBox::information(nullptr, qApp->applicationName(), tr("Restoring some of the objects associated with this table failed. "
                                                                      "This is most likely because some column names changed. "
                                                                      "Here's the SQL statement which you might want to fix and execute manually:\n\n")
                                 + errored_sqls);
    }

    // Release the savepoint - everything went fine
    if(!releaseSavepoint(savepointName))
    {
        lastErrorMessage = tr("Releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
        return false;
    }

    // Success, update the DB schema before returning
    updateSchema();
    return true;
}

bool DBBrowserDB::renameTable(const QString& schema, const QString& from_table, const QString& to_table)
{
    // Do nothing if table names are the same
    if(from_table == to_table)
        return true;

    // Check if table names only differ in case. If they do, we have to rename the table twice because SQLite can't rename 'table' to 'Table'.
    // To solve this we rename 'table' to 'some temp name' and then 'some temp name' to 'Table'.
    if(from_table.compare(to_table, Qt::CaseInsensitive) == 0)
    {
        // Generate a temporary table name and rename the table via that by recusrively calling this function
        QString temp_name = generateTemporaryTableName(schema);
        if(!renameTable(schema, from_table, temp_name))
            return false;
        if(!renameTable(schema, temp_name, to_table))
            return false;

        // Exit here
        return true;
    }

    // The old and the new table names differ (and that not only in case)

    // Rename the table
    QString sql = QString("ALTER TABLE %1.%2 RENAME TO %3")
            .arg(sqlb::escapeIdentifier(schema))
            .arg(sqlb::escapeIdentifier(from_table))
            .arg(sqlb::escapeIdentifier(to_table));
    if(!executeSQL(sql))
    {
        QString error = tr("Error renaming table '%1' to '%2'."
            "Message from database engine:\n%3").arg(from_table).arg(to_table).arg(lastErrorMessage);
        lastErrorMessage = error;
        qWarning() << lastErrorMessage;
        return false;
    } else {
        return true;
    }
}

objectMap DBBrowserDB::getBrowsableObjects(const QString& schema) const
{
    objectMap res;

    for(auto it=schemata[schema].constBegin();it!=schemata[schema].constEnd();++it)
    {
        if(it.key() == "table" || it.key() == "view")
            res.insert(it.key(), it.value());
    }

    return res;
}

void DBBrowserDB::logSQL(QString statement, int msgtype)
{
    // Remove any leading and trailing spaces, tabs, or line breaks first
    statement = statement.trimmed();

    // Replace binary log messages by a placeholder text instead of printing gibberish
    for(int i=0;i<statement.size();i++)
    {
        QChar ch = statement[i];
        if(ch < 32 && ch != '\n' && ch != '\r' && ch != '\t')
        {
            statement.truncate(i>0?i-1:0);
            statement.append(tr("... <string can not be logged, contains binary data> ..."));

            // early exit if we detect a binary character,
            // to prevent checking all characters in a potential big string
            break;
        }
    }

    emit sqlExecuted(statement, msgtype);
}

void DBBrowserDB::updateSchema()
{
    waitForDbRelease();

    schemata.clear();

    // Exit here is no DB is opened
    if(!isOpen())
        return;

    // Get a list of all databases. This list always includes the main and the temp database but can include more items if there are attached databases
    QString db_statement = "PRAGMA database_list;";
    QByteArray db_utf8Statement = db_statement.toUtf8();
    logSQL(db_statement, kLogMsg_App);
    sqlite3_stmt* db_vm;
    if(sqlite3_prepare_v2(_db, db_utf8Statement, db_utf8Statement.length(), &db_vm, nullptr) == SQLITE_OK)
    {
        // Loop through all the databases
        while(sqlite3_step(db_vm) == SQLITE_ROW)
        {
            // Get the schema name which is in column 1 (counting starts with 0). 0 contains an ID and 2 the file path.
            QString schema_name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(db_vm, 1)));

            // Always add the schema to the map. This makes sure it's even then added when there are no objects in the database
            schemata[schema_name] = objectMap();

            // Get a list of all the tables for the current database schema. We need to do this differently for normal databases and the temporary schema
            // because SQLite doesn't understand the "temp.sqlite_master" notation.
            QString statement;
            if(schema_name == "temp")
                statement = QString("SELECT type,name,sql,tbl_name FROM sqlite_temp_master;");
            else
                statement = QString("SELECT type,name,sql,tbl_name FROM %1.sqlite_master;").arg(sqlb::escapeIdentifier(schema_name));
            QByteArray utf8Statement = statement.toUtf8();
            logSQL(statement, kLogMsg_App);

            sqlite3_stmt* vm;
            int err = sqlite3_prepare_v2(_db, utf8Statement, utf8Statement.length(), &vm, nullptr);
            if(err == SQLITE_OK)
            {
                while(sqlite3_step(vm) == SQLITE_ROW)
                {
                    QString val_type = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(vm, 0)));
                    QString val_name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(vm, 1)));
                    QString val_sql = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(vm, 2)));
                    QString val_tblname = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(vm, 3)));
                    val_sql.replace("\r", "");

                    if(!val_sql.isEmpty())
                    {
                        sqlb::ObjectPtr object;
                        if(val_type == "table")
                            object = sqlb::Table::parseSQL(val_sql);
                        else if(val_type == "index")
                            object = sqlb::Index::parseSQL(val_sql);
                        else if(val_type == "trigger")
                            object = sqlb::Trigger::parseSQL(val_sql);
                        else if(val_type == "view")
                            object = sqlb::View::parseSQL(val_sql);
                        else
                            continue;

                        // If parsing wasn't successful set the object name manually, so that at least the name is going to be correct
                        if(!object->fullyParsed())
                            object->setName(val_name);

                        // For virtual tables and views query the column list using the SQLite pragma because for both we can't yet rely on our grammar parser
                        if((object->type() == sqlb::Object::Types::Table && std::dynamic_pointer_cast<sqlb::Table>(object)->isVirtual()) || object->type() == sqlb::Object::Types::View)
                        {
                            auto columns = queryColumnInformation(schema_name, val_name);

                            if(object->type() == sqlb::Object::Types::Table)
                            {
                                sqlb::TablePtr tab = std::dynamic_pointer_cast<sqlb::Table>(object);
                                for(const auto& column : columns)
                                    tab->fields.emplace_back(column.first, column.second);
                            } else {
                                sqlb::ViewPtr view = std::dynamic_pointer_cast<sqlb::View>(object);
                                for(const auto& column : columns)
                                    view->fields.emplace_back(column.first, column.second);
                            }
                        } else if(object->type() == sqlb::Object::Types::Trigger) {
                            // For triggers set the name of the table the trigger operates on here because we don't have a parser for trigger statements yet.
                            sqlb::TriggerPtr trg = std::dynamic_pointer_cast<sqlb::Trigger>(object);
                            trg->setTable(val_tblname);
                        }

                        schemata[schema_name].insert(val_type, object);
                    }
                }
                sqlite3_finalize(vm);
            } else {
                qWarning() << tr("could not get list of db objects: %1, %2").arg(err).arg(sqlite3_errmsg(_db));
            }
        }

        sqlite3_finalize(db_vm);
    } else {
        qWarning() << tr("could not get list of databases: %1").arg(sqlite3_errmsg(_db));
    }

    emit structureUpdated();
}

QString DBBrowserDB::getPragma(const QString& pragma)
{
    QString sql;
    if (pragma=="case_sensitive_like")
        sql = "SELECT 'x' NOT LIKE 'X'";
    else
        sql = QString("PRAGMA %1").arg(pragma);
    return querySingleValueFromDb(sql);
}

bool DBBrowserDB::setPragma(const QString& pragma, const QString& value)
{
    // Set the pragma value
    QString sql = QString("PRAGMA %1 = '%2';").arg(pragma).arg(value);

    // In general, we want to commit changes before running pragmas because most of them can't be rolled back and some of them
    // even fail when run in a transaction. However, the defer_foreign_keys pragma has neither problem and we need it to be settable
    // inside transactions (see the renameColumn() function where it is set and reset at some point and where we don't want the changes
    // to be committed just because of this pragma).
    if(pragma != "defer_foreign_keys")
        releaseSavepoint();

    bool res = executeSQL(sql, false, true); // PRAGMA statements are usually not transaction bound, so we can't revert
    if( !res )
        qWarning() << tr("Error setting pragma %1 to %2: %3").arg(pragma).arg(value).arg(lastErrorMessage);

    // If this is the page_size or the auto_vacuum pragma being set, we need to execute the vacuum command right after the pragma statement or the new
    // settings won't be saved.
    if(res && (pragma == "page_size" || pragma == "auto_vacuum"))
        res = executeSQL("VACUUM;", false, true);

    return res;
}

bool DBBrowserDB::setPragma(const QString& pragma, const QString& value, QString& originalvalue)
{
    if( originalvalue != value )
    {
        if( setPragma(pragma, value))
        {
            originalvalue = value;
            return true;
        }
    }
    return false;
}

bool DBBrowserDB::setPragma(const QString& pragma, int value, int& originalvalue)
{
    if( originalvalue != value )
    {
        QString val = QString::number(value);
        QString origval = QString::number(originalvalue);
        if( setPragma(pragma, val, origval))
        {
            originalvalue = value;
        }
    }
    return false;
}

bool DBBrowserDB::loadExtension(const QString& filePath)
{
    waitForDbRelease();
    if(!_db)
        return false;

    // Check if file exists
    if(!QFile::exists(filePath))
    {
        lastErrorMessage = tr("File not found.");
        return false;
    }

    // Enable extension loading
    sqlite3_enable_load_extension(_db, 1);

    // Try to load extension
    char* error;
    int result = sqlite3_load_extension(_db, filePath.toUtf8(), nullptr, &error);

    // Disable extension loading if so configured
    // (we don't want to leave the possibility of calling load_extension() from SQL without user informed permission)
    if (!Settings::getValue("extensions", "enable_load_extension").toBool())
        sqlite3_enable_load_extension(_db, 0);

    if (result == SQLITE_OK)
    {
        return true;
    } else {
        lastErrorMessage = QString::fromUtf8(error);
        sqlite3_free(error);
        return false;
    }
}


void DBBrowserDB::loadExtensionsFromSettings()
{
    if(!_db)
        return;

    sqlite3_enable_load_extension(_db, Settings::getValue("extensions", "enable_load_extension").toBool());

    QStringList list = Settings::getValue("extensions", "list").toStringList();
    for(const QString& ext : list)
    {
        if(loadExtension(ext) == false)
            QMessageBox::warning(nullptr, QApplication::applicationName(), tr("Error loading extension: %1").arg(lastError()));
    }
}

QVector<QPair<QString, QString>> DBBrowserDB::queryColumnInformation(const QString& schema_name, const QString& object_name)
{
    waitForDbRelease();

    QVector<QPair<QString, QString>> result;
    QString statement = QString("PRAGMA %1.TABLE_INFO(%2);").arg(sqlb::escapeIdentifier(schema_name)).arg(sqlb::escapeIdentifier(object_name));
    logSQL(statement, kLogMsg_App);

    sqlite3_stmt* vm;
    const char* tail;
    if(sqlite3_prepare_v2(_db, statement.toUtf8(), statement.length(), &vm, &tail) == SQLITE_OK)
    {
        while(sqlite3_step(vm) == SQLITE_ROW)
        {
            QString name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(vm, 1)));
            QString type = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(vm, 2)));

            result.push_back(qMakePair(name, type));
        }
        sqlite3_finalize(vm);
    } else{
        lastErrorMessage = tr("could not get column information");
    }

    return result;
}

QString DBBrowserDB::generateSavepointName(const QString& identifier) const
{
    // Generate some sort of unique name for a savepoint for internal use.
    return QString("db4s_%1_%2").arg(identifier).arg(QDateTime::currentMSecsSinceEpoch());
}

QString DBBrowserDB::generateTemporaryTableName(const QString& schema) const
{
    // We're using a static variable as a counter here instead of checking from the beginning onwards every time. This has
    // two reasons: 1) It makes the function thread-safe, and 2) it saves us some time because in case older temporary tables
    // are still in use. Both reasons don't matter too much for now, but just in case...
    static std::atomic_uint counter;

    while(true)
    {
        QString table_name = QString("sqlb_temp_table_%1").arg(++counter);
        if(!getObjectByName(sqlb::ObjectIdentifier(schema, table_name)))
            return table_name;
    }
}

void DBBrowserDB::interruptQuery()
{
    if(!_db)
        return;

    sqlite3_interrupt(_db);
}
