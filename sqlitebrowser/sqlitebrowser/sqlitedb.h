#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <stdlib.h>
#include <qstringlist.h>
#include <qmap.h>
#include <q3valuelist.h>
#include <qobject.h>
#include "sqllogform.h"
#include "sqlite3.h"
#include "sqlitebrowsertypes.h"

#define MAX_DISPLAY_LENGTH 255

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

static QString applicationName = QString("SQLite Database Browser");
static QString applicationIconName = QString("icone16.png");
static QString aboutText = QString("Version 2.0\n\nSQLite Database Browser is a freeware, public domain, open source visual tool used to create, design and edit database files compatible with SQLite 3.x.\n\nIt has been developed originally by Mauricio Piacentini from Tabuleiro Producoes. \n\nIn the spirit of the original SQLite source code, the author disclaims copyright to this source code.");


typedef QMap<int, class DBBrowserField> fieldMap;
typedef QMap<QString, class DBBrowserTable> tableMap;
typedef QMap<QString, class DBBrowserIndex> indexMap;
typedef QMap<int, int> rowIdMap;

typedef QList<QStringList> rowList;
typedef QMap<int, QString> resultMap;

class DBBrowserField
    {    
    public:
        DBBrowserField() : name( 0 ) { }
        DBBrowserField( const QString& wname,const QString& wtype )
            : name( wname), type( wtype )
        { }
        QString getname() const { return name; }
        QString gettype() const { return type; }
 private:
        QString name;
        QString type;
 };

class DBBrowserIndex
    {    
    public:
        DBBrowserIndex() : name( 0 ) { }
       DBBrowserIndex( const QString& wname,const QString& wsql )
            : name( wname), sql( wsql )
        { }
        QString getname() const { return name; }
        QString getsql() const { return sql; }
private:
        QString name;
        QString sql;
 };


class DBBrowserTable
    {    
    public:
        DBBrowserTable() : name( 0 ) { }
        DBBrowserTable( const QString& wname,const QString& wsql )
            : name( wname), sql( wsql )
        { }

        void addField(int order, const QString& wfield,const QString& wtype);

        QString getname() const { return name; }
        QString getsql() const { return sql; }
        fieldMap fldmap;
private:
        QString name;
        QString sql;
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
 bool executeSQL ( const QString & statement);
 bool executeSQLDirect ( const QString & statement);
 void updateSchema() ;
 bool addRecord();
 bool deleteRecord(int wrow);
 bool updateRecord(int wrow, int wcol, const QString & wtext);
 bool browseTable( const QString & tablename );
 QStringList getTableFields(const QString & tablename);
 QStringList getTableTypes(const QString & tablename);
 QStringList getTableNames();
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
 QString GetEncodedQString( const QString & input);
 QString GetDecodedQString( const QString & input);
 sqlite3 * _db;


	QStringList decodeCSV(const QString & csvfilename, char sep, char quote,  int maxrecords, int * numfields);

	tableMap tbmap;
	indexMap idxmap;
	rowIdMap idmap;
	
	rowList browseRecs;
	QStringList browseFields;
	bool hasValidBrowseSet;
	QString curBrowseTableName;
	QString lastErrorMessage;
	QString curDBFilename;
 int curEncoding;
 QString curNewData;
	
	sqlLogForm * logWin;
	

	private:
	    bool dirty;
	void getTableRecords( const QString & tablename );
	
	
};

#endif
