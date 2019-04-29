#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

template<typename C, typename E>
bool contains(const C& container, E element)
{
    return std::find(container.begin(), container.end(), element) != container.end();
}

template<typename T>
bool compare_ci(const T& a, const T& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), [](char c1, char c2) {
        // TODO: Do we need to make this UTF-8-aware?
        return std::tolower(c1) == std::tolower(c2);
    });
}

namespace sqlb {

using StringVector = std::vector<std::string>;

StringVector escapeIdentifier(StringVector ids);
std::string joinStringVector(const StringVector& vec, const std::string& delim);

struct StringVectorHash
{
    size_t operator()(const StringVector& key) const
    {
        // This is taken from Boost
        size_t seed = 0;
        for(const std::string& s : key)
            seed ^= std::hash<std::string>{}(s) + 0x9e3779b9 + (seed << 6) + ( seed >> 2);
        return seed;
    }
};

class Object;
class Table;
class Index;
class View;
class Trigger;
class Field;
class Constraint;
class IndexedColumn;
struct FieldInfo;
using ObjectPtr = std::shared_ptr<Object>;
using TablePtr = std::shared_ptr<Table>;
using IndexPtr = std::shared_ptr<Index>;
using ViewPtr = std::shared_ptr<View>;
using TriggerPtr = std::shared_ptr<Trigger>;
using ConstraintPtr = std::shared_ptr<Constraint>;
using FieldVector = std::vector<Field>;
using IndexedColumnVector = std::vector<IndexedColumn>;
using ConstraintMap = std::unordered_multimap<StringVector, ConstraintPtr, StringVectorHash>;
using FieldInfoList = std::vector<FieldInfo>;

struct FieldInfo
{
    FieldInfo(const std::string& name_, const std::string& type_, const std::string& sql_)
        : name(name_), type(type_), sql(sql_)
    {}

    std::string name;
    std::string type;
    std::string sql;
};

class Object
{
public:
    enum Types
    {
        Table,
        Index,
        View,
        Trigger
    };

    explicit Object(const std::string& name): m_name(name), m_fullyParsed(false) {}
    virtual ~Object() {}

    bool operator==(const Object& rhs) const;

    virtual Types type() const = 0;
    static std::string typeToString(Types type);

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    void setOriginalSql(const std::string& original_sql) { m_originalSql = original_sql; }
    std::string originalSql() const { return m_originalSql; }

    virtual std::string baseTable() const { return std::string(); }

    void setFullyParsed(bool fully_parsed) { m_fullyParsed = fully_parsed; }
    bool fullyParsed() const { return m_fullyParsed; }

    virtual FieldInfoList fieldInformation() const { return FieldInfoList(); }

    /**
     * @brief Returns the CREATE statement for this object
     * @param schema The schema name of the object
     * @param ifNotExists If set to true the "IF NOT EXISTS" qualifier will be added to the create statement
     * @return A std::string with the CREATE statement.
     */
    virtual std::string sql(const std::string& schema = std::string("main"), bool ifNotExists = false) const = 0;

protected:
    std::string m_name;
    std::string m_originalSql;
    bool m_fullyParsed;
};

class Constraint
{
public:
    enum ConstraintTypes
    {
        NoType,
        PrimaryKeyConstraintType,
        UniqueConstraintType,
        ForeignKeyConstraintType,
        CheckConstraintType,
    };

    explicit Constraint(const std::string& name = std::string())
        : m_name(name)
    {
    }
    virtual ~Constraint() {}

    virtual ConstraintTypes type() const = 0;

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    virtual std::string toSql(const StringVector& applyOn) const = 0;

protected:
    std::string m_name;
};

class ForeignKeyClause : public Constraint
{
public:
    ForeignKeyClause(const std::string& table = std::string(), const StringVector& columns = {}, const std::string& constraint = std::string())
        : m_table(table),
          m_columns(columns),
          m_constraint(constraint)
    {
    }

    bool isSet() const;
    std::string toString() const;
    void setFromString(const std::string& fk);

    void setTable(const std::string& table) { m_override.clear(); m_table = table; }
    const std::string& table() const { return m_table; }

    void setColumns(const StringVector& columns) { m_columns = columns; }
    const StringVector& columns() const { return m_columns; }

    void setConstraint(const std::string& constraint) { m_constraint = constraint; }
    const std::string& constraint() const { return m_constraint; }

    std::string toSql(const StringVector& applyOn) const override;

    ConstraintTypes type() const override { return ForeignKeyConstraintType; }

private:
    std::string m_table;
    StringVector m_columns;
    std::string m_constraint;

    std::string m_override;
};

class UniqueConstraint : public Constraint
{
public:
    UniqueConstraint() {}

    std::string toSql(const StringVector& applyOn) const override;

    ConstraintTypes type() const override { return UniqueConstraintType; }
};

class PrimaryKeyConstraint : public Constraint
{
public:
    PrimaryKeyConstraint() {}

    void setConflictAction(const std::string& conflict) { m_conflictAction = conflict; }
    const std::string& conflictAction() const { return m_conflictAction; }

    std::string toSql(const StringVector& applyOn) const override;

    ConstraintTypes type() const override { return PrimaryKeyConstraintType; }

private:
    std::string m_conflictAction;
};

class CheckConstraint : public Constraint
{
public:
    explicit CheckConstraint(const std::string& expr = std::string())
        : m_expression(expr)
    {
    }

    void setExpression(const std::string& expr) { m_expression = expr; }
    const std::string& expression() const { return m_expression; }

    std::string toSql(const StringVector& applyOn) const override;

    ConstraintTypes type() const override { return CheckConstraintType; }

private:
    std::string m_expression;
};

class Field
{
public:
    Field()
        : m_notnull(false),
          m_autoincrement(false),
          m_unique(false)
    {}

    Field(const std::string& name,
          const std::string& type,
          bool notnull = false,
          const std::string& defaultvalue = "",
          const std::string& check = "",
          bool unique = false,
          const std::string& collation = "")
        : m_name(name)
        , m_type(type)
        , m_notnull(notnull)
        , m_check(check)
        , m_defaultvalue(defaultvalue)
        , m_autoincrement(false)
        , m_unique(unique)
        , m_collation(collation)
    {}

    bool operator==(const Field& rhs) const;

    std::string toString(const std::string& indent = "\t", const std::string& sep = "\t") const;

    void setName(const std::string& name) { m_name = name; }
    void setType(const std::string& type) { m_type = type; }
    void setNotNull(bool notnull = true) { m_notnull = notnull; }
    void setCheck(const std::string& check) { m_check = check; }
    void setDefaultValue(const std::string& defaultvalue) { m_defaultvalue = defaultvalue; }
    void setAutoIncrement(bool autoinc) { m_autoincrement = autoinc; }
    void setUnique(bool u) { m_unique = u; }
    void setCollation(const std::string& collation) { m_collation = collation; }

    bool isText() const;
    bool isInteger() const;
    bool isBlob() const;
    bool isReal() const;
    bool isNumeric() const;

    // Type affinity of the column according to SQLite3 rules
    std::string affinity() const;

    const std::string& name() const { return m_name; }
    const std::string& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const std::string& check() const { return m_check; }
    const std::string& defaultValue() const { return m_defaultvalue; }
    bool autoIncrement() const { return m_autoincrement; }
    bool unique() const { return m_unique; }
    const std::string& collation() const { return m_collation; }

private:
    std::string m_name;
    std::string m_type;
    bool m_notnull;
    std::string m_check;
    std::string m_defaultvalue;
    bool m_autoincrement; //! this is stored here for simplification
    bool m_unique;
    std::string m_collation;
};

class Table : public Object
{
public:
    explicit Table(const std::string& name): Object(name), m_withoutRowid(false) {}
    Table& operator=(const Table& rhs);

    bool operator==(const Table& rhs) const;

    Types type() const override { return Object::Table; }

    FieldVector fields;
    using field_type = Field;
    using field_iterator = FieldVector::iterator;

    /**
     * @brief Returns the CREATE TABLE statement for this table object
     * @return A std::string with the CREATE TABLE object.
     */
    std::string sql(const std::string& schema = "main", bool ifNotExists = false) const override;

    StringVector fieldNames() const;

    StringVector rowidColumns() const;
    void setWithoutRowidTable(bool without_rowid) {  m_withoutRowid = without_rowid; }
    bool withoutRowidTable() const { return m_withoutRowid; }

    void setVirtualUsing(const std::string& virt_using) { m_virtual = virt_using; }
    const std::string& virtualUsing() const { return m_virtual; }
    bool isVirtual() const { return !m_virtual.empty(); }

    FieldInfoList fieldInformation() const override;

    void addConstraint(const StringVector& fields, ConstraintPtr constraint);
    void setConstraint(const StringVector& fields, ConstraintPtr constraint);
    void removeConstraints(const StringVector& fields = StringVector(), Constraint::ConstraintTypes type = Constraint::NoType); //! Only removes the first constraint, if any
    ConstraintPtr constraint(const StringVector& fields = StringVector(), Constraint::ConstraintTypes type = Constraint::NoType) const;   //! Only returns the first constraint, if any
    std::vector<ConstraintPtr> constraints(const StringVector& fields = StringVector(), Constraint::ConstraintTypes type = Constraint::NoType) const;
    ConstraintMap allConstraints() const { return m_constraints; }
    void setConstraints(const ConstraintMap& constraints);
    StringVector& primaryKeyRef();
    const StringVector& primaryKey() const;
    void removeKeyFromAllConstraints(const std::string& key);
    void renameKeyInAllConstraints(const std::string& key, const std::string& to);

    /**
     * @brief parseSQL Parses the create Table statement in sSQL.
     * @param sSQL The create table statement.
     * @return The table object. The table object may be empty if parsing failed.
     */
    static TablePtr parseSQL(const std::string& sSQL);
private:
    StringVector fieldList() const;
    bool hasAutoIncrement() const;

private:
    bool m_withoutRowid;
    ConstraintMap m_constraints;
    std::string m_virtual;
};

class IndexedColumn
{
public:
    IndexedColumn(const std::string& name, bool expr, const std::string& order = std::string())
        : m_name(name),
          m_isExpression(expr),
          m_order(order)
    {
    }

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    void setExpression(bool expr) { m_isExpression = expr; }
    bool expression() const { return m_isExpression; }

    void setOrder(const std::string& order) { m_order = order; }
    std::string order() const { return m_order; }

    std::string toString(const std::string& indent = "\t", const std::string& sep = "\t") const;

private:
    std::string m_name;
    bool m_isExpression;
    std::string m_order;
};

class Index : public Object
{
public:
    explicit Index(const std::string& name): Object(name), m_unique(false) {}
    Index& operator=(const Index& rhs);

    Types type() const override { return Object::Index; }

    IndexedColumnVector fields;
    using field_type = IndexedColumn;
    using field_iterator = IndexedColumnVector::iterator;

    std::string baseTable() const override { return m_table; }

    void setUnique(bool unique) { m_unique = unique; }
    bool unique() const { return m_unique; }

    void setTable(const std::string& table) { m_table = table; }
    const std::string& table() const { return m_table; }

    void setWhereExpr(const std::string& expr) { m_whereExpr = expr; }
    const std::string& whereExpr() const { return m_whereExpr; }

    /**
     * @brief Returns the CREATE INDEX statement for this index object
     * @return A std::string with the CREATE INDEX object.
     */
    std::string sql(const std::string& schema = "main", bool ifNotExists = false) const override;

    /**
     * @brief parseSQL Parses the CREATE INDEX statement in sSQL.
     * @param sSQL The create index statement.
     * @return The index object. The index object may be empty if the parsing failed.
     */
    static IndexPtr parseSQL(const std::string& sSQL);

    FieldInfoList fieldInformation() const override;

private:
    StringVector columnSqlList() const;

    bool m_unique;
    std::string m_table;
    std::string m_whereExpr;
};

class View : public Object
{
public:
    explicit View(const std::string& name): Object(name) {}

    Types type() const override { return Object::View; }

    FieldVector fields;

    std::string sql(const std::string& /*schema*/ = "main", bool /*ifNotExists*/ = false) const override
    { /* TODO */ return m_originalSql; }

    static ViewPtr parseSQL(const std::string& sSQL);

    StringVector fieldNames() const;

    FieldInfoList fieldInformation() const override;
};

class Trigger : public Object
{
public:
    explicit Trigger(const std::string& name): Object(name) {}

    Types type() const override { return Object::Trigger; }

    std::string sql(const std::string& /*schema*/ = "main", bool /*ifNotExists*/ = false) const override
    { /* TODO */ return m_originalSql; }

    static TriggerPtr parseSQL(const std::string& sSQL);

    std::string baseTable() const override { return m_table; }

    void setTable(const std::string& table) { m_table = table; }
    std::string table() const { return m_table; }

private:
    std::string m_table;
};

/**
 * @brief findField Finds a field in the database object and returns an iterator to it.
 * @param object
 * @param name
 * @return The iterator pointing to the field in the field container of the object if the field was found.
 *         object.fields.end() if the field couldn't be found.
 */
template<typename T>
typename T::field_iterator findField(T* object, const std::string& name)
{
    return std::find_if(object->fields.begin(), object->fields.end(), [&name](const typename T::field_type& f) {
        return compare_ci(name, f.name());
    });
}
template<typename T>
typename T::field_iterator findField(const T* object, const std::string& name)
{
    return findField(const_cast<T*>(object), name);
}
template<typename T>
typename std::remove_reference<T>::type::field_iterator findField(std::shared_ptr<T> object, const std::string& name)
{
    return findField(object.get(), name);
}
template<typename T>
typename std::remove_reference<T>::type::field_iterator findField(T& object, const std::string& name)
{
    return findField(&object, name);
}

template<typename T> struct is_shared_ptr : std::false_type {};
template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

/**
 * @brief removeField Finds and removes a field in the database object
 * @param object
 * @param name
 * @return true if sucessful, otherwise false
 */
template<typename T>
bool removeField(T* object, const std::string& name)
{
    auto index = findField(object, name);
    if(index != object->fields.end())
    {
        object->fields.erase(index);
        return true;
    }
    return false;
}
template<typename T, typename = typename std::enable_if<is_shared_ptr<T>::value>::type>
bool removeField(T object, const std::string& name)
{
    return removeField(object.get(), name);
}
template<typename T, typename = typename std::enable_if<!is_shared_ptr<T>::value>::type>
bool removeField(T& object, const std::string& name)
{
    return removeField(&object, name);
}

} //namespace sqlb

#endif
