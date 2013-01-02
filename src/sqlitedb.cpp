#include "sqlitedb.h"
#include "sqlbrowser_util.h"
#include <stdlib.h>
#include <QRegExp>
#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include "SQLLogDock.h"
#include <QApplication>

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

void DBBrowserDB::setEncoding( int encoding )
{
    curEncoding = encoding;
}

void DBBrowserDB::setDefaultNewData( const QString & data )
{
    curNewData = data;
}

char * DBBrowserDB::GetEncodedQStringAsPointer( const QString & input)
{
    if (curEncoding==kEncodingUTF8) return (char *) input.toUtf8().constData();
    if (curEncoding==kEncodingLatin1) return (char *) input.toLatin1().constData();

    return (char *) input.constData();
}

QString DBBrowserDB::GetEncodedQString( const QString & input)
{
    if (curEncoding==kEncodingUTF8) return input.toUtf8();
    if (curEncoding==kEncodingLatin1) return input.toLatin1();

    return input;
}

QString DBBrowserDB::GetDecodedQString( const QString & input)
{
    //    if (curEncoding==kEncodingUTF8) return QString::fromUtf8(input);
    //    if (curEncoding==kEncodingLatin1) return QString::fromLatin1(input);

    return input;
}

bool DBBrowserDB::open ( const QString & db)
{
    bool ok=false;
    int  err;

    if (isOpen()) close();

    //try to verify the SQLite version 3 file header
    QFile dbfile(db);
    if ( dbfile.open( QIODevice::ReadOnly ) ) {
        char buffer[16+1];
        dbfile.readLine(buffer, 16);
        QString contents = QString(buffer);
        dbfile.close();
        if (!contents.startsWith("SQLite format 3")) {
            lastErrorMessage = QString("File is not a SQLite 3 database");
            return false;
        }
    } else {
        lastErrorMessage = QString("File could not be read");
        return false;
    }

    lastErrorMessage = QString("no error");

    err = sqlite3_open_v2(db.toUtf8(), &_db, SQLITE_OPEN_READWRITE, NULL);
    if ( err ) {
        lastErrorMessage = sqlite3_errmsg(_db);
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
        sqlite3_exec(_db,"SAVEPOINT RESTOREPOINT;",
                     NULL,NULL,NULL);
        setDirty(false);
    }
    return true;
}

bool DBBrowserDB::save()
{
    if (!isOpen()) return false;

    if (_db){
        sqlite3_exec(_db,"RELEASE RESTOREPOINT;",
                     NULL,NULL,NULL);
        setDirty(false);
    }
    return true;
}

bool DBBrowserDB::revert()
{
    if (!isOpen()) return false;

    if (_db){
        sqlite3_exec(_db,"ROLLBACK TO SAVEPOINT RESTOREPOINT;",
                     NULL,NULL,NULL);
        qDebug(sqlite3_errmsg(_db));
        sqlite3_exec(_db,"RELEASE RESTOREPOINT;",
                     NULL,NULL,NULL);
        qDebug(sqlite3_errmsg(_db));
        setDirty(false);
    }
    return true;
}

bool DBBrowserDB::create ( const QString & db)
{
    bool ok=false;
    
    if (isOpen()) close();

    lastErrorMessage = QString("no error");

    if( sqlite3_open(db.toUtf8(), &_db) != SQLITE_OK ){
        lastErrorMessage = sqlite3_errmsg(_db);
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
            if (QMessageBox::question( 0, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
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
    trgmap.clear();
    viewmap.clear();
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
    FILE * cfile = fopen(filename.toUtf8(), (const char *) "r");
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
    FILE * cfile = fopen(filename.toUtf8(), (const char *) "w");
    if (!cfile)
    {
        return false;
    }
    dump_database(_db, cfile);
    fclose(cfile);
    return true;
}

bool DBBrowserDB::executeSQL ( const QString & statement, bool dirtyDB, bool logsql)
{
    char *errmsg;
    bool ok = false;
    
    if (!isOpen()) return false;

    if (_db){
        if (logsql) logSQL(statement, kLogMsg_App);
        if (dirtyDB) setDirty(true);
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),
                                    NULL,NULL,&errmsg)){
            ok=true;
        }
    }

    if (!ok){
        lastErrorMessage = QString(errmsg);
        return false;
    }
    return true;
}


bool DBBrowserDB::addRecord ( )
{
    char *errmsg;
    if (!hasValidBrowseSet) return false;
    if (!isOpen()) return false;
    bool ok = false;
    int fields = browseFields.count();
    QString emptyvalue = curNewData;

    QString statement = "INSERT INTO ";
    statement.append(GetEncodedQString(curBrowseTableName));
    statement.append(" VALUES(");
    for ( int i=1; i<=fields; i++ ) {
        statement.append(emptyvalue);
        if (i<fields) statement.append(", ");
    }
    statement.append(");");
    lastErrorMessage = QString("no error");
    if (_db){
        logSQL(statement, kLogMsg_App);
        setDirty(true);
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),NULL,NULL, &errmsg)){
            ok=true;
            //int newrowid = sqlite3_last_insert_rowid(_db);
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
    QStringList& rt = tab[wrow];
    QString& rowid = rt[0];
    lastErrorMessage = QString("no error");
    
    QString statement = "DELETE FROM ";
    statement.append(GetEncodedQString(curBrowseTableName));
    statement.append(" WHERE rowid=");
    statement.append(rowid);
    statement.append(";");

    if (_db){
        logSQL(statement, kLogMsg_App);
        setDirty(true);
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),
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
    
    QStringList& rt = browseRecs[wrow];
    QString& rowid = rt[0];
    QString& cv = rt[wcol+1];//must account for rowid
    QString ct = browseFields.at(wcol);
    
    QString statement = "UPDATE ";
    statement.append(GetEncodedQString(curBrowseTableName));
    statement.append(" SET ");
    statement.append(GetEncodedQString(ct));
    statement.append("=");

    QString wenc = GetEncodedQString(wtext);
    char * formSQL = sqlite3_mprintf("%Q", wenc.toUtf8().constData());
    statement.append(formSQL);
    if (formSQL) sqlite3_free(formSQL);
    
    statement.append(" WHERE rowid=");
    statement.append(rowid);
    statement.append(";");

    if (_db){
        logSQL(statement, kLogMsg_App);
        setDirty(true);
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),
                                    NULL,NULL,&errmsg)){
            ok=true;
            /*update local copy*/
            cv = wtext;
        } else {
            lastErrorMessage = QString(errmsg);
        }
    }

    return ok;

}


bool DBBrowserDB::browseTable( const QString & tablename, const QString& orderby )
{
    QStringList testFields = getTableFields( tablename );
    
    if (testFields.count()>0) {//table exists
        getTableRecords( tablename, orderby );
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

bool DBBrowserDB::createColumn( QString tablename, QString fieldname, QString fieldtype ){
    qDebug("create column");
    QString sql = QString("ALTER TABLE `%1` ADD COLUMN `%2` %3").arg(tablename).arg(fieldname).arg(fieldtype);
    qDebug(sql.toUtf8());
    return executeSQL(sql);
}

bool DBBrowserDB::renameColumn(QString tablename, QString from, QString to, QString type) {
    qDebug("rename column");
    // TODO change column name and type. SQLite doesn't support natively...
    //QString sql = QString("ALTER TABLE `%1` MODIFY `%2` %3").arg(tablename).arg(to).arg(type);
    //qDebug(sql.toUtf8());
    //return executeSQL(sql);
    return true;
}

bool DBBrowserDB::renameTable(QString from_table, QString to_table){
    qDebug("renameTable");
    return true;
}

void DBBrowserDB::getTableRecords( const QString & tablename, const QString& orderby )
{
    sqlite3_stmt *vm;
    const char *tail;

    int ncol;
    QStringList r;
    // char *errmsg;
    int err=0;
    // int tabnum = 0;
    browseRecs.clear();
    idmap.clear();
    lastErrorMessage = QString("no error");

    QString statement = "SELECT rowid, *  FROM ";
    statement.append( GetEncodedQString(tablename) );
    statement.append(" ORDER BY ");
    statement.append(orderby);
    statement.append(";");
    //qDebug(statement);
    logSQL(statement, kLogMsg_App);
    err=sqlite3_prepare(_db,statement.toUtf8(),statement.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        int rownum = 0;

        while ( sqlite3_step(vm) == SQLITE_ROW ){
            r.clear();
            ncol = sqlite3_data_count(vm);
            for (int e=0; e<ncol; e++){
                char * strresult = 0;
                QString rv;
                strresult = (char *) sqlite3_column_text(vm, e);
                rv = QString(strresult);
                r << GetDecodedQString(rv);
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
    const char *tail;

    int ncol;

    //   char *errmsg;
    int err=0;
    resultMap res;
    lastErrorMessage = QString("no error");
    QString encstatement = GetEncodedQString(wstatement);
    logSQL(encstatement, kLogMsg_App);
    err=sqlite3_prepare(_db,encstatement.toUtf8(),encstatement.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        int rownum = 0;
        int recnum = 0;
        QString r;
        while ( sqlite3_step(vm) == SQLITE_ROW ){
            ncol = sqlite3_data_count(vm);
            for (int e=0; e<ncol; e++){
                char * strresult = 0;
                strresult = (char *) sqlite3_column_text(vm, e);
                r = QString(strresult);
                if (e==0){
                    rownum = r.toInt();
                    rowIdMap::iterator mit = idmap.find(rownum);
                    recnum = *mit;
                }
            }
            res.insert(recnum, GetDecodedQString(r));
        }

        sqlite3_finalize(vm);
    }else{
        lastErrorMessage = QString(sqlite3_errmsg(_db));
    }
    return res;
}


QStringList DBBrowserDB::getTableNames()
{
    tableMap::ConstIterator it;
    QStringList res;

    for ( it = tbmap.begin(); it != tbmap.end(); ++it ) {
        res.append( it.value().getname() );
    }
    
    return res;
}

QStringList DBBrowserDB::getIndexNames()
{
    objectMap::Iterator it;
    objectMap tmap = idxmap;
    QStringList res;

    for ( it = tmap.begin(); it != tmap.end(); ++it ) {
        res.append( it.value().getname() );
    }
    
    return res;
}

QStringList DBBrowserDB::getTableFields(const QString & tablename)
{
    tableMap::ConstIterator it;
    QStringList res;

    for ( it = tbmap.begin(); it != tbmap.end(); ++it ) {
        if (tablename.compare(it.value().getname())==0 ){
            fieldMap::ConstIterator fit;

            for ( fit = it.value().fldmap.begin(); fit != it.value().fldmap.end(); ++fit ) {
                res.append( fit.value().getname() );
            }
        }
    }
    return res;
}

QStringList DBBrowserDB::getTableTypes(const QString & tablename)
{
    tableMap::ConstIterator it;
    QStringList res;

    for ( it = tbmap.begin(); it != tbmap.end(); ++it ) {
        if (tablename.compare(it.value().getname())==0 ){
            fieldMap::ConstIterator fit;

            for ( fit = it.value().fldmap.begin(); fit != it.value().fldmap.end(); ++fit ) {
                res.append( fit.value().gettype() );
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
        int loglimit = 300;
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
    sqlite3_stmt *vm;
    const char *tail;
    int err=0;

    idxmap.clear();
    tbmap.clear();
    viewmap.clear();
    trgmap.clear();

    lastErrorMessage = QString("no error");
    QString statement = "SELECT type, name, sql FROM sqlite_master;";

    err=sqlite3_prepare(_db, (const char *) statement.toUtf8(),statement.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        logSQL(statement, kLogMsg_App);
        while ( sqlite3_step(vm) == SQLITE_ROW ){
            QString  val1, val2, val3;
            val1 = QString((const char *) sqlite3_column_text(vm, 0));
            val2 = QString((const char *) sqlite3_column_text(vm, 1));
            val3 = QString((const char *) sqlite3_column_text(vm, 2));

            if(val1 == "table")
                tbmap[val2] = DBBrowserTable(GetDecodedQString(val2), GetDecodedQString(val3));
            else if(val1 == "index")
                idxmap[val2] = DBBrowserObject(GetDecodedQString(val2), GetDecodedQString(val3));
            else if(val1 == "view")
                viewmap[val2] = DBBrowserObject(GetDecodedQString(val2), GetDecodedQString(val3));
            else if(val1 == "trigger")
                trgmap[val2] = DBBrowserObject(GetDecodedQString(val2), GetDecodedQString(val3));
            else
                qDebug("unknown object type %s", val1.toStdString().c_str());
        }
        sqlite3_finalize(vm);
    }else{
        qDebug ("could not get list of db objects: %d, %s",err,sqlite3_errmsg(_db));
    }
    qDebug(sqlite3_errmsg(_db));

    //now get the field list for each table in tbmap
    tableMap::Iterator it;
    for ( it = tbmap.begin(); it != tbmap.end(); ++it ) {
        statement = "PRAGMA TABLE_INFO(";
        statement.append( it.value().getname());
        statement.append(");");
        logSQL(statement, kLogMsg_App);
        err=sqlite3_prepare(_db,statement.toUtf8(),statement.length(),
                            &vm, &tail);
        if (err == SQLITE_OK){
            it.value(). fldmap.clear();
            int e = 0;
            while ( sqlite3_step(vm) == SQLITE_ROW ){
                if (sqlite3_column_count(vm)==6) {
                    QString  val1, val2;
                    int ispk= 0;
                    val1 = QString((const char *) sqlite3_column_text(vm, 1));
                    val2 = QString((const char *) sqlite3_column_text(vm, 2));
                    ispk = sqlite3_column_int(vm, 5);
                    if (ispk==1){
                        val2.append(QString(" PRIMARY KEY"));
                    }
                    it.value().addField(e,GetDecodedQString(val1),GetDecodedQString(val2));
                    e++;
                }
            }
            sqlite3_finalize(vm);
        } else{
            lastErrorMessage = QString ("could not get types");
        }
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

    if ( file.open( QIODevice::ReadWrite ) ) {
        QProgressDialog progress("Decoding CSV file...", "Cancel", 0, file.size());
        progress.setWindowModality(Qt::ApplicationModal);
        char c=0;
        while ( c!=-1) {
            file.getChar(&c);
            if (c==quote){
                if (inquotemode){
                    if (inescapemode){
                        inescapemode = false;
                        //add the escaped char here
                        current.append(c);
                    } else {
                        //are we escaping, or just finishing the quote?
                        char d;
                        file.getChar(&d);
                        if (d==quote) {
                            inescapemode = true;
                        } else {
                            inquotemode = false;
                        }
                        file.ungetChar(d);
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
                    progress.setValue(file.pos());
                    if (progress.wasCanceled()) break;
                    if ((recs>maxrecords)&&(maxrecords!=-1))         {
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





