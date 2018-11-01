#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <QString>
#include <QVariant>

template<typename C, typename E>
bool contains(const C& container, E element)
{
    return std::find(container.begin(), container.end(), element) != container.end();
}

namespace sqlb {

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
struct QHashCombine {
    typedef uint result_type;
    template <typename T>
    Q_DECL_CONSTEXPR result_type operator()(uint seed, const T &t) const Q_DECL_NOEXCEPT_EXPR(noexcept(qHash(t)))
    // combiner taken from N3876 / boost::hash_combine
    { return seed ^ (qHash(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2)) ; }
};

template <typename InputIterator>
inline uint qHashRange(InputIterator first, InputIterator last, uint seed = 0)
    Q_DECL_NOEXCEPT_EXPR(noexcept(qHash(*first))) // assume iterator operations don't throw
{
    return std::accumulate(first, last, seed, QHashCombine());
}
#endif

template<typename T>
uint qHash(const QList<T>& key, uint seed = 0)
    Q_DECL_NOEXCEPT_EXPR(noexcept(qHashRange(key.cbegin(), key.cend(), seed)))
{
    return qHashRange(key.cbegin(), key.cend(), seed);
}
#endif

enum escapeQuoting {
    DoubleQuotes,
    GraveAccents,
    SquareBrackets
};

// Set quoting style for escapeIdentifier
void setIdentifierQuoting(escapeQuoting toQuoting);

QString escapeIdentifier(QString id);
std::string escapeIdentifier(std::string id);

class ObjectIdentifier
{
public:
    ObjectIdentifier(const QString& schema, const QString& name)
        : m_schema(schema),
          m_name(name)
    {
    }

    ObjectIdentifier()
        : m_schema("main"),
          m_name(QString())
    {
    }

    explicit ObjectIdentifier(QVariant variant)
    {
        QStringList str = variant.toStringList();
        if(str.size())
        {
            m_schema = str.first();
            if(str.size() >= 2)
                m_name = str.last();
        }
    }

    bool operator==(const ObjectIdentifier& rhs) const
    {
        return (rhs.m_schema == m_schema && rhs.m_name == m_name);
    }

    bool operator<(const ObjectIdentifier& rhs) const
    {
        return toDisplayString() < rhs.toDisplayString();
    }

    const QString& schema() const { return m_schema; }
    const QString& name() const { return m_name; }
    void setSchema(const QString& schema) { m_schema = schema; }
    void setName(const QString& name) { m_name = name; }

    void clear()
    {
        m_schema = "main";
        m_name.clear();
    }

    bool isEmpty() const { return m_name.isEmpty(); }

    // This returns a string which can be used in SQL statements
    QString toString(bool shortName = false) const
    {
        if(shortName && m_schema == "main")
            return sqlb::escapeIdentifier(m_name);
        else
            return QString("%1.%2").arg(sqlb::escapeIdentifier(m_schema)).arg(sqlb::escapeIdentifier(m_name));
    }

    // This returns a string which can be used in the user interface
    QString toDisplayString() const
    {
        if(m_schema == "main")
            return m_name;
        else
            return QString("%1.%2").arg(m_schema).arg(m_name);
    }

    QVariant toVariant() const
    {
        QStringList result;
        result << m_schema << m_name;
        return QVariant(result);
    }

private:
    QString m_schema;
    QString m_name;
};

struct StringListHash
{
    size_t operator()(const QStringList& key) const
    {
        return qHash(key);
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
using ConstraintMap = std::unordered_multimap<QStringList, ConstraintPtr, StringListHash>;
using FieldInfoList = std::vector<FieldInfo>;

struct FieldInfo
{
    FieldInfo(const QString& name_, const QString& type_, const QString& sql_)
        : name(name_), type(type_), sql(sql_)
    {}

    QString name;
    QString type;
    QString sql;
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

    explicit Object(const QString& name): m_name(name), m_fullyParsed(false) {}
    virtual ~Object() {}

    virtual Types type() const = 0;
    static QString typeToString(Types type);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setOriginalSql(const QString& original_sql) { m_originalSql = original_sql; }
    QString originalSql() const { return m_originalSql; }

    virtual QString baseTable() const { return QString(); }

    void setFullyParsed(bool fully_parsed) { m_fullyParsed = fully_parsed; }
    bool fullyParsed() const { return m_fullyParsed; }

    virtual FieldInfoList fieldInformation() const { return FieldInfoList(); }

    /**
     * @brief Returns the CREATE statement for this object
     * @param schema The schema name of the object
     * @param ifNotExists If set to true the "IF NOT EXISTS" qualifier will be added to the create statement
     * @return A QString with the CREATE statement.
     */
    virtual QString sql(const QString& schema = QString("main"), bool ifNotExists = false) const = 0;

protected:
    QString m_name;
    QString m_originalSql;
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

    explicit Constraint(const QString& name = QString())
        : m_name(name)
    {
    }
    virtual ~Constraint() {}

    virtual ConstraintTypes type() const = 0;

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    virtual QString toSql(const QStringList& applyOn) const = 0;

protected:
    QString m_name;
};

class ForeignKeyClause : public Constraint
{
public:
    ForeignKeyClause(const QString& table = QString(), const QStringList& columns = QStringList(), const QString& constraint = QString())
        : m_table(table),
          m_columns(columns),
          m_constraint(constraint),
          m_override(QString())
    {
    }

    bool isSet() const;
    QString toString() const;
    void setFromString(const QString& fk);

    void setTable(const QString& table) { m_override = QString(); m_table = table; }
    const QString& table() const { return m_table; }

    void setColumns(const QStringList& columns) { m_columns = columns; }
    const QStringList& columns() const { return m_columns; }

    void setConstraint(const QString& constraint) { m_constraint = constraint; }
    const QString& constraint() const { return m_constraint; }

    QString toSql(const QStringList& applyOn) const override;

    ConstraintTypes type() const override { return ForeignKeyConstraintType; }

private:
    QString m_table;
    QStringList m_columns;
    QString m_constraint;

    QString m_override;
};

class UniqueConstraint : public Constraint
{
public:
    UniqueConstraint() {}

    QString toSql(const QStringList& applyOn) const override;

    ConstraintTypes type() const override { return UniqueConstraintType; }
};

class PrimaryKeyConstraint : public Constraint
{
public:
    PrimaryKeyConstraint() {}

    void setConflictAction(const QString& conflict) { m_conflictAction = conflict; }
    const QString& conflictAction() const { return m_conflictAction; }

    QString toSql(const QStringList& applyOn) const override;

    ConstraintTypes type() const override { return PrimaryKeyConstraintType; }

private:
    QString m_conflictAction;
};

class CheckConstraint : public Constraint
{
public:
    explicit CheckConstraint(const QString& expr = QString())
        : m_expression(expr)
    {
    }

    void setExpression(const QString& expr) { m_expression = expr; }
    QString expression() const { return m_expression; }

    QString toSql(const QStringList& applyOn) const override;

    ConstraintTypes type() const override { return CheckConstraintType; }

private:
    QString m_expression;
};

class Field
{
public:
    Field()
        : m_notnull(false),
          m_autoincrement(false),
          m_unique(false)
    {}

    Field(const QString& name,
          const QString& type,
          bool notnull = false,
          const QString& defaultvalue = "",
          const QString& check = "",
          bool unique = false,
          const QString& collation = QString())
        : m_name(name)
        , m_type(type)
        , m_notnull(notnull)
        , m_check(check)
        , m_defaultvalue(defaultvalue)
        , m_autoincrement(false)
        , m_unique(unique)
        , m_collation(collation)
    {}

    bool operator==(const Field& rhs) const
    {
        return m_name == rhs.name();
    }

    QString toString(const QString& indent = "\t", const QString& sep = "\t") const;

    void setName(const QString& name) { m_name = name; }
    void setType(const QString& type) { m_type = type; }
    void setNotNull(bool notnull = true) { m_notnull = notnull; }
    void setCheck(const QString& check) { m_check = check; }
    void setDefaultValue(const QString& defaultvalue) { m_defaultvalue = defaultvalue; }
    void setAutoIncrement(bool autoinc) { m_autoincrement = autoinc; }
    void setUnique(bool u) { m_unique = u; }
    void setCollation(const QString& collation) { m_collation = collation; }

    bool isText() const;
    bool isInteger() const;
    bool isBlob() const;
    bool isReal() const;
    bool isNumeric() const;

    // Type affinity of the column according to SQLite3 rules
    QString affinity() const;

    const QString& name() const { return m_name; }
    const QString& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const QString& check() const { return m_check; }
    const QString& defaultValue() const { return m_defaultvalue; }
    bool autoIncrement() const { return m_autoincrement; }
    bool unique() const { return m_unique; }
    const QString& collation() const { return m_collation; }

private:
    QString m_name;
    QString m_type;
    bool m_notnull;
    QString m_check;
    QString m_defaultvalue;
    bool m_autoincrement; //! this is stored here for simplification
    bool m_unique;
    QString m_collation;
};

class Table : public Object
{
public:
    explicit Table(const QString& name): Object(name), m_rowidColumn("_rowid_") {}
    ~Table() override;
    Table& operator=(const Table& rhs);

    Types type() const override { return Object::Table; }

    FieldVector fields;
    using field_type = Field;
    using field_iterator = FieldVector::iterator;

    /**
     * @brief Returns the CREATE TABLE statement for this table object
     * @return A QString with the CREATE TABLE object.
     */
    QString sql(const QString& schema = QString("main"), bool ifNotExists = false) const override;

    QStringList fieldNames() const;

    void setRowidColumn(const QString& rowid) {  m_rowidColumn = rowid; }
    const QString& rowidColumn() const { return m_rowidColumn; }
    bool isWithoutRowidTable() const { return m_rowidColumn != "_rowid_"; }

    void setVirtualUsing(const QString& virt_using) { m_virtual = virt_using; }
    QString virtualUsing() const { return m_virtual; }
    bool isVirtual() const { return !m_virtual.isEmpty(); }

    FieldInfoList fieldInformation() const override;

    void addConstraint(QStringList fields, ConstraintPtr constraint);
    void setConstraint(QStringList fields, ConstraintPtr constraint);
    void removeConstraints(QStringList fields = QStringList(), Constraint::ConstraintTypes type = Constraint::NoType); //! Only removes the first constraint, if any
    ConstraintPtr constraint(QStringList fields = QStringList(), Constraint::ConstraintTypes type = Constraint::NoType) const;   //! Only returns the first constraint, if any
    std::vector<ConstraintPtr> constraints(QStringList fields = QStringList(), Constraint::ConstraintTypes type = Constraint::NoType) const;
    ConstraintMap allConstraints() const { return m_constraints; }
    void setConstraints(const ConstraintMap& constraints);
    QStringList& primaryKeyRef();
    const QStringList& primaryKey() const;
    void removeKeyFromAllConstraints(const QString& key);
    void renameKeyInAllConstraints(const QString& key, const QString& to);

    field_iterator findPk();

    /**
     * @brief parseSQL Parses the create Table statement in sSQL.
     * @param sSQL The create table statement.
     * @return The table object. The table object may be empty if parsing failed.
     */
    static TablePtr parseSQL(const QString& sSQL);
private:
    QStringList fieldList() const;
    bool hasAutoIncrement() const;

private:
    QString m_rowidColumn;
    ConstraintMap m_constraints;
    QString m_virtual;
};

class IndexedColumn
{
public:
    IndexedColumn(const QString& name, bool expr, const QString& order = QString())
        : m_name(name),
          m_isExpression(expr),
          m_order(order)
    {
    }

    void setName(const QString& name) { m_name = name; }
    QString name() const { return m_name; }

    void setExpression(bool expr) { m_isExpression = expr; }
    bool expression() const { return m_isExpression; }

    void setOrder(const QString& order) { m_order = order.toUpper(); }
    QString order() const { return m_order; }

    QString toString(const QString& indent = "\t", const QString& sep = "\t") const;

private:
    QString m_name;
    bool m_isExpression;
    QString m_order;
};

class Index : public Object
{
public:
    explicit Index(const QString& name): Object(name), m_unique(false) {}
    ~Index() override;
    Index& operator=(const Index& rhs);

    Types type() const override { return Object::Index; }

    IndexedColumnVector fields;
    using field_type = IndexedColumn;
    using field_iterator = IndexedColumnVector::iterator;

    QString baseTable() const override { return m_table; }

    void setUnique(bool unique) { m_unique = unique; }
    bool unique() const { return m_unique; }

    void setTable(const QString& table) { m_table = table; }
    const QString& table() const { return m_table; }

    void setWhereExpr(const QString& expr) { m_whereExpr = expr; }
    const QString& whereExpr() const { return m_whereExpr; }

    /**
     * @brief Returns the CREATE INDEX statement for this index object
     * @return A QString with the CREATE INDEX object.
     */
    QString sql(const QString& schema = QString("main"), bool ifNotExists = false) const override;

    /**
     * @brief parseSQL Parses the CREATE INDEX statement in sSQL.
     * @param sSQL The create index statement.
     * @return The index object. The index object may be empty if the parsing failed.
     */
    static IndexPtr parseSQL(const QString& sSQL);

    FieldInfoList fieldInformation() const override;

private:
    QStringList columnSqlList() const;

    bool m_unique;
    QString m_table;
    QString m_whereExpr;
};

class View : public Object
{
public:
    explicit View(const QString& name): Object(name) {}
    ~View() override;

    Types type() const override { return Object::View; }

    FieldVector fields;

    QString sql(const QString& schema = QString("main"), bool ifNotExists = false) const override
    { /* TODO */ Q_UNUSED(schema); Q_UNUSED(ifNotExists); return m_originalSql; }

    static ViewPtr parseSQL(const QString& sSQL);

    QStringList fieldNames() const;

    FieldInfoList fieldInformation() const override;
};

class Trigger : public Object
{
public:
    explicit Trigger(const QString& name): Object(name) {}
    ~Trigger() override {}

    Types type() const override { return Object::Trigger; }

    QString sql(const QString& schema = QString("main"), bool ifNotExists = false) const override
    { /* TODO */ Q_UNUSED(schema); Q_UNUSED(ifNotExists); return m_originalSql; }

    static TriggerPtr parseSQL(const QString& sSQL);

    QString baseTable() const override { return m_table; }

    void setTable(const QString& table) { m_table = table; }
    QString table() const { return m_table; }

private:
    QString m_table;
};

/**
 * @brief findField Finds a field in the database object and returns an iterator to it.
 * @param object
 * @param name
 * @return The iterator pointing to the field in the field container of the object if the field was found.
 *         object.fields.end() if the field couldn't be found.
 */
template<typename T>
typename T::field_iterator findField(T* object, const QString& name)
{
    return std::find_if(object->fields.begin(), object->fields.end(), [&name](const typename T::field_type& f) {return f.name().compare(name, Qt::CaseInsensitive) == 0;});
}
template<typename T>
typename T::field_iterator findField(std::shared_ptr<T> object, const QString& name)
{
    return findField(object.get(), name);
}
template<typename T>
typename T::field_iterator findField(T& object, const QString& name)
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
bool removeField(T* object, const QString& name)
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
bool removeField(T object, const QString& name)
{
    return removeField(object.get(), name);
}
template<typename T, typename = typename std::enable_if<!is_shared_ptr<T>::value>::type>
bool removeField(T& object, const QString& name)
{
    return removeField(&object, name);
}

} //namespace sqlb

#endif
