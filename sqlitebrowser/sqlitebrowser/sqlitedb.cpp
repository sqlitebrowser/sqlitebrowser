#include "sqlitedb.h"
#include "sqlbrowser_util.h"
#include <stdlib.h>
#include <qregexp.h>
#include <qimage.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

namespace {
	uint utf16len(const ushort* utf16)
	{
		uint	len=0;
		if ( utf16 )
			while ( *utf16++ ) ++len;
		return len;
	}

	QString StringFromUTF16(const void* utf16)
	{
		QString	result;
		result.setUnicodeCodes(static_cast<const ushort*>(utf16), utf16len(static_cast<const ushort*>(utf16)));
		return result;
	}

}

void DBBrowserTable::addField(int order, const QString& wfield,const QString& wtype)
{
    fldmap[order] = DBBrowserField(wfield,wtype);
}

bool DBBrowserDB::isOpen ( )
{
    return _db!=0; 
}

void DBBrowserDB::setDirty(bool dirtyval)
{
    if ((dirty==false)&&(dirtyval==true))
    {
	setRestorePoint();
    }
    dirty = dirtyval;
    if (logWin)
    {	
	logWin->msgDBDirtyState(dirty);
    }
}

void DBBrowserDB::setDirtyDirect(bool dirtyval)
{
    dirty = dirtyval;
    if (logWin)
    {	
	logWin->msgDBDirtyState(dirty);
    }
}
	
bool DBBrowserDB::getDirty()
{
    return dirty;
}

bool DBBrowserDB::open ( const QString & db)
{
  bool ok=false;
  int  err;
  
  if (isOpen()) close();
  
  lastErrorMessage = QString("no error");

  err = sqlite3_open16(db.ucs2(), &_db);
  if ( err ) {
    lastErrorMessage = StringFromUTF16(sqlite3_errmsg16(_db));
    sqlite3_close(_db);
    _db = 0;
    return false;
  }

  if (_db){
    if (SQLITE_OK==sqlite3_exec(_db,"PRAGMA empty_result_callbacks = ON;",
                               NULL,NULL,NULL)){
	if (SQLITE_OK==sqlite3_exec(_db,"PRAGMA show_datatypes = ON;",
                               NULL,NULL,NULL)){
	    ok=true;
	    setDirty(false);
	}
	curDBFilename = db;
    }
  }
  return ok;
}

bool DBBrowserDB::setRestorePoint()
{
  if (!isOpen()) return false;

  if (_db){
    if ( SQLITE_OK != sqlite3_exec(_db,"BEGIN TRANSACTION RESTOREPOINT;",
												NULL,NULL,NULL) ) {
		lastErrorMessage = StringFromUTF16(sqlite3_errmsg16(_db));
		return false;
	}
    setDirty(false);
}
  return true;
}

bool DBBrowserDB::save()
{
  if (!isOpen()) return false;

  if (_db){
	  if ( SQLITE_OK != sqlite3_exec(_db,"COMMIT TRANSACTION RESTOREPOINT;",
											NULL,NULL,NULL) ) {
		lastErrorMessage = StringFromUTF16(sqlite3_errmsg16(_db));
		return false;
	}
    setDirty(false);
  }   
  return true;
}

bool DBBrowserDB::revert()
{
    if (!isOpen()) return false;

  if (_db){
    if ( SQLITE_OK != sqlite3_exec(_db,"ROLLBACK TRANSACTION RESTOREPOINT;",
											NULL,NULL,NULL) ) {
	  lastErrorMessage = StringFromUTF16(sqlite3_errmsg16(_db));
	  return false;
	}
	setDirty(false);
  }
  return true;
}

bool DBBrowserDB::create ( const QString & db)
{
  bool ok=false;
    
  if (isOpen()) close();
  
  lastErrorMessage = QString("no error");

    if( sqlite3_open16(db.ucs2(), &_db) != SQLITE_OK ){
        lastErrorMessage = StringFromUTF16(sqlite3_errmsg16(_db));
        sqlite3_close(_db);
        _db = 0;
        return false;
      }

    if (_db){
    if (SQLITE_OK==sqlite3_exec(_db,"PRAGMA empty_result_callbacks = ON;",
                               NULL,NULL,NULL)){
	if (SQLITE_OK==sqlite3_exec(_db,"PRAGMA show_datatypes = ON;",
                               NULL,NULL,NULL)){
	    ok=true;
	    setDirty(false);
	}
	curDBFilename = db;
    }
	
}

  return ok;
}

void DBBrowserDB::close (){
    if (_db)
    {
	if (getDirty())
	{
	    QString msg = "Do you want to save the changes made to the database file ";
	msg.append(curDBFilename);
	msg.append(" ?");
	    if (QMessageBox::question( 0, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
	    {
		save();
	    } else {
		//not really necessary, I think... but will not hurt.
		revert();
	    }
	}
	sqlite3_close(_db);
    }
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
      save();
      logSQL(QString("VACUUM;"), kLogMsg_App);
    if (SQLITE_OK==sqlite3_exec(_db,"VACUUM;",
                               NULL,NULL,&errmsg)){
	    ok=true;
	    setDirty(false);
	}
    }

  if (!ok){
    lastErrorMessage = QString(errmsg);
    return false;
  }else{
    return true;
  }
}

bool DBBrowserDB::reload( const QString & filename, int * lineErr)
{
    /*to avoid a nested transaction error*/
    sqlite3_exec(_db,"COMMIT;", NULL,NULL,NULL);
    FILE * cfile = fopen((const char *) filename, (const char *) "r");
    load_database(_db, cfile, lineErr);
    fclose(cfile);
    setDirty(false);
    if ((*lineErr)!=0)
    {
	return false;
    }
    return true;
}

bool DBBrowserDB::dump( const QString & filename)
{
    FILE * cfile = fopen((const char *) filename, (const char *) "w");
    if (!cfile)
    {
	return false;
    }
    dump_database(_db, cfile);
    fclose(cfile);
    return true;
}

bool DBBrowserDB::executeSQL ( const QString & statement)
{
  char *errmsg;
  bool ok=false;
    
  if (!isOpen()) return false;

  if (_db){
      logSQL(statement, kLogMsg_App);
      setDirty(true);
    if (SQLITE_OK==sqlite3_exec(_db,statement.utf8(),
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

bool DBBrowserDB::executeSQLDirect ( const QString & statement)
{
    //no transaction support
  char *errmsg;
  bool ok=false;
    
  if (!isOpen()) return false;

  if (_db){
      logSQL(statement, kLogMsg_App);
    if (SQLITE_OK==sqlite3_exec(_db,statement.utf8(),
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
	logSQL(statement, kLogMsg_App);
	setDirty(true);
	if (SQLITE_OK==sqlite3_exec(_db,statement.utf8(),NULL,NULL, &errmsg)){
	ok=true;
	int newrowid = sqlite3_last_insert_rowid(_db);
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
	logSQL(statement, kLogMsg_App);
	setDirty(true);
	if (SQLITE_OK==sqlite3_exec(_db,statement.utf8(),
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
    
    QStringList::Iterator ct = browseFields.at(wcol);
    
    QString statement = "UPDATE ";
    statement.append(curBrowseTableName.latin1());
    statement.append(" SET ");
    statement.append(*ct);
    statement.append("=");

   char * formSQL = sqlite3_mprintf("%Q",static_cast<const char*>(wtext.utf8()));			
    statement.append(formSQL);
    if (formSQL) sqlite3_free(formSQL);
    
    statement.append(" WHERE rowid=");
    statement.append(rowid.latin1());
    statement.append(";");

    if (_db){
	logSQL(statement, kLogMsg_App);
	setDirty(true);
	if (SQLITE_OK==sqlite3_exec(_db,statement.utf8(),
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
   sqlite3_stmt *vm;
   const void *tail;
   
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
	logSQL(statement, kLogMsg_App);
	err=sqlite3_prepare16(_db,statement.ucs2(),statement.length(),
                              &vm, &tail);
	if (err == SQLITE_OK){
	    int rownum = 0;
	  while ( sqlite3_step(vm) == SQLITE_ROW ){
	      r.clear();
	      ncol = sqlite3_data_count(vm);
	      for (int e=0; e<ncol; e++){
			  QString rv(StringFromUTF16(sqlite3_column_text16(vm, e)));
			  r << rv;
		      if (e==0){
		        idmap.insert(rv.toInt(),rownum);
		        rownum++;
		      }
	      }
	      browseRecs.append(r);
	  }

          sqlite3_finalize(vm);
        }else{
          lastErrorMessage = QString ("could not get fields");
        }
}

resultMap DBBrowserDB::getFindResults( const QString & wstatement)
{
   sqlite3_stmt *vm;
   const void *tail;
   
   int ncol;
 
   int err=0;
   resultMap res;
   lastErrorMessage = QString("no error");
   
  logSQL(wstatement, kLogMsg_App);
	err=sqlite3_prepare16(_db,wstatement.ucs2(),wstatement.length(),
                              &vm, &tail);
	if (err == SQLITE_OK){
	    int rownum = 0;
	    int recnum = 0;
	    QString r;
	  while ( sqlite3_step(vm) == SQLITE_ROW ){
	       ncol = sqlite3_data_count(vm);
	      for (int e=0; e<ncol; e++){
	      	  r = StringFromUTF16(sqlite3_column_text16(vm, e));
		  if (e==0){
		      rownum = r.toInt();
		      rowIdMap::iterator mit = idmap.find(rownum);
		      recnum = *mit;
		  }
	      }
	        res.insert(recnum, r);
	  }

          sqlite3_finalize(vm);
        }else{
          lastErrorMessage = StringFromUTF16(sqlite3_errmsg16(_db));
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

QStringList DBBrowserDB::getTableTypes(const QString & tablename)
{
    tableMap::Iterator it;
    tableMap tmap = tbmap;
    QStringList res;

        for ( it = tmap.begin(); it != tmap.end(); ++it ) {
	    if (tablename.compare(it.data().getname())==0 ){
	    fieldMap::Iterator fit;
	    fieldMap fmap = it.data().fldmap;

	    for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
		 res.append( fit.data().gettype() );
	    }
	}
	}
    return res;
}

int DBBrowserDB::getRecordCount()
{
    return browseRecs.count();
}

void DBBrowserDB::logSQL(QString statement, int msgtype)
{
    if (logWin)
    {	
	/*limit log message to a sensible size, this will truncate some binary messages*/
	uint loglimit = 300;
	if ((statement.length() > loglimit)&&(msgtype==kLogMsg_App))
	{
	    statement.truncate(32);
	    statement.append("... <string too wide to log, probably contains binary data> ...");
	} 
	logWin->log(statement, msgtype);
    }
}


void DBBrowserDB::updateSchema( )
{
  // qDebug ("Getting list of tables");
   sqlite3_stmt *vm;
   const void *tail;
   int ncol;
   QStringList r;
   int err=0;
   QString num;
   int idxnum =0;
   int tabnum = 0;
   
   idxmap.clear();
   tbmap.clear();
   
   lastErrorMessage = QString("no error");
   QString statement = "SELECT name, sql "
                              "FROM sqlite_master "
                              "WHERE type='table' ;";

   err=sqlite3_prepare16(_db,statement.ucs2(),statement.length()
                              /*"ORDER BY name;"*/ ,
                             &vm, &tail);
        if (err == SQLITE_OK){
	    logSQL(statement, kLogMsg_App);
          while ( sqlite3_step(vm) == SQLITE_ROW ){
	      num.setNum(tabnum);
	      QString  val1, val2;
	      val1 = StringFromUTF16(sqlite3_column_text16(vm, 0));
	      val2 = StringFromUTF16(sqlite3_column_text16(vm, 1));
	      tbmap[num] = DBBrowserTable(val1, val2);
	      tabnum++;
          }
          sqlite3_finalize(vm);
        }else{
          qDebug ("could not get list of tables: %d, %s",err,sqlite3_errmsg(_db));
        }
	
	//now get the field list for each table in tbmap
       tableMap::Iterator it;
        for ( it = tbmap.begin(); it != tbmap.end(); ++it ) {
	statement = "SELECT *  FROM ";
	statement.append( (const char *) it.data().getname().latin1());
	statement.append(" LIMIT 1;");
	logSQL(statement, kLogMsg_App);
	err=sqlite3_prepare16(_db,statement.ucs2(),statement.length(),
                              &vm, &tail);
        if (err == SQLITE_OK){
          err = sqlite3_step(vm);
	  if ((err==SQLITE_ROW)||(err==SQLITE_DONE)){
	      ncol = sqlite3_column_count(vm);
	      it.data(). fldmap.clear();
	      for (int e=0; e<ncol; e++) {
		      it.data().addField(e,StringFromUTF16(sqlite3_column_name16(vm, e)),StringFromUTF16(sqlite3_column_decltype16(vm, e)));
		  }
	  }

          sqlite3_finalize(vm);
        }else{
          lastErrorMessage = QString ("could not get types");
        }
    }
	statement = "SELECT name, sql "
                              "FROM sqlite_master "
                              "WHERE type='index' ";
	 /*"ORDER BY name;"*/
	//finally get indices
	err=sqlite3_prepare16(_db,statement.ucs2(),statement.length(),
                              &vm, &tail);
	logSQL(statement, kLogMsg_App);
	if (err == SQLITE_OK){
	    while ( sqlite3_step(vm) == SQLITE_ROW ){
	      QString  val1, val2;
	      val1 = StringFromUTF16(sqlite3_column_text16(vm, 0));
	      val2 = StringFromUTF16(sqlite3_column_text16(vm, 1));
		    num.setNum(idxnum);
		    idxmap[num] = DBBrowserIndex(val1,val2);
		    idxnum ++;
      }
      sqlite3_finalize(vm);
   }else{
      lastErrorMessage = QString ("could not get list of indices");
   }
}

QStringList DBBrowserDB::decodeCSV(const QString & csvfilename, char sep, char quote, int maxrecords, int * numfields)
{
	QFile file(csvfilename);
	QStringList result;
	QString current = "";
	bool inquotemode = false;
	bool inescapemode = false;
	int recs = 0;
	*numfields = 0;
    if ( file.open( IO_ReadWrite ) ) {
		char c=0;
        while ( c!=-1) {
            c = file.getch();
			if (c==quote){
				if (inquotemode){
					if (inescapemode){
				 	inescapemode = false;
				 	//add the escaped char here
					current.append(c);
				 	} else {
					//are we escaping, or just finishing the quote?
						char d = file.getch();
						if (d==quote) {
							inescapemode = true;
						} else {
							inquotemode = false;
						}
						file.ungetch(d);
					}
				} else {
					inquotemode = true;
				}
        	} else if (c==sep) {
				if (inquotemode){
				   //add the sep here
				   current.append(c);
				} else {
				  //not quoting, start new record
				  result << current;
				  current = "";
				}
			} else if (c==10) {
				if (inquotemode){
				   //add the newline
				   current.append(c);
				} else {
				  //not quoting, start new record
				  result << current;
				  current = "";
				  //for the first line, store the field count
				  if (*numfields == 0){
				      *numfields = result.count();
				  }
				  recs++;
				  if ((recs>maxrecords)&&(maxrecords!=-1))					    {
				      break;
				  }   
				}
			} else if (c==13) {
				if (inquotemode){
				   //add the carrier return if in quote mode only
				   current.append(c);
				}
			} else {//another character type
				current.append(c);
			}
		}
        file.close();
		//do we still have a last result, not appended?
	                //proper csv files should end with a linefeed , so this is not necessary
		//if (current.length()>0) result << current;
    }
	return result;
}





