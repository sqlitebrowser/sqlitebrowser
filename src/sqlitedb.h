#ifndef SQLITEDB_H
#define SQLITEDB_H

#include "sqlitetypes.h"

#include <QStringList>
#include <QMap>
#include <QMultiMap>
#include <sqlite3.h>

class MainWindow;

enum
{
    kLogMsg_User,
    kLogMsg_App
};

typedef QMultiMap<QString, class DBBrowserObject> objectMap;

class DBBrowserObject
{
public:
    DBBrowserObject() : name( "" ) { }
    DBBrowserObject( const QString& wname,const QString& wsql, const QString& wtype, const QString& tbl_name )
        : name( wname), sql( wsql ), type(wtype), table_name(tbl_name)
    { }

    void addField(sqlb::FieldPtr field) { fldmap.push_back(field); }

    QString getname() const { return name; }
    QString getsql() const { return sql; }
    QString gettype() const { return type; }
    QString getTableName() const { return table_name; }
    sqlb::FieldPtr getField(const QString& name) const
    {
        for(int i=0;i<fldmap.size();i++)
            if(fldmap.at(i)->name() == name)
                return fldmap.at(i);
        return sqlb::FieldPtr();
    }
    sqlb::FieldVector fldmap;
private:
    QString name;
    QString sql;
    QString type;
    QString table_name;     // The name of the table this object references, interesting for views, triggers and indices
};


class DBBrowserDB
{
public:
    DBBrowserDB (): _db( 0 ), mainWindow(0) {}
    ~DBBrowserDB (){}
    bool open ( const QString & db);
    bool create ( const QString & db);
    void close ();
    bool setRestorePoint(const QString& pointname = "RESTOREPOINT");
    bool save (const QString& pointname = "RESTOREPOINT");
    bool revert (const QString& pointname = "RESTOREPOINT");
    bool saveAll();
    bool revertAll();
    bool dump( const QString & filename);
    bool executeSQL ( const QString & statement, bool dirtyDB=true, bool logsql=true);
    bool executeMultiSQL(const QString& statement, bool dirty = true, bool log = false);

    /**
     * @brief getTableSQL Returns the create table statement for the given table.
     * @param sTable Table name
     * @return An empty string if the table does not exist
     *         or the create table statement.
     */
    QString getTableSQL(const QString& sTable);
    void updateSchema();
    int addRecord(const QString& sTableName);
    bool deleteRecord(const QString& table, int rowid);
    bool updateRecord(const QString& table, const QString& column, int row, const QByteArray& value);

    bool createTable(const QString& name, const sqlb::FieldVector& structure);
    bool renameTable(const QString& from_table, const QString& to_table);
    bool addColumn(const QString& table, const sqlb::FieldPtr& field);
    bool renameColumn(const QString& tablename, const QString& name, sqlb::FieldPtr to);
    bool dropColumn(const QString& tablename, const QString& column);

    QStringList getTableFields(const QString & tablename) const;
    QStringList getBrowsableObjectNames() const;
    objectMap getBrowsableObjects() const;
    DBBrowserObject getObjectByName(const QString& name) const;
    bool isOpen() const;
    bool getDirty() const;
    void logSQL(const QString& statement, int msgtype);

    QString getPragma(const QString& pragma);
    bool setPragma(const QString& pragma, const QString& value);
    bool setPragma(const QString& pragma, const QString& value, QString& originalvalue);
    bool setPragma(const QString& pragma, int value, int& originalvalue);

    bool loadExtension(const QString& filename);

    sqlite3 * _db;

    QStringList decodeCSV(const QString & csvfilename, char sep, char quote,  int maxrecords, int * numfields);

    objectMap objMap;

    QString lastErrorMessage;
    QString curDBFilename;

    MainWindow* mainWindow;

private:
    QStringList savepointList;
};

#endif
