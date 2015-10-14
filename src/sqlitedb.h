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

typedef QMultiMap<QString, class DBBrowserObject> objectMap;

class DBBrowserObject
{
public:
    DBBrowserObject() : table(""), name( "" ) { }
    DBBrowserObject( const QString& wname,const QString& wsql, const QString& wtype, const QString& tbl_name )
        : table(wname), name( wname), sql( wsql ), type(wtype), table_name(tbl_name)
    { }

    QString getname() const { return name; }
    QString getsql() const { return sql; }
    QString gettype() const { return type; }
    QString getTableName() const { return table_name; }
    sqlb::Table table;
private:
    QString name;
    QString sql;
    QString type;
    QString table_name;     // The name of the table this object references, interesting for views, triggers and indices
};

class DBBrowserDB : public QObject
{
    Q_OBJECT

public:
    explicit DBBrowserDB () : _db(0), isEncrypted(false), isReadOnly(false) {}
    virtual ~DBBrowserDB (){}
    bool open ( const QString & db);
    bool attach(const QString& filename, QString attach_as = "");
    bool create ( const QString & db);
    bool close();
    bool setSavepoint(const QString& pointname = "RESTOREPOINT");
    bool releaseSavepoint(const QString& pointname = "RESTOREPOINT");
    bool revertToSavepoint(const QString& pointname = "RESTOREPOINT");
    bool releaseAllSavepoints();
    bool revertAll();
    bool dump(const QString & filename, const QStringList &tablesToDump, bool insertColNames, bool insertNew, bool exportSchemaOnly);
    bool executeSQL ( const QString & statement, bool dirtyDB=true, bool logsql=true);
    bool executeMultiSQL(const QString& statement, bool dirty = true, bool log = false);

    /**
     * @brief getRow Executes a sqlite statement to get the rowdata(columns)
     *        for the given rowid.
     * @param sTableName Table to query.
     * @param rowid The rowid to fetch.
     * @param rowdata A list of QByteArray containing the row data.
     * @return true if statement execution was ok, else false.
     */
    bool getRow(const QString& sTableName, const QString& rowid, QList<QByteArray>& rowdata);

    /**
     * @brief max Queries the table t for the max value of field.
     * @param t Table to query
     * @param field Field to get the max value
     * @return the max value of the field or 0 on error
     */
    QString max(const sqlb::Table& t, sqlb::FieldPtr field) const;

    void updateSchema();
    QString addRecord(const QString& sTableName);

    /**
     * @brief Creates an empty insert statement.
     * @param pk_value This optional parameter can be used to manually set a specific value for the primary key column
     * @return An sqlite conform INSERT INTO statement with empty values. (NULL,'',0)
     */
    QString emptyInsertStmt(const sqlb::Table& t, const QString& pk_value = QString()) const;
    bool deleteRecord(const QString& table, const QString& rowid);
    bool updateRecord(const QString& table, const QString& column, const QString& rowid, const QByteArray& value, bool itsBlob);

    bool createTable(const QString& name, const sqlb::FieldVector& structure);
    bool renameTable(const QString& from_table, const QString& to_table);
    bool addColumn(const QString& table, const sqlb::FieldPtr& field);

    /**
     * @brief renameColumn Can be used to rename, modify or drop an existing column of a given table
     * @param tablename Specifies the table name
     * @param name Name of the column to edit
     * @param to The new field definition with changed name, type or the like. If Null-Pointer is given the column is dropped.
     * @param move Set this to a value != 0 to move the new column to a different position
     * @return true if renaming was successful, false if not. In the latter case also lastErrorMessage is set
     */
    bool renameColumn(const QString& tablename, const QString& name, sqlb::FieldPtr to, int move = 0);

    objectMap getBrowsableObjects() const;
    DBBrowserObject getObjectByName(const QString& name) const;
    bool isOpen() const;
    bool encrypted() const { return isEncrypted; }
    bool readOnly() const { return isReadOnly; }
    bool getDirty() const;
    void logSQL(QString statement, int msgtype);

    QString getPragma(const QString& pragma);
    bool setPragma(const QString& pragma, const QString& value);
    bool setPragma(const QString& pragma, const QString& value, QString& originalvalue);
    bool setPragma(const QString& pragma, int value, int& originalvalue);

    bool loadExtension(const QString& filename);

    sqlite3 * _db;

    objectMap objMap;

    QString lastErrorMessage;
    QString curDBFilename;

signals:
    void sqlExecuted(QString sql, int msgtype);
    void dbChanged(bool dirty);

private:
    QStringList savepointList;

    bool isEncrypted;
    bool isReadOnly;

    bool tryEncryptionSettings(const QString& filename, bool* encrypted, CipherDialog*& cipherSettings);
};

#endif
