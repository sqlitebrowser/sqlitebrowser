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

#include "sqlitedb.h"
#include <stdlib.h>
#include <qregexp.h>


void DBBrowserTable::addField(int order, const QString& wfield,const QString& wtype)
{
    fldmap[order] = DBBrowserField(wfield,wtype);
}

bool DBBrowserDB::open ( const QString & db)
{
  char *errmsg;
  bool ok=false;
    
  if (isOpen()) close();
  
  lastErrorMessage = QString("no error");

    _db = sqlite_open(db.latin1(), 0666, &errmsg);
    if( _db==0 ){
      _db = sqlite_open(db.latin1(), 0444, &errmsg);
      if( _db==0 ){
        if( errmsg ){
	  lastErrorMessage = QString(errmsg);
        }
        return false;
      }
  }

  if (_db){
    if (SQLITE_OK==sqlite_exec(_db,"PRAGMA empty_result_callbacks = ON;",
                               NULL,NULL,NULL)){
	if (SQLITE_OK==sqlite_exec(_db,"PRAGMA show_datatypes = ON;",
                               NULL,NULL,NULL)){
	    ok=true;
	}
	curDBFilename = db;
    }
  }
  return ok;
}

bool DBBrowserDB::create ( const QString & db)
{
  char *errmsg;
  bool ok=false;
    
  if (isOpen()) close();
  
  lastErrorMessage = QString("no error");

    _db = sqlite_open(db.latin1(), 0666, &errmsg);
    if( _db==0 ){
        if( errmsg ){
          	  lastErrorMessage = QString(errmsg);
        }
        return false;
      }

    if (_db){
    if (SQLITE_OK==sqlite_exec(_db,"PRAGMA empty_result_callbacks = ON;",
                               NULL,NULL,NULL)){
	if (SQLITE_OK==sqlite_exec(_db,"PRAGMA show_datatypes = ON;",
                               NULL,NULL,NULL)){
	    ok=true;
	}
	curDBFilename = db;
    }
}

  return ok;
}

void DBBrowserDB::close (){
    if (_db)
	sqlite_close(_db);
   _db = 0;
   idxmap.clear();
   tbmap.clear();
   idmap.clear();
   browseRecs.clear();
   browseFields.clear();
   hasValidBrowseSet = false;
}

bool DBBrowserDB::compact ( )
{
  char *errmsg;
  bool ok=false;
    
  if (!isOpen()) return false;

  if (_db){
    if (SQLITE_OK==sqlite_exec(_db,"VACUUM;",
                               NULL,NULL,&errmsg)){
	    ok=true;
	}
    }

  if (!ok){
    lastErrorMessage = QString(errmsg);
    return false;
  }else{
    return true;
  }
}


bool DBBrowserDB::executeSQL ( const QString & statement)
{
  char *errmsg;
  bool ok=false;
    
  if (!isOpen()) return false;

  if (_db){
    if (SQLITE_OK==sqlite_exec(_db,statement.latin1(),
                               NULL,NULL,&errmsg)){
	    ok=true;
	}
    }

  if (!ok){
    lastErrorMessage = QString(errmsg);
    return false;
  }else{
    return true;
  }
}


bool DBBrowserDB::addRecord ( )
{
    char *errmsg;
    if (!hasValidBrowseSet) return false;
    if (!isOpen()) return false;
    bool ok = false;
    QString statement = "INSERT INTO ";
    statement.append(curBrowseTableName);
    statement.append("(");
    statement.append(browseFields.first());
    statement.append(") VALUES(NULL);");
    lastErrorMessage = QString("no error");
    if (_db){
	if (SQLITE_OK==sqlite_exec(_db,statement.latin1(),NULL,NULL, &errmsg)){
	ok=true;
	int newrowid = sqlite_last_insert_rowid(_db);
	//append to recordlist
	QStringList newRec;
	QString strowid;
	strowid.setNum(newrowid);
	newRec << strowid;
	for (uint a=0; a< browseFields.count();a++){
	    newRec << "";
	}
	browseRecs.append(newRec);
	//insert rowid into idmap, mapping to record in browseFields (0 based)
	idmap.insert(newrowid,browseRecs.count()-1);
    } else {
	lastErrorMessage = QString(errmsg);
    }
}

    return ok;
}

bool DBBrowserDB::deleteRecord( int wrow)
{
    char * errmsg;
    if (!hasValidBrowseSet) return false;
    if (!isOpen()) return false;
    bool ok = false;    
    rowList tab = browseRecs;
    rowList::iterator rt = tab.at(wrow);
    QString rowid = (*rt).first();
    lastErrorMessage = QString("no error");
    
    QString statement = "DELETE FROM ";
    statement.append(curBrowseTableName.latin1());   
    statement.append(" WHERE rowid=");
    statement.append(rowid.latin1());
    statement.append(";");

    if (_db){
	if (SQLITE_OK==sqlite_exec(_db,statement.latin1(),
                               NULL,NULL,&errmsg)){
	ok=true;
    } else {
	lastErrorMessage = QString(errmsg);
    }	
}
    return ok;
}

bool DBBrowserDB::updateRecord(int wrow, int wcol, const QString & wtext)
{
    char * errmsg;
    if (!hasValidBrowseSet) return false;
    if (!isOpen()) return false;
    bool ok = false;
    
    lastErrorMessage = QString("no error");
    
    rowList::iterator rt = browseRecs.at(wrow);
    QString rowid = (*rt).first();
    QStringList::Iterator cv = (*rt).at(wcol+1);//must account for rowid
    //qDebug(*cv);
#ifdef ARCABUILD
    if((*cv).compare(binarySignature)==0)
	return false;
#endif
    
    QStringList::Iterator ct = browseFields.at(wcol);
    
    QString statement = "UPDATE ";
    statement.append(curBrowseTableName.latin1());
    statement.append(" SET ");
    statement.append(*ct);
    statement.append("=");

   char * formSQL = sqlite_mprintf("%Q",wtext.latin1());			
    statement.append(formSQL);
    if (formSQL) sqlite_freemem(formSQL);
    
    statement.append(" WHERE rowid=");
    statement.append(rowid.latin1());
    statement.append(";");

    if (_db){
	if (SQLITE_OK==sqlite_exec(_db,statement.latin1(),
                               NULL,NULL,&errmsg)){
	ok=true;
	/*update local copy*/
	(*cv) = wtext.latin1();
    } else {
	lastErrorMessage = QString(errmsg);
    }	
    }	
    
    /*rowIdMap::iterator mit = idmap.find(rowid.toInt());
    qDebug("column with rowid %s is %d",rowid.latin1(),*mit);*/
    return ok;
    
}

bool DBBrowserDB::browseTable( const QString & tablename )
{
    QStringList testFields = getTableFields( tablename );
    
    if (testFields.count()>0) {//table exists
	getTableRecords( tablename );
	browseFields = testFields;
	hasValidBrowseSet = true;
	curBrowseTableName = tablename;
    } else {
	hasValidBrowseSet = false;
	curBrowseTableName = QString(" ");
	browseFields.clear();
	browseRecs.clear();
	idmap.clear();
    }
    return hasValidBrowseSet;
}

void DBBrowserDB::getTableRecords( const QString & tablename )
{
   sqlite_vm *vm;
   const char *tail;
   const char **vals;
   const char **names;

   int ncol;
   QStringList r;
  // char *errmsg;
   int err=0;
  // int tabnum = 0; 
   browseRecs.clear();
   idmap.clear();
   lastErrorMessage = QString("no error");
   
	QString statement = "SELECT rowid, *  FROM ";
	statement.append( tablename.latin1());
	statement.append(" ORDER BY rowid; ");
	//qDebug(statement);
	err=sqlite_compile(_db,statement.latin1(),
                              &tail, &vm, NULL);
	if (err == SQLITE_OK){
	    int rownum = 0;
	  while ( sqlite_step(vm,&ncol,&vals, &names) == SQLITE_ROW ){
	      r.clear();
	      for (int e=0; e<ncol; e++){
#ifdef ARCABUILD
		  if (QString(vals[e]).startsWith(binaryPattern)){
		      r<< binarySignature;
		  } else {
		      r << vals[e];
		  }
#else
		  r << vals[e];
#endif
		  if (e==0){
		      idmap.insert(QString(vals[e]).toInt(),rownum);
		      rownum++;
		  }
	      }
	      browseRecs.append(r);
	  }

          sqlite_finalize(vm, NULL);
        }else{
          lastErrorMessage = QString ("could not get fields");
        }
}

resultMap DBBrowserDB::getFindResults( const QString & wstatement)
{
   sqlite_vm *vm;
   const char *tail;
   const char **vals;
   const char **names;

   int ncol;
 
   char *errmsg;
   int err=0;
  resultMap res;
  lastErrorMessage = QString("no error");
   
	err=sqlite_compile(_db,wstatement.latin1(),
                              &tail, &vm, &errmsg);
	if (err == SQLITE_OK){
	    int rownum = 0;
	    int recnum = 0;
	    QString r;
	  while ( sqlite_step(vm,&ncol,&vals, &names) == SQLITE_ROW ){
	      for (int e=0; e<ncol; e++){
#ifdef ARCABUILD
		  if (QString(vals[e]).startsWith(binaryPattern)){
		      r = binarySignature;
		  } else {
		      r = vals[e];
		  }
#else
		  r = vals[e];
#endif
		  if (e==0){
		      rownum = QString(vals[e]).toInt();
		      rowIdMap::iterator mit = idmap.find(rownum);
		      recnum = *mit;
		  }
	      }
	        res.insert(recnum, r);
	  }

          sqlite_finalize(vm, NULL);
        }else{
          lastErrorMessage = QString(errmsg);
        }
	return res;
}


QStringList DBBrowserDB::getTableNames()
{
    tableMap::Iterator it;
    tableMap tmap = tbmap;
    QStringList res;

    for ( it = tmap.begin(); it != tmap.end(); ++it ) {
	res.append( it.data().getname() );
    }
    
   return res;
}

QStringList DBBrowserDB::getIndexNames()
{
    indexMap::Iterator it;
    indexMap tmap = idxmap;
    QStringList res;

    for ( it = tmap.begin(); it != tmap.end(); ++it ) {
	res.append( it.data().getname() );
    }
    
   return res;
}

QStringList DBBrowserDB::getTableFields(const QString & tablename)
{
    tableMap::Iterator it;
    tableMap tmap = tbmap;
    QStringList res;

        for ( it = tmap.begin(); it != tmap.end(); ++it ) {
	    if (tablename.compare(it.data().getname())==0 ){
	    fieldMap::Iterator fit;
	    fieldMap fmap = it.data().fldmap;

	    for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
		 res.append( fit.data().getname() );
	    }
	}
	}
    return res;
}

int DBBrowserDB::getRecordCount()
{
    return browseRecs.count();
}
void DBBrowserDB::updateSchema( )
{
  // qDebug ("Getting list of tables");
   sqlite_vm *vm;
   const char *tail;
   const char **vals;
   const char **names;

   int ncol;
   QStringList r;
   char *errmsg;
   int err=0;
   QString num;
   int idxnum =0;
   int tabnum = 0;
   
   idxmap.clear();
   tbmap.clear();
   
   lastErrorMessage = QString("no error");

   err=sqlite_compile(_db,"SELECT name, sql "
                              "FROM sqlite_master "
                              "WHERE type='table' ;"
                              /*"ORDER BY name;"*/ ,
                              &tail, &vm, &errmsg);
        if (err == SQLITE_OK){
	    
          while ( sqlite_step(vm,&ncol,&vals, &names) == SQLITE_ROW ){
	      num.setNum(tabnum);
	      tbmap[num] = DBBrowserTable(vals[0],vals[1]);
	      tabnum++;
          }
          sqlite_finalize(vm, NULL);
        }else{
          qDebug ("could not get list of tables: %d, %s",err,errmsg);
        }
	
	//now get the field list for each table in tbmap
       tableMap::Iterator it;
        for ( it = tbmap.begin(); it != tbmap.end(); ++it ) {
	QString statement = "SELECT *  FROM ";
	statement.append( (const char *) it.data().getname().latin1());
	statement.append(" LIMIT 1;");
	err=sqlite_compile(_db,statement.latin1(),
                              &tail, &vm, NULL);
        if (err == SQLITE_OK){
          err = sqlite_step(vm,&ncol,&vals, &names);
	  if ((err==SQLITE_ROW)||(err==SQLITE_DONE)){
	      it.data(). fldmap.clear();
	      for (int e=0; e<ncol; e++)
		  it.data().addField(e,names[e],names[ncol+e]);
	  }

          sqlite_finalize(vm, NULL);
        }else{
          lastErrorMessage = QString ("could not get types");
        }
    }
	
	//finally get indices
	err=sqlite_compile(_db,"SELECT name, sql "
                              "FROM sqlite_master "
                              "WHERE type='index' "
                              /*"ORDER BY name;"*/,
                              &tail, &vm, &errmsg);
	if (err == SQLITE_OK){
	    while ( sqlite_step(vm,&ncol,&vals, &names) == SQLITE_ROW ){
		num.setNum(idxnum);
		idxmap[num] = DBBrowserIndex(vals[0],vals[1]);
		idxnum ++;
          }
          sqlite_finalize(vm, NULL);
        }else{
          lastErrorMessage = QString ("could not get list of indices");
        }
}


