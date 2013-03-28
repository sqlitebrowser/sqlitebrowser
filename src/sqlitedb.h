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

/*types for encoded media data*/
enum
{
    kSQLiteMediaType_Void,
    kSQLiteMediaType_Integer,
    kSQLiteMediaType_String,
    kSQLiteMediaType_Binary
};

typedef QMap<int, class DBBrowserField> fieldMap;
typedef QMultiMap<QString, class DBBrowserObject> objectMap;
typedef QMap<int, int> rowIdMap;

typedef QList<QList<QByteArray> > rowList;
typedef QMap<int, QString> resultMap;

class DBBrowserField
{
public:
    DBBrowserField() : name( "" ) { }
    DBBrowserField( const QString& wname,const QString& wtype )
        : name( wname), type( wtype )
    { }
    QString getname() const { return name; }
    QString gettype() const { return type; }
private:
    QString name;
    QString type;
};

class DBBrowserObject
{
public:
    DBBrowserObject() : name( "" ) { }
    DBBrowserObject( const QString& wname,const QString& wsql, const QString& wtype )
        : name( wname), sql( wsql ), type(wtype)
    { }

    void addField(int order, const QString& wfield,const QString& wtype);

    QString getname() const { return name; }
    QString getsql() const { return sql; }
    QString gettype() const { return type; }
    DBBrowserField getField(const QString& name) const
    {
        for(fieldMap::ConstIterator i=fldmap.begin();i!=fldmap.end();++i)
            if(i.value().getname() == name)
                return *i;
        return DBBrowserField();
    }
    fieldMap fldmap;
private:
    QString name;
    QString sql;
    QString type;
};


class DBBrowserDB
{
public:
    DBBrowserDB (): _db( 0 ) , hasValidBrowseSet(false), mainWindow(0) {}
    ~DBBrowserDB (){}
    bool open ( const QString & db);
    bool create ( const QString & db);
    void close ();
    bool compact ();
    bool setRestorePoint(const QString& pointname = "RESTOREPOINT");
    bool save (const QString& pointname = "RESTOREPOINT");
    bool revert (const QString& pointname = "RESTOREPOINT");
    bool dump( const QString & filename);
    bool reload( const QString & filename, int * lineErr);
    bool executeSQL ( const QString & statement, bool dirtyDB=true, bool logsql=true);
    bool executeMultiSQL(const QString& statement, bool dirty = true, bool log = false);

    /**
     * @brief getTableSQL Returns the create table statement for the given table.
     * @param sTable Table name
     * @return An empty string if the table does not exist
     *         or the create table statement.
     */
    QString getTableSQL(const QString& sTable);
    void updateSchema() ;
    bool addRecord(const QString& sTableName);
    bool deleteRecord(int wrow);
    bool updateRecord(int wrow, int wcol, const QByteArray& wtext);
    bool browseTable( const QString & tablename, const QString& orderby = "rowid" );

    bool createTable(const QString& name, const QList<DBBrowserField>& structure);
    bool renameTable(const QString& from_table, const QString& to_table);
    bool addColumn(const QString& table, const sqlb::FieldPtr& field);
    bool renameColumn(const QString& tablename, const QString& from, const QString& to, const QString& type);
    bool dropColumn(const QString& tablename, const QString& column);

    QStringList getTableFields(const QString & tablename) const;
    QStringList getTableTypes(const QString & tablename) const;
    QStringList getBrowsableObjectNames() const;
    objectMap getBrowsableObjects() const;
    DBBrowserObject getObjectByName(const QString& name) const;
    QStringList getIndexNames() const;
    resultMap getFindResults( const QString & wstatement);
    int getRecordCount() const;
    bool isOpen() const;
    void setDirty(bool dirtyval);
    bool getDirty() const;
    void logSQL(const QString& statement, int msgtype);

    QString getPragma(const QString& pragma);
    bool setPragma(const QString& pragma, const QString& value);
    bool setPragma(const QString& pragma, const QString& value, QString& originalvalue);
    bool setPragma(const QString& pragma, int value, int& originalvalue);
    sqlite3 * _db;


    QStringList decodeCSV(const QString & csvfilename, char sep, char quote,  int maxrecords, int * numfields);

    objectMap objMap;
    rowIdMap idmap;

    rowList browseRecs;
    QStringList browseFields;
    bool hasValidBrowseSet;
    QString curBrowseTableName;
    QString lastErrorMessage;
    QString curDBFilename;

    MainWindow* mainWindow;


private:
    bool dirty;
    void getTableRecords( const QString & tablename, const QString& orderby = "rowid" );


};

#endif
