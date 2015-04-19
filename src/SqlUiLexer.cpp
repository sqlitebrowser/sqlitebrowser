#include "SqlUiLexer.h"
#include "Qsci/qsciapis.h"

SqlUiLexer::SqlUiLexer(QObject* parent) :
    QsciLexerSQL(parent)
{
    // Setup auto completion
    autocompleteApi = new QsciAPIs(this);
    setupAutoCompletion();
    autocompleteApi->prepare();

    // Setup folding
    setFoldComments(true);
    setFoldCompact(false);
}

void SqlUiLexer::setupAutoCompletion()
{
    // Set keywords
    QStringList keywordPatterns;
    keywordPatterns
            // Keywords
            << "ABORT" << "ACTION" << "ADD" << "AFTER" << "ALL"
            << "ALTER" << "ANALYZE" << "AND" << "AS" << "ASC"
            << "ATTACH" << "AUTOINCREMENT" << "BEFORE" << "BEGIN" << "BETWEEN"
            << "BY" << "CASCADE" << "CASE" << "CAST" << "CHECK"
            << "COLLATE" << "COLUMN" << "COMMIT" << "CONFLICT" << "CONSTRAINT"
            << "CREATE" << "CROSS" << "CURRENT_DATE" << "CURRENT_TIME" << "CURRENT_TIMESTAMP"
            << "DATABASE" << "DEFAULT" << "DEFERRABLE" << "DEFERRED" << "DELETE"
            << "DESC" << "DETACH" << "DISTINCT" << "DROP" << "EACH"
            << "ELSE" << "END" << "ESCAPE" << "EXCEPT" << "EXCLUSIVE"
            << "EXISTS" << "EXPLAIN" << "FAIL" << "FOR" << "FOREIGN"
            << "FROM" << "FULL" << "GLOB" << "GROUP" << "HAVING"
            << "IF" << "IGNORE" << "IMMEDIATE" << "IN" << "INDEX"
            << "INDEXED" << "INITIALLY" << "INNER" << "INSERT" << "INSTEAD"
            << "INTERSECT" << "INTO" << "IS" << "ISNULL" << "JOIN"
            << "KEY" << "LEFT" << "LIKE" << "LIMIT" << "MATCH"
            << "NATURAL" << "NO" << "NOT" << "NOTNULL" << "NULL"
            << "OF" << "OFFSET" << "ON" << "OR" << "ORDER"
            << "OUTER" << "PLAN" << "PRAGMA" << "PRIMARY" << "QUERY"
            << "RAISE" << "RECURSIVE" << "REFERENCES" << "REGEXP" << "REINDEX" << "RELEASE"
            << "RENAME" << "REPLACE" << "RESTRICT" << "RIGHT" << "ROLLBACK"
            << "ROW" << "SAVEPOINT" << "SELECT" << "SET" << "TABLE"
            << "TEMP" << "TEMPORARY" << "THEN" << "TO" << "TRANSACTION"
            << "TRIGGER" << "UNION" << "UNIQUE" << "UPDATE" << "USING"
            << "VACUUM" << "VALUES" << "VIEW" << "VIRTUAL" << "WHEN"
            << "WHERE" << "WITH" << "WITHOUT"
            // Data types
            << "INT" << "INTEGER" << "REAL" << "TEXT" << "BLOB" << "NUMERIC" << "CHAR";
    foreach(const QString& keyword, keywordPatterns)
    {
        autocompleteApi->add(keyword + "?" + QString::number(ApiCompleterIconIdKeyword));
        autocompleteApi->add(keyword.toLower() + "?" + QString::number(ApiCompleterIconIdKeyword));
    }

    // Functions
    QStringList functionPatterns;
    functionPatterns
            // Core functions
            << "abs" + tr("(X) The abs(X) function returns the absolute value of the numeric argument X.")
            << "changes" + tr("() The changes() function returns the number of database rows that were changed or inserted or deleted by the most recently completed INSERT, DELETE, or UPDATE statement.")
            << "char" + tr("(X1,X2,...) The char(X1,X2,...,XN) function returns a string composed of characters having the unicode code point values of integers X1 through XN, respectively. ")
            << "coalesce" + tr("(X,Y,...) The coalesce() function returns a copy of its first non-NULL argument, or NULL if all arguments are NULL")
            << "glob" + tr("(X,Y) The glob(X,Y) function is equivalent to the expression \"Y GLOB X\".")
            << "ifnull" + tr("(X,Y) The ifnull() function returns a copy of its first non-NULL argument, or NULL if both arguments are NULL.")
            << "instr" + tr("(X,Y) The instr(X,Y) function finds the first occurrence of string Y within string X and returns the number of prior characters plus 1, or 0 if Y is nowhere found within X.")
            << "hex" + tr("(X) The hex() function interprets its argument as a BLOB and returns a string which is the upper-case hexadecimal rendering of the content of that blob.")
            << "last_insert_rowid" + tr("() The last_insert_rowid() function returns the ROWID of the last row insert from the database connection which invoked the function.")
            << "length" + tr("(X) For a string value X, the length(X) function returns the number of characters (not bytes) in X prior to the first NUL character.")
            << "like" + tr("(X,Y) The like() function is used to implement the \"Y LIKE X\" expression.")
            << "like" + tr("(X,Y,Z) The like() function is used to implement the \"Y LIKE X ESCAPE Z\" expression.")
            << "load_extension" + tr("(X) The load_extension(X) function loads SQLite extensions out of the shared library file named X.")
            << "load_extension" + tr("(X,Y) The load_extension(X) function loads SQLite extensions out of the shared library file named X using the entry point Y.")
            << "lower" + tr("(X) The lower(X) function returns a copy of string X with all ASCII characters converted to lower case.")
            << "ltrim" + tr("(X) ltrim(X) removes spaces from the left side of X.")
            << "ltrim" + tr("(X,Y) The ltrim(X,Y) function returns a string formed by removing any and all characters that appear in Y from the left side of X.")
            << "max" + tr("(X,Y,...) The multi-argument max() function returns the argument with the maximum value, or return NULL if any argument is NULL.")
            << "min" + tr("(X,Y,...) The multi-argument min() function returns the argument with the minimum value.")
            << "nullif" + tr("(X,Y) The nullif(X,Y) function returns its first argument if the arguments are different and NULL if the arguments are the same.")
            << "printf" + tr("(FORMAT,...) The printf(FORMAT,...) SQL function works like the sqlite3_mprintf() C-language function and the printf() function from the standard C library.")
            << "quote" + tr("(X) The quote(X) function returns the text of an SQL literal which is the value of its argument suitable for inclusion into an SQL statement.")
            << "random" + tr("() The random() function returns a pseudo-random integer between -9223372036854775808 and +9223372036854775807.")
            << "randomblob" + tr("(N) The randomblob(N) function return an N-byte blob containing pseudo-random bytes.")
            << "replace" + tr("(X,Y,Z) The replace(X,Y,Z) function returns a string formed by substituting string Z for every occurrence of string Y in string X.")
            << "round" + tr("(X) The round(X) function returns a floating-point value X rounded to zero digits to the right of the decimal point.")
            << "round" + tr("(X,Y) The round(X,Y) function returns a floating-point value X rounded to Y digits to the right of the decimal point.")
            << "rtrim" + tr("(X) rtrim(X) removes spaces from the right side of X.")
            << "rtrim" + tr("(X,Y) The rtrim(X,Y) function returns a string formed by removing any and all characters that appear in Y from the right side of X.")
            << "soundex" + tr("(X) The soundex(X) function returns a string that is the soundex encoding of the string X.")
            << "substr" + tr("(X,Y) substr(X,Y) returns all characters through the end of the string X beginning with the Y-th.")
            << "substr" + tr("(X,Y,Z) The substr(X,Y,Z) function returns a substring of input string X that begins with the Y-th character and which is Z characters long.")
            << "total_changes" + tr("() The total_changes() function returns the number of row changes caused by INSERT, UPDATE or DELETE statements since the current database connection was opened.")
            << "trim" + tr("(X) trim(X) removes spaces from both ends of X.")
            << "trim" + tr("(X,Y) The trim(X,Y) function returns a string formed by removing any and all characters that appear in Y from both ends of X.")
            << "typeof" + tr("(X) The typeof(X) function returns a string that indicates the datatype of the expression X.")
            << "unicode" + tr("(X) The unicode(X) function returns the numeric unicode code point corresponding to the first character of the string X.")
            << "upper" + tr("(X) The upper(X) function returns a copy of input string X in which all lower-case ASCII characters are converted to their upper-case equivalent.")
            << "zeroblob" + tr("(N) The zeroblob(N) function returns a BLOB consisting of N bytes of 0x00.")
            // Date and time functions
            << "date" + tr("(timestring,modifier,modifier,...)")
            << "time" + tr("(timestring,modifier,modifier,...)")
            << "datetime" + tr("(timestring,modifier,modifier,...)")
            << "julianday" + tr("(timestring,modifier,modifier,...)")
            << "strftime" + tr("(format,timestring,modifier,modifier,...)")
            // Aggregate functions
            << "avg" + tr("(X) The avg() function returns the average value of all non-NULL X within a group.")
            << "count" + tr("(X) The count(X) function returns a count of the number of times that X is not NULL in a group.")
            << "group_concat" + tr("(X) The group_concat() function returns a string which is the concatenation of all non-NULL values of X.")
            << "group_concat" + tr("(X,Y) The group_concat() function returns a string which is the concatenation of all non-NULL values of X. If parameter Y is present then it is used as the separator between instances of X.")
            << "max" + tr("(X) The max() aggregate function returns the maximum value of all values in the group.")
            << "min" + tr("(X) The min() aggregate function returns the minimum non-NULL value of all values in the group.")
            << "sum" + tr("(X) The sum() and total() aggregate functions return sum of all non-NULL values in the group.")
            << "total" + tr("(X) The sum() and total() aggregate functions return sum of all non-NULL values in the group.");

    listFunctions.clear();
    foreach(const QString& keyword, functionPatterns)
    {
        QString fn = keyword.left(keyword.indexOf('('));
        QString descr = keyword.mid(keyword.indexOf('('));

        autocompleteApi->add(fn + "?" + QString::number(ApiCompleterIconIdFunction) + descr);
        autocompleteApi->add(fn.toUpper() + "?" + QString::number(ApiCompleterIconIdFunction) + descr);

        // Store all function names in order to highlight them in a different colour
        listFunctions.append(fn);
    }
}

void SqlUiLexer::setTableNames(const TablesAndColumnsMap& tables)
{
    // Update list for auto completion
    autocompleteApi->clear();
    listTables.clear();
    setupAutoCompletion();
    for(TablesAndColumnsMap::ConstIterator it=tables.constBegin();it!=tables.constEnd();++it)
    {
        foreach(const QString& field, it.value())
            autocompleteApi->add(it.key() + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdTable) + "." +
                                 field + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdColumn));

        // Store the table name list in order to highlight them in a different colour
        listTables.append(it.key());
    }
    autocompleteApi->prepare();
}

const char* SqlUiLexer::keywords(int set) const
{
    // Function and table names are generated automatically but need to be returned to the calling functions.
    // In order to not have them deleted after this function ends they are stored as static variables. Because
    // the functions list doesn't change after the first call it's initialised here whereas the tables list, which
    // can change, is updated for each call
    static std::string functions = listFunctions.join(" ").toUtf8().constData();
    static std::string tables;

    if(set == 6)            // This corresponds to the QsciLexerSQL::KeywordSet6 style in SqlTextEdit
    {
        tables = listTables.join(" ").toLower().toUtf8().constData();
        return tables.c_str();
    } else if(set == 7) {   // This corresponds to the QsciLexerSQL::KeywordSet7 style in SqlTextEdit
        return functions.c_str();
    } else {
        // For all other keyword sets simply call the parent implementation
        return QsciLexerSQL::keywords(set);
    }
}

QStringList SqlUiLexer::autoCompletionWordSeparators() const
{
    // The only word seperator for auto completion in SQL is "." as in "tablename.columnname".
    // Because this isn't implemented in the default QScintilla SQL lexer for some reason we add it here.

    QStringList wl;
    wl << ".";
    return wl;
}
