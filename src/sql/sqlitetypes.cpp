#include "sqlitetypes.h"
#include "ObjectIdentifier.h"
#include "grammar/Sqlite3Lexer.hpp"
#include "grammar/Sqlite3Parser.hpp"

#include <clocale>          // This include seems to only be necessary for the Windows build
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>

namespace {
bool starts_with_ci(const std::string& str, const std::string& with)
{
    if(str.size() < with.size())
        return false;
    else
        return compare_ci(str.substr(0, with.size()), with);
}
}

namespace sqlb {

StringVector escapeIdentifier(StringVector ids)
{
    std::transform(ids.begin(), ids.end(), ids.begin(), [](const std::string& id) {
        return escapeIdentifier(id);
    });
    return ids;
}

std::string joinStringVector(const StringVector& vec, const std::string& delim)
{
    return std::accumulate(vec.begin(), vec.end(), std::string(), [delim](const std::string& so_far, const std::string& s) {
        return so_far.empty() ? s : so_far + delim + s;
    });
}

/**
 * @brief The SetLocaleToC class
 * This is a stupid helper class which sets the current locale as used by the C++ standard library to the C locale.
 * Upon destruction it resets it to whatever the previous locale was. This is used to work around a problem in Antlr's
 * string comparison which because it is case-independent relies on the current locale. However, when parsind SQL
 * statements we don't want the locale to interfere here. Especially the Turkish locale is problematic here because
 * of the dotted I problem.
 */
class SetLocaleToC
{
public:
    SetLocaleToC()
        : oldLocale(std::setlocale(LC_CTYPE, nullptr))      // Query current locale and save it
    {
        // Set locale for standard library functions
        std::setlocale(LC_CTYPE, "C.UTF-8");
    }

    ~SetLocaleToC()
    {
        // Reset old locale
        std::setlocale(LC_CTYPE, oldLocale.c_str());
    }

private:
    std::string oldLocale;
};

/**
 * @brief The CreateTableWalker class
 * Goes trough the createtable AST and returns
 * Table object.
 */
class CreateTableWalker
{
public:
    explicit CreateTableWalker(antlr::RefAST r)
        : m_root(r)
    {}

    TablePtr table();

private:
    void parsecolumn(Table* table, antlr::RefAST c);
    std::string parseConflictClause(antlr::RefAST c);

private:
    antlr::RefAST m_root;
};

/**
 * @brief The CreateIndexWalker class
 * Goes trough the createtable AST and returns
 * Index object.
 */
class CreateIndexWalker
{
public:
    explicit CreateIndexWalker(antlr::RefAST r)
        : m_root(r)
    {}

    IndexPtr index();

private:
    void parsecolumn(Index* index, antlr::RefAST c);

private:
    antlr::RefAST m_root;
};

bool Object::operator==(const Object& rhs) const
{
    if(m_name != rhs.m_name)
        return false;
    if(m_fullyParsed != rhs.m_fullyParsed)  // We check for the fully parsed flag to make sure not to lose anything in some corner cases
        return false;

    // We don't care about the original SQL text

    return true;
}

std::string Object::typeToString(Types type)
{
    switch(type)
    {
    case Types::Table: return "table";
    case Types::Index: return "index";
    case Types::View: return "view";
    case Types::Trigger: return "trigger";
    }
    return "";
}

bool ForeignKeyClause::isSet() const
{
    return m_override.size() || m_table.size();
}

std::string ForeignKeyClause::toString() const
{
    if(!isSet())
        return "";

    if(m_override.size())
        return m_override;

    std::string result = escapeIdentifier(m_table);

    if(m_columns.size())
        result += "(" + joinStringVector(escapeIdentifier(m_columns), ",") + ")";

    if(m_constraint.size())
        result += " " + m_constraint;

    return result;
}

void ForeignKeyClause::setFromString(const std::string& fk)
{
    m_override = fk;
}

std::string ForeignKeyClause::toSql(const StringVector& applyOn) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "FOREIGN KEY(" + joinStringVector(escapeIdentifier(applyOn), ",") + ") REFERENCES " + this->toString();

    return result;
}

std::string UniqueConstraint::toSql(const StringVector& applyOn) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "UNIQUE(" + joinStringVector(escapeIdentifier(applyOn), ",") + ")";

    return result;
}

std::string PrimaryKeyConstraint::toSql(const StringVector& applyOn) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "PRIMARY KEY(" + joinStringVector(escapeIdentifier(applyOn), ",") + ")";

    if(!m_conflictAction.empty())
        result += " ON CONFLICT " + m_conflictAction;

    return result;
}

std::string CheckConstraint::toSql(const StringVector&) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "CHECK(" + m_expression + ")";

    return result;
}

bool Field::operator==(const Field& rhs) const
{
    if(m_name != rhs.m_name)
        return false;
    if(m_type != rhs.m_type)
        return false;
    if(m_notnull != rhs.m_notnull)
        return false;
    if(m_check != rhs.m_check)
        return false;
    if(m_defaultvalue != rhs.m_defaultvalue)
        return false;
    if(m_autoincrement != rhs.m_autoincrement)
        return false;
    if(m_unique != rhs.m_unique)
        return false;
    if(m_collation != rhs.m_collation)
        return false;

    return true;
}

std::string Field::toString(const std::string& indent, const std::string& sep) const
{
    std::string str = indent + escapeIdentifier(m_name) + sep + m_type;
    if(m_notnull)
        str += " NOT NULL";
    if(!m_defaultvalue.empty())
        str += " DEFAULT " + m_defaultvalue;
    if(!m_check.empty())
        str += " CHECK(" + m_check + ")";
    if(m_autoincrement)
        str += " PRIMARY KEY AUTOINCREMENT";
    if(m_unique)
        str += " UNIQUE";
    if(!m_collation.empty())
        str += " COLLATE " + m_collation;
    return str;
}

bool Field::isText() const
{
    if(starts_with_ci(m_type, "character")) return true;
    if(starts_with_ci(m_type, "varchar")) return true;
    if(starts_with_ci(m_type, "varying character")) return true;
    if(starts_with_ci(m_type, "nchar")) return true;
    if(starts_with_ci(m_type, "native character")) return true;
    if(starts_with_ci(m_type, "nvarchar")) return true;
    if(compare_ci(m_type, "text")) return true;
    if(compare_ci(m_type, "clob")) return true;
    return false;
}

bool Field::isInteger() const
{
    if(compare_ci(m_type, "int")) return true;
    if(compare_ci(m_type, "integer")) return true;
    if(compare_ci(m_type, "tinyint")) return true;
    if(compare_ci(m_type, "smallint")) return true;
    if(compare_ci(m_type, "mediumint")) return true;
    if(compare_ci(m_type, "bigint")) return true;
    if(compare_ci(m_type, "unsigned big int")) return true;
    if(compare_ci(m_type, "int2")) return true;
    if(compare_ci(m_type, "int8")) return true;
    return false;
}

bool Field::isReal() const
{
    if(compare_ci(m_type, "real")) return true;
    if(compare_ci(m_type, "double")) return true;
    if(compare_ci(m_type, "double precision")) return true;
    if(compare_ci(m_type, "float")) return true;
    return false;
}

bool Field::isNumeric() const
{
    if(starts_with_ci(m_type, "decimal")) return true;
    if(compare_ci(m_type, "numeric")) return true;
    if(compare_ci(m_type, "boolean")) return true;
    if(compare_ci(m_type, "date")) return true;
    if(compare_ci(m_type, "datetime")) return true;
    return false;
}

bool Field::isBlob() const
{
    if(m_type.empty()) return true;
    if(compare_ci(m_type, "blob")) return true;
    return false;
}

std::string Field::affinity() const
{
    if (isInteger()) return "INTEGER";

    if (isText()) return "TEXT";

    if (isBlob()) return "BLOB";

    if (isReal()) return "REAL";

    return "NUMERIC";
}

Table& Table::operator=(const Table& rhs)
{
    // Base class
    Object::operator=(rhs);

    // Just assign the simple values
    m_withoutRowid = rhs.m_withoutRowid;
    m_virtual = rhs.m_virtual;

    // Clear the fields and the constraints first in order to avoid duplicates and/or old data in the next step
    fields.clear();
    m_constraints.clear();

    // Make copies of the fields and the constraints. This is necessary in order to avoid any unwanted changes to the application's main database
    // schema representation just by modifying a reference to the fields or constraints and thinking it operates on a copy.
    std::copy(rhs.fields.begin(), rhs.fields.end(), std::back_inserter(fields));
    m_constraints = rhs.m_constraints;

    return *this;
}

bool Table::operator==(const Table& rhs) const
{
    if(!Object::operator==(rhs))
        return false;

    if(m_withoutRowid != rhs.m_withoutRowid)
        return false;
    if(m_virtual != rhs.m_virtual)
        return false;
    if(fields != rhs.fields)
        return false;

    // We need to compare the constraint maps manually here. The reason is that the values are pointers and the default implementation
    // would compare the pointers not the actual objects.
    if(m_constraints.size() != rhs.m_constraints.size())
        return false;
    for(auto it=m_constraints.cbegin();it!=m_constraints.end();++it)
    {
        // For each element in this map we get the list of all elements with the same key from the other map.
        // Then we loop through that list and check if we find an element of the same type which produces the same SQL substring. We use this
        // approach to avoid casting both objects to their actual type, then dereferencing it etc.
        auto range = rhs.m_constraints.equal_range(it->first);
        bool found_something = false;
        for(auto jt=range.first;jt!=range.second;++jt)
        {
            if(it->second->type() == jt->second->type() && it->second->toSql(it->first) == jt->second->toSql(jt->first))
            {
                found_something = true;
                break;
            }
        }

        // If no match was found, the constraint maps aren't equal
        if(!found_something)
            return false;
    }

    return true;
}

StringVector Table::fieldList() const
{
    StringVector sl;

    for(const Field& f : fields)
        sl.push_back(f.toString());

    return sl;
}

StringVector Table::fieldNames() const
{
    StringVector sl;

    for(const Field& f : fields)
        sl.push_back(f.name());

    return sl;
}

StringVector Table::rowidColumns() const
{
    // For WITHOUT ROWID tables this function returns the names of the primary key column. For ordinary tables with a rowid column, it returns "_rowid_"
    if(m_withoutRowid)
        return primaryKey();
    else
        return {"_rowid_"};
}

FieldInfoList Table::fieldInformation() const
{
    FieldInfoList result;
    for(const Field& f : fields)
        result.emplace_back(f.name(), f.type(), f.toString("  ", " "));
    return result;
}

bool Table::hasAutoIncrement() const
{
    return std::any_of(fields.begin(), fields.end(), [](const Field& f) {return f.autoIncrement(); });
}

TablePtr Table::parseSQL(const std::string& sSQL)
{
    SetLocaleToC locale;

    std::stringstream s;
    s << sSQL;
    Sqlite3Lexer lex(s);

    Sqlite3Parser parser(lex);

    antlr::ASTFactory ast_factory;
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    try
    {
        if(sSQL.find("[ blank]") != sSQL.npos)
        {
            int a = 0;
            a++;
        }

        parser.createtable();
        CreateTableWalker ctw(parser.getAST());

        auto t = ctw.table();
        t->setOriginalSql(sSQL);
        return t;
    }
    catch(antlr::ANTLRException& ex)
    {
        std::cerr << "Sqlite parse error: " << ex.toString() << "(" << sSQL << ")" << std::endl;
    }
    catch(...)
    {
        std::cerr << "Sqlite parse error: " << sSQL << std::endl; //TODO
    }

    return TablePtr(new Table(""));
}

std::string Table::sql(const std::string& schema, bool ifNotExists) const
{
    // Special handling for virtual tables: just build an easy create statement and copy the using part in there
    if(isVirtual())
        return "CREATE VIRTUAL TABLE " + ObjectIdentifier(schema, m_name).toString(true) + " USING " +  m_virtual + ";";

    // This is a normal table, not a virtual one
    std::string sql = "CREATE TABLE ";
    if(ifNotExists)
        sql += "IF NOT EXISTS ";
    sql += ObjectIdentifier(schema, m_name).toString(true);
    sql += " (\n";

    sql += joinStringVector(fieldList(), ",\n");

    // Constraints
    ConstraintMap::const_iterator it = m_constraints.cbegin();
    bool autoincrement = hasAutoIncrement();
    while(it != m_constraints.cend())
    {
        // Ignore auto increment primary key constraint
        if((!autoincrement || it->second->type() != Constraint::PrimaryKeyConstraintType))
        {
            // Ignore all constraints without any fields, except for check constraints which don't rely on a field vector
            if(!(it->first.empty() && it->second->type() != Constraint::CheckConstraintType))
            {
                sql += ",\n\t";
                sql += it->second->toSql(it->first);
            }
        }
        ++it;
    }

    sql += "\n)";

    // without rowid
    if(withoutRowidTable())
        sql += " WITHOUT ROWID";

    return sql + ";";
}

void Table::addConstraint(const StringVector& vStrFields, ConstraintPtr constraint)
{
    m_constraints.insert({vStrFields, constraint});
}

void Table::setConstraint(const StringVector& vStrFields, ConstraintPtr constraint)
{
    // Delete any old constraints of this type for these fields
    removeConstraints(vStrFields, constraint->type());

    // Add the new constraint to the table, effectively overwriting all old constraints for that fields/type combination
    addConstraint(vStrFields, constraint);
}

void Table::removeConstraints(const StringVector& vStrFields, Constraint::ConstraintTypes type)
{
    for(auto it = m_constraints.begin();it!=m_constraints.end();)
    {
        if(it->first == vStrFields && it->second->type() == type)
            m_constraints.erase(it++);
        else
            ++it;
    }
}

ConstraintPtr Table::constraint(const StringVector& vStrFields, Constraint::ConstraintTypes type) const
{
    auto list = constraints(vStrFields, type);
    if(list.size())
        return list.at(0);
    else
        return ConstraintPtr(nullptr);
}

std::vector<ConstraintPtr> Table::constraints(const StringVector& vStrFields, Constraint::ConstraintTypes type) const
{
    ConstraintMap::const_iterator begin, end;
    if(vStrFields.empty())
    {
        begin = m_constraints.begin();
        end = m_constraints.end();
    } else {
        std::tie(begin, end) = m_constraints.equal_range(vStrFields);
    }

    std::vector<ConstraintPtr> clist;
    std::transform(begin, end, std::back_inserter(clist), [](std::pair<StringVector, ConstraintPtr> elem){return elem.second;});

    if(type == Constraint::NoType)
    {
        return clist;
    } else {
        std::vector<ConstraintPtr> clist_typed;
        for(const ConstraintPtr& ptr : clist)
        {
            if(ptr->type() == type)
                clist_typed.push_back(ptr);
        }
        return clist_typed;
    }
}

void Table::setConstraints(const ConstraintMap& constraints)
{
    m_constraints = constraints;
}

StringVector& Table::primaryKeyRef()
{
    return const_cast<StringVector&>(static_cast<const Table*>(this)->primaryKey());
}

const StringVector& Table::primaryKey() const
{
    auto it = m_constraints.cbegin();
    while(it != m_constraints.cend())
    {
        if(it->second->type() == Constraint::PrimaryKeyConstraintType)
            return it->first;
        ++it;
    }

    static StringVector emptyFieldVector;
    return emptyFieldVector;
}

void Table::removeKeyFromAllConstraints(const std::string& key)
{
    // First remove all constraints with exactly that one key
    m_constraints.erase({key});

    // Then delete all occurrences of the key in compound columns
    for(auto it=m_constraints.begin();it!=m_constraints.end();)
    {
        if(contains(it->first, key))
        {
            StringVector k = it->first;
            k.erase(std::remove(k.begin(), k.end(), key), k.end());
            m_constraints.insert({k, it->second});
            it = m_constraints.erase(it);
        } else {
            ++it;
        }
    }
}

void Table::renameKeyInAllConstraints(const std::string& key, const std::string& to)
{
    // Do nothing if the key hasn't really changed
    if(key == to)
        return;

    // Find all occurrences of the key and change it to the new one
    for(auto it=m_constraints.begin();it!=m_constraints.end();)
    {
        if(contains(it->first, key))
        {
            StringVector k = it->first;
            std::replace(k.begin(), k.end(), key, to);
            m_constraints.insert({k, it->second});
            it = m_constraints.erase(it);
        } else {
            ++it;
        }
    }
}

namespace
{
std::string unescape_identifier(std::string str, char quote_char)
{
    std::string quote(2, quote_char);

    size_t pos = 0;
    while((pos = str.find(quote, pos)) != std::string::npos)
    {
        str.erase(pos, 1);
        pos += 1;               // Don't remove the other quote char too
    }
    return str;
}

std::string identifier(antlr::RefAST ident)
{
    std::string sident = ident->getText();
    if(ident->getType() == sqlite3TokenTypes::QUOTEDID ||
       ident->getType() == Sqlite3Lexer::QUOTEDLITERAL ||
       ident->getType() == sqlite3TokenTypes::STRINGLITERAL)
    {
        // Remember the way the identifier is quoted
        char quoteChar = sident.at(0);

        // Remove first and final character, i.e. the quotes
        sident = sident.substr(1, sident.size() - 2);

        // Replace all remaining occurences of two succeeding quote characters and replace them
        // by a single instance. This is done because two quotes can be used as a means of escaping
        // the quote character, thus only the visual representation has its two quotes, the actual
        // name contains only one.
        sident = unescape_identifier(sident, quoteChar);
    }

    return sident;
}

std::string textAST(antlr::RefAST t)
{
    // When this is called for a KEYWORDASTABLENAME token, we must take the child's content to get the actual value
    // instead of 'KEYWORDASTABLENAME' as a string. The same applies for  KEYWORDASCOLUMNNAME tokens.
    if(t != antlr::nullAST && (t->getType() == sqlite3TokenTypes::KEYWORDASTABLENAME || t->getType() == sqlite3TokenTypes::KEYWORDASCOLUMNNAME))
        return t->getFirstChild()->getText();
    else
        return t->getText();
}

std::string concatTextAST(antlr::RefAST t, bool withspace = false)
{
    StringVector stext;
    while(t != antlr::nullAST)
    {
        stext.push_back(textAST(t));
        t = t->getNextSibling();
    }
    return joinStringVector(stext, withspace ? " " : "");
}

std::string concatExprAST(antlr::RefAST t)
{
    std::string expr;

    int num_paren = 1;
    while(t)
    {
        if(t->getType() == sqlite3TokenTypes::LPAREN)
            num_paren++;
        else if(t->getType() == sqlite3TokenTypes::RPAREN)
            num_paren--;

        if(num_paren == 0)
            break;

        switch(t->getType())
        {
        case sqlite3TokenTypes::AND:
        case sqlite3TokenTypes::OR:
        case sqlite3TokenTypes::IN:
        case sqlite3TokenTypes::LIKE:
        case sqlite3TokenTypes::MATCH:
        case sqlite3TokenTypes::REGEXP:
        case sqlite3TokenTypes::EXISTS:
        case sqlite3TokenTypes::GLOB:
        case sqlite3TokenTypes::BETWEEN:
            expr += " " + textAST(t) + " ";
            break;
        case sqlite3TokenTypes::NOT:
            expr += " " + textAST(t);
            break;
        default:
            expr += textAST(t);
        }

        t = t->getNextSibling();
    }

    return expr;
}
}

namespace {
std::string tablename(const antlr::RefAST& n)
{
    if(n->getType() == sqlite3TokenTypes::KEYWORDASTABLENAME)
        return concatTextAST(n->getFirstChild());
    else
        return identifier(n);
}
std::string columnname(const antlr::RefAST& n)
{
    if(n->getType() == sqlite3TokenTypes::KEYWORDASCOLUMNNAME)
        return concatTextAST(n->getFirstChild());
    else if(n->getType() == sqlite3TokenTypes::INDEXEDCOLUMN)
        return columnname(n->getFirstChild());
    else
        return identifier(n);
}
}

TablePtr CreateTableWalker::table()
{
    Table* tab = new Table("");
    tab->setFullyParsed(true);

    if( m_root ) //CREATE TABLE
    {
        antlr::RefAST s = m_root->getFirstChild();

        // If the primary tree isn't filled, this isn't a normal CREATE TABLE statement. Switch to the next alternative tree.
        if(s == nullptr)
            s = m_root->getNextSibling();

        // Skip to table name
        bool is_virtual_table = false;
        while(s->getType() != Sqlite3Lexer::ID &&
              s->getType() != Sqlite3Lexer::QUOTEDID &&
              s->getType() != Sqlite3Lexer::QUOTEDLITERAL &&
              s->getType() != Sqlite3Lexer::STRINGLITERAL &&
              s->getType() != sqlite3TokenTypes::KEYWORDASTABLENAME)
        {
            // Is this one of these virtual tables?
            if(s->getType() == Sqlite3Lexer::VIRTUAL)
                is_virtual_table = true;

            s = s->getNextSibling();
        }

        // Extract and set table name
        tab->setName(tablename(s));

        // Special handling for virtual tables. If this is a virtual table, extract the USING part and skip all the
        // rest of this function because virtual tables don't have column definitons
        if(is_virtual_table)
        {
            s = s->getNextSibling(); // USING
            s = s->getNextSibling(); // module name
            tab->setVirtualUsing(concatTextAST(s, true));
            tab->setFullyParsed(false);

            return TablePtr(tab);
        }

        // This is a normal table, not a virtual one
        s = s->getNextSibling(); // LPAREN
        s = s->getNextSibling(); // first column name
        antlr::RefAST column = s;
        // loop columndefs
        while(column != antlr::nullAST && column->getType() == sqlite3TokenTypes::COLUMNDEF)
        {
            parsecolumn(tab, column->getFirstChild());
            column = column->getNextSibling(); //COMMA or RPAREN
            column = column->getNextSibling(); //null or tableconstraint

            s = s->getNextSibling();            // COLUMNDEF
            s = s->getNextSibling();            // COMMA or RPAREN
        }

        // now we are finished or it is a tableconstraint
        while(s != antlr::nullAST)
        {
            // Is this a 'without rowid' definiton?
            if(s->getType() != sqlite3TokenTypes::WITHOUT)
            {
                // It's not, so treat this as table constraints

                antlr::RefAST tc = s->getFirstChild();

                // Extract constraint name, if there is any
                std::string constraint_name;
                if(tc->getType() == sqlite3TokenTypes::CONSTRAINT)
                {
                    tc = tc->getNextSibling();          // CONSTRAINT
                    constraint_name = identifier(tc);
                    tc = tc->getNextSibling();          // identifier
                }

                switch(tc->getType())
                {
                case sqlite3TokenTypes::PRIMARY:
                {
                    PrimaryKeyConstraint* pk = new PrimaryKeyConstraint;
                    pk->setName(constraint_name);

                    tc = tc->getNextSibling()->getNextSibling(); // skip primary and key
                    tc = tc->getNextSibling(); // skip LPAREN

                    StringVector fields;
                    do
                    {
                        antlr::RefAST indexed_column = tc->getFirstChild();

                        std::string col = columnname(indexed_column);
                        fields.push_back(col);

                        indexed_column = indexed_column->getNextSibling();
                        if(indexed_column != antlr::nullAST
                                && (indexed_column->getType() == sqlite3TokenTypes::ASC
                                    || indexed_column->getType() == sqlite3TokenTypes::DESC))
                        {
                            // TODO save ASC / DESC information?
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();
                        }

                        if(indexed_column != antlr::nullAST && indexed_column->getType() == sqlite3TokenTypes::COLLATE)
                        {
                            indexed_column = indexed_column->getNextSibling();      // COLLATE
                            // TODO save collation name
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();      // collation name
                        }

                        if(indexed_column != antlr::nullAST && indexed_column->getType() == sqlite3TokenTypes::AUTOINCREMENT)
                        {
                            auto field = findField(tab, col);
                            field->setAutoIncrement(true);
                            indexed_column = indexed_column->getNextSibling();
                        }

                        tc = tc->getNextSibling();      // indexed column

                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        }
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    // We're either done now or there is a conflict clause
                    tc = tc->getNextSibling();          // skip RPAREN
                    pk->setConflictAction(parseConflictClause(tc));

                    tab->addConstraint(fields, ConstraintPtr(pk));
                }
                break;
                case sqlite3TokenTypes::UNIQUE:
                {
                    UniqueConstraint* unique = new UniqueConstraint;
                    unique->setName(constraint_name);

                    tc = tc->getNextSibling(); // skip UNIQUE
                    tc = tc->getNextSibling(); // skip LPAREN
                    StringVector fields;
                    do
                    {
                        antlr::RefAST indexed_column = tc->getFirstChild();

                        std::string col = columnname(indexed_column);
                        auto field = findField(tab, col);
                        fields.push_back(field->name());

                        indexed_column = indexed_column->getNextSibling();
                        if(indexed_column != antlr::nullAST
                                && (indexed_column->getType() == sqlite3TokenTypes::ASC
                                    || indexed_column->getType() == sqlite3TokenTypes::DESC))
                        {
                            // TODO save ASC / DESC information?
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();
                        }

                        if(indexed_column != antlr::nullAST && indexed_column->getType() == sqlite3TokenTypes::COLLATE)
                        {
                            indexed_column = indexed_column->getNextSibling();      // COLLATE
                            // TODO save collation name
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();      // collation name
                        }

                        tc = tc->getNextSibling();      // indexed column

                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        }
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    if(fields.size() == 1 && constraint_name.empty())
                    {
                        findField(tab, fields[0])->setUnique(true);
                        delete unique;
                    } else {
                        tab->addConstraint(fields, ConstraintPtr(unique));
                    }
                }
                break;
                case sqlite3TokenTypes::FOREIGN:
                {
                    ForeignKeyClause* fk = new ForeignKeyClause;
                    fk->setName(constraint_name);

                    tc = tc->getNextSibling();  // FOREIGN
                    tc = tc->getNextSibling();  // KEY
                    tc = tc->getNextSibling();  // LPAREN

                    StringVector fields;
                    do
                    {
                        std::string col = columnname(tc);
                        fields.push_back(findField(tab, col)->name());

                        tc = tc->getNextSibling();

                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                            tc = tc->getNextSibling(); // skip ident and comma
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    tc = tc->getNextSibling();
                    tc = tc->getNextSibling();  // REFERENCES

                    fk->setTable(identifier(tc));
                    tc = tc->getNextSibling();       // identifier

                    if(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::LPAREN)
                    {
                        tc = tc->getNextSibling();  // LPAREN

                        StringVector fk_cols;
                        while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN)
                        {
                            if(tc->getType() != sqlite3TokenTypes::COMMA)
                                fk_cols.push_back(identifier(tc));
                            tc = tc->getNextSibling();
                        }
                        fk->setColumns(fk_cols);

                        tc = tc->getNextSibling();  // RPAREN
                    }

                    fk->setConstraint(concatTextAST(tc, true));
                    tab->addConstraint(fields, ConstraintPtr(fk));
                }
                break;
                case sqlite3TokenTypes::CHECK:
                {
                    CheckConstraint* check = new CheckConstraint;
                    check->setName(constraint_name);

                    tc = tc->getNextSibling(); // skip CHECK
                    tc = tc->getNextSibling(); // skip LPAREN

                    check->setExpression(concatExprAST(tc));
                    tab->addConstraint(StringVector(), ConstraintPtr(check));
                }
                break;
                default:
                {
                    std::cout << "unknown table constraint in " << tab->name() << std::endl;
                    tab->setFullyParsed(false);
                }
                    break;
                }

                s = s->getNextSibling(); //COMMA or RPAREN
                if(s->getType() == sqlite3TokenTypes::COMMA || s->getType() == sqlite3TokenTypes::RPAREN)
                    s = s->getNextSibling();
            } else {
                // It is

                s = s->getNextSibling();    // WITHOUT
                s = s->getNextSibling();    // ROWID

                tab->setWithoutRowidTable(true);
            }
        }
    }

    return TablePtr(tab);
}

void CreateTableWalker::parsecolumn(Table* table, antlr::RefAST c)
{
    std::string colname;
    std::string type = "TEXT";
    bool autoincrement = false;
    bool notnull = false;
    bool unique = false;
    std::string defaultvalue;
    std::string check;
    std::string collation;
    sqlb::PrimaryKeyConstraint* primaryKey = nullptr;
    std::vector<sqlb::ForeignKeyClause*> foreignKeys;

    colname = columnname(c);
    c = c->getNextSibling(); //type?
    if(c != antlr::nullAST && c->getType() == sqlite3TokenTypes::TYPE_NAME)
    {
        antlr::RefAST t = c->getFirstChild();

        if(t != antlr::nullAST)
        {
            type.clear();
        }

        while(t != antlr::nullAST)
        {
            int thisType = t->getType();
            type += textAST(t);
            t = t->getNextSibling();
            if(t != antlr::nullAST)
            {
                int nextType = t->getType();
                if(nextType != sqlite3TokenTypes::LPAREN && nextType != sqlite3TokenTypes::RPAREN &&
                   thisType != sqlite3TokenTypes::LPAREN)
                {
                    type.append(" ");
                }
            }
        }
        c = c->getNextSibling();
    }

    // finished with type parsing
    // now columnconstraints
    while(c != antlr::nullAST)
    {
        antlr::RefAST con = c->getFirstChild();

        // Extract constraint name, if there is any
        std::string constraint_name;
        if(con->getType() == sqlite3TokenTypes::CONSTRAINT)
        {
            con = con->getNextSibling();          // CONSTRAINT
            constraint_name = identifier(con);
            con = con->getNextSibling();          // identifier
        }

        switch(con->getType())
        {
        case sqlite3TokenTypes::PRIMARY:
        {
            // If we have already allocated a primary key object, delete it first. This should never happen in real world situations because
            // SQLite only supports one primary key constraint per field.
            if(primaryKey)
                delete primaryKey;

            primaryKey = new PrimaryKeyConstraint;
            primaryKey->setName(constraint_name);

            con = con->getNextSibling()->getNextSibling(); // skip KEY
            if(con != antlr::nullAST && (con->getType() == sqlite3TokenTypes::ASC
                                         || con->getType() == sqlite3TokenTypes::DESC))
            {
                table->setFullyParsed(false);
                con = con->getNextSibling(); //skip
            }

            primaryKey->setConflictAction(parseConflictClause(con));

            if(con != antlr::nullAST && con->getType() == sqlite3TokenTypes::AUTOINCREMENT)
                autoincrement = true;
        }
        break;
        case sqlite3TokenTypes::NOT:
        {
            // TODO Support constraint names here
            if(!constraint_name.empty())
                table->setFullyParsed(false);

            notnull = true;
        }
        break;
        case sqlite3TokenTypes::NULL_T:
        {
            notnull = false;
        }
        break;
        case sqlite3TokenTypes::CHECK:
        {
            con = con->getNextSibling(); //CHECK
            con = con->getNextSibling(); //LPAREN

            check = concatExprAST(con);

            // If we have a constraint name, convert this constraint from a column into a table constaint in order to save it with our data model
            if(!constraint_name.empty())
            {
                CheckConstraint* check_constraint = new CheckConstraint(check);
                check_constraint->setName(constraint_name);
                table->addConstraint({colname}, ConstraintPtr(check_constraint));
                check.clear();
            }
        }
        break;
        case sqlite3TokenTypes::DEFAULT:
        {
            // TODO Support constraint names here
            if(!constraint_name.empty())
                table->setFullyParsed(false);

            con = con->getNextSibling(); //SIGNEDNUMBER,STRING,LPAREN
            defaultvalue = concatTextAST(con);
        }
        break;
        case sqlite3TokenTypes::UNIQUE:
        {
            // TODO Support constraint names here
            if(!constraint_name.empty())
                table->setFullyParsed(false);

            unique = true;
        }
        break;
        case sqlite3TokenTypes::REFERENCES:
        {
            con = con->getNextSibling();    // REFERENCES

            sqlb::ForeignKeyClause* foreignKey = new ForeignKeyClause;
            foreignKey->setTable(identifier(con));
            foreignKey->setName(constraint_name);
            con = con->getNextSibling();    // identifier

            if(con != antlr::nullAST && con->getType() == sqlite3TokenTypes::LPAREN)
            {
                con = con->getNextSibling();    // LPAREN

                StringVector fk_cols;
                while(con != antlr::nullAST && con->getType() != sqlite3TokenTypes::RPAREN)
                {
                    if(con->getType() != sqlite3TokenTypes::COMMA)
                        fk_cols.push_back(identifier(con));
                    con = con->getNextSibling();
                }
                foreignKey->setColumns(fk_cols);

                con = con->getNextSibling();    // RPAREN
            }

            foreignKey->setConstraint(concatTextAST(con, true));
            foreignKeys.push_back(foreignKey);
        }
        break;
        case sqlite3TokenTypes::COLLATE:
        {
            con = con->getNextSibling();    // COLLATE
            collation = identifier(con);
            con = con->getNextSibling();    // collation name
        }
        break;
        default:
        {
            std::cout << "unknown column constraint in " << table->name() << "." << colname << std::endl;
            table->setFullyParsed(false);
        }
        break;
        }
        c = c->getNextSibling();
    }

    Field f(colname, type, notnull, defaultvalue, check, unique, collation);
    f.setAutoIncrement(autoincrement);
    table->fields.push_back(f);

    for(sqlb::ForeignKeyClause* fk : foreignKeys)
        table->addConstraint({f.name()}, ConstraintPtr(fk));
    if(primaryKey)
    {
        StringVector v;
        if(table->constraint(v, Constraint::PrimaryKeyConstraintType))
        {
            table->primaryKeyRef().push_back(f.name());

            // Delete useless primary key constraint. There already is a primary key object for this table, we
            // don't need another one.
            delete primaryKey;
        } else {
            table->addConstraint({f.name()}, ConstraintPtr(primaryKey));
        }
    }
}

std::string CreateTableWalker::parseConflictClause(antlr::RefAST c)
{
    std::string conflictAction;

    if(c != antlr::nullAST && c->getType() == sqlite3TokenTypes::ON && c->getNextSibling()->getType() == sqlite3TokenTypes::CONFLICT)
    {
        c = c->getNextSibling();      // skip ON
        c = c->getNextSibling();      // skip CONFLICT
        conflictAction = identifier(c);
        c = c->getNextSibling();      // skip action
    }

    return conflictAction;
}



std::string IndexedColumn::toString(const std::string& indent, const std::string& sep) const
{
    std::string name = m_isExpression ? m_name : escapeIdentifier(m_name);
    std::string order = (m_order.empty() ? "" : (sep + m_order));
    return indent + name + order;
}

Index& Index::operator=(const Index& rhs)
{
    // Base class
    Object::operator=(rhs);

    // Just assign the easy stuff
    m_unique = rhs.m_unique;
    m_table = rhs.m_table;
    m_whereExpr = rhs.m_whereExpr;

    // Make copies of the column
    std::copy(rhs.fields.begin(), rhs.fields.end(), std::back_inserter(fields));

    return *this;
}

StringVector Index::columnSqlList() const
{
    StringVector sl;

    for(const IndexedColumn& c : fields)
        sl.push_back(c.toString());

    return sl;
}

std::string Index::sql(const std::string& schema, bool ifNotExists) const
{
    // Start CREATE (UNIQUE) INDEX statement
    std::string sql;
    if(m_unique)
        sql = "CREATE UNIQUE INDEX ";
    else
        sql = "CREATE INDEX ";
    if(ifNotExists)
        sql += "IF NOT EXISTS ";
    sql += ObjectIdentifier(schema, m_name).toString(true);
    sql += " ON ";
    sql += sqlb::escapeIdentifier(m_table);
    sql += " (\n";

    // Add column list
    sql += joinStringVector(columnSqlList(), ",\n");

    // Add partial index bit
    sql += "\n)";
    if(!m_whereExpr.empty())
        sql += " WHERE " + m_whereExpr;

    return sql + ";";
}

FieldInfoList Index::fieldInformation() const
{
    FieldInfoList result;
    for(const IndexedColumn& c : fields)
        result.emplace_back(c.name(), c.order(), c.toString("  ", " "));
    return result;
}

IndexPtr Index::parseSQL(const std::string& sSQL)
{
    SetLocaleToC locale;

    std::stringstream s;
    s << sSQL;
    Sqlite3Lexer lex(s);

    Sqlite3Parser parser(lex);

    antlr::ASTFactory ast_factory;
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    try
    {
        parser.createindex();
        CreateIndexWalker ctw(parser.getAST());

        auto i = ctw.index();
        i->setOriginalSql(sSQL);
        return i;
    }
    catch(antlr::ANTLRException& ex)
    {
        std::cerr << "Sqlite parse error: " << ex.toString() << "(" << sSQL << ")" << std::endl;
    }
    catch(...)
    {
        std::cerr << "Sqlite parse error: " << sSQL << std::endl; //TODO
    }

    return IndexPtr(new Index(""));
}

IndexPtr CreateIndexWalker::index()
{
    Index* index = new Index("");
    index->setFullyParsed(true);

    if(m_root)  // CREATE INDEX
    {
        antlr::RefAST s = m_root->getFirstChild();

        // Skip to index name
        while(s->getType() != Sqlite3Lexer::ID &&
              s->getType() != Sqlite3Lexer::QUOTEDID &&
              s->getType() != Sqlite3Lexer::QUOTEDLITERAL &&
              s->getType() != Sqlite3Lexer::STRINGLITERAL &&
              s->getType() != sqlite3TokenTypes::KEYWORDASTABLENAME)
        {
            // Is this a unique index?
            if(s->getType() == Sqlite3Lexer::UNIQUE)
                index->setUnique(true);

            s = s->getNextSibling();
        }

        // Extract and set index name
        index->setName(tablename(s));

        // Get table name
        s = s->getNextSibling(); // ON
        s = s->getNextSibling(); // table name
        index->setTable(tablename(s));

        s = s->getNextSibling(); // LPAREN
        s = s->getNextSibling(); // first column name
        antlr::RefAST column = s;
        // loop columndefs
        while(column != antlr::nullAST && column->getType() == sqlite3TokenTypes::INDEXEDCOLUMN)
        {
            parsecolumn(index, column->getFirstChild());
            column = column->getNextSibling(); // COMMA or RPAREN
            column = column->getNextSibling(); // null or WHERE

            s = s->getNextSibling();            // COLUMNDEF
            s = s->getNextSibling();            // COMMA or RPAREN
        }

        // Now we are finished or it is a partial index
        if(s != antlr::nullAST)
        {
            // This should be a 'where' then
            if(s->getType() != sqlite3TokenTypes::WHERE)
            {
                // It is something else
                index->setFullyParsed(false);
            } else {
                s = s->getNextSibling();        // expr
                index->setWhereExpr(concatTextAST(s, true));
            }
        }
    }

    return IndexPtr(index);
}

void CreateIndexWalker::parsecolumn(Index* index, antlr::RefAST c)
{
    std::string name;
    bool isExpression;
    std::string order;

    // First count the number of nodes used for the name or the expression. We reach the end of the name nodes list when we either
    // get to the end of the list, get to a COMMA or a RPAREN, or get to the COLLATE keyword or get to the ASC/DESC keywords.
    // Then see how many items there are: if it's one it's a normal index column with only a column name. In this case get the identifier.
    // If it's more than one item it's an expression. In this case get all the items as they are.
    int number_of_name_items = 0;
    antlr::RefAST n = c;
    while(n != antlr::nullAST
          && n->getType() != sqlite3TokenTypes::COLLATE
          && n->getType() != sqlite3TokenTypes::ASC
          && n->getType() != sqlite3TokenTypes::DESC
          && n->getType() != sqlite3TokenTypes::COMMA
          && n->getType() != sqlite3TokenTypes::RPAREN)
    {
        number_of_name_items++;
        n = n->getNextSibling();
    }
    if(number_of_name_items == 1)
    {
        name = identifier(c);
        isExpression = false;
        c = c->getNextSibling();
    } else {
        for(int i=0;i<number_of_name_items;i++)
        {
            name += c->getText() + " ";
            c = c->getNextSibling();
        }
        name = name.substr(0, name.size()-1);
        isExpression = true;
    }

    // Parse the rest of the column definition
    while(c != antlr::nullAST)
    {
        switch(c->getType())
        {
        case sqlite3TokenTypes::ASC:
        case sqlite3TokenTypes::DESC:
            order = c->getText().c_str();
            break;
        default:
            // TODO Add support for COLLATE
            index->setFullyParsed(false);
        }

        c = c->getNextSibling();
    }

    index->fields.emplace_back(name, isExpression, order);
}



ViewPtr View::parseSQL(const std::string& sSQL)
{
    // TODO

    auto v = ViewPtr(new View(""));
    v->setOriginalSql(sSQL);
    return v;
}

StringVector View::fieldNames() const
{
    StringVector sl;

    for(const Field& f : fields)
        sl.push_back(f.name());

    return sl;
}

FieldInfoList View::fieldInformation() const
{
    FieldInfoList result;
    for(const Field& f : fields)
        result.emplace_back(f.name(), f.type(), f.toString("  ", " "));
    return result;
}


TriggerPtr Trigger::parseSQL(const std::string& sSQL)
{
    // TODO

    auto t = TriggerPtr(new Trigger(""));
    t->setOriginalSql(sSQL);
    return t;
}

} //namespace sqlb
