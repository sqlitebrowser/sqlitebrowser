#include "sqlitedb.h"
#include "sqlite.h"
#include "sqlitetablemodel.h"
#include "CipherDialog.h"
#include "CipherSettings.h"
#include "DotenvFormat.h"
#include "Settings.h"
#include "Data.h"

#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QThread>
#include <QRegularExpression>

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstring>
#include <functional>

QStringList DBBrowserDB::Datatypes = {"INTEGER", "TEXT", "BLOB", "REAL", "NUMERIC"};
QStringList DBBrowserDB::DatatypesStrict = {"INT", "INTEGER", "TEXT", "BLOB", "REAL", "ANY"};

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

namespace sqlb {
QString escapeIdentifier(const QString& id)
{
    return QString::fromStdString(escapeIdentifier(id.toStdString()));
}
QString escapeString(const QString& literal)
{
    return QString::fromStdString(escapeString(literal.toStdString()));
}
QString escapeByteArray(const QByteArray& literal)
{
    if(isTextOnly(literal))
        return sqlb::escapeString(literal);
    else
        return QString("X'%1'").arg(QString(literal.toHex()));
}}

// collation callbacks
int collCompare(void* /*pArg*/, int sizeA, const void* sA, int sizeB, const void* sB)
{
    if(sizeA == sizeB)
        return memcmp(sA, sB, static_cast<size_t>(sizeA));
    return sizeA - sizeB;
}

static int sqlite_compare_utf16( void* /*arg*/,int size1, const void *str1, int size2, const void* str2)
{
    const QString string1 = QString::fromRawData(reinterpret_cast<const QChar*>(str1), static_cast<int>(static_cast<size_t>(size1) / sizeof(QChar)));
    const QString string2 = QString::fromRawData(reinterpret_cast<const QChar*>(str2), static_cast<int>(static_cast<size_t>(size2) / sizeof(QChar)));

    return QString::compare(string1, string2, Qt::CaseSensitive);
}

static int sqlite_compare_utf16ci( void* /*arg*/,int size1, const void *str1, int size2, const void* str2)
{
    const QString string1 = QString::fromRawData(reinterpret_cast<const QChar*>(str1), static_cast<int>(static_cast<size_t>(size1) / sizeof(QChar)));
    const QString string2 = QString::fromRawData(reinterpret_cast<const QChar*>(str2), static_cast<int>(static_cast<size_t>(size2) / sizeof(QChar)));

    return QString::compare(string1, string2, Qt::CaseInsensitive);
}

static void sqlite_make_single_value(sqlite3_context* ctx, int num_arguments, sqlite3_value* arguments[])
{
    QByteArray output;
    for(int i=0;i<num_arguments;i++)
        output += QByteArray::number(sqlite3_value_bytes(arguments[i])) + ":" + reinterpret_cast<const char*>(sqlite3_value_text(arguments[i]));

    char* output_str = new char[static_cast<size_t>(output.size()) + 1];
    std::strcpy(output_str, output);

    sqlite3_result_text(ctx, output_str, output.size(), [](void* ptr) {
        char* cptr = static_cast<char*>(ptr);
        delete cptr;
    });
}

DBBrowserDB::DBBrowserDB() :
    _db(nullptr),
    db_used(false),
    isEncrypted(false),
    isReadOnly(false),
    disableStructureUpdateChecks(false)
{
    // Register error log callback. This needs to be done before SQLite is first used
    Callback<void(void*, int, const char*)>::func = std::bind(&DBBrowserDB::errorLogCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    void (*log_callback)(void*, int, const char*) = static_cast<decltype(log_callback)>(Callback<void(void*, int, const char*)>::callback);
    sqlite3_config(SQLITE_CONFIG_LOG, log_callback, nullptr);

    // Enable URI filenames
    sqlite3_config(SQLITE_CONFIG_URI, 1);
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

void DBBrowserDB::errorLogCallback(void* /*user_data*/, int error_code, const char* message)
{
    QString msg = QString("(%1) %2").arg(error_code).arg(message);

    logSQL(msg, kLogMsg_ErrorLog);
}

static void regexp(sqlite3_context* ctx, int /*argc*/, sqlite3_value* argv[])
{
    // This is a cache for the last 50 regular expressions. Compiling them takes some time, so we want to cache the compiled
    // regular expressions for performance purposes.
    static std::array<std::pair<QString, QRegularExpression>, 50> regex_cache;

    // Check if pattern is in cache
    QString pattern{reinterpret_cast<const char*>(sqlite3_value_text(argv[0]))};
    QRegularExpression regex;
    const auto it = std::find_if(regex_cache.begin(), regex_cache.end(), [pattern](const std::pair<QString, QRegularExpression>& val) {
        return val.first == pattern;
    });
    if(it == regex_cache.end())
    {
        // Pattern is not in cache. Create a new regular expressions object, compile it, and insert it into the cache
        regex.setPattern(pattern);
        regex.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
        if(!regex.isValid())
            return sqlite3_result_error(ctx, "invalid operand", -1);
        regex.optimize();

        static size_t regex_cache_size;
        regex_cache_size = (regex_cache_size + 1) % regex_cache.size();
        regex_cache[regex_cache_size] = {pattern, regex};
    } else {
        // Pattern is in the cache. Just retrieve it
        regex = it->second;
    }

    // Get arguments and check their values
    QString arg2{reinterpret_cast<const char*>(sqlite3_value_text(argv[1]))};

    // Perform the actual matching and return the result.
    // SQLite expects a 0 for not found and a 1 for found.
    sqlite3_result_int(ctx, regex.match(arg2).hasMatch());
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
    disableStructureUpdateChecks = false;

    // Get encryption settings for database file
    CipherSettings cipherSettings;
    if(tryEncryptionSettings(db, &isEncrypted, &cipherSettings) == false)
        return false;

    // Open database file
    if(sqlite3_open_v2(db.toUtf8(), &_db, readOnly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK)
    {
        lastErrorMessage = QString::fromUtf8(sqlite3_errmsg(_db));
        return false;
    }

    // Set encryption details if database is encrypted
#ifdef ENABLE_SQLCIPHER
    if(isEncrypted)
    {
        executeSQL("PRAGMA key = " + cipherSettings.getPassword(), false, false);
        executeSQL("PRAGMA cipher_page_size = " + std::to_string(cipherSettings.getPageSize()), false, false);
        executeSQL("PRAGMA kdf_iter = " + std::to_string(cipherSettings.getKdfIterations()), false, false);
        executeSQL("PRAGMA cipher_hmac_algorithm = " + cipherSettings.getHmacAlgorithm(), false, false);
        executeSQL("PRAGMA cipher_kdf_algorithm = " + cipherSettings.getKdfAlgorithm(), false, false);
        executeSQL("PRAGMA cipher_plaintext_header_size = " + std::to_string(cipherSettings.getPlaintextHeaderSize()), false, false);
    }
#endif

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

        // Register our internal helper function for putting multiple values into a single column
        sqlite3_create_function_v2(
            _db,
            "sqlb_make_single_value",
            -1,
            SQLITE_UTF8 | SQLITE_DETERMINISTIC,
            nullptr,
            sqlite_make_single_value,
            nullptr,
            nullptr,
            nullptr
        );

        // Check if file is read only. In-memory databases are never read only
        if(db == ":memory:")
        {
            isReadOnly = false;
        } else {
            QFileInfo fi(db);
            QFileInfo fid(fi.absoluteDir().absolutePath());
            isReadOnly = readOnly || !fi.isWritable() || !fid.isWritable();
        }

        // Load extensions
        loadExtensionsFromSettings();

        // Execute default SQL
        if(!isReadOnly)
        {
            QByteArray default_sql = Settings::getValue("db", "defaultsqltext").toByteArray();
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

/**
  detaches a previously attached database identified with its alias-name
**/
bool DBBrowserDB::detach(const std::string& attached_as)
{
    if(!_db)
    {
        return false;
    }

    waitForDbRelease();

    // detach database
    if(!executeSQL("DETACH " + sqlb::escapeIdentifier(attached_as) + ";", false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }


    // Update schema to load database schema of the newly attached database
    updateSchema();

    return true;
}


bool DBBrowserDB::attach(const QString& filePath, QString attach_as)
{
    if(!_db)
        return false;

    waitForDbRelease();

    // In shared cache mode, attempting to attach the same database file more than once results in an error.
    // So no need for a check if this file has already been attached and abort if this is the case

    // Ask for name to be given to the attached database if none was provided
    if(attach_as.isEmpty())
    {
        attach_as = QInputDialog::getText(nullptr,
                                          qApp->applicationName(),
                                          tr("Please specify the database name under which you want to access the attached database"),
                                          QLineEdit::Normal,
                                          QFileInfo(filePath).baseName()
                                          ).trimmed();
    }
    if(attach_as.isNull())
        return false;

#ifdef ENABLE_SQLCIPHER
    // Try encryption settings
    CipherSettings cipherSettings;
    bool is_encrypted;
    if(tryEncryptionSettings(filePath, &is_encrypted, &cipherSettings) == false)
        return false;

    // Attach database
    std::string key;
    if(is_encrypted)
        key = "KEY " + cipherSettings.getPassword();
    else
        key = "KEY ''";

    // Only apply cipher settings if the database is encrypted
    if(is_encrypted)
    {
        if(!executeSQL("PRAGMA cipher_default_page_size = " + std::to_string(cipherSettings.getPageSize()), false))
        {
            QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
            return false;
        }
        if(!executeSQL("PRAGMA cipher_default_kdf_iter = " + std::to_string(cipherSettings.getKdfIterations()), false))
        {
            QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
            return false;
        }
        if(!executeSQL("PRAGMA cipher_hmac_algorithm = " + cipherSettings.getHmacAlgorithm(), false))
        {
            QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
            return false;
        }
        if(!executeSQL("PRAGMA cipher_kdf_algorithm = " + cipherSettings.getKdfAlgorithm(), false))
        {
            QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
            return false;
        }
        if(!executeSQL("PRAGMA cipher_plaintext_header_size = " + std::to_string(cipherSettings.getPlaintextHeaderSize()), false))
        {
            QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
            return false;
        }
    }

    if(!executeSQL("ATTACH " + sqlb::escapeString(filePath.toStdString()) + " AS " + sqlb::escapeIdentifier(attach_as.toStdString()) + " " + key, false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }

    // Clean up cipher settings
#else
    // Attach database
    if(!executeSQL("ATTACH " + sqlb::escapeString(filePath.toStdString()) + " AS " + sqlb::escapeIdentifier(attach_as.toStdString()), false))
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), lastErrorMessage);
        return false;
    }
#endif

    // Update schema to load database schema of the newly attached database
    updateSchema();

    return true;
}

bool DBBrowserDB::tryEncryptionSettings(const QString& filePath, bool* encrypted, CipherSettings* cipherSettings) const
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
    int enc_default_plaintext_header_size = 0;
    std::string enc_default_hmac_algorithm, enc_default_kdf_algorithm;
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
    while(true)
    {
        const std::string statement = "SELECT COUNT(*) FROM sqlite_master;";
        sqlite3_stmt* vm;
        const char* tail;
        int err = sqlite3_prepare_v2(dbHandle, statement.c_str(), static_cast<int>(statement.size()), &vm, &tail);
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
                    std::string password = passwordValue.toString().toStdString();

                    QVariant keyFormatValue = dotenv.value(databaseFileName + "_keyFormat", QVariant(CipherSettings::KeyFormats::Passphrase));
                    CipherSettings::KeyFormats keyFormat = CipherSettings::getKeyFormat(keyFormatValue.toInt());

                    int pageSize = dotenv.value(databaseFileName + "_pageSize", enc_default_page_size).toInt();
                    int kdfIterations = dotenv.value(databaseFileName + "_kdfIter", enc_default_kdf_iter).toInt();
                    int plaintextHeaderSize = dotenv.value(databaseFileName + "_plaintextHeaderSize", enc_default_plaintext_header_size).toInt();
                    std::string hmacAlgorithm = dotenv.value(databaseFileName + "_hmacAlgorithm", QString::fromStdString(enc_default_hmac_algorithm)).toString().toStdString();
                    std::string kdfAlgorithm = dotenv.value(databaseFileName + "_kdfAlgorithm", QString::fromStdString(enc_default_kdf_algorithm)).toString().toStdString();

                    cipherSettings->setKeyFormat(keyFormat);
                    cipherSettings->setPassword(password);
                    cipherSettings->setPageSize(pageSize);
                    cipherSettings->setKdfIterations(kdfIterations);
                    cipherSettings->setHmacAlgorithm("HMAC_" + hmacAlgorithm);
                    cipherSettings->setKdfAlgorithm("PBKDF2_HMAC_" + kdfAlgorithm);
                    cipherSettings->setPlaintextHeaderSize(plaintextHeaderSize);
                }
            }

            if(foundDotenvPassword)
            {
                // Skip the CipherDialog prompt for now to test if the dotenv settings are correct
            } else {
	            CipherDialog *cipherDialog = new CipherDialog(nullptr, false);
	            if(cipherDialog->exec())
                {
                    *cipherSettings = cipherDialog->getCipherSettings();
	            } else {
	                sqlite3_close(dbHandle);
                    *encrypted = false;
	                return false;
	            }
	        }

            // Close and reopen database first to be in a clean state after the failed read attempt from above
            sqlite3_close(dbHandle);
            if(sqlite3_open_v2(filePath.toUtf8(), &dbHandle, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
                return false;

            // Set the key
            sqlite3_exec(dbHandle, ("PRAGMA key = " + cipherSettings->getPassword()).c_str(), nullptr, nullptr, nullptr);

            // Set the settings if they differ from the default values
            if(cipherSettings->getPageSize() != enc_default_page_size)
                sqlite3_exec(dbHandle, ("PRAGMA cipher_page_size = " + std::to_string(cipherSettings->getPageSize())).c_str(), nullptr, nullptr, nullptr);
            if(cipherSettings->getKdfIterations() != enc_default_kdf_iter)
                sqlite3_exec(dbHandle, ("PRAGMA kdf_iter = " + std::to_string(cipherSettings->getKdfIterations())).c_str(), nullptr, nullptr, nullptr);
            if(cipherSettings->getHmacAlgorithm() != enc_default_hmac_algorithm)
                sqlite3_exec(dbHandle, ("PRAGMA cipher_hmac_algorithm = " + cipherSettings->getHmacAlgorithm()).c_str(), nullptr, nullptr, nullptr);
            if(cipherSettings->getKdfAlgorithm() != enc_default_kdf_algorithm)
                sqlite3_exec(dbHandle, ("PRAGMA cipher_kdf_algorithm = " + cipherSettings->getKdfAlgorithm()).c_str(), nullptr, nullptr, nullptr);
            if(cipherSettings->getPlaintextHeaderSize() != enc_default_plaintext_header_size)
                sqlite3_exec(dbHandle, ("PRAGMA cipher_plaintext_header_size = " + std::to_string(cipherSettings->getPlaintextHeaderSize())).c_str(), nullptr, nullptr, nullptr);

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

bool DBBrowserDB::setSavepoint(const std::string& pointname)
{
    if(!isOpen())
        return false;
    if(isReadOnly) {
        qWarning() << "setSavepoint: not done. DB is read-only";
        return false;
    }
    if(contains(savepointList, pointname))
        return true;

    executeSQL("SAVEPOINT " + sqlb::escapeIdentifier(pointname) + ";", false, true);
    savepointList.push_back(pointname);
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::releaseSavepoint(const std::string& pointname)
{
    if(!isOpen())
        return false;
    if(contains(savepointList, pointname) == false)
        // If there is no such savepoint in the list,
        // we have already released it, so in this case
        // the operation should be successfull
        return true;

    if(!executeSQL("RELEASE " + sqlb::escapeIdentifier(pointname) + ";", false, true))
        return false;
    // SQLite releases all savepoints that were created between
    // creation of given savepoint and releasing of it,
    // so we should too
    auto it = std::find(savepointList.rbegin(), savepointList.rend(), pointname).base() - 1;
    savepointList.erase(it, savepointList.end());
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::revertToSavepoint(const std::string& pointname)
{
    if(!isOpen() || contains(savepointList, pointname) == false)
        return false;

    executeSQL("ROLLBACK TO SAVEPOINT " + sqlb::escapeIdentifier(pointname) + ";", false, true);
    executeSQL("RELEASE " + sqlb::escapeIdentifier(pointname) + ";", false, true);
    // SQLite releases all savepoints that were created between
    // creation of given savepoint and releasing of it,
    // so we should too
    auto it = std::find(savepointList.rbegin(), savepointList.rend(), pointname).base() - 1;
    savepointList.erase(it, savepointList.end());
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::releaseAllSavepoints()
{
    if(!_db)
        return false;

    waitForDbRelease();

    while(!savepointList.empty())
    {
        if(!releaseSavepoint(savepointList.front()))
            return false;
    }

    // When still in a transaction, commit that too
    if(sqlite3_get_autocommit(_db) == 0)
        executeSQL("COMMIT;", false, true);

    return true;
}

bool DBBrowserDB::revertAll()
{
    while(!savepointList.empty())
    {
        if(!revertToSavepoint(savepointList.front()))
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
        // force sqlite3 do write proper file header
        // if we don't create and drop the table we might end up
        // with a 0 byte file, if the user cancels the create table dialog
        {
            NoStructureUpdateChecks nup(*this);
            executeSQL("CREATE TABLE notempty (id integer primary key);", false, false);
            executeSQL("DROP TABLE notempty;", false, false);
        }

        // Close database and open it through the code for opening existing database files. This is slightly less efficient but saves us some duplicate
        // code.
        sqlite3_close(_db);
        return open(db);
    } else {
        return false;
    }
}

bool DBBrowserDB::close()
{
    // Do nothing if no file is opened
    if(!_db)
        return true;

    waitForDbRelease();

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
                                          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
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
    curDBFilename.clear();
    schemata.clear();
    savepointList.clear();

    emit dbChanged(getDirty());
    emit structureUpdated();

    // Return true to tell the calling function that the closing wasn't cancelled by the user
    return true;
}

bool DBBrowserDB::saveAs(const std::string& filename) {
    if(!_db)
        return false;

    waitForDbRelease();

    // Open the database file identified by filename. Exit early if this fails
    // for any reason.
    sqlite3* pTo;
    int rc = sqlite3_open(filename.c_str(), &pTo);
    if(rc!=SQLITE_OK) {
        qWarning() << tr("Cannot open destination file: '%1'").arg(filename.c_str());
        return false;
    } else {
        // Set up the backup procedure to copy from the "main" database of
        // connection _db to the main database of connection pTo.
        // If something goes wrong, pBackup will be set to nullptr and an error
        // code and message left in connection pTo.
        //
        // If the backup object is successfully created, call backup_step()
        // to copy data from _db to pTo. Then call backup_finish()
        // to release resources associated with the pBackup object.  If an
        // error occurred, then an error code and message will be left in
        // connection pTo. If no error occurred, then the error code belonging
        // to pTo is set to SQLITE_OK.
        //
        sqlite3_backup* pBackup = sqlite3_backup_init(pTo, "main", _db, "main");
        if(pBackup == nullptr) {
            qWarning() << tr("Cannot backup to file: '%1'. Message: %2").arg(filename.c_str(), sqlite3_errmsg(pTo));
            sqlite3_close(pTo);
            return false;
        } else {
            sqlite3_backup_step(pBackup, -1);
            sqlite3_backup_finish(pBackup);
        }
        rc = sqlite3_errcode(pTo);
    }

    if(rc == SQLITE_OK) {
        // Close current database and set backup as current
        sqlite3_close(_db);
        _db = pTo;
        curDBFilename = QString::fromStdString(filename);

        return true;
    } else {
        qWarning() << tr("Cannot backup to file: '%1'. Message: %2").arg(filename.c_str(), sqlite3_errmsg(pTo));
        // Close failed database connection.
        sqlite3_close(pTo);
        return false;
    }
}

DBBrowserDB::db_pointer_type DBBrowserDB::get(const QString& user, bool force_wait)
{
    if(!_db)
        return nullptr;

    waitForDbRelease(force_wait ? Wait : Ask);

    db_user = user;
    db_used = true;
    emit databaseInUseChanged(true, user);

    return db_pointer_type(_db, DatabaseReleaser(this));
}

void DBBrowserDB::waitForDbRelease(ChoiceOnUse choice) const
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
    const std::vector<std::string>& tablesToDump,
    bool insertColNames,
    bool insertNewSyntx,
    bool keepOriginal,
    bool exportSchema,
    bool exportData,
    bool keepOldSchema) const
{
    waitForDbRelease();

    // Open file
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        // Count the total number of all records in all tables for the progress dialog
        size_t numRecordsTotal = 0;
        objectMap objMap = schemata.at("main");             // We only always export the main database, not the attached databases
        std::vector<sqlb::TablePtr> tables;
        for(const auto& it : objMap.tables)
        {
            // Never export the sqlite_stat1 and the sqlite_sequence tables if they exist. Also only export any tables which are selected for export.
            if(!it.second->isView() && it.first != "sqlite_stat1" && it.first != "sqlite_sequence" && contains(tablesToDump, it.first))
            {
                // Get the number of records in this table and remember to export it
                tables.push_back(it.second);
                numRecordsTotal += querySingleValueFromDb("SELECT COUNT(*) FROM " + sqlb::ObjectIdentifier("main", it.first).toString()).toUInt();
            }
        }

        QProgressDialog progress(tr("Exporting database to SQL file..."),
                                 tr("Cancel"), 0, static_cast<int>(numRecordsTotal));
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
            for(const auto& it : tables)
            {
                // Write the SQL string used to create this table to the output file
                if(!keepOldSchema)
                    stream << QString("DROP TABLE IF EXISTS %1;\n").arg(QString::fromStdString(sqlb::escapeIdentifier(it->name())));

                if(it->fullyParsed() && !keepOriginal)
                    stream << QString::fromStdString(it->sql("main", keepOldSchema)) << "\n";
                else {
                    QString statement = QString::fromStdString(it->originalSql());
                    if(keepOldSchema) {
                        // The statement is guaranteed by SQLite to start with "CREATE TABLE"
                        const int createTableLength = 12;
                        statement.replace(0, createTableLength, "CREATE TABLE IF NOT EXISTS");
                    }
                    stream << statement << ";\n";
                }
            }
        }

        // Now export the data as well
        if(exportData)
        {
            for(const auto& it : tables)
            {
                // get columns
                sqlb::StringVector cols = it->fieldNames();

                std::string sQuery = "SELECT * FROM " + sqlb::escapeIdentifier(it->name());
                sqlite3_stmt *stmt;
                QString lineSep(QString(")%1\n").arg(insertNewSyntx?',':';'));

                int status = sqlite3_prepare_v2(_db, sQuery.c_str(), static_cast<int>(sQuery.size()), &stmt, nullptr);
                if(SQLITE_OK == status)
                {
                    int columns = sqlite3_column_count(stmt);
                    size_t counter = 0;
                    size_t numRecordsCurrent = 0;
                    qApp->processEvents();
                    while(sqlite3_step(stmt) == SQLITE_ROW)
                    {
                        if (counter) stream << lineSep;

                        if (!insertNewSyntx || !counter)
                        {
                            stream << "INSERT INTO " << QString::fromStdString(sqlb::escapeIdentifier(it->name()));
                            if (insertColNames)
                                stream << " (" << QString::fromStdString(sqlb::joinStringVector(sqlb::escapeIdentifier(cols), ",")) << ")";
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

                            switch(fieldtype)
                            {
                            case SQLITE_BLOB:
                                stream << QString("X'%1'").arg(QString(bcontent.toHex()));
                                break;
                            case SQLITE_TEXT:
                                stream << sqlb::escapeByteArray(bcontent);
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
                            if(i != columns - 1)
                                stream << ',';
                        }

                        progress.setValue(static_cast<int>(++numRecordsCurrent));
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
            auto writeSchema = [&stream, &tablesToDump, keepOldSchema, keepOriginal](const QString& type, auto objects) {
                for(const auto& obj : objects)
                {
                    const auto& it = obj.second;

                    // If this object is based on a table (e.g. is an index for that table) it depends on the existence of this table.
                    // So if we didn't export the base table this depends on, don't export this object either.
                    if(!sqlb::getBaseTable(it).empty() && !contains(tablesToDump, sqlb::getBaseTable(it)))
                        continue;

                    // Write the SQL string used to create this object to the output file
                    if(!it->originalSql().empty())
                    {
                        if(!keepOldSchema)
                            stream << QString("DROP %1 IF EXISTS %2;\n").arg(
                                      type.toUpper(),
                                      QString::fromStdString(sqlb::escapeIdentifier(it->name())));

                        if(it->fullyParsed() && !keepOriginal)
                            stream << QString::fromStdString(it->sql("main", keepOldSchema)) << "\n";
                        else
                            stream << QString::fromStdString(it->originalSql()) << ";\n";
                    }
                }
            };

            std::map<std::string, sqlb::TablePtr> views;
            std::copy_if(objMap.tables.begin(), objMap.tables.end(), std::inserter(views, views.end()), [](const auto& t) { return t.second->isView(); });
            writeSchema("view", views);
            writeSchema("index", objMap.indices);
            writeSchema("trigger", objMap.triggers);
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

// Callback for sqlite3_exec. It receives the user callback in the first parameter. Converts parameters
// to C++ classes and calls user callback.
int DBBrowserDB::callbackWrapper (void* callback, int numberColumns, char** values, char** columnNames)
{
    std::vector<QByteArray> valuesList;
    std::vector<QByteArray> namesList;

    for (int i=0; i<numberColumns; i++) {
        valuesList.emplace_back(values[i]);
        namesList.emplace_back(columnNames[i]);
    }

    execCallback userCallback = *(static_cast<execCallback*>(callback));
    return userCallback(numberColumns, valuesList, namesList);
}

bool DBBrowserDB::executeSQL(const std::string& statement, bool dirtyDB, bool logsql, execCallback callback)
{
    waitForDbRelease();
    if(!_db)
    {
        lastErrorMessage = tr("No database file opened");
        return false;
    }

    if (dirtyDB) setSavepoint();
    if (logsql) logSQL(QString::fromStdString(statement), kLogMsg_App);

    char* errmsg;
    if (SQLITE_OK == sqlite3_exec(_db, statement.c_str(), callback ? callbackWrapper : nullptr, &callback, &errmsg))
    {
        // Update DB structure after executing an SQL statement. But try to avoid doing unnecessary updates.
        if(!disableStructureUpdateChecks && (starts_with_ci(statement, "ALTER") ||
                starts_with_ci(statement, "CREATE") ||
                starts_with_ci(statement, "DROP") ||
                starts_with_ci(statement, "ROLLBACK")))
            updateSchema();

        return true;
    } else {
        lastErrorMessage = QString("%1 (%2)").arg(QString::fromUtf8(errmsg), QString::fromStdString(statement));
        qWarning() << "executeSQL: " << lastErrorMessage;
        sqlite3_free(errmsg);

        return false;
    }
}

bool DBBrowserDB::executeMultiSQL(QByteArray query, bool dirty, bool log)
{
    waitForDbRelease();
    if(!_db)
    {
        lastErrorMessage = tr("No database file opened");
        return false;
    }

    // Log the statement if needed
    if(log)
        logSQL(query, kLogMsg_App);

    // Show progress dialog
    QProgressDialog progress(tr("Executing SQL..."),
                             tr("Cancel"), 0, 100);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.show();

    // Execute the statement by looping until SQLite stops giving back a tail string
    sqlite3_stmt* vm;
    const char *tail = query.constData();
    const char * const tail_start = tail;
    const char * const tail_end = tail + query.size() + 1;
    size_t total_tail_length = static_cast<size_t>(tail_end - tail_start);
    unsigned int line = 0;
    bool structure_updated = false;
    int last_progress_value = -1;
    std::string savepoint_name;
    while(tail && *tail != 0)
    {
        line++;

        // Update progress dialog, keep UI responsive. Make sure to not spend too much time updating the progress dialog in case there are many small statements.
        int progress_value = static_cast<int>(static_cast<float>(tail - tail_start) / static_cast<float>(total_tail_length) * 100.0f);
        if(progress_value > last_progress_value)
        {
            progress.setValue(progress_value);
            qApp->processEvents();
            if(progress.wasCanceled())
            {
                lastErrorMessage = tr("Action cancelled.");
                return false;
            }
            last_progress_value = progress_value;
        }

        // Check next statement
        {
            // Ignore all whitespace at the start of the current tail
            const char* tail_ptr = tail;
            while(std::isspace(*tail_ptr))
                tail_ptr++;

            // Convert the first couple of bytes of the tail to a C++ string for easier handling. We only need the first 8 bytes (in cae it's a ROLLBACK
            // statement), so no need to convert the entire tail. If the tail is less than 8 bytes long, make sure not to read past its end.
            size_t length = std::min(static_cast<size_t>(tail_end - tail + 1), static_cast<size_t>(8));
            std::string next_statement(tail_ptr, length);
            std::transform(next_statement.begin(), next_statement.end(), next_statement.begin(), ::toupper);

            // Check for transaction statements and skip until the next semicolon
            if(next_statement.compare(0, 6, "COMMIT") == 0 ||
                    next_statement.compare(0, 4, "END ") == 0 ||
                    next_statement.compare(0, 6, "BEGIN ") == 0)
            {
                while(tail != tail_end)
                {
                    if(*tail++ == ';')
                        break;
                }

                // Set DB to dirty and create a restore point if we haven't done that yet
                if(savepoint_name.empty())
                {
                    savepoint_name = generateSavepointName("execmultisql");
                    setSavepoint(savepoint_name);
                    dirty = true;
                }

                // Don't just execute next statement. Start next statement with the same checks
                continue;
            }

            // Check whether the DB structure is changed by this statement
            if(!disableStructureUpdateChecks && !structure_updated)
            {
                // Check if it's a modifying statement
                if(next_statement.compare(0, 5, "ALTER") == 0 ||
                        next_statement.compare(0, 6, "CREATE") == 0 ||
                        next_statement.compare(0, 4, "DROP") == 0 ||
                        next_statement.compare(0, 8, "ROLLBACK") == 0)
                    structure_updated = true;
            }
        }

        // Execute next statement
        if(sqlite3_prepare_v2(_db, tail, static_cast<int>(tail_end - tail + 1), &vm, &tail) == SQLITE_OK)
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
                lastErrorMessage = tr("Error in statement #%1: %2.\nAborting execution%3.").arg(
                        QString::number(line),
                        sqlite3_errmsg(_db),
                        dirty ? tr(" and rolling back") : "");
                qWarning() << lastErrorMessage;

                // Clean up
                sqlite3_finalize(vm);
                if(dirty)
                    revertToSavepoint(savepoint_name);
                return false;
            }
        } else {
            lastErrorMessage = tr("Error in statement #%1: %2.\nAborting execution%3.").arg(
                    QString::number(line),
                    sqlite3_errmsg(_db),
                    dirty ? tr(" and rolling back") : "");
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

QByteArray DBBrowserDB::querySingleValueFromDb(const std::string& sql, bool log, ChoiceOnUse choice) const
{
    waitForDbRelease(choice);
    if(!_db)
        return QByteArray();

    if(log)
        logSQL(QString::fromStdString(sql), kLogMsg_App);

    QByteArray retval;

    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(_db, sql.c_str(), static_cast<int>(sql.size()), &stmt, nullptr) == SQLITE_OK)
    {
        // Execute the statement. We distinguish three types of results:
        // SQLITE_ROW in case some data was returned from the database. This data is then used as a return value.
        // SQLITE_DONE in case the statement executed successfully but did not return any data. We do nothing in this case, leaving the return value empty.
        // Any other case is an error, so we need to prepare an error message.
        int result = sqlite3_step(stmt);
        if(result == SQLITE_ROW)
        {
            if(sqlite3_column_count(stmt) > 0 && sqlite3_column_type(stmt, 0) != SQLITE_NULL)
            {
                int bytes = sqlite3_column_bytes(stmt, 0);
                if(bytes)
                    retval = QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, 0)), bytes);
                else
                    retval = "";
            }
        } else if(result != SQLITE_DONE) {
            lastErrorMessage = tr("didn't receive any output from %1").arg(QString::fromStdString(sql));
            qWarning() << lastErrorMessage;
        }

        sqlite3_finalize(stmt);
    } else {
        lastErrorMessage = tr("could not execute command: %1").arg(sqlite3_errmsg(_db));
        qWarning() << lastErrorMessage;
    }

    return retval;
}

bool DBBrowserDB::getRow(const sqlb::ObjectIdentifier& table, const QString& rowid, std::vector<QByteArray>& rowdata) const
{
    waitForDbRelease();
    if(!_db)
        return false;

    std::string query = "SELECT * FROM " + table.toString() + " WHERE ";

    // For a single rowid column we can use a simple WHERE condition, for multiple rowid columns we have to use sqlb_make_single_value to decode the composed rowid values.
    sqlb::StringVector pks = getTableByName(table)->rowidColumns();
    if(pks.size() == 1)
        query += sqlb::escapeIdentifier(pks.front()) + "='" + rowid.toStdString() + "'";
    else
        query += "sqlb_make_single_value(" + sqlb::joinStringVector(sqlb::escapeIdentifier(pks), ",") + ")=" + sqlb::escapeString(rowid.toStdString());

    sqlite3_stmt *stmt;
    bool ret = false;
    if(sqlite3_prepare_v2(_db, query.c_str(), static_cast<int>(query.size()), &stmt, nullptr) == SQLITE_OK)
    {
        // even this is a while loop, the statement should always only return 1 row
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            for (int i = 0; i < sqlite3_column_count(stmt); ++i)
            {
                if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                    rowdata.emplace_back();
                } else {
                    int bytes = sqlite3_column_bytes(stmt, i);
                    if(bytes)
                        rowdata.emplace_back(static_cast<const char*>(sqlite3_column_blob(stmt, i)), bytes);
                    else
                        rowdata.emplace_back("");
                }
            }
            ret = true;
        }
    }
    sqlite3_finalize(stmt);

    return ret;
}

unsigned long DBBrowserDB::max(const sqlb::ObjectIdentifier& tableName, const std::string& field) const
{
    // This query returns the maximum value of the given table and column
    std::string query = "SELECT MAX(CAST(" + sqlb::escapeIdentifier(field) + " AS INTEGER)) FROM " + tableName.toString() + ")";

    // If, however, there is a sequence table in this database and the given column is the primary key of the table, we try to look up a value in the sequence table
    if(schemata.at(tableName.schema()).tables.count("sqlite_sequence"))
    {
        auto pk = getTableByName(tableName)->primaryKeyColumns();
        if(pk.size() == 1 && pk.front().name() == field)
        {
            // This SQL statement tries to do two things in one statement: get the current sequence number for this table from the sqlite_sequence table or, if there is no record for the table, return the highest integer value in the given column.
            // This works by querying the sqlite_sequence table and using an aggregate function (SUM in this case) to make sure to always get exactly one result row, no matter if there is a sequence record or not. We then let COALESCE decide
            // whether to return that sequence value if there is one or fall back to the SELECT MAX statement from avove if there is no sequence value.
            query = "SELECT COALESCE(SUM(seq), (" + query + ") FROM sqlite_sequence WHERE name=" + sqlb::escapeString(tableName.name());
         }
    }

    return querySingleValueFromDb(query).toULong();
}

std::string DBBrowserDB::emptyInsertStmt(const std::string& schemaName, const sqlb::Table& t, const QString& pk_value) const
{
    std::string stmt = "INSERT INTO " + sqlb::escapeIdentifier(schemaName) + "." + sqlb::escapeIdentifier(t.name());

    const auto pk = t.primaryKeyColumns();

    sqlb::StringVector vals;
    sqlb::StringVector fields;
    for(const sqlb::Field& f : t.fields)
    {
        // Never insert into a generated column
        if(f.generated())
            continue;

        if(pk.size() == 1 &&  pk.at(0) == f.name())
        {
            fields.push_back(f.name());

            if(!pk_value.isNull())
            {
                vals.push_back(f.isText()? "'" + pk_value.toStdString() + "'" : pk_value.toStdString());
            } else {
                if(f.notnull())
                {
                    unsigned long maxval = this->max(sqlb::ObjectIdentifier(schemaName, t.name()), f.name());
                    std::string newval = std::to_string(maxval + 1);
                    vals.push_back(f.isText()? "'" + newval + "'" : newval);
                } else {
                    vals.push_back("NULL");
                }
            }
        } else if(f.notnull() && f.defaultValue().length() == 0) {
            fields.push_back(f.name());

            if(f.isInteger())
                vals.push_back("0");
            else
                vals.push_back("''");
        } else {
            // don't insert into fields with a default value
            // or we will never see it.
            if(f.defaultValue().length() == 0)
            {
                fields.push_back(f.name());
                vals.push_back("NULL");
            }
        }
    }

    if(fields.empty())
    {
        stmt.append(" DEFAULT VALUES;");
    } else {
        stmt.append("(");
        stmt.append(sqlb::joinStringVector(sqlb::escapeIdentifier(fields), ","));
        stmt.append(") VALUES (");
        stmt.append(sqlb::joinStringVector(vals, ","));
        stmt.append(");");
    }

    return stmt;
}

QString DBBrowserDB::addRecord(const sqlb::ObjectIdentifier& tablename)
{
    waitForDbRelease();
    if(!_db)
        return QString();

    sqlb::TablePtr table = getTableByName(tablename);
    if(!table)
        return QString();

    // For tables without rowid we have to set the primary key by ourselves. We do so by querying for the largest value in the PK column
    // and adding one to it.
    std::string sInsertstmt;
    QString pk_value;
    if(table->withoutRowidTable())
    {
        // For multiple rowid columns we just use the value of the last one and increase that one by one. If this doesn't yield a valid combination
        // the insert record dialog should pop up automatically.
        pk_value = QString::number(max(tablename, table->rowidColumns().back()) + 1);
        sInsertstmt = emptyInsertStmt(tablename.schema(), *table, pk_value);
    } else {
        sInsertstmt = emptyInsertStmt(tablename.schema(), *table);
    }

    if(!executeSQL(sInsertstmt))
    {
        qWarning() << "addRecord: " << lastErrorMessage;
        return QString();
    } else {
        if(table->withoutRowidTable())
            return pk_value;
        else
            return QString::number(sqlite3_last_insert_rowid(_db));
    }
}

bool DBBrowserDB::deleteRecords(const sqlb::ObjectIdentifier& table, const std::vector<QByteArray>& rowids, const sqlb::StringVector& pseudo_pk)
{
    if (!isOpen()) return false;

    // Get primary key of the object to edit.
    sqlb::StringVector pks = primaryKeyForEditing(table, pseudo_pk);
    if(pks.empty())
    {
        lastErrorMessage = tr("Cannot delete this object");
        return false;
    }

    // Quote all values in advance
    std::vector<std::string> quoted_rowids;
    std::transform(rowids.begin(), rowids.end(), std::back_inserter(quoted_rowids), [](const auto& rowid) { return sqlb::escapeByteArray(rowid).toStdString(); });

    // For a single rowid column we can use a SELECT ... IN(...) statement which is faster.
    // For multiple rowid columns we have to use sqlb_make_single_value to decode the composed rowid values.
    std::string statement;
    if(pks.size() == 1)
    {
        statement = "DELETE FROM " + table.toString() + " WHERE " + pks.at(0) + " IN ("+ sqlb::joinStringVector(quoted_rowids, ", ") + ");";
    } else {
        statement = "DELETE FROM " + table.toString() + " WHERE ";

        statement += "sqlb_make_single_value(";
        for(const auto& pk : pks)
            statement += sqlb::escapeIdentifier(pk) + ",";
        statement.erase(statement.end()-1);
        statement += ") IN (" + sqlb::joinStringVector(quoted_rowids, ", ") + ")";
    }

    if(executeSQL(statement))
    {
        return true;
    } else {
        qWarning() << "deleteRecord: " << lastErrorMessage;
        return false;
    }
}

bool DBBrowserDB::updateRecord(const sqlb::ObjectIdentifier& table, const std::string& column,
                               const QByteArray& rowid, const QByteArray& value, int force_type, const sqlb::StringVector& pseudo_pk)
{
    waitForDbRelease();
    if (!isOpen()) return false;

    // Get primary key of the object to edit.
    sqlb::StringVector pks = primaryKeyForEditing(table, pseudo_pk);
    if(pks.empty())
    {
        lastErrorMessage = tr("Cannot set data on this object");
        return false;
    }

    std::string sql = "UPDATE " + table.toString() + " SET " + sqlb::escapeIdentifier(column) + "=? WHERE ";

    // For a single rowid column we can use a simple WHERE condition, for multiple rowid columns we have to use sqlb_make_single_value to decode the composed rowid values.
    if(pks.size() == 1)
      sql += sqlb::escapeIdentifier(pks.front()) + "=" + sqlb::escapeByteArray(rowid).toStdString();
    else
        sql += "sqlb_make_single_value(" + sqlb::joinStringVector(sqlb::escapeIdentifier(pks), ",") + ")=" + sqlb::escapeString(rowid.toStdString());

    setSavepoint();
    logSQL(QString::fromStdString(sql), kLogMsg_App);

    // If we get a NULL QByteArray we insert a NULL value, and for that
    // we can pass NULL to sqlite3_bind_text() so that it behaves like sqlite3_bind_null()
    const char *rawValue = value.isNull() ? nullptr : value.constData();

    sqlite3_stmt* stmt;
    int success = 1;
    if(sqlite3_prepare_v2(_db, sql.c_str(), static_cast<int>(sql.size()), &stmt, nullptr) != SQLITE_OK)
        success = 0;
    if(success == 1) {
        if(force_type == SQLITE_BLOB)
        {
            if(sqlite3_bind_blob(stmt, 1, rawValue, value.length(), SQLITE_STATIC))
                success = -1;
        } else if(force_type == SQLITE_INTEGER) {
            if(sqlite3_bind_int64(stmt, 1, value.toLongLong()))
                success = -1;
        } else if(force_type == SQLITE_FLOAT) {
            if(sqlite3_bind_double(stmt, 1, value.toDouble()))
                success = -1;
        } else {
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

sqlb::StringVector DBBrowserDB::primaryKeyForEditing(const sqlb::ObjectIdentifier& table, const sqlb::StringVector& pseudo_pk) const
{
    // This function returns the primary key of the object to edit. For views we support 'pseudo' primary keys which must be specified manually.
    // If no pseudo pk is specified we'll take the rowid column of the table instead. If this neither a table nor was a pseudo-PK specified,
    // it is most likely a view that hasn't been configured for editing yet. In this case we return a null string to abort.

    if(pseudo_pk.empty())
    {
        sqlb::TablePtr tbl = getTableByName(table);
        if(tbl)
            return tbl->rowidColumns();
    } else {
        return pseudo_pk;
    }

    return sqlb::StringVector();
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
    return executeSQL("ALTER TABLE " + tablename.toString() + " ADD COLUMN " + field.toString());
}

bool DBBrowserDB::alterTable(const sqlb::ObjectIdentifier& tablename, const sqlb::Table& new_table, AlterTableTrackColumns track_columns, std::string newSchemaName)
{
    // This function is split into three different parts:
    // Part 1 checks the arguments and prepares them for processing. It also prepares the transaction etc.
    // Part 2 uses the built-in abilities of SQLite to change as much of the table schema as possible.
    // Part 3 does the rest of the changes to the table schema. It also finishes the transaction etc.

    //
    // P A R T   1
    //

    // If no new schema name has been set, we just use the old schema name
    if(newSchemaName.empty())
    {
        newSchemaName = tablename.schema();

        // When renaming the table in the current schema, check if it doesn't exist already in there
        if(tablename.name() != new_table.name() && getTableByName(sqlb::ObjectIdentifier(newSchemaName, new_table.name())) != nullptr)
        {
            lastErrorMessage = tr("A table with the name '%1' already exists in schema '%2'.").arg(QString::fromStdString(new_table.name()), QString::fromStdString(newSchemaName));
            return false;
        }
    } else {
        // We're moving the table to a different schema. So check first if it doesn't already exist in the new schema.
        if(newSchemaName != tablename.schema() && getTableByName(sqlb::ObjectIdentifier(newSchemaName, new_table.name())) != nullptr)
        {
            lastErrorMessage = tr("A table with the name '%1' already exists in schema '%2'.").arg(QString::fromStdString(new_table.name()), QString::fromStdString(newSchemaName));
            return false;
        }
    }

    // Get old table schema
    sqlb::TablePtr old_table_ptr = getTableByName(tablename);
    if(old_table_ptr == nullptr)
    {
        lastErrorMessage = tr("No table with name '%1' exists in schema '%2'.").arg(QString::fromStdString(tablename.name()), QString::fromStdString(tablename.schema()));
        return false;
    }
    sqlb::Table old_table(*old_table_ptr);

    // Check if tracked fields actually exist in the old table
    for(const auto& old_it : track_columns)
    {
        if(!old_it.first.isNull() && sqlb::findField(old_table, old_it.first.toStdString()) == old_table.fields.end())
        {
            lastErrorMessage = tr("Cannot find column %1.").arg(old_it.first);
            return false;
        }
    }

    // Check if there are any columns in the old table which are not mentioned in the tracked columns list.
    // We do this before checking if all tracked fields are in the new table to make sure the following check includes them.
    for(const auto& field : old_table.fields)
    {
        if(track_columns.find(QString::fromStdString(field.name())) == track_columns.end())
        {
            // If a field isn't tracked, add it to the list and indicate explicitly that it has the same name in the new table
            track_columns[QString::fromStdString(field.name())] = QString::fromStdString(field.name());
        }
    }

    // Check if tracked fields actually exist in the new table
    for(const auto& new_name_it : track_columns)
    {
        if(!new_name_it.second.isNull() && sqlb::findField(new_table, new_name_it.second.toStdString()) == new_table.fields.end())
        {
            lastErrorMessage = tr("Cannot find column %1.").arg(new_name_it.second);
            return false;
        }
    }

    // Check if any changes were made to the table schema
    if(old_table == new_table)
        return true;

    // Create savepoint to be able to go back to it in case of any error
    std::string savepointName = generateSavepointName("renamecolumn");
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
    for(const auto& field : new_table.fields)
    {
        // We loop through all the fields of the new table schema and check for each of them if they are new.
        // If so, we add that field. The reason for looping through the new table schema instead of the track_columns
        // map is that this way we make sure to preserve their order which increases our chances that we are done after
        // this step.
        if(std::any_of(track_columns.begin(), track_columns.end(), [&field](const std::pair<QString, QString>& p) {
                       return p.first.isNull() && p.second.toStdString() == field.name();
        }))
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
    for(const auto& old_name_it : track_columns)
    {
        QString old_name = old_name_it.first;

        QString new_name = track_columns[old_name];
        if(!old_name.isNull() && !new_name.isNull() && new_name != old_name)
        {
            if(!executeSQL("ALTER TABLE " + sqlb::ObjectIdentifier(tablename.schema(), new_table.name()).toString() + " RENAME COLUMN " +
                           sqlb::escapeIdentifier(old_name.toStdString()) + " TO " + sqlb::escapeIdentifier(new_name.toStdString())))
            {
                QString error(tr("Renaming the column failed. DB says:\n%1").arg(lastErrorMessage));
                revertToSavepoint(savepointName);
                lastErrorMessage = error;
                return false;
            }

            changed_something = true;
            new_track_columns.insert({new_name, new_name});
        } else {
            new_track_columns.insert({old_name, new_name});
        }
    }
    track_columns.swap(new_track_columns);
#endif

    // Update our schema representation to get the new table and all the changed triggers, views and indices
    if(changed_something)
    {
        updateSchema();
        old_table = *getTableByName(sqlb::ObjectIdentifier(tablename.schema(), new_table.name()));
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
    std::string new_table_name = new_table.name();
    sqlb::Table new_table_with_random_name(new_table);
    new_table_with_random_name.setName(generateTemporaryTableName(newSchemaName));
    if(!executeSQL(new_table_with_random_name.sql(newSchemaName), true, true))
    {
        QString error(tr("Creating new table failed. DB says: %1").arg(lastErrorMessage));
        revertToSavepoint(savepointName);
        lastErrorMessage = error;
        return false;
    }

    // Assemble list of column names to copy from in the old table and list of column names to into into in the new table
    sqlb::StringVector copy_values_from;
    sqlb::StringVector copy_values_to;
    for(const auto& from_it : track_columns)
    {
        const auto& from = from_it.first;

        // Ignore new fields
        if(from.isNull())
            continue;

        // Ignore deleted fields
        QString to = track_columns[from];
        if(to.isNull())
            continue;

        copy_values_from.push_back(from.toStdString());
        copy_values_to.push_back(to.toStdString());
    }

    // Copy the data from the old table to the new one
    if(!executeSQL("INSERT INTO " + sqlb::escapeIdentifier(newSchemaName) + "." + sqlb::escapeIdentifier(new_table_with_random_name.name()) +
                   " (" + sqlb::joinStringVector(sqlb::escapeIdentifier(copy_values_to), ",") + ") SELECT " +
                   sqlb::joinStringVector(sqlb::escapeIdentifier(copy_values_from), ",") + " FROM " +
                   sqlb::escapeIdentifier(tablename.schema()) + "." + sqlb::escapeIdentifier(old_table.name())))
    {
        QString error(tr("Copying data to new table failed. DB says:\n%1").arg(lastErrorMessage));
        revertToSavepoint(savepointName);
        lastErrorMessage = error;
        return false;
    }

    // Save all indices, triggers and views associated with this table because SQLite deletes them when we drop the table in the next step
    std::vector<std::string> otherObjectsSql;
    auto saveRelatedObjects = [old_table, track_columns, &otherObjectsSql, newSchemaName](const auto& objects) {
        for(const auto& obj : objects)
        {
            const auto& it = obj.second;

            // If this object references the table save its SQL string
            if(sqlb::getBaseTable(it) == old_table.name())
            {
                // If this is an index, update the fields first. This highly increases the chance that the SQL statement won't throw an
                // error later on when we try to recreate it.
                if(std::is_same<decltype(objects), decltype(objectMap::indices)>::value)
                {
                    sqlb::IndexPtr idx = std::dynamic_pointer_cast<sqlb::Index>(it);

                    // Loop through all changes to the table schema. For indices only the column names are relevant, so it suffices to look at the
                    // list of tracked columns
                    for(const auto& from_it : track_columns)
                    {
                        const auto& from = from_it.first;
                        const auto& to = from_it.second;

                        // Are we updating the field name or are we removing the field entirely?
                        if(!to.isNull())
                        {
                            // We're updating the field name. So search for it in the index and replace it wherever it is found
                            for(size_t i=0;i<idx->fields.size();i++)
                            {
                                if(idx->fields[i].name() == from.toStdString())
                                    idx->fields[i].setName(to.toStdString());
                            }
                        } else {
                            // We're removing a field. So remove it from any indices, too.
                            while(sqlb::removeField(idx, from.toStdString()))
                                ;
                        }
                    }

                    // Only try to add the index later if it has any columns remaining. Also use the new schema name here, too, to basically move
                    // any index that references the table to the same new schema as the table.
                    if(idx->fields.size())
                        otherObjectsSql.push_back(idx->sql(newSchemaName));
                } else {
                    // If it's a view or a trigger we don't have any chance to corrections yet. Just store the statement as is and
                    // hope for the best.
                    otherObjectsSql.push_back(it->originalSql() + ";");
                }
            }
        }
    };
    saveRelatedObjects(schemata[tablename.schema()].tables);        // We can safely pass the tables along with the views here since they never have a base table set
    saveRelatedObjects(schemata[tablename.schema()].indices);
    saveRelatedObjects(schemata[tablename.schema()].triggers);

    // We need to disable foreign keys here. The reason is that in the next step the entire table will be dropped and there might be foreign keys
    // in other tables that reference this table. These foreign keys would then cause the drop command in the next step to fail. However, we can't
    // simply disable foreign keys here since that is not allowed from inside a transaction and we definitely are inside a transaction at that point.
    // So what we do instead is defer foreign key enforcement until the end of the transaction which effectively disables foreign keys for us here.
    // But because we don't really want to defer foreign keys, the former value of that pragma is saved here in order to restore the old value later.
    QString foreignKeysOldSettings = getPragma("defer_foreign_keys");
    setPragma("defer_foreign_keys", "1");

    // Delete the old table
    if(!executeSQL("DROP TABLE " + sqlb::escapeIdentifier(tablename.schema()) + "." + sqlb::escapeIdentifier(old_table.name()), true, true))
    {
        QString error(tr("Deleting old table failed. DB says: %1").arg(lastErrorMessage));
        revertToSavepoint(savepointName);
        lastErrorMessage = error;
        return false;
    }

    // Rename the temporary table
    if(!renameTable(newSchemaName, new_table_with_random_name.name(), new_table.name()))
    {
        revertToSavepoint(savepointName);
        return false;
    }

    // Restore the former foreign key settings
    setPragma("defer_foreign_keys", foreignKeysOldSettings);

    // Restore the saved triggers, views and indices
    std::string errored_sqls;
    for(const std::string& sql : otherObjectsSql)
    {
        if(!executeSQL(sql, true, true))
            errored_sqls += sql + "\n";
    }
    if(!errored_sqls.empty())
    {
        QMessageBox::information(nullptr, qApp->applicationName(), tr("Restoring some of the objects associated with this table failed. "
                                                                      "This is most likely because some column names changed. "
                                                                      "Here's the SQL statement which you might want to fix and execute manually:\n\n")
                                 + QString::fromStdString(errored_sqls));
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

bool DBBrowserDB::renameTable(const std::string& schema, const std::string& from_table, const std::string& to_table)
{
    // Do nothing if table names are the same
    if(from_table == to_table)
        return true;

    // Check if table names only differ in case. If they do, we have to rename the table twice because SQLite can't rename 'table' to 'Table'.
    // To solve this we rename 'table' to 'some temp name' and then 'some temp name' to 'Table'.
    if(compare_ci(from_table, to_table))
    {
        // Generate a temporary table name and rename the table via that by recusrively calling this function
        std::string temp_name = generateTemporaryTableName(schema);
        if(!renameTable(schema, from_table, temp_name))
            return false;
        if(!renameTable(schema, temp_name, to_table))
            return false;

        // Exit here
        return true;
    }

    // The old and the new table names differ (and that not only in case)

    // Rename the table
    std::string sql = "ALTER TABLE " + sqlb::escapeIdentifier(schema) + "." + sqlb::escapeIdentifier(from_table) + " RENAME TO " + sqlb::escapeIdentifier(to_table);
    if(!executeSQL(sql))
    {
        QString error = tr("Error renaming table '%1' to '%2'.\n"
            "Message from database engine:\n%3").arg(QString::fromStdString(from_table), QString::fromStdString(to_table), lastErrorMessage);
        lastErrorMessage = error;
        qWarning() << lastErrorMessage;
        return false;
    } else {
        return true;
    }
}

void DBBrowserDB::logSQL(const QString& statement, LogMessageType msgtype) const
{
    // Remove any leading and trailing spaces, tabs, or line breaks first
    emit sqlExecuted(statement.trimmed(), msgtype);
}

void DBBrowserDB::updateSchema()
{
    waitForDbRelease();

    schemata.clear();

    // Exit here is no DB is opened
    if(!isOpen())
        return;

    // Get a list of all databases. This list always includes the main and the temp database but can include more items if there are attached databases
    if(!executeSQL("PRAGMA database_list;", false, true, [this](int, std::vector<QByteArray> db_values, std::vector<QByteArray>) -> bool {
        // Get the schema name which is in column 1 (counting starts with 0). 0 contains an ID and 2 the file path.
        const std::string schema_name = db_values.at(1).toStdString();

        // Always add the schema to the map. This makes sure it's even then added when there are no objects in the database
        objectMap& object_map = schemata[schema_name];

        // Get a list of all the tables for the current database schema. We need to do this differently for normal databases and the temporary schema
        // because SQLite doesn't understand the "temp.sqlite_master" notation.
        std::string statement;
        if(schema_name == "temp")
            statement = "SELECT type,name,sql,tbl_name FROM sqlite_temp_master;";
        else
            statement = "SELECT type,name,sql,tbl_name FROM " + sqlb::escapeIdentifier(schema_name) + ".sqlite_master;";

        if(!executeSQL(statement, false, true, [this, schema_name, &object_map](int, std::vector<QByteArray> values, std::vector<QByteArray>) -> bool {
            const std::string val_type = values.at(0).toStdString();
            const std::string val_name = values.at(1).toStdString();
            std::string val_sql = values.at(2).toStdString();
            const std::string val_tblname = values.at(3).toStdString();

            if(!val_sql.empty())
            {
                val_sql.erase(std::remove(val_sql.begin(), val_sql.end(), '\r'), val_sql.end());

                if(val_type == "table" || val_type == "view")
                {
                    sqlb::TablePtr table;
                    if(val_type == "table")
                        table = sqlb::Table::parseSQL(val_sql);
                    else
                        table = sqlb::View::parseSQL(val_sql);

                    if(!table->fullyParsed())
                        table->setName(val_name);

                    // For virtual tables, views, and tables we could not parse at all,
                    // query the column list using the SQLite pragma to at least get
                    // some information on them when our parser does not.
                    if((!table->fullyParsed() && table->fields.empty()) || table->isVirtual())
                    {
                        const auto columns = queryColumnInformation(schema_name, val_name);

                        for(const auto& column : columns)
                            table->fields.emplace_back(column.first, column.second);
                    }

                    object_map.tables.insert({val_name, table});
                } else if(val_type == "index") {
                    sqlb::IndexPtr index = sqlb::Index::parseSQL(val_sql);
                    if(!index->fullyParsed())
                        index->setName(val_name);

                    object_map.indices.insert({val_name, index});
                } else if(val_type == "trigger") {
                    sqlb::TriggerPtr trigger = sqlb::Trigger::parseSQL(val_sql);
                    trigger->setName(val_name);
                    trigger->setOriginalSql(val_sql);

                    // For triggers set the name of the table the trigger operates on here because we don't have a parser for trigger statements yet.
                    trigger->setTable(val_tblname);

                    object_map.triggers.insert({val_name, trigger});
                }
            }

            return false;
        }))
        {
            qWarning() << tr("could not get list of db objects: %1").arg(sqlite3_errmsg(_db));
        }

        return false;
    }))
    {
        qWarning() << tr("could not get list of databases: %1").arg(sqlite3_errmsg(_db));
    }

    emit structureUpdated();
}

QString DBBrowserDB::getPragma(const std::string& pragma) const
{
    if (pragma=="case_sensitive_like")
        return querySingleValueFromDb("SELECT 'x' NOT LIKE 'X';");
    else
        return querySingleValueFromDb("PRAGMA " + pragma + ";");
}

bool DBBrowserDB::setPragma(const std::string& pragma, const QString& value)
{
    // Set the pragma value
    std::string sql = "PRAGMA " + pragma + " = '" + value.toStdString() + "';";

    // In general, we want to commit changes before running pragmas because most of them can't be rolled back and some of them
    // even fail when run in a transaction. However, the defer_foreign_keys pragma has neither problem and we need it to be settable
    // inside transactions (see the renameColumn() function where it is set and reset at some point and where we don't want the changes
    // to be committed just because of this pragma).
    if(pragma != "defer_foreign_keys")
        releaseSavepoint();

    bool res = executeSQL(sql, false, true); // PRAGMA statements are usually not transaction bound, so we can't revert
    if( !res )
        qWarning() << tr("Error setting pragma %1 to %2: %3").arg(QString::fromStdString(pragma), value, lastErrorMessage);

    // If this is the page_size or the auto_vacuum pragma being set, we need to execute the vacuum command right after the pragma statement or the new
    // settings won't be saved.
    if(res && (pragma == "page_size" || pragma == "auto_vacuum"))
        res = executeSQL("VACUUM;", false, true);

    return res;
}

bool DBBrowserDB::setPragma(const std::string& pragma, const QString& value, QString& originalvalue)
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

bool DBBrowserDB::setPragma(const std::string& pragma, int value, int& originalvalue)
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

    const QStringList list = Settings::getValue("extensions", "list").toStringList();
    for(const QString& ext : list)
    {
        if(loadExtension(ext) == false)
            QMessageBox::warning(nullptr, QApplication::applicationName(), tr("Error loading extension: %1").arg(lastError()));
    }
}

std::vector<std::pair<std::string, std::string>> DBBrowserDB::queryColumnInformation(const std::string& schema_name, const std::string& object_name) const
{
    waitForDbRelease();

    std::vector<std::pair<std::string, std::string>> result;
    std::string statement = "PRAGMA " + sqlb::escapeIdentifier(schema_name) + ".TABLE_INFO(" + sqlb::escapeIdentifier(object_name) + ");";
    logSQL(QString::fromStdString(statement), kLogMsg_App);

    sqlite3_stmt* vm;
    const char* tail;
    if(sqlite3_prepare_v2(_db, statement.c_str(), static_cast<int>(statement.size()), &vm, &tail) == SQLITE_OK)
    {
        while(sqlite3_step(vm) == SQLITE_ROW)
        {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(vm, 1));
            std::string type = reinterpret_cast<const char*>(sqlite3_column_text(vm, 2));

            result.push_back(std::make_pair(name, type));
        }
        sqlite3_finalize(vm);
    } else{
        lastErrorMessage = tr("could not get column information");
    }

    return result;
}

std::string DBBrowserDB::generateSavepointName(const std::string& identifier) const
{
    // Generate some sort of unique name for a savepoint for internal use.
    return "db4s_" + identifier + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

std::string DBBrowserDB::generateTemporaryTableName(const std::string& schema) const
{
    // We're using a static variable as a counter here instead of checking from the beginning onwards every time. This has
    // two reasons: 1) It makes the function thread-safe, and 2) it saves us some time because in case older temporary tables
    // are still in use. Both reasons don't matter too much for now, but just in case...
    static std::atomic_uint counter;

    while(true)
    {
        std::string table_name = "sqlb_temp_table_" + std::to_string(++counter);
        if(!getTableByName(sqlb::ObjectIdentifier(schema, table_name)))
            return table_name;
    }
}

void DBBrowserDB::interruptQuery()
{
    if(!_db)
        return;

    sqlite3_interrupt(_db);
}
