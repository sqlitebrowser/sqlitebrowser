#include <algorithm>

#include "SqlUiLexer.h"
#include "Qsci/qsciapis.h"
#include "Settings.h"
#include "sqlitedb.h"

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
    keywordPatterns
            // Keywords
            << "ABORT" << "ACTION" << "ADD" << "AFTER" << "ALL"
            << "ALTER" << "ANALYZE" << "AND" << "AS" << "ASC"
            << "ATTACH" << "AUTOINCREMENT" << "BEFORE" << "BEGIN" << "BETWEEN"
            << "BY" << "CASCADE" << "CASE" << "CAST" << "CHECK"
            << "COLLATE" << "COLUMN" << "COMMIT" << "CONFLICT" << "CONSTRAINT"
            << "CREATE" << "CROSS" << "CURRENT" << "CURRENT_DATE" << "CURRENT_TIME" << "CURRENT_TIMESTAMP"
            << "DATABASE" << "DEFAULT" << "DEFERRABLE" << "DEFERRED" << "DELETE"
            << "DESC" << "DETACH" << "DISTINCT" << "DO" << "DROP" << "EACH"
            << "ELSE" << "END" << "ESCAPE" << "EXCEPT" << "EXCLUSIVE"
            << "EXISTS" << "EXPLAIN" << "FAIL" << "FILTER" << "FOLLOWING" << "FOR" << "FOREIGN"
            << "FROM" << "FULL" << "GLOB" << "GROUP" << "HAVING"
            << "IF" << "IGNORE" << "IMMEDIATE" << "IN" << "INDEX"
            << "INDEXED" << "INITIALLY" << "INNER" << "INSERT" << "INSTEAD"
            << "INTERSECT" << "INTO" << "IS" << "ISNULL" << "JOIN"
            << "KEY" << "LEFT" << "LIKE" << "LIMIT" << "MATCH"
            << "NATURAL" << "NO" << "NOT" << "NOTHING" << "NOTNULL" << "NULL"
            << "OF" << "OFFSET" << "ON" << "OR" << "ORDER"
            << "OUTER" << "OVER" << "PARTITION" << "PLAN" << "PRAGMA" << "PRECEDING" << "PRIMARY" << "QUERY"
            << "RAISE" << "RANGE" << "RECURSIVE" << "REFERENCES" << "REGEXP" << "REINDEX" << "RELEASE"
            << "RENAME" << "REPLACE" << "RESTRICT" << "RIGHT" << "ROLLBACK"
            << "ROWID" << "ROW" << "ROWS" << "SAVEPOINT" << "SELECT" << "SET" << "TABLE"
            << "TEMP" << "TEMPORARY" << "THEN" << "TO" << "TRANSACTION"
            << "TRIGGER" << "UNBOUNDED" << "UNION" << "UNIQUE" << "UPDATE" << "USING"
            << "VACUUM" << "VALUES" << "VIEW" << "VIRTUAL" << "WHEN"
            << "WHERE" << "WINDOW" << "WITH" << "WITHOUT"
            // Data types
            << "INT" << "INTEGER" << "REAL" << "TEXT" << "BLOB" << "NUMERIC" << "CHAR";
    bool upperKeywords = Settings::getValue("editor", "upper_keywords").toBool();
    for(const QString& keyword : keywordPatterns)
    {
        if (upperKeywords)
            autocompleteApi->add(keyword + "?" + QString::number(ApiCompleterIconIdKeyword));
        else
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
            << "load_extension" + tr("(X) The load_extension(X) function loads SQLite extensions out of the shared library file named X.\nUse of this function must be authorized from Preferences.")
            << "load_extension" + tr("(X,Y) The load_extension(X) function loads SQLite extensions out of the shared library file named X using the entry point Y.\nUse of this function must be authorized from Preferences.")
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
            << "total" + tr("(X) The sum() and total() aggregate functions return sum of all non-NULL values in the group.")
            // Window functions
            << "row_number" + tr("() The number of the row within the current partition. Rows are numbered starting from 1 in the order defined by the ORDER BY clause in the window definition, or in arbitrary order otherwise.")
            << "rank" + tr("() The row_number() of the first peer in each group - the rank of the current row with gaps. If there is no ORDER BY clause, then all rows are considered peers and this function always returns 1.")
            << "dense_rank" + tr("() The number of the current row's peer group within its partition - the rank of the current row without gaps. Partitions are numbered starting from 1 in the order defined by the ORDER BY clause in the window definition. If there is no ORDER BY clause, then all rows are considered peers and this function always returns 1. ")
            << "percent_rank" + tr("() Despite the name, this function always returns a value between 0.0 and 1.0 equal to (rank - 1)/(partition-rows - 1), where rank is the value returned by built-in window function rank() and partition-rows is the total number of rows in the partition. If the partition contains only one row, this function returns 0.0. ")
            << "cume_dist" + tr("() The cumulative distribution. Calculated as row-number/partition-rows, where row-number is the value returned by row_number() for the last peer in the group and partition-rows the number of rows in the partition.")
            << "ntile" + tr("(N) Argument N is handled as an integer. This function divides the partition into N groups as evenly as possible and assigns an integer between 1 and N to each group, in the order defined by the ORDER BY clause, or in arbitrary order otherwise. If necessary, larger groups occur first. This function returns the integer value assigned to the group that the current row is a part of.")
            << "lag" + tr("(expr) Returns the result of evaluating expression expr against the previous row in the partition. Or, if there is no previous row (because the current row is the first), NULL.")
            << "lag" + tr("(expr,offset) If the offset argument is provided, then it must be a non-negative integer. In this case the value returned is the result of evaluating expr against the row offset rows before the current row within the partition. If offset is 0, then expr is evaluated against the current row. If there is no row offset rows before the current row, NULL is returned.")
            << "lag" + tr("(expr,offset,default) If default is also provided, then it is returned instead of NULL if the row identified by offset does not exist.")
            << "lead" + tr("(expr) Returns the result of evaluating expression expr against the next row in the partition. Or, if there is no next row (because the current row is the last), NULL.")
            << "lead" + tr("(expr,offset) If the offset argument is provided, then it must be a non-negative integer. In this case the value returned is the result of evaluating expr against the row offset rows after the current row within the partition. If offset is 0, then expr is evaluated against the current row. If there is no row offset rows after the current row, NULL is returned.")
            << "lead" + tr("(expr,offset,default) If default is also provided, then it is returned instead of NULL if the row identified by offset does not exist.")
            << "first_value" + tr("(expr) This built-in window function calculates the window frame for each row in the same way as an aggregate window function. It returns the value of expr evaluated against the first row in the window frame for each row.")
            << "last_value" + tr("(expr) This built-in window function calculates the window frame for each row in the same way as an aggregate window function. It returns the value of expr evaluated against the last row in the window frame for each row.")
            << "nth_value" + tr("(expr,N) This built-in window function calculates the window frame for each row in the same way as an aggregate window function. It returns the value of expr evaluated against the row N of the window frame. Rows are numbered within the window frame starting from 1 in the order defined by the ORDER BY clause if one is present, or in arbitrary order otherwise. If there is no Nth row in the partition, then NULL is returned.")
            ;

    listFunctions.clear();
    for(const QString& keyword : functionPatterns)
    {
        QString fn = keyword.left(keyword.indexOf('('));
        QString descr = keyword.mid(keyword.indexOf('('));

        autocompleteApi->add(fn + "?" + QString::number(ApiCompleterIconIdFunction) + descr);
        autocompleteApi->add(fn.toUpper() + "?" + QString::number(ApiCompleterIconIdFunction) + descr);

        // Store all function names in order to highlight them in a different colour
        listFunctions.append(fn);
    }
}

void SqlUiLexer::setTableNames(const QualifiedTablesMap& tables)
{
    // Update list for auto completion
    autocompleteApi->clear();
    listTables.clear();
    setupAutoCompletion();
    for(auto itSchemas=tables.constBegin();itSchemas!=tables.constEnd();++itSchemas)
    {
        for(auto itTables=itSchemas.value().constBegin();itTables!=itSchemas.value().constEnd();++itTables)
        {
            // Completion for schema.table
            autocompleteApi->add(itSchemas.key() + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdSchema) + "." +
                                 itTables.key() + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdTable));

            for(const QString& field : itTables.value()) {
                // Completion for table.field
                autocompleteApi->add(itTables.key() + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdTable) + "." +
                                     field + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdColumn));

                // Completion for isolated field
                autocompleteApi->add(field + "?" + QString::number(SqlUiLexer::ApiCompleterIconIdColumn));
            }
            // Store the table name list in order to highlight them in a different colour
            listTables.append(itTables.key());
        }
    }
    autocompleteApi->prepare();
}

const char* SqlUiLexer::keywords(int set) const
{
    // Function and table names are generated automatically but need to be returned to the calling functions.
    // In order to not have them deleted after this function ends they are stored as static variables. Because
    // the keywords and functions lists don't change after the first call it's initialised here whereas the tables
    // list, which can change, is updated for each call
    static std::string sqliteKeywords = keywordPatterns.join(" ").toLower().toUtf8().constData();
    static std::string functions = listFunctions.join(" ").toUtf8().constData();
    static std::string tables;

     if(set == 1) {         // This corresponds to the QsciLexerSQL::Keyword style in SqlTextEdit
        return sqliteKeywords.c_str();
    } else if(set == 6)     // This corresponds to the QsciLexerSQL::KeywordSet6 style in SqlTextEdit
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
    // The only word separator for auto completion in SQL is "." as in "tablename.columnname".
    // Because this isn't implemented in the default QScintilla SQL lexer for some reason we add it here.
    // We also need to consider quoted identifiers as in "tablename"."columnname" with whatever quote character
    // is configured.
    QStringList wl;

    QString escapeSeparator = sqlb::escapeIdentifier(QString("."));
    // Case for non symetric quotes, e.g. "[.]" to "].["
    std::reverse(escapeSeparator.begin(), escapeSeparator.end());

    wl << "." << escapeSeparator;
    return wl;
}

bool SqlUiLexer::caseSensitive() const
{
    return false;
}
