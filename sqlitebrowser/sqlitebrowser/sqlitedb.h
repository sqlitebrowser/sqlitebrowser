/*
** This file is part of SQLite Database Browser
** http://sqlitebrowser.sourceforge.net
**
** Originally developed by Mauricio Piacentini, Tabuleiro
**
** The author disclaims copyright to this source code.  
** Consult the LICENSING file for known restrictions
**
*/


#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <stdlib.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qvaluelist.h>
#include "sqlite_source/sqlite.h"

static QString applicationName = QString("SQLite Database Browser");
static QString applicationIconName = QString("icone16.png");
static QString aboutText = QString("Version 1.01\n\nSQLite Database Browser is a freeware, public domain, open source visual tool used to create, design and edit database files compatible with SQLite 2.x.\n\nIt has been developed originally by Mauricio Piacentini from Tabuleiro Producoes, as the Arca Database Browser. The original code is used with the Arca Database Xtra, a commercial product that uses SQLite databases with some additional extensions for handling of compressed and binary data. \n\n A generic version of the code was also produced to be compatible with standard SQLite 2.1 databases, and was released into Public Domain by the original author. \n\n You can find the latest versions of the source code of SQLite Database browser at http://sqlitebrowser.sourceforge.net. \n\nIn the spirit of the original SQLite source code, the author disclaims copyright to this source code.");


typedef QMap<int, class DBBrowserField> fieldMap;
typedef QMap<QString, class DBBrowserTable> tableMap;
typedef QMap<QString, class DBBrowserIndex> indexMap;
typedef QMap<int, int> rowIdMap;

typedef QValueList<QStringList> rowList;
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
	DBBrowserDB (): _db( 0 ) , hasValidBrowseSet(false) {}
	~DBBrowserDB (){}
	bool open ( const QString & db);
	bool create ( const QString & db);
	void close ();
	bool compact ();
	bool executeSQL ( const QString & statement);
	void updateSchema() ;
	bool addRecord();
	bool deleteRecord(int wrow);
	bool updateRecord(int wrow, int wcol, const QString & wtext);
	bool browseTable( const QString & tablename );
	QStringList getTableFields(const QString & tablename);
	QStringList getTableNames();
	QStringList getIndexNames();
	resultMap getFindResults( const QString & wstatement);
	int getRecordCount();
	bool isOpen() const { return _db!=0; }
	sqlite * _db;
	
	tableMap tbmap;
	indexMap idxmap;
	rowIdMap idmap;
	
	rowList browseRecs;
	QStringList browseFields;
	bool hasValidBrowseSet;
	QString curBrowseTableName;
	QString lastErrorMessage;
	QString curDBFilename;
	private:
	void getTableRecords( const QString & tablename );
};

#endif
