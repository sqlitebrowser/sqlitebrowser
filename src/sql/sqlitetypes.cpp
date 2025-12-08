#include "sqlitetypes.h"
#include "ObjectIdentifier.h"
#include "parser/ParserDriver.h"

#include <iostream>
#include <iterator>
#include <numeric>

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

std::string joinStringVector(const IndexedColumnVector& vec, const std::string& delim)
{
    return std::accumulate(vec.begin(), vec.end(), std::string(), [delim](const std::string& so_far, const IndexedColumn& c) {
        return so_far.empty() ? c.toString("", " ") : so_far + delim + c.toString("", "");
    });
}

bool Object::operator==(const Object& rhs) const
{
    if(m_name != rhs.m_name)
        return false;
    if(m_fullyParsed != rhs.m_fullyParsed)  // We check for the fully parsed flag to make sure not to lose anything in some corner cases
        return false;

    // We don't care about the original SQL text

    return true;
}

std::string ForeignKeyClause::toString() const
{
    std::string result = escapeIdentifier(m_table);

    if(m_columns.size())
        result += "(" + joinStringVector(escapeIdentifier(m_columns), ",") + ")";

    if(m_constraint.size())
        result += " " + m_constraint;

    return result;
}

std::string ForeignKeyClause::toSql(const StringVector& columns) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "FOREIGN KEY(" + joinStringVector(escapeIdentifier(columns), ",") + ") REFERENCES " + this->toString();

    return result;
}

std::string UniqueConstraint::toSql(const IndexedColumnVector& columns) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";

    result += "UNIQUE";

    if(columns.size())
        result += "(" + joinStringVector(columns, ",") + ")";

    if(!m_conflictAction.empty())
        result += " ON CONFLICT " + m_conflictAction;

    return result;
}

std::string NotNullConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";

    result += "NOT NULL";

    if(!m_conflictAction.empty())
        result += " ON CONFLICT " + m_conflictAction;

    return result;
}

std::string PrimaryKeyConstraint::toSql(const IndexedColumnVector& columns) const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";

    result += "PRIMARY KEY(" + joinStringVector(columns, ",") + (m_auto_increment ? " AUTOINCREMENT" : "") + ")";

    if(!m_conflictAction.empty())
        result += " ON CONFLICT " + m_conflictAction;

    return result;
}

std::string CheckConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "CHECK(" + m_expression + ")";

    return result;
}

std::string DefaultConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "DEFAULT " + m_value;

    return result;
}

std::string CollateConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "COLLATE " + m_collation;

    return result;
}

std::string GeneratedColumnConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "GENERATED ALWAYS AS (" + m_expression + ")" + " " + storage();

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
        str += " " + m_notnull->toSql();
    if(m_defaultvalue)
        str += " " + m_defaultvalue->toSql();
    if(m_check)
        str += " " + m_check->toSql();
    if(m_unique)
        str += " " + m_unique->toSql({});
    if(m_collation)
        str += " " + m_collation->toSql();
    if(m_generated)
        str += " " + m_generated->toSql();
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

Field::Affinity Field::affinity() const
{
    if (isInteger()) return IntegerAffinity;

    if (isText()) return TextAffinity;

    if (isBlob()) return BlobAffinity;

    if (isReal() || isNumeric()) return FloatAffinity;

    return BlobAffinity;
}

Table::Table(const Table& table)
    : Object(table.name())
{
    *this = table;
}

Table& Table::operator=(const Table& rhs)
{
    // Base class
    Object::operator=(rhs);

    // Just assign the simple values
    m_options = rhs.m_options;
    m_virtual = rhs.m_virtual;

    // Clear the fields and the constraints first in order to avoid duplicates and/or old data in the next step
    fields.clear();
    m_indexConstraints.clear();
    m_foreignKeys.clear();
    m_checkConstraints.clear();

    // Make copies of the fields and the constraints. This is necessary in order to avoid any unwanted changes to the application's main database
    // schema representation just by modifying a reference to the fields or constraints and thinking it operates on a copy.
    std::copy(rhs.fields.begin(), rhs.fields.end(), std::back_inserter(fields));
    for(const auto& e : rhs.m_indexConstraints)
    {
        if(e.second->isPrimaryKey())
            m_indexConstraints.insert(std::make_pair(e.first, std::make_shared<PrimaryKeyConstraint>(*std::dynamic_pointer_cast<PrimaryKeyConstraint>(e.second))));
        else
            m_indexConstraints.insert(std::make_pair(e.first, std::make_shared<UniqueConstraint>(*e.second)));
    }
    for(const auto& e : rhs.m_foreignKeys)
        m_foreignKeys.insert(std::make_pair(e.first, std::make_shared<ForeignKeyClause>(*e.second)));
    std::transform(rhs.m_checkConstraints.begin(), rhs.m_checkConstraints.end(), std::back_inserter(m_checkConstraints), [](const auto& e) { return std::make_shared<CheckConstraint>(*e); });

    return *this;
}

bool Table::operator==(const Table& rhs) const
{
    if(!Object::operator==(rhs))
        return false;

    if(m_options != rhs.m_options)
        return false;
    if(m_virtual != rhs.m_virtual)
        return false;
    if(fields != rhs.fields)
        return false;
    if(m_indexConstraints != rhs.m_indexConstraints)
        return false;
    if(m_foreignKeys != rhs.m_foreignKeys)
        return false;
    if(m_checkConstraints != rhs.m_checkConstraints)
        return false;

    return true;
}

StringVector Table::fieldList() const
{
    StringVector sl;
    std::transform(fields.begin(), fields.end(), std::back_inserter(sl), [](const auto& f) { return f.toString(); });
    return sl;
}

StringVector Table::fieldNames() const
{
    StringVector sl;
    std::transform(fields.begin(), fields.end(), std::back_inserter(sl), [](const auto& f) { return f.name(); });
    return sl;
}

StringVector Table::rowidColumns() const
{
    // For WITHOUT ROWID tables this function returns the names of the primary key column. For ordinary tables with a rowid column, it returns "_rowid_"
    if(withoutRowidTable())
    {
        auto columns = primaryKeyColumns();
        StringVector result;
        std::transform(columns.begin(), columns.end(), std::back_inserter(result), [](const auto& e) { return e.name(); });
        return result;
    } else
        return {"_rowid_"};
}

TablePtr Table::parseSQL(const std::string& sSQL)
{
    parser::ParserDriver drv;
    if(!drv.parse(sSQL))
    {
        TablePtr t = std::dynamic_pointer_cast<Table>(drv.result);
        t->setOriginalSql(sSQL);
        return t;
    } else {
        std::cerr << "Sqlite parse error: " << sSQL << std::endl;
        TablePtr t = std::make_shared<Table>("");
        t->setOriginalSql(sSQL);
        return t;
    }
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
    for(const auto& it : m_indexConstraints)
        sql += ",\n\t" + it.second->toSql(it.first);
    for(const auto& it : m_foreignKeys)
        sql += ",\n\t" + it.second->toSql(it.first);
    for(const auto& it : m_checkConstraints)
        sql += ",\n\t" + it->toSql();

    sql += "\n)";

    // Table options
    bool first_option = true;
    for(size_t i=0;i<NumOptions;i++)
    {
        if(m_options.test(i))
        {
            if(first_option)
            {
                sql += " ";
                first_option = false;
            } else {
                sql += ",";
            }

            switch(i)
            {
            case WithoutRowid:
                sql += "WITHOUT ROWID";
                break;
            case Strict:
                sql += "STRICT";
                break;
            }
        }
    }

    return sql + ";";
}

void Table::addConstraint(const StringVector& columns, std::shared_ptr<PrimaryKeyConstraint> constraint)
{
    IndexedColumnVector c;
    std::transform(columns.begin(), columns.end(), std::back_inserter(c), [](const auto& e) { return IndexedColumn(e, false); });
    m_indexConstraints.insert(std::make_pair(c, constraint));
}

void Table::addConstraint(const StringVector& columns, std::shared_ptr<UniqueConstraint> constraint)
{
    IndexedColumnVector c;
    std::transform(columns.begin(), columns.end(), std::back_inserter(c), [](const auto& e) { return IndexedColumn(e, false); });
    m_indexConstraints.insert(std::make_pair(c, constraint));
}

void Table::addConstraint(const IndexedColumnVector& columns, std::shared_ptr<ForeignKeyClause> constraint)
{
    StringVector c;
    std::transform(columns.begin(), columns.end(), std::back_inserter(c), [](const auto& e) { return e.name(); });
    m_foreignKeys.insert(std::make_pair(c, constraint));
}

void Table::removeConstraint(std::shared_ptr<UniqueConstraint> constraint)
{
    auto c = std::find_if(m_indexConstraints.begin(), m_indexConstraints.end(), [constraint](const auto& e) { return e.second == constraint; });
    if(c != m_indexConstraints.end())
        m_indexConstraints.erase(c);
}

void Table::removeConstraint(std::shared_ptr<ForeignKeyClause> constraint)
{
    auto c = std::find_if(m_foreignKeys.begin(), m_foreignKeys.end(), [constraint](const auto& e) { return e.second == constraint; });
    if(c != m_foreignKeys.end())
        m_foreignKeys.erase(c);
}

void Table::removeConstraint(std::shared_ptr<CheckConstraint> constraint)
{
    m_checkConstraints.erase(std::remove_if(m_checkConstraints.begin(), m_checkConstraints.end(), [constraint](const auto& e) { return e == constraint; }));
}

void Table::addKeyToConstraint(std::shared_ptr<UniqueConstraint> constraint, const std::string& key)
{
    // Search for matching constraint
    for(auto it=m_indexConstraints.begin();it!=m_indexConstraints.end();)
    {
        if(it->second == constraint)
        {
            // Add key to the column list
            auto new_columns = it->first;
            new_columns.emplace_back(key, false);

            m_indexConstraints.insert(std::make_pair(new_columns, it->second));
            it = m_indexConstraints.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Table::removeKeyFromConstraint(std::shared_ptr<UniqueConstraint> constraint, const std::string& key)
{
    for(auto it=m_indexConstraints.begin();it!=m_indexConstraints.end();)
    {
        if(it->second == constraint)
        {
            // Remove key from the column list
            std::remove_const_t<decltype(it->first)> new_columns;
            std::copy_if(it->first.begin(), it->first.end(), std::back_inserter(new_columns), [key](const auto& c) { return c != key; });

            // If the column list is empty now, remove the entire constraint. Otherwise save the updated column list
            if(new_columns.empty())
            {
                it = m_indexConstraints.erase(it);
            } else {
                m_indexConstraints.insert(std::make_pair(new_columns, it->second));
                it = m_indexConstraints.erase(it);
            }
        } else {
            ++it;
        }
    }
}

void Table::removeKeyFromAllConstraints(const std::string& key)
{
    auto match_and_remove = [key](auto& container, auto& it) {
        // Check if they contain the old key name
        if(contains(it->first, key))
        {
            // If so, remove it from the column list
            std::remove_const_t<decltype(it->first)> new_columns;
            std::copy_if(it->first.begin(), it->first.end(), std::back_inserter(new_columns), [key](const auto& c) { return c != key; });

            // If the column list is empty now, remove the entire constraint. Otherwise save the updated column list
            if(new_columns.empty())
            {
                it = container.erase(it);
            } else {
                container.insert(std::make_pair(new_columns, it->second));
                it = container.erase(it);
            }
        } else {
            ++it;
        }
    };

    for(auto it=m_indexConstraints.begin();it!=m_indexConstraints.end();)
        match_and_remove(m_indexConstraints, it);

    for(auto it=m_foreignKeys.begin();it!=m_foreignKeys.end();)
        match_and_remove(m_foreignKeys, it);
}

void Table::renameKeyInAllConstraints(const std::string& key, const std::string& to)
{
    // Do nothing if the key hasn't really changed
    if(key == to)
        return;

    const auto match_and_rename = [key, to](auto& container, auto& it, const auto& rename) {
        // Check if they contain the old key name
        if(contains(it->first, key))
        {
            // If so, update it in the column list
            std::remove_const_t<decltype(it->first)> new_columns;
            std::transform(it->first.begin(), it->first.end(), std::back_inserter(new_columns), [rename](auto c) {
                return rename(c);
            });

            container.insert(std::make_pair(new_columns, it->second));
            it = container.erase(it);
        }
        else
        {
            ++it;
        }
    };

    // Update all constraints
    for(auto it=m_indexConstraints.begin();it!=m_indexConstraints.end();)
    {
        match_and_rename(m_indexConstraints, it, [key, to](IndexedColumn c) {
            if(c == key)
                c.setName(to);
            return c;
        });
    }

    for(auto it=m_foreignKeys.begin();it!=m_foreignKeys.end();++it)
        match_and_rename(m_foreignKeys, it, [key, to](const std::string& c) { return c == key ? to : c; });
}

std::shared_ptr<PrimaryKeyConstraint> Table::primaryKey() const
{
    const auto it = std::find_if(m_indexConstraints.begin(), m_indexConstraints.end(), [](const auto& e) { return e.second->isPrimaryKey(); });
    if(it != m_indexConstraints.end())
        return std::dynamic_pointer_cast<PrimaryKeyConstraint>(it->second);
    return nullptr;
}

IndexedColumnVector Table::primaryKeyColumns() const
{
    const auto it = std::find_if(m_indexConstraints.begin(), m_indexConstraints.end(), [](const auto& e) { return e.second->isPrimaryKey(); });
    if(it != m_indexConstraints.end())
        return it->first;
    return {};
}

std::shared_ptr<ForeignKeyClause> Table::foreignKey(const StringVector& columns) const
{
    const auto it = std::find_if(m_foreignKeys.begin(), m_foreignKeys.end(), [columns](const auto& e) { return e.first == columns; });
    if(it != m_foreignKeys.end())
        return it->second;
    return nullptr;
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
    std::transform(fields.begin(), fields.end(), std::back_inserter(sl), [](const auto& c) { return c.toString(); });
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

IndexPtr Index::parseSQL(const std::string& sSQL)
{
    parser::ParserDriver drv;
    if(!drv.parse(sSQL))
    {
        IndexPtr i = std::dynamic_pointer_cast<Index>(drv.result);
        i->setOriginalSql(sSQL);
        return i;
    } else {
        std::cerr << "Sqlite parse error: " << sSQL << std::endl;
        IndexPtr i = std::make_shared<Index>("");
        i->setOriginalSql(sSQL);
        return i;
    }
}

template<>
std::string getBaseTable<Index>(IndexPtr object)
{
    return object->table();
}



ViewPtr View::parseSQL(const std::string& sSQL)
{
    // TODO

    auto v = std::make_shared<View>("");
    v->setOriginalSql(sSQL);
    return v;
}


TriggerPtr Trigger::parseSQL(const std::string& sSQL)
{
    // TODO

    auto t = std::make_shared<Trigger>("");
    t->setOriginalSql(sSQL);
    return t;
}

template<>
std::string getBaseTable<Trigger>(TriggerPtr object)
{
    return object->table();
}

} //namespace sqlb
