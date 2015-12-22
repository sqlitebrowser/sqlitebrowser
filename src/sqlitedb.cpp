#include "sqlitedb.h"
#include "sqlite.h"
#include "sqlitetablemodel.h"
#include "CipherDialog.h"

#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>

// collation callbacks
int collCompare(void* /*pArg*/, int /*eTextRepA*/, const void* sA, int /*eTextRepB*/, const void* sB)
{
    size_t sizeA = strlen((const char*)sA);
    size_t sizeB = strlen((const char*)sB);

    if(sizeA == sizeB)
        return memcmp(sA, sB, sizeA);
    return sizeA - sizeB;
}

static int sqlite_compare_utf16( void* /*arg*/,int size1, const void *str1, int size2, const void* str2)
{
    const QString string1 = QString::fromRawData(reinterpret_cast<const QChar*>(str1), size1 / sizeof(QChar));
    const QString string2 = QString::fromRawData(reinterpret_cast<const QChar*>(str2), size2 / sizeof(QChar));

    return QString::compare(string1, string2, Qt::CaseSensitive);
}

static int sqlite_compare_utf16ci( void* /*arg*/,int size1, const void *str1, int size2, const void* str2)
{
    const QString string1 = QString::fromRawData(reinterpret_cast<const QChar*>(str1), size1 / sizeof(QChar));
    const QString string2 = QString::fromRawData(reinterpret_cast<const QChar*>(str2), size2 / sizeof(QChar));

    return QString::compare(string1, string2, Qt::CaseInsensitive);
}

void collation_needed(void* /*pData*/, sqlite3* db, int eTextRep, const char* sCollationName)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
                0,
                QObject::tr("Collation needed! Proceed?"),
                QObject::tr("A table in this database requires a special collation function '%1' "
                            "that this application can't provide without further knowledge.\n"
                            "If you choose to proceed, be aware bad things can happen to your database.\n"
                            "Create a backup!").arg(sCollationName), QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        sqlite3_create_collation(db, sCollationName, eTextRep, NULL, collCompare);
}


static void regexp(sqlite3_context* ctx, int /*argc*/, sqlite3_value* argv[])
{
    // Get arguments and check their values
    QRegExp arg1((const char*)sqlite3_value_text(argv[0]));
    QString arg2((const char*)sqlite3_value_text(argv[1]));
    if(!arg1.isValid())
        return sqlite3_result_error(ctx, "invalid operand", -1);

    // Set the pattern matching syntax to a Perl-like one. This is the default in Qt 4.x but Qt 5
    // changes this to a greedy one (QRegExp::RegExp2). To make sure the behaviour of our application
    // doesn't change depending on the build environment, we make sure to always set the same pattern
    // matching syntax.
    arg1.setPatternSyntax(QRegExp::RegExp);

    // Perform the actual matching and return the result. Note that Qt's QRegExp returns -1 if the regex
    // doesn't match and the position in the string otherwise; SQLite expects a 0 for not found and a 1 for found.
    sqlite3_result_int(ctx, arg1.indexIn(arg2) >= 0);
}

bool DBBrowserDB::isOpen ( ) const
{
    return _db!=0;
}

bool DBBrowserDB::getDirty() const
{
    return !savepointList.empty();
}

bool DBBrowserDB::open(const QString& db)
{
    if (isOpen()) close();

    isEncrypted = false;

    // Get encryption settings for database file
    CipherDialog* cipher = 0;
    if(tryEncryptionSettings(db, &isEncrypted, cipher) == false)
    {
        lastErrorMessage = QString::fromUtf8((const char*)sqlite3_errmsg(_db));
        return false;
    }

    // Open database file
    if(sqlite3_open_v2(db.toUtf8(), &_db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
    {
        lastErrorMessage = QString::fromUtf8((const char*)sqlite3_errmsg(_db));
        return false;
    }

    // Set encryption details if database is encrypted
#ifdef ENABLE_SQLCIPHER
    if(isEncrypted && cipher)
    {
        sqlite3_key(_db, cipher->password().toUtf8(), cipher->password().toUtf8().length());
        if(cipher->pageSize() != 1024)
            executeSQL(QString("PRAGMA cipher_page_size = %1;").arg(cipher->pageSize()), false, false);
    }
#endif
    delete cipher;

    if (_db)
    {
        // add UTF16 collation (comparison is performed by QString functions)
        sqlite3_create_collation(_db, "UTF16", SQLITE_UTF16, 0, sqlite_compare_utf16);
        // add UTF16CI (case insensitive) collation (comparison is performed by QString functions)
        sqlite3_create_collation(_db, "UTF16CI", SQLITE_UTF16, 0, sqlite_compare_utf16ci);
       
        // register collation callback
        sqlite3_collation_needed(_db, NULL, collation_needed);

        // set preference defaults
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.sync();
        bool foreignkeys = settings.value( "/db/foreignkeys", false ).toBool();
        setPragma("foreign_keys", foreignkeys ? "1" : "0");

        // Enable extension loading
        sqlite3_enable_load_extension(_db, 1);

        // Register REGEXP function
        if(settings.value("/extensions/disableregex", false) == false)
            sqlite3_create_function(_db, "REGEXP", 2, SQLITE_UTF8, NULL, regexp, NULL, NULL);

        // Check if file is read only
        QFileInfo fi(db);
        QFileInfo fid(fi.absoluteDir().absolutePath());
        isReadOnly = !fi.isWritable() || !fid.isWritable();

        // Execute default SQL
        if(!isReadOnly)
        {
            QString default_sql = settings.value( "/db/defaultsqltext", "").toString();
            if(!default_sql.isEmpty())
                executeMultiSQL(default_sql, false, true);
        }

        curDBFilename = db;
        return true;
    } else {
        return false;
    }
}

bool DBBrowserDB::attach(const QString& filename, QString attach_as)
{
    // Ask for name to be given to the attached database if none was provided
    if(attach_as.isEmpty())
        attach_as = QInputDialog::getText(0,
                                          qApp->applicationName(),
                                          tr("Please specify the database name under which you want to access the attached database")
                                          ).trimmed();
    if(attach_as.isEmpty())
        return false;

#ifdef ENABLE_SQLCIPHER
    // Try encryption settings
    CipherDialog* cipher = 0;
    bool is_encrypted;
    if(tryEncryptionSettings(filename, &is_encrypted, cipher) == false)
        return false;

    // Attach database
    QString key;
    if(cipher) key = cipher->password();
    if(!executeSQL(QString("ATTACH '%1' AS %2 KEY '%3'").arg(filename).arg(sqlb::escapeIdentifier(attach_as)).arg(key), false))
    {
        QMessageBox::warning(0, qApp->applicationName(), lastErrorMessage);
        return false;
    }
    if(cipher && cipher->pageSize() != 1024)
    {
        if(!executeSQL(QString("PRAGMA %1.cipher_page_size = %2").arg(sqlb::escapeIdentifier(attach_as)).arg(cipher->pageSize()), false))
        {
            QMessageBox::warning(0, qApp->applicationName(), lastErrorMessage);
            return false;
        }
    }
#else
    // Attach database
    if(!executeSQL(QString("ATTACH '%1' AS %2").arg(filename).arg(sqlb::escapeIdentifier(attach_as)), false))
    {
        QMessageBox::warning(0, qApp->applicationName(), lastErrorMessage);
        return false;
    }
#endif

    return true;
}

bool DBBrowserDB::tryEncryptionSettings(const QString& filename, bool* encrypted, CipherDialog*& cipherSettings)
{
    // Open database file
    sqlite3* dbHandle;
    if(sqlite3_open_v2(filename.toUtf8(), &dbHandle, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return false;

    // Try reading from database
    *encrypted = false;
    cipherSettings = 0;
    while(true)
    {
        QString statement = "SELECT COUNT(*) FROM sqlite_master;";
        QByteArray utf8Statement = statement.toUtf8();
        sqlite3_stmt* vm;
        const char* tail;
        sqlite3_prepare_v2(dbHandle, utf8Statement, utf8Statement.length(), &vm, &tail);
        if(sqlite3_step(vm) != SQLITE_ROW)
        {
            sqlite3_finalize(vm);
#ifdef ENABLE_SQLCIPHER
            delete cipherSettings;
            cipherSettings = new CipherDialog(0, false);
            if(cipherSettings->exec())
            {
                // Close and reopen database first to be in a clean state after the failed read attempt from above
                sqlite3_close(dbHandle);
                if(sqlite3_open_v2(filename.toUtf8(), &dbHandle, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
                {
                    delete cipherSettings;
                    cipherSettings = 0;
                    return false;
                }

                // Set key and, if it differs from the default value, the page size
                sqlite3_key(dbHandle, cipherSettings->password().toUtf8(), cipherSettings->password().toUtf8().length());
                if(cipherSettings->pageSize() != 1024)
                    sqlite3_exec(dbHandle, QString("PRAGMA cipher_page_size = %1;").arg(cipherSettings->pageSize()).toUtf8(), NULL, NULL, NULL);

                *encrypted = true;
            } else {
                sqlite3_close(dbHandle);
                *encrypted = false;
                delete cipherSettings;
                cipherSettings = 0;
                return false;
            }
#else
            sqlite3_close(dbHandle);
            return false;
#endif
        } else {
            sqlite3_finalize(vm);
            sqlite3_close(dbHandle);
            return true;
        }
    }
}

bool DBBrowserDB::setSavepoint(const QString& pointname)
{
    if(!isOpen())
        return false;
    if(savepointList.contains(pointname))
        return true;

    QString query = QString("SAVEPOINT %1;").arg(pointname);
    executeSQL(query, false, false);
    savepointList.append(pointname);
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::releaseSavepoint(const QString& pointname)
{
    if(!isOpen() || savepointList.contains(pointname) == false)
        return false;

    QString query = QString("RELEASE %1;").arg(pointname);
    executeSQL(query, false, false);
    savepointList.removeAll(pointname);
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::revertToSavepoint(const QString& pointname)
{
    if(!isOpen() || savepointList.contains(pointname) == false)
        return false;

    QString query = QString("ROLLBACK TO SAVEPOINT %1;").arg(pointname);
    executeSQL(query, false, false);
    query = QString("RELEASE %1;").arg(pointname);
    executeSQL(query, false, false);
    savepointList.removeAll(pointname);
    emit dbChanged(getDirty());

    return true;
}

bool DBBrowserDB::releaseAllSavepoints()
{
    foreach(const QString& point, savepointList)
    {
        if(!releaseSavepoint(point))
            return false;
    }
    executeSQL("COMMIT;", false, false);  // Just to be sure
    return true;
}

bool DBBrowserDB::revertAll()
{
    foreach(const QString& point, savepointList)
    {
        if(!revertToSavepoint(point))
            return false;
    }
    return true;
}

bool DBBrowserDB::create ( const QString & db)
{
    if (isOpen()) close();

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

    if (_db)
    {
        // set preference defaults
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.sync();
        bool foreignkeys = settings.value( "/db/foreignkeys", false ).toBool();
        setPragma("foreign_keys", foreignkeys ? "1" : "0");

        // Enable extension loading
        sqlite3_enable_load_extension(_db, 1);

        // force sqlite3 do write proper file header
        // if we don't create and drop the table we might end up
        // with a 0 byte file, if the user cancels the create table dialog
        executeSQL("CREATE TABLE notempty (id integer primary key);", false, false);
        executeSQL("DROP TABLE notempty;", false, false);
        executeSQL("COMMIT;", false, false);

        // Execute default SQL
        QString default_sql = settings.value( "/db/defaultsqltext", "").toString();
        if(!default_sql.isEmpty())
            executeMultiSQL(default_sql, false, true);

        curDBFilename = db;
        isEncrypted = false;
        isReadOnly = false;
        return true;
    } else {
        return false;
    }
}


bool DBBrowserDB::close()
{
    if(_db)
    {
        if (getDirty())
        {
            QMessageBox::StandardButton reply = QMessageBox::question(0,
                                                                      QApplication::applicationName(),
                                                                      tr("Do you want to save the changes "
                                                                         "made to the database file %1?").arg(curDBFilename),
                                                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

            // If the user clicked the cancel button stop here and return false
            if(reply == QMessageBox::Cancel)
                return false;

            // If he didn't it was either yes or no
            if(reply == QMessageBox::Yes)
                releaseAllSavepoints();
            else
                revertAll(); //not really necessary, I think... but will not hurt.
        }
        sqlite3_close(_db);
    }
    _db = 0;
    objMap.clear();
    savepointList.clear();
    emit dbChanged(getDirty());

    // Return true to tell the calling function that the closing wasn't cancelled by the user
    return true;
}

bool DBBrowserDB::dump(const QString& filename,
    const QStringList & tablesToDump,
    bool insertColNames,
    bool insertNewSyntx,
    bool exportSchemaOnly)
{
    // Open file
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        size_t numRecordsTotal = 0, numRecordsCurrent = 0;
        QList<DBBrowserObject> tables = objMap.values("table");
        QMutableListIterator<DBBrowserObject> it(tables);
        while(it.hasNext())
        {
            it.next();

            // Remove the sqlite_stat1 table if there is one
            if(it.value().getname() == "sqlite_stat1" || it.value().getname() == "sqlite_sequence")
            {
                it.remove();
            } else {
                // Otherwise get the number of records in this table
                SqliteTableModel tableModel(0, this);
                tableModel.setTable(it.value().getname());
                numRecordsTotal += tableModel.totalRowCount();
            }
        }

        QProgressDialog progress(tr("Exporting database to SQL file..."),
                                 tr("Cancel"), 0, numRecordsTotal);
        progress.setWindowModality(Qt::ApplicationModal);
        progress.show();
        qApp->processEvents();

        // Open text stream to the file
        QTextStream stream(&file);

        // Put the SQL commands in a transaction block
        stream << "BEGIN TRANSACTION;\n";

        // Loop through all tables first as they are required to generate views, indices etc. later
        for(QList<DBBrowserObject>::ConstIterator it=tables.begin();it!=tables.end();++it)
        {
            if (tablesToDump.indexOf(it->getTableName()) == -1)
                continue;

            // Write the SQL string used to create this table to the output file
            stream << it->getsql() << ";\n";

            if (exportSchemaOnly)
                continue;

            // get columns
            QStringList cols(it->table.fieldNames());

            QString sQuery = QString("SELECT * FROM %1;").arg(sqlb::escapeIdentifier(it->getTableName()));
            QByteArray utf8Query = sQuery.toUtf8();
            sqlite3_stmt *stmt;
            QString lineSep(QString(")%1\n").arg(insertNewSyntx?',':';'));

            int status = sqlite3_prepare_v2(this->_db, utf8Query.data(), utf8Query.size(), &stmt, NULL);
            if(SQLITE_OK == status)
            {
                int columns = sqlite3_column_count(stmt);
                size_t counter = 0;
                qApp->processEvents();
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    if (counter) stream << lineSep;

                    if (!insertNewSyntx || !counter)
                    {
                        stream << "INSERT INTO " << sqlb::escapeIdentifier(it->getTableName());
                        if (insertColNames)
                            stream << " (" << cols.join(",") << ")";
                        stream << " VALUES (";
                    }
                    else
                    {
                        stream << " (";
                    }

                    for (int i = 0; i < columns; ++i)
                    {
                        int fieldsize = sqlite3_column_bytes(stmt, i);
                        int fieldtype = sqlite3_column_type(stmt, i);
                        QByteArray bcontent(
                                    (const char*)sqlite3_column_blob(stmt, i),
                                    fieldsize);

                        if(bcontent.left(2048).contains('\0')) // binary check
                        {
                            stream << QString("X'%1'").arg(QString(bcontent.toHex()));
                        }
                        else
                        {
                            switch(fieldtype)
                            {
                            case SQLITE_TEXT:
                            case SQLITE_BLOB:
                                stream << "'" << bcontent.replace("'", "''") << "'";
                            break;
                            case SQLITE_NULL:
                                stream << "NULL";
                            break;
                            case SQLITE_FLOAT:
                                if(bcontent.indexOf("Inf") != -1)
                                    stream << "'" << bcontent << "'";
                                else
                                    stream << bcontent;
                            break;
                            default:
                                stream << bcontent;
                            }
                        }
                        if(i != columns - 1)
                            stream << ',';
                    }

                    progress.setValue(++numRecordsCurrent);
                    if(counter % 5000 == 0)
                        qApp->processEvents();
                    counter++;

                    if(progress.wasCanceled())
                    {
                        sqlite3_finalize(stmt);
                        file.close();
                        file.remove();
                        QApplication::restoreOverrideCursor();
                        return false;
                    }
                }
                if (counter > 0) stream << ");\n";
            }
            sqlite3_finalize(stmt);
        }

        // Now dump all the other objects
        for(objectMap::ConstIterator it=objMap.begin();it!=objMap.end();++it)
        {
            // Make sure it's not a table again
            if(it.value().gettype() == "table")
                continue;

            // Write the SQL string used to create this object to the output file
            if(!it->getsql().isEmpty())
            {
                stream << it->getsql() << ";\n";
            }
        }

        // Done
        stream << "COMMIT;\n";
        file.close();

        QApplication::restoreOverrideCursor();
        qApp->processEvents();
        return true;
    }
    return false;
}

bool DBBrowserDB::executeSQL ( const QString & statement, bool dirtyDB, bool logsql)
{
    char *errmsg;

    if (!isOpen())
        return false;

    if (logsql) logSQL(statement, kLogMsg_App);
    if (dirtyDB) setSavepoint();

    if (SQLITE_OK == sqlite3_exec(_db, statement.toUtf8(), NULL, NULL, &errmsg))
    {
        return true;
    } else {
        lastErrorMessage = QString("%1 (%2)").arg(QString::fromUtf8(errmsg)).arg(statement);
        qWarning() << "executeSQL: " << statement << "->" << errmsg;
        return false;
    }
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
        setSavepoint();

    // Show progress dialog
    int statement_size = statement.size();
    QProgressDialog progress(tr("Executing SQL..."),
                             tr("Cancel"), 0, statement_size);
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
            lastErrorMessage = tr("Action cancelled.");
            return false;
        }

        // Execute next statement
        res = sqlite3_prepare_v2(_db, tail, tail_length, &vm, &tail);
        if(res == SQLITE_OK)
        {
            if(sqlite3_step(vm) == SQLITE_ERROR)
            {
                sqlite3_finalize(vm);
                lastErrorMessage = tr("Error in statement #%1: %2.\n"
                    "Aborting execution.").arg(line).arg(sqlite3_errmsg(_db));
                qWarning() << lastErrorMessage;
                return false;
            } else {
                sqlite3_finalize(vm);
            }
        } else {
            lastErrorMessage = tr("Error in statement #%1: %2.\n"
                "Aborting execution.").arg(line).arg(sqlite3_errmsg(_db));
            qWarning() << lastErrorMessage;
            return false;
        }
    } while(tail && *tail != 0 && (res == SQLITE_OK || res == SQLITE_DONE));

    // Exit
    return true;
}

bool DBBrowserDB::getRow(const QString& sTableName, const QString& rowid, QList<QByteArray>& rowdata)
{
    QString sQuery = QString("SELECT * FROM %1 WHERE %2='%3';")
            .arg(sqlb::escapeIdentifier(sTableName))
            .arg(sqlb::escapeIdentifier(getObjectByName(sTableName).table.rowidColumn()))
            .arg(rowid);

    QByteArray utf8Query = sQuery.toUtf8();
    sqlite3_stmt *stmt;
    bool ret = false;

    int status = sqlite3_prepare_v2(_db, utf8Query, utf8Query.size(), &stmt, NULL);
    if(SQLITE_OK == status)
    {
        // even this is a while loop, the statement should always only return 1 row
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            for (int i = 0; i < sqlite3_column_count(stmt); ++i)
            {
                if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                    rowdata.append(QByteArray());
                } else {
                    int bytes = sqlite3_column_bytes(stmt, i);
                    if(bytes)
                        rowdata.append(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, i)), bytes));
                    else
                        rowdata.append(QByteArray(""));
                }
            }
            ret = true;
        }
    }
    sqlite3_finalize(stmt);

    return ret;
}

QString DBBrowserDB::max(const sqlb::Table& t, sqlb::FieldPtr field) const
{
    QString sQuery = QString("SELECT MAX(CAST(%2 AS INTEGER)) FROM %1;").arg(sqlb::escapeIdentifier(t.name())).arg(sqlb::escapeIdentifier(field->name()));
    QByteArray utf8Query = sQuery.toUtf8();
    sqlite3_stmt *stmt;
    QString ret = "0";

    int status = sqlite3_prepare_v2(_db, utf8Query, utf8Query.size(), &stmt, NULL);
    if(SQLITE_OK == status)
    {
        // even this is a while loop, the statement should always only return 1 row
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            if(sqlite3_column_count(stmt) == 1)
            {
                ret = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            }
        }
    }
    sqlite3_finalize(stmt);

    return ret;
}

QString DBBrowserDB::emptyInsertStmt(const sqlb::Table& t, const QString& pk_value) const
{
    QString stmt = QString("INSERT INTO %1").arg(sqlb::escapeIdentifier(t.name()));

    QStringList vals;
    QStringList fields;
    foreach(sqlb::FieldPtr f, t.fields()) {
        if(f->primaryKey()) {
            fields << f->name();

            if(!pk_value.isNull())
            {
                vals << pk_value;
            } else {
                if(f->notnull())
                {
                    QString maxval = this->max(t, f);
                    vals << QString::number(maxval.toLongLong() + 1);
                }
                else
                {
                    vals << "NULL";
                }
            }
        } else if(f->notnull() && f->defaultValue().length() == 0) {
            fields << f->name();

            if(f->isInteger())
                vals << "0";
            else
                vals << "''";
        } else {
            // don't insert into fields with a default value
            // or we will never see it.
            if(f->defaultValue().length() == 0)
            {
                fields << f->name();
                vals << "NULL";
            }
        }
    }

    if(fields.empty())
    {
        stmt.append(" DEFAULT VALUES;");
    } else {
        stmt.append("(");
        foreach(const QString& f, fields)
            stmt.append(sqlb::escapeIdentifier(f) + ",");
        stmt.chop(1);
        stmt.append(") VALUES (");
        stmt.append(vals.join(","));
        stmt.append(");");
    }

    return stmt;
}

QString DBBrowserDB::addRecord(const QString& sTableName)
{
    if (!isOpen()) return QString();

    sqlb::Table table = getObjectByName(sTableName).table;

    // For tables without rowid we have to set the primary key by ourselves. We do so by querying for the largest value in the PK column
    // and adding one to it.
    QString sInsertstmt;
    QString pk_value;
    if(table.isWithoutRowidTable())
    {
        pk_value = QString::number(max(table, table.fields().at(table.findField(table.rowidColumn()))).toLongLong() + 1);
        sInsertstmt = emptyInsertStmt(table, pk_value);
    } else {
        sInsertstmt = emptyInsertStmt(table);
    }

    if(!executeSQL(sInsertstmt))
    {
        qWarning() << "addRecord: " << lastErrorMessage;
        return QString();
    } else {
        if(table.isWithoutRowidTable())
            return pk_value;
        else
            return QString::number(sqlite3_last_insert_rowid(_db));
    }
}

bool DBBrowserDB::deleteRecord(const QString& table, const QString& rowid)
{
    if (!isOpen()) return false;
    bool ok = false;

    QString statement = QString("DELETE FROM %1 WHERE %2='%3';")
            .arg(sqlb::escapeIdentifier(table))
            .arg(sqlb::escapeIdentifier(getObjectByName(table).table.rowidColumn()))
            .arg(rowid);
    if(executeSQL(statement))
        ok = true;
    else
        qWarning() << "deleteRecord: " << lastErrorMessage;

    return ok;
}

bool DBBrowserDB::updateRecord(const QString& table, const QString& column, const QString& rowid, const QByteArray& value, bool itsBlob)
{
    if (!isOpen()) return false;

    QString sql = QString("UPDATE %1 SET %2=? WHERE %3='%4';")
            .arg(sqlb::escapeIdentifier(table))
            .arg(sqlb::escapeIdentifier(column))
            .arg(sqlb::escapeIdentifier(getObjectByName(table).table.rowidColumn()))
            .arg(rowid);

    logSQL(sql, kLogMsg_App);
    setSavepoint();

    // If we get a NULL QByteArray we insert a NULL value, and for that
    // we can pass NULL to sqlite3_bind_text() so that it behaves like sqlite3_bind_null()
    const char *rawValue = value.isNull() ? NULL : value.constData();

    sqlite3_stmt* stmt;
    int success = 1;
    if(sqlite3_prepare_v2(_db, sql.toUtf8(), -1, &stmt, 0) != SQLITE_OK)
        success = 0;
    if(success == 1) {
        if(itsBlob)
        {
            if(sqlite3_bind_blob(stmt, 1, rawValue, value.length(), SQLITE_STATIC))
                success = -1;
        }
        else
        {
            if(sqlite3_bind_text(stmt, 1, rawValue, value.length(), SQLITE_STATIC))
                success = -1;
        }
    }
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

bool DBBrowserDB::createTable(const QString& name, const sqlb::FieldVector& structure)
{
    // Build SQL statement
    sqlb::Table table(name);
    for(int i=0;i<structure.size();i++)
        table.addField(structure.at(i));

    // Execute it and update the schema
    bool result = executeSQL(table.sql());
    updateSchema();
    return result;
}

bool DBBrowserDB::addColumn(const QString& tablename, const sqlb::FieldPtr& field)
{
    QString sql = QString("ALTER TABLE %1 ADD COLUMN %2").arg(sqlb::escapeIdentifier(tablename)).arg(field->toString());

    // Execute it and update the schema
    bool result = executeSQL(sql);
    updateSchema();
    return result;
}

bool DBBrowserDB::renameColumn(const QString& tablename, const QString& name, sqlb::FieldPtr to, int move)
{
    // NOTE: This function is working around the incomplete ALTER TABLE command in SQLite.
    // If SQLite should fully support this command one day, this entire
    // function can be changed to executing something like this:
    //QString sql;
    //if(to.isNull())
    //    sql = QString("ALTER TABLE %1 DROP COLUMN %2;").arg(sqlb::escapeIdentifier(table)).arg(sqlb::escapeIdentifier(column));
    //else
    //    sql = QString("ALTER TABLE %1 MODIFY %2 %3").arg(sqlb::escapeIdentifier(tablename)).arg(sqlb::escapeIdentifier(to)).arg(type);    // This is wrong...
    //return executeSQL(sql);

    // Collect information on the current DB layout
    QString tableSql = getObjectByName(tablename).getsql();
    if(tableSql.isEmpty())
    {
        lastErrorMessage = tr("renameColumn: cannot find table %1.").arg(tablename);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create table schema
    sqlb::Table oldSchema = sqlb::Table::parseSQL(tableSql).first;

    // Check if field actually exists
    if(oldSchema.findField(name) == -1)
    {
        lastErrorMessage = tr("renameColumn: cannot find column %1.").arg(name);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create savepoint to be able to go back to it in case of any error
    if(!setSavepoint("sqlitebrowser_rename_column"))
    {
        lastErrorMessage = tr("renameColumn: creating savepoint failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Create a new table with a name that hopefully doesn't exist yet.
    // Its layout is exactly the same as the one of the table to change - except for the column to change
    // of course
    sqlb::Table newSchema = oldSchema;
    newSchema.setName("sqlitebrowser_rename_column_new_table");
    QString select_cols;
    if(to.isNull())
    {
        // We want drop the column - so just remove the field
        newSchema.removeField(name);

        for(int i=0;i<newSchema.fields().count();++i)
            select_cols.append(sqlb::escapeIdentifier(newSchema.fields().at(i)->name()) + ',');
        select_cols.chop(1);    // remove last comma
    } else {
        // We want to modify it

        // Move field
        int index = newSchema.findField(name);
        sqlb::FieldPtr temp = newSchema.fields().at(index);
        newSchema.setField(index, newSchema.fields().at(index + move));
        newSchema.setField(index + move, temp);

        // Get names of fields to select from old table now - after the field has been moved and before it might be renamed
        for(int i=0;i<newSchema.fields().count();++i)
            select_cols.append(sqlb::escapeIdentifier(newSchema.fields().at(i)->name()) + ',');
        select_cols.chop(1);    // remove last comma

        // Modify field
        newSchema.setField(index + move, to);
    }

    // Create the new table
    if(!executeSQL(newSchema.sql()))
    {
        QString error(tr("renameColumn: creating new table failed. DB says: %1").arg(lastErrorMessage));
        qWarning() << error;
        revertToSavepoint("sqlitebrowser_rename_column");
        lastErrorMessage = error;
        return false;
    }

    // Copy the data from the old table to the new one
    if(!executeSQL(QString("INSERT INTO sqlitebrowser_rename_column_new_table SELECT %1 FROM %2;").arg(select_cols).arg(sqlb::escapeIdentifier(tablename))))
    {
        QString error(tr("renameColumn: copying data to new table failed. DB says:\n%1").arg(lastErrorMessage));
        qWarning() << error;
        revertToSavepoint("sqlitebrowser_rename_column");
        lastErrorMessage = error;
        return false;
    }

    // Save all indices, triggers and views associated with this table because SQLite deletes them when we drop the table in the next step
    QString otherObjectsSql;
    for(objectMap::ConstIterator it=objMap.begin();it!=objMap.end();++it)
    {
        // If this object references the table and it's not the table itself save it's SQL string
        if((*it).getTableName() == tablename && (*it).gettype() != "table")
            otherObjectsSql += (*it).getsql() + "\n";
    }

    // Store the current foreign key settings and then disable the foreign keys being enforced to make sure the table can be dropped without errors
    QString foreignKeysOldSettings = getPragma("foreign_keys");
    setPragma("foreign_keys", "0");

    // Delete the old table
    if(!executeSQL(QString("DROP TABLE %1;").arg(sqlb::escapeIdentifier(tablename))))
    {
        QString error(tr("renameColumn: deleting old table failed. DB says: %1").arg(lastErrorMessage));
        qWarning() << error;
        revertToSavepoint("sqlitebrowser_rename_column");
        lastErrorMessage = error;
        return false;
    }

    // Rename the temporary table
    if(!renameTable("sqlitebrowser_rename_column_new_table", tablename))
    {
        revertToSavepoint("sqlitebrowser_rename_column");
        return false;
    }

    // Restore the former foreign key settings
    setPragma("foreign_keys", foreignKeysOldSettings);

    // Restore the saved triggers, views and indices
    if(!executeMultiSQL(otherObjectsSql, true, true))
    {
        QMessageBox::information(0, qApp->applicationName(), tr("Restoring some of the objects associated with this table failed. "
                                                                "This is most likely because some column names changed. "
                                                                "Here's the SQL statement which you might want to fix and execute manually:\n\n")
                                 + otherObjectsSql);
    }

    // Release the savepoint - everything went fine
    if(!releaseSavepoint("sqlitebrowser_rename_column"))
    {
        lastErrorMessage = tr("renameColumn: releasing savepoint failed. DB says: %1").arg(lastErrorMessage);
        qWarning() << lastErrorMessage;
        return false;
    }

    // Success, update the DB schema before returning
    updateSchema();
    return true;
}

bool DBBrowserDB::renameTable(const QString& from_table, const QString& to_table)
{
    QString sql = QString("ALTER TABLE %1 RENAME TO %2").arg(sqlb::escapeIdentifier(from_table)).arg(sqlb::escapeIdentifier(to_table));
    if(!executeSQL(sql))
    {
        QString error = tr("Error renaming table '%1' to '%2'."
            "Message from database engine:\n%3").arg(from_table).arg(to_table).arg(lastErrorMessage);
        lastErrorMessage = error;
        qWarning() << lastErrorMessage;
        return false;
    } else {
        updateSchema();
        return true;
    }
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

DBBrowserObject DBBrowserDB::getObjectByName(const QString& name) const
{
    for (objectMap::ConstIterator it = objMap.begin(); it != objMap.end(); ++it )
    {
        if((*it).getname() == name)
            return *it;
    }
    return DBBrowserObject();
}

void DBBrowserDB::logSQL(QString statement, int msgtype)
{
    // Remove any leading and trailing spaces, tabs, or line breaks first
    statement = statement.trimmed();

    // Replace binary log messages by a placeholder text instead of printing gibberish
    for(int i=0;i<statement.size();i++)
    {
        QChar ch = statement[i];
        if(ch < 32 && ch != '\n' && ch != '\r' && ch != '\t')
        {
            statement.truncate(i>0?i-1:0);
            statement.append(tr("... <string can not be logged, contains binary data> ..."));

            // early exit if we detect a binary character,
            // to prevent checking all characters in a potential big string
            break;
        }
    }

    emit sqlExecuted(statement, msgtype);
}

void DBBrowserDB::updateSchema( )
{
    sqlite3_stmt *vm;
    const char *tail;
    int err=0;

    objMap.clear();

    // Exit here is no DB is opened
    if(!isOpen())
        return;

    QString statement = "SELECT type,name,sql,tbl_name FROM sqlite_master UNION SELECT type,name,sql,tbl_name FROM sqlite_temp_master;";

    QByteArray utf8Statement = statement.toUtf8();
    err=sqlite3_prepare_v2(_db, utf8Statement, utf8Statement.length(),
                        &vm, &tail);
    if (err == SQLITE_OK){
        logSQL(statement, kLogMsg_App);
        while ( sqlite3_step(vm) == SQLITE_ROW ){
            QString val1 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 0));
            QString val2 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 1));
            QString val3 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 2));
            QString val4 = QString::fromUtf8((const char*)sqlite3_column_text(vm, 3));
            val3.replace("\r", "");

            if(val1 == "table" || val1 == "index" || val1 == "view" || val1 == "trigger")
                objMap.insert(val1, DBBrowserObject(val2, val3, val1, val4));
            else
                qWarning() << tr("unknown object type %1").arg(val1);
        }
        sqlite3_finalize(vm);
    }else{
        qWarning() << tr("could not get list of db objects: %1, %2").arg(err).arg(sqlite3_errmsg(_db));
    }

    //now get the field list for each table
    objectMap::Iterator it;
    for ( it = objMap.begin(); it != objMap.end(); ++it )
    {
        // Use our SQL parser to generate the field list for tables. For views we currently have to fall back to the
        // pragma SQLite offers.
        if((*it).gettype() == "table")
        {
            (*it).table = sqlb::Table::parseSQL((*it).getsql()).first;
        } else if((*it).gettype() == "view") {
            statement = QString("PRAGMA TABLE_INFO(%1);").arg(sqlb::escapeIdentifier((*it).getname()));
            logSQL(statement, kLogMsg_App);
            err=sqlite3_prepare_v2(_db,statement.toUtf8(),statement.length(),
                                &vm, &tail);
            if (err == SQLITE_OK){
                while ( sqlite3_step(vm) == SQLITE_ROW ){
                    if (sqlite3_column_count(vm)==6)
                    {
                        QString val_name = QString::fromUtf8((const char *)sqlite3_column_text(vm, 1));
                        QString val_type = QString::fromUtf8((const char *)sqlite3_column_text(vm, 2));

                        sqlb::FieldPtr f(new sqlb::Field(val_name, val_type));
                        (*it).table.addField(f);
                    }
                }
                sqlite3_finalize(vm);
            } else{
                lastErrorMessage = tr("could not get types");
            }
        }
    }
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
            qWarning() << tr("didn't receive any output from pragma %1").arg(pragma);

        sqlite3_finalize(vm);
    } else {
        qWarning() << tr("could not execute pragma command: %1, %2").arg(err).arg(sqlite3_errmsg(_db));
    }

    // Return it
    return retval;
}

bool DBBrowserDB::setPragma(const QString& pragma, const QString& value)
{
    // Set the pragma value
    QString sql = QString("PRAGMA %1 = \"%2\";").arg(pragma).arg(value);

    releaseSavepoint();
    bool res = executeSQL(sql, false, true); // PRAGMA statements are usually not transaction bound, so we can't revert
    if( !res )
        qWarning() << tr("Error setting pragma %1 to %2: %3").arg(pragma).arg(value).arg(lastErrorMessage);
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
        lastErrorMessage = tr("File not found.");
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
