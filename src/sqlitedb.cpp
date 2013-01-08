#include "sqlitedb.h"
#include "sqlbrowser_util.h"
#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include "SQLLogDock.h"
#include <QApplication>

void DBBrowserObject::addField(int order, const QString& wfield,const QString& wtype)
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
    objMap.clear();
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

    QString statement = QString("INSERT INTO `%1` VALUES(").arg(GetEncodedQString(curBrowseTableName));
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
    
    QString statement = QString("DELETE FROM `%1` WHERE rowid=%2;").arg(GetEncodedQString(curBrowseTableName)).arg(rowid);

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
    
    QString statement = QString("UPDATE `%1` SET `%2`=").arg(GetEncodedQString(curBrowseTableName)).arg(ct);

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

bool DBBrowserDB::createTable(QString name, const QList<DBBrowserField>& structure)
{
    // Build SQL statement
    QString sql = QString("CREATE TABLE `%1` (").arg(name);
    for(int i=0;i<structure.count();i++)
        sql.append(QString("`%1` %2,").arg(structure.at(i).getname()).arg(structure.at(i).gettype()));
    sql.remove(sql.count() - 1, 1);     // Remove last comma
    sql.append(");");

    // Execute it
    return executeSQL(sql);
}

bool DBBrowserDB::createColumn(QString tablename, QString fieldname, QString fieldtype)
{
    QString sql = QString("ALTER TABLE `%1` ADD COLUMN `%2` %3").arg(tablename).arg(fieldname).arg(fieldtype);
    return executeSQL(sql);
}

bool DBBrowserDB::renameColumn(QString tablename, QString from, QString to, QString type)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite. If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql = QString("ALTER TABLE `%1` MODIFY `%2` %3").arg(tablename).arg(to).arg(type);
    //return executeSQL(sql);

    // Collect information on the current DB layout
    DBBrowserObject table = getObjectByName(tablename);
    if(table.getname() == "" || table.getField(from).getname() == "")
    {
        lastErrorMessage = QString("renameColumn: cannot find table %1 with column %2").arg(tablename).arg(from);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!executeSQL("SAVEPOINT sqlitebrowser_rename_column"))
    {
        lastErrorMessage = "renameColumn: creating savepoint failed";
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Create a new table with a name that hopefully doesn't exist yet. Its layout is exactly the same as the one of the table to change - except for the column to change
    // of course
    QList<DBBrowserField> new_table_structure;
    for(int i=0;i<table.fldmap.count();i++)
    {
        // Is this the column to rename?
        if(table.fldmap.value(i).getname() == from)
            new_table_structure.push_back(DBBrowserField(to, type));
        else
            new_table_structure.push_back(DBBrowserField(table.fldmap.value(i).getname(), table.fldmap.value(i).gettype()));
    }
    if(!createTable("sqlitebrowser_rename_column_new_table", new_table_structure))
    {
        lastErrorMessage = QString("renameColumn: creating new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Copy the data from the old table to the new one
    if(!executeSQL(QString("INSERT INTO sqlitebrowser_rename_column_new_table SELECT * FROM `%1`;").arg(tablename)))
    {
        lastErrorMessage = QString("renameColumn: copying data to new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE `%1`;").arg(tablename)))
    {
        lastErrorMessage = QString("renameColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Rename the temporary table
    if(!renameTable("sqlitebrowser_rename_column_new_table", tablename))
    {
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Release the savepoint - everything went fine
    if(!executeSQL("RELEASE SAVEPOINT sqlitebrowser_rename_column;"))
    {
        lastErrorMessage = QString("renameColumn: releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Success
    return true;
}

bool DBBrowserDB::dropColumn(QString tablename, QString column)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite. If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql = QString("ALTER TABLE `%1` DROP COLUMN `%2`;").arg(table).arg(column);
    //return executeSQL(sql);

    // Collect information on the current DB layout
    DBBrowserObject table = getObjectByName(tablename);
    if(table.getname() == "" || table.getField(column).getname() == "" || table.fldmap.count() == 1)
    {
        lastErrorMessage = QString("dropColumn: cannot find table %1 or column %2. Also you can not delete the last column").arg(tablename).arg(column);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!executeSQL("SAVEPOINT sqlitebrowser_drop_column"))
    {
        lastErrorMessage = "dropColumn: creating savepoint failed";
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Create a new table with a name that hopefully doesn't exist yet. Its layout is exactly the same as the one of the table to change - except for the column to drop
    // of course. Also prepare the columns to be copied in a later step now.
    QList<DBBrowserField> new_table_structure;
    QString select_cols;
    for(int i=0;i<table.fldmap.count();i++)
    {
        // Only add this if it is not the column to drop?
        if(table.fldmap.value(i).getname() != column)
        {
            new_table_structure.push_back(DBBrowserField(table.fldmap.value(i).getname(), table.fldmap.value(i).gettype()));
            select_cols.append(QString("`%1`,").arg(table.fldmap.value(i).getname()));
        }
    }
    if(!createTable("sqlitebrowser_drop_column_new_table", new_table_structure))
    {
        lastErrorMessage = QString("dropColumn: creating new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;");
        return false;
    }
    select_cols.remove(select_cols.count() - 1, 1);

    // Copy the data from the old table to the new one
    if(!executeSQL(QString("INSERT INTO sqlitebrowser_drop_column_new_table SELECT %1 FROM `%2`;").arg(select_cols).arg(tablename)))
    {
        lastErrorMessage = QString("dropColumn: copying data to new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;");
        return false;
    }

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE `%1`;").arg(tablename)))
    {
        lastErrorMessage = QString("dropColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;");
        return false;
    }

    // Rename the temporary table
    if(!renameTable("sqlitebrowser_drop_column_new_table", tablename))
    {
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;");
        return false;
    }

    // Release the savepoint - everything went fine
    if(!executeSQL("RELEASE SAVEPOINT sqlitebrowser_drop_column;"))
    {
        lastErrorMessage = QString("dropColumn: releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Success
    return true;
}

bool DBBrowserDB::renameTable(QString from_table, QString to_table)
{
    QString sql = QString("ALTER TABLE `%1` RENAME TO `%2`").arg(from_table, to_table);
    if(!executeSQL(sql))
    {
        QString error = QString("Error renaming table '%1' to '%2'. Message from database engine:\n%3").arg(from_table).arg(to_table).arg(lastErrorMessage);
        lastErrorMessage = error;
        qDebug(error.toStdString().c_str());
        return false;
    } else {
        return true;
    }
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

    QString statement = QString("SELECT rowid, *  FROM `%1` ORDER BY %2;").arg(GetEncodedQString(tablename)).arg(orderby);
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


QStringList DBBrowserDB::getBrowsableObjectNames()
{
    objectMap::ConstIterator it;
    QStringList res;

    for(it=objMap.begin();it!=objMap.end();++it)
    {
        if(it.key() == "table" || it.key() == "view")
            res.append(it.value().getname());
    }
    
    return res;
}

objectMap DBBrowserDB::getBrowsableObjects()
{
    objectMap::ConstIterator it;
    objectMap res;

    for(it=objMap.begin();it!=objMap.end();++it)
    {
        if(it.key() == "table" || it.key() == "view")
            res.insert(it.key(), it.value());
    }

    return res;
}

QStringList DBBrowserDB::getIndexNames()
{
    QList<DBBrowserObject> tmap = objMap.values("index");
    QList<DBBrowserObject>::ConstIterator it;
    QStringList res;

    for ( it = tmap.begin(); it != tmap.end(); ++it ) {
        res.append( (*it).getname() );
    }
    
    return res;
}

QStringList DBBrowserDB::getTableFields(const QString & tablename)
{
    objectMap::ConstIterator it;
    QStringList res;

    for ( it = objMap.begin(); it != objMap.end(); ++it )
    {
        if((*it).getname() == tablename)
        {
            fieldMap::ConstIterator fit;

            for ( fit = (*it).fldmap.begin(); fit != (*it).fldmap.end(); ++fit ) {
                res.append( fit.value().getname() );
            }
        }
    }
    return res;
}

QStringList DBBrowserDB::getTableTypes(const QString & tablename)
{
    objectMap::ConstIterator it;
    QStringList res;

    for ( it = objMap.begin(); it != objMap.end(); ++it )
    {
        if((*it).getname() == tablename)
        {
            fieldMap::ConstIterator fit;

            for ( fit = (*it).fldmap.begin(); fit != (*it).fldmap.end(); ++fit ) {
                res.append( fit.value().gettype() );
            }
        }
    }
    return res;
}

DBBrowserObject DBBrowserDB::getObjectByName(const QString& name)
{
    objectMap::ConstIterator it;
    QStringList res;

    for ( it = objMap.begin(); it != objMap.end(); ++it )
    {
        if((*it).getname() == name)
            return *it;
    }
    return DBBrowserObject();
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

    objMap.clear();

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

            if(val1 == "table" || val1 == "index" || val1 == "view" || val1 == "trigger")
                objMap.insert(val1, DBBrowserObject(GetDecodedQString(val2), GetDecodedQString(val3), GetDecodedQString(val1)));
            else
                qDebug("unknown object type %s", val1.toStdString().c_str());
        }
        sqlite3_finalize(vm);
    }else{
        qDebug ("could not get list of db objects: %d, %s",err,sqlite3_errmsg(_db));
    }

    //now get the field list for each table
    objectMap::Iterator it;
    for ( it = objMap.begin(); it != objMap.end(); ++it )
    {
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            statement = QString("PRAGMA TABLE_INFO(`%1`);").arg((*it).getname());
            logSQL(statement, kLogMsg_App);
            err=sqlite3_prepare(_db,statement.toUtf8(),statement.length(),
                                &vm, &tail);
            if (err == SQLITE_OK){
                (*it).fldmap.clear();
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
                        (*it).addField(e,GetDecodedQString(val1),GetDecodedQString(val2));
                        e++;
                    }
                }
                sqlite3_finalize(vm);
            } else{
                lastErrorMessage = QString ("could not get types");
            }
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
        while(file.getChar(&c))
        {
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
                    qApp->processEvents();
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

QString DBBrowserDB::getPragma(QString pragma)
{
    if(!isOpen())
        return "";

    QString sql = QString("PRAGMA %1").arg(pragma);
    sqlite3_stmt* vm;
    const char* tail;
    QString retval = "";

    // Get value from DB
    int err = sqlite3_prepare(_db, sql.toStdString().c_str(), sql.length(), &vm, &tail);
    if(err == SQLITE_OK){
        logSQL(sql, kLogMsg_App);
        if(sqlite3_step(vm) == SQLITE_ROW)
        {
            retval = QString((const char *) sqlite3_column_text(vm, 0));
            sqlite3_finalize(vm);
        } else {
            sqlite3_finalize(vm);
            qDebug("didn't receive any output from pragma %s", pragma.toStdString().c_str());
        }
    } else {
        qDebug ("could not execute pragma command: %d, %s", err, sqlite3_errmsg(_db));
    }

    // Make changes to the value when needed
    if(pragma == "journal_mode")
    {
        retval = retval.toLower();
        if(retval == "delete")
            retval = "0";
        else if(retval == "truncate")
            retval = "1";
        else if(retval == "persist")
            retval = "2";
        else if(retval == "memory")
            retval = "3";
        else if(retval == "wal")
            retval = "4";
        else if(retval == "off")
            retval = "5";
        else
            retval = "0";
    } else if(pragma == "locking_mode") {
        retval = retval.toLower();
        if(retval == "normal")
            retval = "0";
        else if(retval == "exclusive")
            retval = "1";
        else
            retval = "0";
    } else if(pragma == "encoding") {
        retval = retval.toLower();
        if(retval == "utf-8")
            retval = "0";
        else if(retval == "utf-16")
            retval = "1";
        else if(retval == "utf-16le")
            retval = "2";
        else if(retval == "utf-16be")
            retval = "3";
        else
            retval = "0";
    }

    // Return it
    return retval;
}

bool DBBrowserDB::setPragma(QString pragma, QString value)
{
    // Make changes to the value when needed
    if(pragma == "journal_mode")
    {
        if(value == "0")
            value = "delete";
        else if(value == "1")
            value = "truncate";
        else if(value == "2")
            value = "persist";
        else if(value == "3")
            value = "memory";
        else if(value == "4")
            value = "wal";
        else if(value == "5")
            value = "off";
        else
            value = "delete";
    } else if(value == "locking_mode") {
        if(value == "0")
            value = "normal";
        else if(value == "1")
            value = "exclusive";
        else
            value = "normal";
    } else if(pragma == "encoding") {
        if(value == "0")
            value = "\"utf-8\"";
        else if(value == "1")
            value = "\"utf-16\"";
        else if(value == "2")
            value = "\"utf-16le\"";
        else if(value == "3")
            value = "\"utf-16be\"";
        else
            value = "\"utf-8\"";
    }

    // Set the pragma value
    QString sql = QString("PRAGMA %1 = %2;").arg(pragma).arg(value);
    if(!executeSQL(sql))
    {
        qDebug("Error setting pragma %s to %s: %s", pragma.toStdString().c_str(), value.toStdString().c_str(), lastErrorMessage.toStdString().c_str());
        return false;
    } else {
        return true;
    }
}
