#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include <algorithm>
#include <cctype>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

template<typename C, typename E>
bool contains(const C& container, E element)
{
    return std::find(container.begin(), container.end(), element) != container.end();
}

template<typename T1, typename T2, typename E>
bool contains(const std::map<T1, T2>& container, E element)
{
    return container.find(element) != container.end();
}

template<typename T>
bool compare_ci(const T& a, const T& b)
{
    // Note: This function does not have to be (actually it must not be) fully UTF-8 aware because SQLite itself is not either.

    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](unsigned char c1, unsigned char c2) {
        return std::tolower(c1) == std::tolower(c2);
    });
}

template<typename T>
bool compare_ci(const T& a, const char* b)
{
    return compare_ci(a, std::string(b));
}

inline bool starts_with_ci(const std::string& str, const std::string& with)
{
    if(str.size() < with.size())
        return false;
    else
        return compare_ci(str.substr(0, with.size()), with);
}

namespace sqlb {

using StringVector = std::vector<std::string>;

StringVector escapeIdentifier(StringVector ids);
std::string joinStringVector(const StringVector& vec, const std::string& delim);

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
using ConstraintSet = std::set<ConstraintPtr>;
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
    virtual ~Object() = default;

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
        PrimaryKeyConstraintType,
        UniqueConstraintType,
        ForeignKeyConstraintType,
        CheckConstraintType,
        GeneratedColumnConstraintType,
        NotNullConstraintType,
        DefaultConstraintType,
        CollateConstraintType,

        NoType = 999,
    };

    explicit Constraint(const StringVector& columns = {}, const std::string& name = std::string())
        : column_list(columns),
          m_name(name)
    {
    }
    virtual ~Constraint() = default;

    static ConstraintPtr makeConstraint(ConstraintTypes type);

    virtual ConstraintTypes type() const = 0;

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    StringVector columnList() const { return column_list; }
    virtual void setColumnList(const StringVector& list) { column_list = list; }
    virtual void addToColumnList(const std::string& key) { column_list.push_back(key); }
    virtual void replaceInColumnList(const std::string& from, const std::string& to);
    virtual void removeFromColumnList(const std::string& key);

    virtual std::string toSql() const = 0;

protected:
    StringVector column_list;
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

    std::string toSql() const override;

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
    explicit UniqueConstraint(const IndexedColumnVector& columns = {});
    explicit UniqueConstraint(const StringVector& columns);

    void setConflictAction(const std::string& conflict) { m_conflictAction = conflict; }
    const std::string& conflictAction() const { return m_conflictAction; }

    // We override these because we maintain our own copy of the column_list variable in m_columns.
    // This needs to be done because in a unique constraint we can add expressions, sort order, etc. to the
    // list of columns.
    void setColumnList(const StringVector& list) override;
    void addToColumnList(const std::string& key) override;
    void replaceInColumnList(const std::string& from, const std::string& to) override;
    void removeFromColumnList(const std::string& key) override;

    std::string toSql() const override;

    ConstraintTypes type() const override { return UniqueConstraintType; }

protected:
    IndexedColumnVector m_columns;
    std::string m_conflictAction;
};


class NotNullConstraint : public Constraint
{
public:
    void setConflictAction(const std::string& conflict) { m_conflictAction = conflict; }
    const std::string& conflictAction() const { return m_conflictAction; }

    std::string toSql() const override;

    ConstraintTypes type() const override { return NotNullConstraintType; }

protected:
    std::string m_conflictAction;
};


class PrimaryKeyConstraint : public UniqueConstraint
{
    // Primary keys are a sort of unique constraint for us. This matches quite nicely as both can have a conflict action
    // and both need to maintain a copy of the column list with sort order information etc.

public:
    explicit PrimaryKeyConstraint(const IndexedColumnVector& columns = {});
    explicit PrimaryKeyConstraint(const StringVector& columns);

    void setAutoIncrement(bool ai) { m_auto_increment = ai; }
    bool autoIncrement() const { return m_auto_increment; }

    std::string toSql() const override;

    ConstraintTypes type() const override { return PrimaryKeyConstraintType; }

private:
    bool m_auto_increment;
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

    std::string toSql() const override;

    ConstraintTypes type() const override { return CheckConstraintType; }

private:
    std::string m_expression;
};

class DefaultConstraint : public Constraint
{
public:
    explicit DefaultConstraint(const std::string& value = std::string())
        : m_value(value)
    {
    }

    void setValue(const std::string& value) { m_value = value; }
    const std::string& value() const { return m_value; }

    std::string toSql() const override;

    ConstraintTypes type() const override { return DefaultConstraintType; }

private:
    std::string m_value;
};

class CollateConstraint : public Constraint
{
public:
    explicit CollateConstraint(const std::string& collation)
        : m_collation(collation)
    {
    }

    void setCollation(const std::string& collation) { m_collation = collation; }
    const std::string& collation() const { return m_collation; }

    std::string toSql() const override;

    ConstraintTypes type() const override { return CollateConstraintType; }

private:
    std::string m_collation;
};

class GeneratedColumnConstraint : public Constraint
{
public:
    explicit GeneratedColumnConstraint(const std::string& expr = std::string(), const std::string& storage = "VIRTUAL")
        : m_expression(expr),
          m_storage(storage)
    {
    }

    void setExpression(const std::string& expr) { m_expression = expr; }
    const std::string& expression() const { return m_expression; }

    void setStorage(const std::string& storage) { m_storage = storage; }
    std::string storage() const { return m_storage.empty() ? "VIRTUAL" : m_storage; }

    std::string toSql() const override;

    ConstraintTypes type() const override { return GeneratedColumnConstraintType; }

private:
    std::string m_expression;
    std::string m_storage;
};

class Field
{
public:
    Field()
    {}

    Field(const std::string& name,
          const std::string& type,
          bool notnull = false,
          const std::string& defaultvalue = std::string(),
          const std::string& check = std::string(),
          bool unique = false,
          const std::string& collation = std::string())
        : m_name(name)
        , m_type(type)
        , m_notnull(notnull ? std::make_shared<NotNullConstraint>() : nullptr)
        , m_check(check.empty() ? nullptr : std::make_shared<CheckConstraint>(check))
        , m_defaultvalue(defaultvalue.empty() ? nullptr : std::make_shared<DefaultConstraint>(defaultvalue))
        , m_unique(unique ? std::make_shared<UniqueConstraint>() : nullptr)
        , m_collation(collation.empty() ? nullptr : std::make_shared<CollateConstraint>(collation))
    {}

    bool operator==(const Field& rhs) const;

    std::string toString(const std::string& indent = "\t", const std::string& sep = "\t") const;

    void setName(const std::string& name) { m_name = name; }
    void setType(const std::string& type) { m_type = type; }
    void setNotNull(std::shared_ptr<NotNullConstraint> notnull) { m_notnull = notnull; }
    void setNotNull(bool notnull = true) { if(notnull) m_notnull = std::make_shared<NotNullConstraint>(); else m_notnull.reset(); }
    void setCheck(std::shared_ptr<CheckConstraint> check) { m_check = check; }
    void setCheck(const std::string& check) { if(!check.empty()) m_check = std::make_shared<CheckConstraint>(check); else m_check.reset(); }
    void setDefaultValue(std::shared_ptr<DefaultConstraint> defaultvalue) { m_defaultvalue = defaultvalue; }
    void setDefaultValue(const std::string& value) { if(!value.empty()) m_defaultvalue = std::make_shared<DefaultConstraint>(value); else m_defaultvalue.reset(); }
    void setUnique(std::shared_ptr<UniqueConstraint> u) { m_unique = u; }
    void setUnique(bool u) { if(u) m_unique = std::make_shared<UniqueConstraint>(); else m_unique.reset(); }
    void setCollation(std::shared_ptr<CollateConstraint> c) { m_collation = c; }
    void setCollation(const std::string& collation) { if(!collation.empty()) m_collation = std::make_shared<CollateConstraint>(collation); else m_collation.reset(); }

    bool isText() const;
    bool isInteger() const;
    bool isBlob() const;
    bool isReal() const;
    bool isNumeric() const;

    // Type affinity of the column according to SQLite3 rules.
    // The Affinity enum values match the SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_BLOB, and SQLITE_TEXT constants
    enum Affinity
    {
        IntegerAffinity = 1,
        FloatAffinity = 2,
        TextAffinity = 3,
        BlobAffinity = 4,
    };
    Affinity affinity() const;

    const std::string& name() const { return m_name; }
    const std::string& type() const { return m_type; }
    bool notnull() const { return m_notnull ? true : false; }
    std::string check() const { return m_check ? m_check->expression() : std::string{}; }
    std::string defaultValue() const { return m_defaultvalue ? m_defaultvalue->value() : std::string{}; }
    bool unique() const { return m_unique ? true : false; }
    std::string collation() const { return m_collation ? m_collation->collation() : std::string{}; }

    const std::shared_ptr<GeneratedColumnConstraint> generated() const { return m_generated; }
    std::shared_ptr<GeneratedColumnConstraint> generated() { return m_generated; }
    void setGenerated(std::shared_ptr<GeneratedColumnConstraint> gen) { m_generated = gen; }

private:
    std::string m_name;
    std::string m_type;
    std::shared_ptr<NotNullConstraint> m_notnull;
    std::shared_ptr<CheckConstraint> m_check;
    std::shared_ptr<DefaultConstraint> m_defaultvalue;
    std::shared_ptr<UniqueConstraint> m_unique;
    std::shared_ptr<CollateConstraint> m_collation;
    std::shared_ptr<GeneratedColumnConstraint> m_generated;
};

class Table : public Object
{
public:
    explicit Table(const std::string& name): Object(name), m_withoutRowid(false) {}
    explicit Table(const Table& table);
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

    void addConstraint(ConstraintPtr constraint);
    void setConstraint(ConstraintPtr constraint);
    void removeConstraint(ConstraintPtr constraint);
    void removeConstraints(const StringVector& vStrFields = StringVector(), Constraint::ConstraintTypes type = Constraint::NoType);
    ConstraintPtr constraint(const StringVector& vStrFields = StringVector(), Constraint::ConstraintTypes type = Constraint::NoType) const;   //! Only returns the first constraint, if any
    std::vector<ConstraintPtr> constraints(const StringVector& vStrFields = StringVector(), Constraint::ConstraintTypes type = Constraint::NoType) const;
    ConstraintSet allConstraints() const { return m_constraints; }
    void setConstraints(const ConstraintSet& constraints);
    void replaceConstraint(ConstraintPtr from, ConstraintPtr to);
    std::shared_ptr<PrimaryKeyConstraint> primaryKey();
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

private:
    bool m_withoutRowid;
    ConstraintSet m_constraints;
    std::string m_virtual;
};

class IndexedColumn
{
public:
    IndexedColumn()
        : m_isExpression(false)
    {
    }

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
