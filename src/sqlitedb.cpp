#include "sqlitedb.h"
#include "MainWindow.h"

#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <QTextStream>
#include <QSettings>
#include <QDebug>

void DBBrowserObject::addField(int order, const QString& wfield,const QString& wtype)
{
    fldmap[order] = DBBrowserField(wfield,wtype);
}

bool DBBrowserDB::isOpen ( ) const
{
    return _db!=0;
}

void DBBrowserDB::setDirty(bool dirtyval)
{
    dirty = dirtyval;
    if(mainWindow)
        mainWindow->dbState(dirtyval);
}

bool DBBrowserDB::getDirty() const
{
    return dirty;
}

void DBBrowserDB::setDefaultNewData( const QString & data )
{
    curNewData = data;
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
            lastErrorMessage = QObject::tr("File is not a SQLite 3 database");
            return false;
        }
    } else {
        lastErrorMessage = QObject::tr("File could not be read");
        return false;
    }

    lastErrorMessage = QObject::tr("no error");

    err = sqlite3_open_v2(db.toUtf8(), &_db, SQLITE_OPEN_READWRITE, NULL);
    if ( err ) {
        lastErrorMessage = QString::fromUtf8((const char*)sqlite3_errmsg(_db));
        sqlite3_close(_db);
        _db = 0;
        return false;
    }

    if (_db){
        // set preference defaults
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.sync();
        bool foreignkeys = settings.value( "/db/foreignkeys", false ).toBool();
        setPragma("foreign_keys", foreignkeys ? "1" : "0");

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
    if (dirty) return false;

    if (_db){
        sqlite3_exec(_db,"SAVEPOINT RESTOREPOINT;",
                     NULL,NULL,NULL);
        setDirty(true);
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
        sqlite3_exec(_db,"RELEASE RESTOREPOINT;",
                     NULL,NULL,NULL);
        setDirty(false);
    }
    return true;
}

bool DBBrowserDB::create ( const QString & db)
{
    bool ok=false;
    
    if (isOpen()) close();

    lastErrorMessage = QObject::tr("no error");

    // read encoding from settings and open with sqlite3_open for utf8
    // and sqlite3_open16 for utf16
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    QString sEncoding = settings.value("/db/defaultencoding", "UTF-8").toString();

    int openresult = SQLITE_OK;

    if(sEncoding == "UTF-8" || sEncoding == "UTF8" || sEncoding == "Latin1")
        openresult = sqlite3_open(db.toUtf8(), &_db);
    else
        openresult = sqlite3_open16(db.utf16(), &_db);

    if( openresult != SQLITE_OK ){
        lastErrorMessage = QString::fromUtf8((const char*)sqlite3_errmsg(_db));
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
            QString msg = QObject::tr("Do you want to save the changes made to the database file %1?").arg(curDBFilename);
            if (QMessageBox::question( 0, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
                save();
            else
                revert(); //not really necessary, I think... but will not hurt.
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

bool DBBrowserDB::dump(const QString& filename)
{
    // Open file
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        // Create progress dialog. For this count the number of all table rows to be exported first; this does neither take the table creation itself nor
        // indices, views or triggers into account but compared to the number of rows those should be neglectable
        unsigned int numRecordsTotal = 0, numRecordsCurrent = 0;
        QList<DBBrowserObject> tables = objMap.values("table");
        for(QList<DBBrowserObject>::ConstIterator it=tables.begin();it!=tables.end();++it)
            numRecordsTotal += getFindResults(QString("SELECT COUNT(*) FROM `%1`;").arg((*it).getname())).value(0).toInt();
        QProgressDialog progress(QObject::tr("Exporting database to SQL file..."), QObject::tr("Cancel"), 0, numRecordsTotal);
        progress.setWindowModality(Qt::ApplicationModal);

        // Regular expression to check for numeric strings
        QRegExp regexpIsNumeric("\\d*");

        // Open text stream to the file
        QTextStream stream(&file);

        // Put the SQL commands in a transaction block
        stream << "BEGIN TRANSACTION;\n";

        // Loop through all tables first as they are required to generate views, indices etc. later
        for(QList<DBBrowserObject>::ConstIterator it=tables.begin();it!=tables.end();++it)
        {
            // Write the SQL string used to create this table to the output file
            stream << (*it).getsql() << ";\n";

            // Get data of this table
            browseTable((*it).getname());

            // Dump all the content of the table
            rowList data = browseRecs;
            for(int row=0;row<data.size();row++)
            {
                stream << "INSERT INTO `" << (*it).getname() << "` VALUES(";
                for(int col=1;col<data[row].size();col++)
                {
                    QString content = data[row][col];
                    content.replace("'", "''");
                    if(content.isNull())
                        content = "NULL";
                    else if(content.length() && !regexpIsNumeric.exactMatch(content))
                        content = "'" + content + "'";
                    else if(content.length() == 0)
                        content = "''";

                    stream << content;
                    if(col < data[row].count() - 1)
                        stream << ",";
                    else
                        stream << ");\n";
                }

                // Update progress dialog
                progress.setValue(++numRecordsCurrent);
                qApp->processEvents();
                if(progress.wasCanceled())
                {
                    file.close();
                    file.remove();
                    return false;
                }
            }
        }

        // Now dump all the other objects
        for(objectMap::ConstIterator it=objMap.begin();it!=objMap.end();++it)
        {
            // Make sure it's not a table again
            if(it.value().gettype() == "table")
                continue;

            // Write the SQL string used to create this object to the output file
            stream << (*it).getsql() << ";\n";
        }

        // Done
        stream << "COMMIT;\n";
        file.close();
        return true;
    } else {
        return false;
    }
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
        lastErrorMessage = QString::fromUtf8(errmsg);
        qWarning() << "executeSQL: " << statement << "->" << lastErrorMessage;
        return false;
    }
    return true;
}

bool DBBrowserDB::executeMultiSQL(const QString& statement, bool dirty, bool log)
{
    // First check if a DB is opened
    if(!isOpen())
        return false;

    // Log the statement if needed
    if(log)
        logSQL(statement, kLogMsg_App);

    // Execute the statement by looping until SQLite stops giving back a tail string
    sqlite3_stmt* vm;
    QByteArray utf8Query = statement.toUtf8();
    const char *tail = utf8Query.data();
    int res = 0;
    unsigned int line = 0;
    do
    {
        line++;
        res = sqlite3_prepare_v2(_db, tail, strlen(tail), &vm, &tail);
        if(res == SQLITE_OK)
        {
            if(sqlite3_step(vm) == SQLITE_ERROR)
            {
                sqlite3_finalize(vm);
                lastErrorMessage = QObject::tr("Error in statement #%1: %2.\nAborting execution.").arg(line).arg(sqlite3_errmsg(_db));
                qDebug(lastErrorMessage.toStdString().c_str());
                return false;
            } else {
                sqlite3_finalize(vm);
            }
        } else {
            lastErrorMessage = QObject::tr("Error in statement #%1: %2.\nAborting execution.").arg(line).arg(sqlite3_errmsg(_db));
            qDebug(lastErrorMessage.toStdString().c_str());
            return false;
        }
    } while(tail && *tail != 0 && (res == SQLITE_OK || res == SQLITE_DONE));

    // Set dirty flag
    if(dirty)
        setDirty(true);

    // Exit
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

    QString statement = QString("INSERT INTO `%1` VALUES(").arg(curBrowseTableName);
    for ( int i=1; i<=fields; ++i ) {
        statement.append(emptyvalue);
        if (i<fields) statement.append(", ");
    }
    statement.append(");");
    lastErrorMessage = QObject::tr("no error");
    if (_db){
        logSQL(statement, kLogMsg_App);
        setRestorePoint();
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),NULL,NULL, &errmsg)){
            ok=true;
            //int newrowid = sqlite3_last_insert_rowid(_db);
        } else {
            lastErrorMessage = QString::fromUtf8(errmsg);
            qCritical() << "addRecord: " << lastErrorMessage;
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
    QList<QByteArray> rt = tab[wrow];
    QString rowid = rt[0];
    lastErrorMessage = QString("no error");
    
    QString statement = QString("DELETE FROM `%1` WHERE rowid=%2;").arg(curBrowseTableName).arg(rowid);

    if (_db){
        logSQL(statement, kLogMsg_App);
        setRestorePoint();
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),
                                    NULL,NULL,&errmsg)){
            ok=true;
        } else {
            lastErrorMessage = QString::fromUtf8(errmsg);
            qCritical() << "deleteRecord: " << lastErrorMessage;
        }
    }
    return ok;
}

bool DBBrowserDB::updateRecord(int wrow, int wcol, const QByteArray& wtext)
{
    if (!hasValidBrowseSet) return false;
    if (!isOpen()) return false;
    
    lastErrorMessage = QString("no error");
    
    QList<QByteArray>& rt = browseRecs[wrow];
    QString rowid = rt[0];
    QByteArray& cv = rt[wcol+1];//must account for rowid
    QString ct = browseFields.at(wcol);
    
    QString sql = QString("UPDATE `%1` SET `%2`=? WHERE rowid=%4;").arg(curBrowseTableName).arg(ct).arg(rowid);

    logSQL(sql, kLogMsg_App);
    setRestorePoint();

    sqlite3_stmt* stmt;
    int success = 1;
    if(sqlite3_prepare(_db, sql.toUtf8(), -1, &stmt, 0) != SQLITE_OK)
        success = 0;
    if(success == 1 && sqlite3_bind_text(stmt, 1, wtext.constData(), wtext.length(), SQLITE_STATIC) != SQLITE_OK)
        success = -1;
    if(success == 1 && sqlite3_step(stmt) != SQLITE_DONE)
        success = -1;
    if(success != 0 && sqlite3_finalize(stmt) != SQLITE_OK)
        success = -1;

    if(success == 1)
    {
        cv = wtext;
        return true;
    } else {
        lastErrorMessage = sqlite3_errmsg(_db);
        qCritical() << "updateRecord: " << lastErrorMessage;
        return false;
    }
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

bool DBBrowserDB::createTable(const QString& name, const QList<DBBrowserField>& structure)
{
    // Build SQL statement
    QString sql = QString("CREATE TABLE `%1` (").arg(name);
    for(int i=0;i<structure.count();i++)
        sql.append(QString("`%1` %2,").arg(structure.at(i).getname()).arg(structure.at(i).gettype()));
    sql.remove(sql.count() - 1, 1);     // Remove last comma
    sql.append(");");

    // Execute it
    return executeSQL(sql, false);
}

bool DBBrowserDB::createColumn(const QString& tablename, const QString& fieldname, const QString& fieldtype)
{
    QString sql = QString("ALTER TABLE `%1` ADD COLUMN `%2` %3").arg(tablename).arg(fieldname).arg(fieldtype);
    return executeSQL(sql);
}

bool DBBrowserDB::renameColumn(const QString& tablename, const QString& from, const QString& to, const QString& type)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite. If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql = QString("ALTER TABLE `%1` MODIFY `%2` %3").arg(tablename).arg(to).arg(type);
    //return executeSQL(sql);

    // Collect information on the current DB layout
    DBBrowserObject table = getObjectByName(tablename);
    if(table.getname() == "" || table.getField(from).getname() == "")
    {
        lastErrorMessage = QObject::tr("renameColumn: cannot find table %1 with column %2").arg(tablename).arg(from);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!executeSQL("SAVEPOINT sqlitebrowser_rename_column"))
    {
        lastErrorMessage = QObject::tr("renameColumn: creating savepoint failed. DB says: %1").arg(lastErrorMessage);
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
        lastErrorMessage = QObject::tr("renameColumn: creating new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Copy the data from the old table to the new one
    if(!executeSQL(QString("INSERT INTO sqlitebrowser_rename_column_new_table SELECT * FROM `%1`;").arg(tablename)))
    {
        lastErrorMessage = QObject::tr("renameColumn: copying data to new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE `%1`;").arg(tablename)))
    {
        lastErrorMessage = QObject::tr("renameColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage);
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
        lastErrorMessage = QObject::tr("renameColumn: releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Success
    return true;
}

bool DBBrowserDB::dropColumn(const QString& tablename, const QString& column)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite. If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql = QString("ALTER TABLE `%1` DROP COLUMN `%2`;").arg(table).arg(column);
    //return executeSQL(sql);

    // Collect information on the current DB layout
    DBBrowserObject table = getObjectByName(tablename);
    if(table.getname() == "" || table.getField(column).getname() == "" || table.fldmap.count() == 1)
    {
        lastErrorMessage = QObject::tr("dropColumn: cannot find table %1 or column %2. Also you can not delete the last column").arg(tablename).arg(column);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!executeSQL("SAVEPOINT sqlitebrowser_drop_column;", false))
    {
        lastErrorMessage = QObject::tr("dropColumn: creating savepoint failed");
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
        lastErrorMessage = QObject::tr("dropColumn: creating new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;", false);
        return false;
    }
    select_cols.remove(select_cols.count() - 1, 1);

    // Copy the data from the old table to the new one
    if(!executeSQL(
          QString("INSERT INTO sqlitebrowser_drop_column_new_table SELECT %1 FROM `%2`;").arg(select_cols).arg(tablename),
          false))
    {
        lastErrorMessage = QObject::tr("dropColumn: copying data to new table failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;");
        return false;
    }

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE `%1`;").arg(tablename), false))
    {
        lastErrorMessage = QObject::tr("dropColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage);
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
    if(!executeSQL("RELEASE SAVEPOINT sqlitebrowser_drop_column;", false))
    {
        lastErrorMessage = QObject::tr("dropColumn: releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
        qDebug(lastErrorMessage.toStdString().c_str());
        return false;
    }

    // Success
    return true;
}

bool DBBrowserDB::renameTable(const QString& from_table, const QString& to_table)
{
    QString sql = QString("ALTER TABLE `%1` RENAME TO `%2`").arg(from_table, to_table);
    if(!executeSQL(sql, false))
    {
        QString error = QObject::tr("Error renaming table '%1' to '%2'. Message from database engine:\n%3").arg(from_table).arg(to_table).arg(lastErrorMessage);
        lastErrorMessage = error;
        qDebug(error.toStdString().c_str());
        return false;
    } else {
        return true;
    }
}

void DBBrowserDB::getTableRecords( const QString & tablename, const QString& orderby )
{
    sqlite3_stmt* stmt;

    int ncol;
    QList<QByteArray> r;
    browseRecs.clear();
    idmap.clear();
    lastErrorMessage = QObject::tr("no error");

    QString sql = QString("SELECT rowid, *  FROM `%1` ORDER BY %2;").arg(tablename).arg(orderby);
    logSQL(sql, kLogMsg_App);
    if(sqlite3_prepare(_db, sql.toUtf8(), -1, &stmt, 0) != SQLITE_OK)
    {
        lastErrorMessage = QObject::tr("could not get fields");
        return;
    }

    int rownum = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        r.clear();
        ncol = sqlite3_data_count(stmt);
        for(int e=0;e<ncol;++e)
        {
            QByteArray rv = QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, e)), sqlite3_column_bytes(stmt, e));
            r.append(rv);

            if(e == 0)
            {
                idmap.insert(rv.toInt(), rownum);
                rownum++;
            }
        }
        browseRecs.append(r);
    }

    sqlite3_finalize(stmt);
}

resultMap DBBrowserDB::getFindResults( const QString & wstatement)
{
    sqlite3_stmt *vm;
    const char *tail;

    int ncol;

    //   char *errmsg;
    int err=0;
    resultMap res;
    lastErrorMessage = QObject::tr("no error");
    logSQL(wstatement, kLogMsg_App);
    QByteArray statementutf8 = wstatement.toUtf8();
    err=sqlite3_prepare(_db, statementutf8, statementutf8.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        int rownum = 0;
        int recnum = 0;
        QString r;
        while ( sqlite3_step(vm) == SQLITE_ROW ){
            ncol = sqlite3_data_count(vm);
            for (int e=0; e<ncol; ++e){
                r = QString::fromUtf8((const char *) sqlite3_column_text(vm, e));
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
        lastErrorMessage = QString::fromUtf8((const char*)sqlite3_errmsg(_db));
    }
    return res;
}


QStringList DBBrowserDB::getBrowsableObjectNames() const
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

objectMap DBBrowserDB::getBrowsableObjects() const
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

QStringList DBBrowserDB::getIndexNames() const
{
    QList<DBBrowserObject> tmap = objMap.values("index");
    QList<DBBrowserObject>::ConstIterator it;
    QStringList res;

    for ( it = tmap.begin(); it != tmap.end(); ++it ) {
        res.append( (*it).getname() );
    }
    
    return res;
}

QStringList DBBrowserDB::getTableFields(const QString & tablename) const
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

QStringList DBBrowserDB::getTableTypes(const QString & tablename) const
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

DBBrowserObject DBBrowserDB::getObjectByName(const QString& name) const
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

int DBBrowserDB::getRecordCount() const
{
    return browseRecs.count();
}

void DBBrowserDB::logSQL(const QString& statement, int msgtype)
{
    if(mainWindow)
    {
        /*limit log message to a sensible size, this will truncate some binary messages*/
        int loglimit = 300;
        if ((statement.length() > loglimit)&&(msgtype==kLogMsg_App))
        {
            QString logst = statement;
            logst.truncate(32);
            logst.append(QObject::tr("... <string too wide to log, probably contains binary data> ..."));
            mainWindow->logSql(logst, msgtype);
        }
        else
        {
            mainWindow->logSql(statement, msgtype);
        }
    }
}

QString DBBrowserDB::getTableSQL(const QString& sTable)
{
    sqlite3_stmt *vm;
    const char *tail;
    int err;
    QString sTableSQL;

    QString statement = QString("SELECT sql FROM sqlite_master WHERE name='%1';").arg(sTable);

    QByteArray utf8Statement = statement.toUtf8();
    err=sqlite3_prepare(_db, utf8Statement, utf8Statement.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        logSQL(statement, kLogMsg_App);
        if( sqlite3_step(vm) == SQLITE_ROW )
        {
            sTableSQL = QString::fromUtf8((const char*)sqlite3_column_text(vm, 0));
        }
        sqlite3_finalize(vm);
    }else{
        qCritical() << "Unable to get sql for table: " << sTable;
    }

    return sTableSQL;
}

void DBBrowserDB::updateSchema( )
{
    sqlite3_stmt *vm;
    const char *tail;
    int err=0;

    objMap.clear();

    lastErrorMessage = QObject::tr("no error");
    QString statement = "SELECT type, name, sql FROM sqlite_master;";

    QByteArray utf8Statement = statement.toUtf8();
    err=sqlite3_prepare(_db, utf8Statement, utf8Statement.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        logSQL(statement, kLogMsg_App);
        while ( sqlite3_step(vm) == SQLITE_ROW ){
            QString val1 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 0));
            QString val2 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 1));
            QString val3 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 2));

            if(val1 == "table" || val1 == "index" || val1 == "view" || val1 == "trigger")
                objMap.insert(val1, DBBrowserObject(val2, val3, val1));
            else
                qDebug(QObject::tr("unknown object type %1").arg(val1).toStdString().c_str());
        }
        sqlite3_finalize(vm);
    }else{
        qDebug(QObject::tr("could not get list of db objects: %1, %2").arg(err).arg(sqlite3_errmsg(_db)).toStdString().c_str());
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
                    if (sqlite3_column_count(vm)==6)
                    {
                        int ispk= 0;
                        QString val1 = QString::fromUtf8((const char *) sqlite3_column_text(vm, 1));
                        QString val2 = QString::fromUtf8((const char *) sqlite3_column_text(vm, 2));
                        ispk = sqlite3_column_int(vm, 5);
                        if(ispk==1)
                            val2.append(QString(" PRIMARY KEY"));

                        (*it).addField(e, val1, val2);
                        e++;
                    }
                }
                sqlite3_finalize(vm);
            } else{
                lastErrorMessage = QObject::tr("could not get types");
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
        QProgressDialog progress(QObject::tr("Decoding CSV file..."), QObject::tr("Cancel"), 0, file.size());
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

QString DBBrowserDB::getPragma(const QString& pragma)
{
    if(!isOpen())
        return "";

    QString sql = QString("PRAGMA %1").arg(pragma);
    sqlite3_stmt* vm;
    const char* tail;
    QString retval = "";

    // Get value from DB
    int err = sqlite3_prepare(_db, sql.toUtf8(), sql.toUtf8().length(), &vm, &tail);
    if(err == SQLITE_OK){
        logSQL(sql, kLogMsg_App);
        if(sqlite3_step(vm) == SQLITE_ROW)
        {
            retval = QString::fromUtf8((const char *) sqlite3_column_text(vm, 0));
            //qDebug((pragma + ": " + retval).toStdString().c_str());
        } else {
            qDebug(QObject::tr("didn't receive any output from pragma %1").arg(pragma).toStdString().c_str());
        }
        sqlite3_finalize(vm);
    } else {
        qDebug(QObject::tr("could not execute pragma command: %1, %2").arg(err).arg(sqlite3_errmsg(_db)).toStdString().c_str());
    }

    // Return it
    return retval;
}

bool DBBrowserDB::setPragma(const QString& pragma, const QString& value)
{
    // Set the pragma value
    QString sql = QString("PRAGMA %1 = \"%2\";").arg(pragma).arg(value);
    //qDebug(sql.toStdString().c_str());

    save();
    bool res = executeSQL(sql, false, true); // PRAGMA statements are usually not transaction bound, so we can't revert
    if( !res )
        qDebug(QObject::tr("Error setting pragma %1 to %2: %3").arg(pragma).arg(value).arg(lastErrorMessage).toStdString().c_str());
    return res;
}

bool DBBrowserDB::setPragma(const QString& pragma, const QString& value, QString& originalvalue)
{
    if( originalvalue != value )
    {
        if( setPragma(pragma, value))
        {
            originalvalue = value;
            return true;
        }
    }
    return false;
}

bool DBBrowserDB::setPragma(const QString& pragma, int value, int& originalvalue)
{
    if( originalvalue != value )
    {
        QString val = QString::number(value);
        QString origval = QString::number(originalvalue);
        if( setPragma(pragma, val, origval))
        {
            originalvalue = value;
        }
    }
    return false;
}
