#include "sqlitedb.h"
#include "sqlitetablemodel.h"
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

bool DBBrowserDB::getDirty() const
{
    return !savepointList.empty();
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
            }
            curDBFilename = db;
        }

        // Enable extension loading
        sqlite3_enable_load_extension(_db, 1);
    }

    return ok;
}

bool DBBrowserDB::setRestorePoint(const QString& pointname)
{
    if(!isOpen())
        return false;
    if(savepointList.contains(pointname))
        return true;

    if(_db)
    {
        QString query = QString("SAVEPOINT %1;").arg(pointname);
        sqlite3_exec(_db, query.toUtf8(), NULL, NULL, NULL);
        savepointList.append(pointname);
        if(mainWindow) mainWindow->dbState(getDirty());
    }
    return true;
}

bool DBBrowserDB::save(const QString& pointname)
{
    if(!isOpen() || savepointList.contains(pointname) == false)
        return false;

    if(_db)
    {
        QString query = QString("RELEASE %1;").arg(pointname);
        sqlite3_exec(_db, query.toUtf8(), NULL,NULL,NULL);
        savepointList.removeAll(pointname);
        if(mainWindow) mainWindow->dbState(getDirty());
    }
    return true;
}

bool DBBrowserDB::revert(const QString& pointname)
{
    if(!isOpen() || savepointList.contains(pointname) == false)
        return false;

    if(_db)
    {
        QString query = QString("ROLLBACK TO SAVEPOINT %1;").arg(pointname);
        sqlite3_exec(_db, query.toUtf8(), NULL, NULL, NULL);
        query = QString("RELEASE %1;").arg(pointname);
        sqlite3_exec(_db, query.toUtf8(), NULL, NULL, NULL);
        savepointList.removeAll(pointname);
        if(mainWindow) mainWindow->dbState(getDirty());
    }
    return true;
}

bool DBBrowserDB::saveAll()
{
    foreach(const QString& point, savepointList)
    {
        if(!save(point))
            return false;
    }
    return true;
}

bool DBBrowserDB::revertAll()
{
    foreach(const QString& point, savepointList)
    {
        if(!revert(point))
            return false;
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
            }
            curDBFilename = db;
        }

        // Enable extension loading
        sqlite3_enable_load_extension(_db, 1);
    }

    return ok;
}


void DBBrowserDB::close (){
    if (_db)
    {
        if (getDirty())
        {
            if (QMessageBox::question( 0,
                                       QApplication::applicationName(),
                                       QObject::tr("Do you want to save the changes "
                                                   "made to the database file %1?").arg(curDBFilename),
                                       QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
                saveAll();
            else
                revertAll(); //not really necessary, I think... but will not hurt.
        }
        sqlite3_close(_db);
    }
    _db = 0;
    objMap.clear();
    savepointList.clear();
    if(mainWindow) mainWindow->dbState(getDirty());
}

bool DBBrowserDB::dump(const QString& filename)
{
    // Open file
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        // Create progress dialog. For this count the number of all table rows to be exported first;
        // this does neither take the table creation itself nor
        // indices, views or triggers into account but compared to the number of rows those should be neglectable
        unsigned int numRecordsTotal = 0, numRecordsCurrent = 0;
        QList<DBBrowserObject> tables = objMap.values("table");
        QMutableListIterator<DBBrowserObject> it(tables);
        while(it.hasNext())
        {
            it.next();

            // Remove the sqlite_stat1 table if there is one
            if(it.value().getname() == "sqlite_stat1")
            {
                it.remove();
            } else {
                // Otherwise get the number of records in this table
                SqliteTableModel tableModel(0, this);
                tableModel.setTable(it.value().getname());
                numRecordsTotal += tableModel.totalRowCount();
            }
        }
        QProgressDialog progress(QObject::tr("Exporting database to SQL file..."),
                                 QObject::tr("Cancel"), 0, numRecordsTotal);
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
            SqliteTableModel tableModel(0, this);
            tableModel.setTable((*it).getname());
            while(tableModel.canFetchMore())
                tableModel.fetchMore();

            // Dump all the content of the table
            for(int row=0;row<tableModel.totalRowCount();row++)
            {
                stream << "INSERT INTO `" << (*it).getname() << "` VALUES(";
                for(int col=1;col<tableModel.columnCount();col++)
                {
                    QString content;
                    if(tableModel.isBinary(tableModel.index(row, col)))
                    {
                        content = QString("X'%1'").arg(QString(tableModel.data(tableModel.index(row, col), Qt::EditRole).toByteArray().toHex()));
                        if(content.isNull())
                            content = "NULL";
                    } else {
                        content = tableModel.data(tableModel.index(row, col)).toString().replace("'", "''");
                        if(content.isNull())
                            content = "NULL";
                        else if(content.length() && !regexpIsNumeric.exactMatch(content))
                            content = "'" + content + "'";
                        else if(content.length() == 0)
                            content = "''";
                    }

                    stream << content;
                    if(col < tableModel.columnCount() - 1)
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
        if (dirtyDB) setRestorePoint();
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

    // Set DB to dirty/create restore point if necessary
    if(dirty)
        setRestorePoint();

    // Show progress dialog
    int statement_size = statement.size();
    QProgressDialog progress(QObject::tr("Executing SQL..."),
                             QObject::tr("Cancel"), 0, statement_size);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.show();

    // Execute the statement by looping until SQLite stops giving back a tail string
    sqlite3_stmt* vm;
    QByteArray utf8Query = statement.toUtf8();
    const char *tail = utf8Query.data();
    int res = 0;
    unsigned int line = 0;
    do
    {
        line++;
        size_t tail_length = strlen(tail);

        // Update progress dialog, keep UI responsive
        progress.setValue(statement_size - tail_length);
        qApp->processEvents();
        if(progress.wasCanceled())
        {
            lastErrorMessage = QObject::tr("Action cancelled.");
            return false;
        }

        // Execute next statement
        res = sqlite3_prepare_v2(_db, tail, tail_length, &vm, &tail);
        if(res == SQLITE_OK)
        {
            if(sqlite3_step(vm) == SQLITE_ERROR)
            {
                sqlite3_finalize(vm);
                lastErrorMessage = QObject::tr("Error in statement #%1: %2.\n"
                    "Aborting execution.").arg(line).arg(sqlite3_errmsg(_db));
                qWarning() << lastErrorMessage;
                return false;
            } else {
                sqlite3_finalize(vm);
            }
        } else {
            lastErrorMessage = QObject::tr("Error in statement #%1: %2.\n"
                "Aborting execution.").arg(line).arg(sqlite3_errmsg(_db));
            qWarning() << lastErrorMessage;
            return false;
        }
    } while(tail && *tail != 0 && (res == SQLITE_OK || res == SQLITE_DONE));

    // Exit
    return true;
}

int DBBrowserDB::addRecord(const QString& sTableName)
{
    char *errmsg;
    if (!isOpen()) return false;

    // add record is seldom called, for now this is ok
    // but if we ever going to add a lot of records
    // we should cache the parsed tables somewhere
    sqlb::Table table = sqlb::Table::parseSQL(getTableSQL(sTableName));
    QString sInsertstmt = table.emptyInsertStmt();
    lastErrorMessage = "";
    logSQL(sInsertstmt, kLogMsg_App);
    setRestorePoint();

    if (SQLITE_OK != sqlite3_exec(_db, sInsertstmt.toUtf8(), NULL, NULL, &errmsg))
    {
        lastErrorMessage = QString::fromUtf8(errmsg);
        qWarning() << "addRecord: " << lastErrorMessage;
        return -1;
    } else {
        return sqlite3_last_insert_rowid(_db);
    }
}

bool DBBrowserDB::deleteRecord(const QString& table, int rowid)
{
    char * errmsg;
    if (!isOpen()) return false;
    bool ok = false;
    lastErrorMessage = QString("no error");
    
    QString statement = QString("DELETE FROM `%1` WHERE rowid=%2;").arg(table).arg(rowid);

    if (_db){
        logSQL(statement, kLogMsg_App);
        setRestorePoint();
        if (SQLITE_OK==sqlite3_exec(_db,statement.toUtf8(),
                                    NULL,NULL,&errmsg)){
            ok=true;
        } else {
            lastErrorMessage = QString::fromUtf8(errmsg);
            qWarning() << "deleteRecord: " << lastErrorMessage;
        }
    }
    return ok;
}

bool DBBrowserDB::updateRecord(const QString& table, const QString& column, int row, const QByteArray& value)
{
    if (!isOpen()) return false;

    lastErrorMessage = QString("no error");

    QString sql = QString("UPDATE `%1` SET `%2`=? WHERE rowid=%3;").arg(table).arg(column).arg(row);

    logSQL(sql, kLogMsg_App);
    setRestorePoint();

    sqlite3_stmt* stmt;
    int success = 1;
    if(sqlite3_prepare_v2(_db, sql.toUtf8(), -1, &stmt, 0) != SQLITE_OK)
        success = 0;
    if(success == 1 && sqlite3_bind_text(stmt, 1, value.constData(), value.length(), SQLITE_STATIC) != SQLITE_OK)
        success = -1;
    if(success == 1 && sqlite3_step(stmt) != SQLITE_DONE)
        success = -1;
    if(success != 0 && sqlite3_finalize(stmt) != SQLITE_OK)
        success = -1;

    if(success == 1)
    {
        return true;
    } else {
        lastErrorMessage = sqlite3_errmsg(_db);
        qWarning() << "updateRecord: " << lastErrorMessage;
        return false;
    }
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
    return executeSQL(sql);
}

bool DBBrowserDB::addColumn(const QString& tablename, const sqlb::FieldPtr& field)
{
    QString sql = QString("ALTER TABLE `%1` ADD COLUMN %2").arg(tablename).arg(field->toString());
    return executeSQL(sql);
}

bool DBBrowserDB::renameColumn(const QString& tablename, const QString& from, const QString& to, const QString& type)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite.
    // If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql = QString("ALTER TABLE `%1` MODIFY `%2` %3").arg(tablename).arg(to).arg(type);
    //return executeSQL(sql);

    // Collect information on the current DB layout
    DBBrowserObject table = getObjectByName(tablename);
    if(table.getname() == "" || table.getField(from).getname() == "")
    {
        lastErrorMessage = QObject::tr("renameColumn: cannot find table %1 with column %2").arg(tablename).arg(from);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!executeSQL("SAVEPOINT sqlitebrowser_rename_column"))
    {
        lastErrorMessage = QObject::tr("renameColumn: creating savepoint failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create a new table with a name that hopefully doesn't exist yet.
    // Its layout is exactly the same as the one of the table to change - except for the column to change
    // of course
    QList<DBBrowserField> new_table_structure;
    for(int i=0;i<table.fldmap.count();i++)
    {
        // Is this the column to rename?
        if(table.fldmap.value(i).getname() == from)
            new_table_structure.push_back(DBBrowserField(to, type));
        else
            new_table_structure.push_back(
                        DBBrowserField(table.fldmap.value(i).getname(), table.fldmap.value(i).gettype())
            );
    }
    if(!createTable("sqlitebrowser_rename_column_new_table", new_table_structure))
    {
        lastErrorMessage = QObject::tr("renameColumn: creating new table failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Copy the data from the old table to the new one
    if(!executeSQL(QString("INSERT INTO sqlitebrowser_rename_column_new_table SELECT * FROM `%1`;").arg(tablename)))
    {
        lastErrorMessage = QObject::tr("renameColumn: copying data to new table failed. DB says:\n"
                                       "%1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_rename_column;");
        return false;
    }

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE `%1`;").arg(tablename)))
    {
        lastErrorMessage = QObject::tr("renameColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
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
        qWarning() << lastErrorMessage;
        return false;
    }

    // Success
    return true;
}

bool DBBrowserDB::dropColumn(const QString& tablename, const QString& column)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite.
    // If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql = QString("ALTER TABLE `%1` DROP COLUMN `%2`;").arg(table).arg(column);
    //return executeSQL(sql);

    // Collect information on the current DB layout
    QString sTableSql = getTableSQL(tablename);
    if(sTableSql.isEmpty())
    {
        lastErrorMessage = QObject::tr("dropColumn: cannot find table %1.").arg(tablename);
        qWarning() << lastErrorMessage;
        return false;
    }

    sqlb::Table oldSchema = sqlb::Table::parseSQL(sTableSql);

    if(oldSchema.findField(column) == -1 || oldSchema.fields().count() == 1)
    {
        lastErrorMessage = QObject::tr("dropColumn: cannot find column %1. "
            "Also you can not delete the last column").arg(column);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!executeSQL("SAVEPOINT sqlitebrowser_drop_column;", false))
    {
        lastErrorMessage = QObject::tr("dropColumn: creating savepoint failed");
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create a new table with a name that hopefully doesn't exist yet.
    // Its layout is exactly the same as the one of the table to change - except for the column to drop
    // of course. Also prepare the columns to be copied in a later step now.
    sqlb::Table newSchema = oldSchema;
    newSchema.setName("sqlitebrowser_drop_column_new_table");
    newSchema.removeField(column);

    QString select_cols;
    for(int i=0; i<newSchema.fields().count(); ++i)
    {
        select_cols.append(QString("`%1`,").arg(newSchema.fields().at(i)->name()));
    }
    // remove last ,
    select_cols.remove(select_cols.count() -1, 1);

    // create the new table
    if(!executeSQL(newSchema.sql(), false))
    {
        lastErrorMessage = QObject::tr("dropColumn: creating new table failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;", false);
        return false;
    }

    // Copy the data from the old table to the new one
    if(!executeSQL(
          QString("INSERT INTO sqlitebrowser_drop_column_new_table SELECT %1 FROM `%2`;").arg(select_cols).arg(tablename),
          false))
    {
        lastErrorMessage = QObject::tr("dropColumn: copying data to new table failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        executeSQL("ROLLBACK TO SAVEPOINT sqlitebrowser_drop_column;");
        return false;
    }

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE `%1`;").arg(tablename), false))
    {
        lastErrorMessage = QObject::tr("dropColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
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
        qWarning() << lastErrorMessage;
        return false;
    }

    // Success
    return true;
}

bool DBBrowserDB::renameTable(const QString& from_table, const QString& to_table)
{
    QString sql = QString("ALTER TABLE `%1` RENAME TO `%2`").arg(from_table, to_table);
    if(!executeSQL(sql))
    {
        QString error = QObject::tr("Error renaming table '%1' to '%2'."
            "Message from database engine:\n%3").arg(from_table).arg(to_table).arg(lastErrorMessage);
        lastErrorMessage = error;
        qWarning() << lastErrorMessage;
        return false;
    } else {
        return true;
    }
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
    err=sqlite3_prepare_v2(_db, utf8Statement, utf8Statement.length(),
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
    err=sqlite3_prepare_v2(_db, utf8Statement, utf8Statement.length(),
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
                qWarning() << QObject::tr("unknown object type %1").arg(val1);
        }
        sqlite3_finalize(vm);
    }else{
        qWarning() << QObject::tr("could not get list of db objects: %1, %2").arg(err).arg(sqlite3_errmsg(_db));
    }

    //now get the field list for each table
    objectMap::Iterator it;
    for ( it = objMap.begin(); it != objMap.end(); ++it )
    {
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            statement = QString("PRAGMA TABLE_INFO(`%1`);").arg((*it).getname());
            logSQL(statement, kLogMsg_App);
            err=sqlite3_prepare_v2(_db,statement.toUtf8(),statement.length(),
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
    int err = sqlite3_prepare_v2(_db, sql.toUtf8(), sql.toUtf8().length(), &vm, &tail);
    if(err == SQLITE_OK){
        logSQL(sql, kLogMsg_App);
        if(sqlite3_step(vm) == SQLITE_ROW)
            retval = QString::fromUtf8((const char *) sqlite3_column_text(vm, 0));
        else
            qWarning() << QObject::tr("didn't receive any output from pragma %1").arg(pragma);

        sqlite3_finalize(vm);
    } else {
        qWarning() << QObject::tr("could not execute pragma command: %1, %2").arg(err).arg(sqlite3_errmsg(_db));
    }

    // Return it
    return retval;
}

bool DBBrowserDB::setPragma(const QString& pragma, const QString& value)
{
    // Set the pragma value
    QString sql = QString("PRAGMA %1 = \"%2\";").arg(pragma).arg(value);

    save();
    bool res = executeSQL(sql, false, true); // PRAGMA statements are usually not transaction bound, so we can't revert
    if( !res )
        qWarning() << QObject::tr("Error setting pragma %1 to %2: %3").arg(pragma).arg(value).arg(lastErrorMessage);
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

bool DBBrowserDB::loadExtension(const QString& filename)
{
    if(!isOpen())
        return false;

    // Check if file exists
    if(!QFile::exists(filename))
    {
        lastErrorMessage = QObject::tr("File not found.");
        return false;
    }

    // Try to load extension
    char* error;
    if(sqlite3_load_extension(_db, filename.toUtf8(), 0, &error) == SQLITE_OK)
    {
        return true;
    } else {
        lastErrorMessage = QString::fromUtf8(error);
        sqlite3_free(error);
        return false;
    }
}
