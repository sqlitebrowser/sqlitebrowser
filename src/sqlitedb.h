#ifndef SQLITEDB_H
#define SQLITEDB_H

#include "sqlitetypes.h"

#include <QStringList>
#include <QMultiMap>
#include <QByteArray>

struct sqlite3;
class CipherDialog;

enum
{
    kLogMsg_User,
    kLogMsg_App
};

typedef QMultiMap<QString, sqlb::ObjectPtr> objectMap;      // Maps from object type (table, index, view, trigger) to a pointer to the object representation
typedef QMap<QString, objectMap> schemaMap;                 // Maps from the schema name (main, temp, attached schemas) to the object map for that schema

int collCompare(void* pArg, int eTextRepA, const void* sA, int eTextRepB, const void* sB);

class DBBrowserDB : public QObject
{
    Q_OBJECT

public:
    explicit DBBrowserDB () : _db(0), isEncrypted(false), isReadOnly(false), dontCheckForStructureUpdates(false) {}
    virtual ~DBBrowserDB (){}
    bool open(const QString& db, bool readOnly = false);
    bool attach(const QString& filename, QString attach_as = "");
    bool create ( const QString & db);
    bool close();
    bool setSavepoint(const QString& pointname = "RESTOREPOINT");
    bool releaseSavepoint(const QString& pointname = "RESTOREPOINT");
    bool revertToSavepoint(const QString& pointname = "RESTOREPOINT");
    bool releaseAllSavepoints();
    bool revertAll();
    bool dump(const QString & filename, const QStringList &tablesToDump, bool insertColNames, bool insertNew, bool exportSchema, bool exportData, bool keepOldSchema);
    bool executeSQL(QString statement, bool dirtyDB = true, bool logsql = true);
    bool executeMultiSQL(const QString& statement, bool dirty = true, bool log = false);
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
    bool getRow(const sqlb::ObjectIdentifier& table, const QString& rowid, QVector<QByteArray>& rowdata);

    /**
     * @brief max Queries the table t for the max value of field.
     * @param tableName Table to query
     * @param field Field to get the max value
     * @return the max value of the field or 0 on error
     */
    QString max(const sqlb::ObjectIdentifier& tableName, sqlb::FieldPtr field) const;

    void updateSchema();
    QString addRecord(const sqlb::ObjectIdentifier& tablename);

    /**
     * @brief Creates an empty insert statement.
     * @param schemaName The name of the database schema in which to find the table
     * @param pk_value This optional parameter can be used to manually set a specific value for the primary key column
     * @return An sqlite conform INSERT INTO statement with empty values. (NULL,'',0)
     */
    QString emptyInsertStmt(const QString& schemaName, const sqlb::Table& t, const QString& pk_value = QString()) const;
    bool deleteRecords(const sqlb::ObjectIdentifier& table, const QStringList& rowids);
    bool updateRecord(const sqlb::ObjectIdentifier& table, const QString& column, const QString& rowid, const QByteArray& value, bool itsBlob, const QString& pseudo_pk = QString());

    bool createTable(const sqlb::ObjectIdentifier& name, const sqlb::FieldVector& structure);
    bool renameTable(const QString& schema, const QString& from_table, const QString& to_table);
    bool addColumn(const sqlb::ObjectIdentifier& tablename, const sqlb::FieldPtr& field);

    /**
     * @brief renameColumn Can be used to rename, modify or drop an existing column of a given table
     * @param schema Specifies the name of the schema, i.e. the database name, of the table
     * @param tablename Specifies the name of the table to edit
     * @param table Specifies the table to edit. The table constraints are used from this but not the columns
     * @param name Name of the column to edit
     * @param to The new field definition with changed name, type or the like. If Null-Pointer is given the column is dropped.
     * @param move Set this to a value != 0 to move the new column to a different position
     * @param newSchema Set this to a non-empty string to move the table to a new schema
     * @return true if renaming was successful, false if not. In the latter case also lastErrorMessage is set
     */
    bool renameColumn(const sqlb::ObjectIdentifier& tablename, const sqlb::Table& table, const QString& name, sqlb::FieldPtr to, int move = 0, QString newSchemaName = QString());

    objectMap getBrowsableObjects(const QString& schema) const;
    const sqlb::ObjectPtr getObjectByName(const sqlb::ObjectIdentifier& name) const;
    bool isOpen() const;
    bool encrypted() const { return isEncrypted; }
    bool readOnly() const { return isReadOnly; }
    bool getDirty() const;
    QString currentFile() const { return curDBFilename; }
    void logSQL(QString statement, int msgtype);

    QString getPragma(const QString& pragma);
    bool setPragma(const QString& pragma, const QString& value);
    bool setPragma(const QString& pragma, const QString& value, QString& originalvalue);
    bool setPragma(const QString& pragma, int value, int& originalvalue);

    bool loadExtension(const QString& filename);

    QVector<QPair<QString, QString>> queryColumnInformation(const QString& schema_name, const QString& object_name);

    QString generateSavepointName(const QString& identifier = QString()) const;

    sqlite3 * _db;

    schemaMap schemata;

signals:
    void sqlExecuted(QString sql, int msgtype);
    void dbChanged(bool dirty);
    void structureUpdated();
    void requestCollation(QString name, int eTextRep);

private:
    QString curDBFilename;
    QString lastErrorMessage;
    QStringList savepointList;
    bool isEncrypted;
    bool isReadOnly;

    void collationNeeded(void* pData, sqlite3* db, int eTextRep, const char* sCollationName);

    bool tryEncryptionSettings(const QString& filename, bool* encrypted, CipherDialog*& cipherSettings);

    bool dontCheckForStructureUpdates;

    class NoStructureUpdateChecks
    {
    public:
        NoStructureUpdateChecks(DBBrowserDB& db) : m_db(db) { m_db.dontCheckForStructureUpdates = true; }
        ~NoStructureUpdateChecks() { m_db.dontCheckForStructureUpdates = false; }

    private:
          DBBrowserDB& m_db;
    };
};

#endif
