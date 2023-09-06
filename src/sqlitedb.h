#ifndef SQLITEDB_H
#define SQLITEDB_H

#include "sql/ObjectIdentifier.h"
#include "sql/sqlitetypes.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <functional>
#include <vector>
#include <map>

#include <QObject>
#include <QByteArray>
#include <QStringList>

struct sqlite3;
class CipherSettings;

enum LogMessageType
{
    kLogMsg_User,
    kLogMsg_App,
    kLogMsg_ErrorLog
};

struct objectMap
{
    // These map from object name to object pointer
    std::map<std::string, sqlb::TablePtr> tables;       // This stores tables AND views
    std::map<std::string, sqlb::IndexPtr> indices;
    std::map<std::string, sqlb::TriggerPtr> triggers;

    bool empty() const
    {
        return tables.empty() && indices.empty() && triggers.empty();
    }
};

using schemaMap = std::map<std::string, objectMap>;             // Maps from the schema name (main, temp, attached schemas) to the object map for that schema

int collCompare(void* pArg, int sizeA, const void* sA, int sizeB, const void* sB);

namespace sqlb
{
QString escapeIdentifier(const QString& id);
QString escapeString(const QString& literal);
QString escapeByteArray(const QByteArray& literal);
}

/// represents a single SQLite database. except when noted otherwise,
/// all member functions are to be called from the main UI thread
/// only.
class DBBrowserDB : public QObject
{
    Q_OBJECT

private:
    /// custom unique_ptr deleter releases database for further use by others
    struct DatabaseReleaser
    {
        explicit DatabaseReleaser(DBBrowserDB * pParent_ = nullptr) : pParent(pParent_) {}

        DBBrowserDB * pParent;

        void operator() (const sqlite3* db) const
        {
            if(!db || !pParent)
                return;

            std::unique_lock<std::mutex> lk(pParent->m);
            pParent->db_used = false;
            lk.unlock();
            emit pParent->databaseInUseChanged(false, QString());
            pParent->cv.notify_one();
        }
    };

public:

    explicit DBBrowserDB();
    ~DBBrowserDB () override = default;

    bool open(const QString& db, bool readOnly = false);
    bool attach(const QString& filename, QString attach_as = QString());

    /**
      detaches a previously attached database identified with its alias-name

      \param attached_as the alias-name as witch a additional database file has been attached to the connection
    **/
    bool detach(const std::string& attached_as);
    bool create ( const QString & db);
    bool close();
    bool saveAs(const std::string& filename);

    // This returns the SQLite version as well as the SQLCipher if DB4S is compiled with encryption support
    static void getSqliteVersion(QString& sqlite, QString& sqlcipher);

    using db_pointer_type = std::unique_ptr<sqlite3, DatabaseReleaser>;

    /**
       borrow exclusive address to the currently open database, until
       releasing the returned unique_ptr.

       the intended use case is that the main UI thread can call this
       any time, and then optionally pass the obtained pointer to a
       background worker, or release it after doing work immediately.

       if database is currently used by somebody else, opens a dialog
       box and gives user the opportunity to sqlite3_interrupt() the
       operation of the current owner, then tries again.

       \param user a string that identifies the new user, and which
       can be displayed in the dialog box.

       \param force_wait if set to true we won't ask the user to cancel
       the running query but just wait until it is done.

       \returns a unique_ptr containing the SQLite database handle, or
       nullptr in case no database is open.
    **/
    db_pointer_type get (const QString& user, bool force_wait = false);

    bool setSavepoint(const std::string& pointname = "RESTOREPOINT", bool unique = true);
    bool releaseSavepoint(const std::string& pointname = "RESTOREPOINT");
    bool revertToSavepoint(const std::string& pointname = "RESTOREPOINT");
    bool releaseAllSavepoints();
    bool revertAll();

    // Set a non-unique savepoint for the general undoing mechanism (undoing only last write).
    bool setUndoSavepoint() { return setSavepoint("UNDOPOINT", /* unique */ false); };
    bool revertToUndoSavepoint() { return revertToSavepoint("UNDOPOINT"); };

    bool dump(const QString& filename, const std::vector<std::string>& tablesToDump,
              bool insertColNames, bool insertNew, bool keepOriginal, bool exportSchema, bool exportData, bool keepOldSchema) const;

    enum ChoiceOnUse
    {
        Ask,
        Wait,
        CancelOther
    };
    // Callback to get results from executeSQL(). It is invoked for
    // each result row coming out of the evaluated SQL statements. If
    // a callback returns true (abort), the executeSQL() method
    // returns false (error) without invoking the callback again and
    // without running any subsequent SQL statements. The 1st argument
    // is the number of columns in the result. The 2nd argument to the
    // callback is the text representation of the values, one for each
    // column. The 3rd argument is a list of strings where each entry
    // represents the name of corresponding result column.
    using execCallback = std::function<bool(int, std::vector<QByteArray>, std::vector<QByteArray>)>;
    bool executeSQL(const std::string& statement, bool dirtyDB = true, bool logsql = true, execCallback callback = nullptr);
    bool executeMultiSQL(QByteArray query, bool dirty = true, bool log = false);
    QByteArray querySingleValueFromDb(const std::string& sql, bool log = true, ChoiceOnUse choice = Ask) const;

    const QString& lastError() const { return lastErrorMessage; }

    /**
     * @brief getRow Executes a sqlite statement to get the rowdata(columns)
     *        for the given rowid.
     * @param schemaName Name of the database schema.
     * @param sTableName Table to query.
     * @param rowid The rowid to fetch.
     * @param rowdata A list of QByteArray containing the row data.
     * @return true if statement execution was ok, else false.
     */
    bool getRow(const sqlb::ObjectIdentifier& table, const QString& rowid, std::vector<QByteArray>& rowdata) const;

    /**
     * @brief Interrupts the currently running statement as soon as possible.
     */
    void interruptQuery();

private:
    /**
     * @brief max Queries the table t for the max value of field.
     * @param tableName Table to query
     * @param field Name of the field to get the max value
     * @return the max value of the field or 0 on error
     */
    unsigned long max(const sqlb::ObjectIdentifier& tableName, const std::string& field) const;

    static int callbackWrapper (void* callback, int numberColumns, char** values, char** columnNames);

public:
    void updateSchema();    // Please don't call this from threads other than the main thread.

private:
    /**
     * @brief Creates an empty insert statement.
     * @param schemaName The name of the database schema in which to find the table
     * @param pk_value This optional parameter can be used to manually set a specific value for the primary key column
     * @return An sqlite conform INSERT INTO statement with empty values. (NULL,'',0)
     */
    std::string emptyInsertStmt(const std::string& schemaName, const sqlb::Table& t, const QString& pk_value = QString()) const;

public:
    QString addRecord(const sqlb::ObjectIdentifier& tablename);
    bool deleteRecords(const sqlb::ObjectIdentifier& table, const std::vector<QByteArray>& rowids, const sqlb::StringVector& pseudo_pk = {});
    bool updateRecord(const sqlb::ObjectIdentifier& table, const std::string& column, const QByteArray& rowid, const QByteArray& value, int force_type = 0, const sqlb::StringVector& pseudo_pk = {});

    bool createTable(const sqlb::ObjectIdentifier& name, const sqlb::FieldVector& structure);
    bool renameTable(const std::string& schema, const std::string& from_table, const std::string& to_table);
    bool addColumn(const sqlb::ObjectIdentifier& tablename, const sqlb::Field& field);

    /**
     * @brief This type maps from old column names to new column names. Given the old and the new table definition, this suffices to
     * track fields between the two.
     * USE CASES:
     * 1) Don't specify a column at all or specify equal column names: Keep its name as-is.
     * 2) Specify different column names: Rename the field.
     * 3) Map from an existing column name to a Null string: Delete the column.
     * 4) Map from a Null column name to a new column name: Add the column.
     */
    using AlterTableTrackColumns = std::map<QString, QString>;

    /**
     * @brief alterTable Can be used to rename, modify or drop existing columns of a given table
     * @param tablename Specifies the schema and name of the table to edit
     * @param new_table Specifies the new table schema. This is exactly how the new table is going to look like.
     * @param track_columns Maps old column names to new column names. This is used to copy the data from the old table to the new one.
     * @param newSchema Set this to a non-empty string to move the table to a new schema
     * @return true if renaming was successful, false if not. In the latter case also lastErrorMessage is set
     */
    bool alterTable(const sqlb::ObjectIdentifier& tablename, const sqlb::Table& new_table, AlterTableTrackColumns track_columns, std::string newSchemaName = std::string());

    const sqlb::TablePtr getTableByName(const sqlb::ObjectIdentifier& name) const
    {
        if(schemata.empty() || name.schema().empty())
            return sqlb::TablePtr{};
        const auto& schema = schemata.at(name.schema());
        if(schema.tables.count(name.name()))
            return schema.tables.at(name.name());
        return sqlb::TablePtr{};
    }

    const sqlb::IndexPtr getIndexByName(const sqlb::ObjectIdentifier& name) const
    {
        if(schemata.empty() || name.schema().empty())
            return sqlb::IndexPtr{};
        const auto& schema = schemata.at(name.schema());
        if(schema.indices.count(name.name()))
            return schema.indices.at(name.name());
        return sqlb::IndexPtr{};
    }

    const sqlb::TriggerPtr getTriggerByName(const sqlb::ObjectIdentifier& name) const
    {
        if(schemata.empty() || name.schema().empty())
            return sqlb::TriggerPtr{};
        const auto& schema = schemata.at(name.schema());
        if(schema.triggers.count(name.name()))
            return schema.triggers.at(name.name());
        return sqlb::TriggerPtr{};
    }

    bool isOpen() const;
    bool encrypted() const { return isEncrypted; }
    bool readOnly() const { return isReadOnly; }
    bool getDirty() const;
    QString currentFile() const { return curDBFilename; }

    /// log an SQL statement [thread-safe]
    void logSQL(const QString& statement, LogMessageType msgtype) const;

    QString getPragma(const std::string& pragma) const;
    bool setPragma(const std::string& pragma, const QString& value);
    bool setPragma(const std::string& pragma, const QString& value, QString& originalvalue);
    bool setPragma(const std::string& pragma, int value, int& originalvalue);

    bool loadExtension(const QString& filename);
    void loadExtensionsFromSettings();

    static QStringList Datatypes;
    static QStringList DatatypesStrict;

private:
    std::vector<std::pair<std::string, std::string> > queryColumnInformation(const std::string& schema_name, const std::string& object_name) const;

public:
    std::string generateSavepointName(const std::string& identifier = std::string()) const;

    // This function generates the name for a temporary table. It guarantees that there is no table with this name yet
    std::string generateTemporaryTableName(const std::string& schema) const;

    schemaMap schemata;

signals:
    void sqlExecuted(QString sql, int msgtype) const;
    void dbChanged(bool dirty);
    void structureUpdated();
    void requestCollation(QString name, int eTextRep);
    void databaseInUseChanged(bool busy, QString user);

private:
    /// external code needs to go through get() to obtain access to the database
    sqlite3 * _db;
    mutable std::mutex m;
    mutable std::condition_variable cv;
    bool db_used;
    QString db_user;

    /// wait for release of the DB locked through a previous get(),
    /// giving users the option to discard running task through a
    /// message box.
    void waitForDbRelease(ChoiceOnUse choice = Ask) const;

    QString curDBFilename;
    mutable QString lastErrorMessage;
    std::vector<std::string> savepointList;
    bool isEncrypted;
    bool isReadOnly;

    sqlb::StringVector primaryKeyForEditing(const sqlb::ObjectIdentifier& table, const sqlb::StringVector& pseudo_pk) const;

    // SQLite Callbacks
    void collationNeeded(void* pData, sqlite3* db, int eTextRep, const char* sCollationName);
    void errorLogCallback(void* user_data, int error_code, const char* message);

    bool tryEncryptionSettings(const QString& filename, bool* encrypted, CipherSettings* cipherSettings) const;

    bool disableStructureUpdateChecks;

    class NoStructureUpdateChecks
    {
    public:
        explicit NoStructureUpdateChecks(DBBrowserDB& db) : m_db(db) { m_db.disableStructureUpdateChecks = true; }
        ~NoStructureUpdateChecks() { m_db.disableStructureUpdateChecks = false; }

    private:
          DBBrowserDB& m_db;
    };
};

#endif
