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
    return std::string();
}

ConstraintPtr Constraint::makeConstraint(ConstraintTypes type)
{
    switch(type)
    {
    case PrimaryKeyConstraintType:
        return std::make_shared<PrimaryKeyConstraint>();
    case UniqueConstraintType:
        return std::make_shared<UniqueConstraint>();
    case ForeignKeyConstraintType:
        return std::make_shared<ForeignKeyClause>();
    case CheckConstraintType:
        return std::make_shared<CheckConstraint>();
    default:
        return nullptr;
    }
}

void Constraint::replaceInColumnList(const std::string& from, const std::string& to)
{
    std::replace(column_list.begin(), column_list.end(), from, to);
}

void Constraint::removeFromColumnList(const std::string& key)
{
    column_list.erase(std::remove(column_list.begin(), column_list.end(), key), column_list.end());
}

bool ForeignKeyClause::isSet() const
{
    return m_override.size() || m_table.size();
}

std::string ForeignKeyClause::toString() const
{
    if(!isSet())
        return std::string();

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

std::string ForeignKeyClause::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";
    result += "FOREIGN KEY(" + joinStringVector(escapeIdentifier(column_list), ",") + ") REFERENCES " + this->toString();

    return result;
}

UniqueConstraint::UniqueConstraint(const IndexedColumnVector& columns) :
    m_columns(columns)
{
    // Extract column names and give them to the column list in the base class
    for(const auto& c : columns)
        column_list.push_back(c.name());
}

UniqueConstraint::UniqueConstraint(const StringVector& columns) :
    Constraint(columns)
{
    setColumnList(columns);
}

void UniqueConstraint::setColumnList(const StringVector& list)
{
    Constraint::setColumnList(list);

    // Create our own column list without sort orders etc
    m_columns.clear();
    for(const auto& c : list)
        m_columns.push_back(IndexedColumn(c, false));
}

void UniqueConstraint::addToColumnList(const std::string& key)
{
    Constraint::addToColumnList(key);

    // Also add to our own column list
    m_columns.push_back(IndexedColumn(key, false));
}

void UniqueConstraint::replaceInColumnList(const std::string& from, const std::string& to)
{
    Constraint::replaceInColumnList(from, to);

    for(auto& c : m_columns)
    {
        if(c.name() == from)
            c.setName(to);
    }
}

void UniqueConstraint::removeFromColumnList(const std::string& key)
{
    Constraint::removeFromColumnList(key);

    m_columns.erase(std::remove_if(m_columns.begin(), m_columns.end(), [key](const IndexedColumn& c) {
        if(c.name() == key)
            return true;
        else
            return false;
    }), m_columns.end());
}

std::string UniqueConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";

    result += "UNIQUE";

    if(m_columns.size())
    {
        std::vector<std::string> u_columns;
        for(const auto& c : m_columns)
            u_columns.push_back(c.toString("", " "));
        result += "(" + joinStringVector(u_columns, ",") + ")";
    }

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

PrimaryKeyConstraint::PrimaryKeyConstraint(const IndexedColumnVector& columns) :
    UniqueConstraint(columns),
    m_auto_increment(false)
{
}

PrimaryKeyConstraint::PrimaryKeyConstraint(const StringVector& columns) :
    UniqueConstraint(columns),
    m_auto_increment(false)
{
}

std::string PrimaryKeyConstraint::toSql() const
{
    std::string result;
    if(!m_name.empty())
        result = "CONSTRAINT " + escapeIdentifier(m_name) + " ";

    std::vector<std::string> pk_columns;
    for(const auto& c : m_columns)
        pk_columns.push_back(c.toString("", " "));
    result += "PRIMARY KEY(" + joinStringVector(pk_columns, ",") + (m_auto_increment ? " AUTOINCREMENT" : "") + ")";

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
        str += " " + m_unique->toSql();
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
    if(m_constraints != rhs.m_constraints)
        return false;

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
        return const_cast<Table*>(this)->primaryKey()->columnList();
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
        return std::make_shared<Table>("");
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
    for(const auto& it : m_constraints)
    {
        // Ignore all constraints without any fields, except for check constraints which don't rely on a field vector
        if(!it->columnList().empty() || it->type() == Constraint::CheckConstraintType)
        {
            sql += ",\n\t";
            sql += it->toSql();
        }
    }

    sql += "\n)";

    // without rowid
    if(withoutRowidTable())
        sql += " WITHOUT ROWID";

    return sql + ";";
}

void Table::addConstraint(ConstraintPtr constraint)
{
    m_constraints.insert(constraint);
}

void Table::setConstraint(ConstraintPtr constraint)
{
    // Delete any old constraints of this type for these fields
    removeConstraints(constraint->columnList(), constraint->type());

    // Add the new constraint to the table, effectively overwriting all old constraints for that fields/type combination
    addConstraint(constraint);
}

void Table::removeConstraint(ConstraintPtr constraint)
{
    for(auto it = m_constraints.begin();it!=m_constraints.end();++it)
    {
        if((*it)->toSql() == constraint->toSql())
        {
            m_constraints.erase(it);

            // Only remove the first constraint matching these criteria
            return;
        }
    }
}

void Table::removeConstraints(const StringVector& vStrFields, Constraint::ConstraintTypes type)
{
    for(auto it = m_constraints.begin();it!=m_constraints.end();)
    {
        if((*it)->columnList() == vStrFields && (*it)->type() == type)
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
    std::vector<ConstraintPtr> clist;
    for(const auto& it : m_constraints)
    {
        if((type == Constraint::NoType || it->type() == type) && (vStrFields.empty() || it->columnList() == vStrFields))
            clist.push_back(it);
    }
    return clist;
}

void Table::setConstraints(const ConstraintSet& constraints)
{
    m_constraints = constraints;
}

void Table::replaceConstraint(ConstraintPtr from, ConstraintPtr to)
{
    auto it = m_constraints.find(from);
    if(it == m_constraints.end())
            return;

    m_constraints.erase(it);    // Erase old constraint
    m_constraints.insert(to);   // Insert new constraint
}

std::shared_ptr<PrimaryKeyConstraint> Table::primaryKey()
{
    const auto c = constraint({}, Constraint::PrimaryKeyConstraintType);
    if(c)
        return std::dynamic_pointer_cast<PrimaryKeyConstraint>(c);
    else
        return nullptr;
}

void Table::removeKeyFromAllConstraints(const std::string& key)
{
    // Update all constraints
    for(auto it=m_constraints.begin();it!=m_constraints.end();)
    {
        // Check if they contain the old key name
        if(contains((*it)->columnList(), key))
        {
            // If so, remove it from the column list
            (*it)->removeFromColumnList(key);

            // If the column list is empty now, remove the entire constraint. Otherwise save the updated column list
            if((*it)->columnList().empty())
                it = m_constraints.erase(it);
            else
                ++it;
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
    for(auto& it : m_constraints)
    {
        if(contains(it->columnList(), key))
            it->replaceInColumnList(key, to);
    }
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
    parser::ParserDriver drv;
    if(!drv.parse(sSQL))
    {
        IndexPtr i = std::dynamic_pointer_cast<Index>(drv.result);
        i->setOriginalSql(sSQL);
        return i;
    } else {
        std::cerr << "Sqlite parse error: " << sSQL << std::endl;
        return std::make_shared<Index>("");
    }
}



ViewPtr View::parseSQL(const std::string& sSQL)
{
    // TODO

    auto v = std::make_shared<View>("");
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

    auto t = std::make_shared<Trigger>("");
    t->setOriginalSql(sSQL);
    return t;
}

} //namespace sqlb
