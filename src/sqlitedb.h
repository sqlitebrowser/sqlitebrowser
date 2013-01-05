#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <QStringList>
#include <QMap>
#include <QMultiMap>
#include <sqlite3.h>

class SQLLogDock;

enum
{
    kLogMsg_User,
    kLogMsg_App
};

enum
{
    kEncodingUTF8,
    kEncodingLatin1,
    kEncodingNONE
};

/*types for encoded media data*/
enum
{
    kSQLiteMediaType_Void,
    kSQLiteMediaType_Integer,
    kSQLiteMediaType_String,
    kSQLiteMediaType_Binary
};

static QString g_sApplicationNameShort = QString("sqlitebrowser");
static QString g_applicationIconName = QString(":/oldimages/icon16");

typedef QMap<int, class DBBrowserField> fieldMap;
typedef QMultiMap<QString, class DBBrowserObject> objectMap;
typedef QMap<int, int> rowIdMap;

typedef QList<QStringList> rowList;
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
    fieldMap fldmap;
private:
    QString name;
    QString sql;
    QString type;
};


class DBBrowserDB
{
public:
    DBBrowserDB (): _db( 0 ) , hasValidBrowseSet(false), curEncoding(kEncodingUTF8) {}
    ~DBBrowserDB (){}
    bool open ( const QString & db);
    bool create ( const QString & db);
    void close ();
    bool compact ();
    bool setRestorePoint();
    bool save ();
    bool revert ();
    bool dump( const QString & filename);
    bool reload( const QString & filename, int * lineErr);
    bool executeSQL ( const QString & statement, bool dirtyDB=true, bool logsql=true);
    void updateSchema() ;
    bool addRecord();
    bool deleteRecord(int wrow);
    bool updateRecord(int wrow, int wcol, const QString & wtext);
    bool browseTable( const QString & tablename, const QString& orderby = "rowid" );

    bool renameTable(QString from_table, QString to_table);
    bool createColumn(QString table, QString field, QString type);
    bool renameColumn(QString tablename, QString from, QString to, QString type);

    QStringList getTableFields(const QString & tablename);
    QStringList getTableTypes(const QString & tablename);
    QStringList getBrowsableObjectNames();
    objectMap getBrowsableObjects();
    DBBrowserObject getObjectByName(const QString& name);
    QStringList getIndexNames();
    resultMap getFindResults( const QString & wstatement);
    int getRecordCount();
    bool isOpen();
    void setDirty(bool dirtyval);
    void setDirtyDirect(bool dirtyval);
    bool getDirty();
    void logSQL(QString statement, int msgtype);
    void setEncoding( int encoding );
    void setDefaultNewData( const QString & data );
    char * GetEncodedQStringAsPointer( const QString & input);
    QString GetEncodedQString( const QString & input);
    QString GetDecodedQString( const QString & input);
    QString getPragma(QString pragma);
    bool setPragma(QString pragma, QString value);
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
    int curEncoding;
    QString curNewData;

    SQLLogDock * logWin;


private:
    bool dirty;
    void getTableRecords( const QString & tablename, const QString& orderby = "rowid" );


};

#endif
