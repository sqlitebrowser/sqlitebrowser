#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <QStringList>
#include <QMultiHash>

namespace sqlb {

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
template<typename T>
uint qHash(const QVector<T>& key, uint seed = 0)
    Q_DECL_NOEXCEPT_EXPR(noexcept(qHashRange(key.cbegin(), key.cend(), seed)))
{
    return qHashRange(key.cbegin(), key.cend(), seed);
}
#endif

QString escapeIdentifier(QString id);

class Object;
class Table;
class Index;
class View;
class Trigger;
class Field;
class Constraint;
class IndexedColumn;
struct FieldInfo;
typedef QSharedPointer<Object> ObjectPtr;
typedef QSharedPointer<Table> TablePtr;
typedef QSharedPointer<Index> IndexPtr;
typedef QSharedPointer<View> ViewPtr;
typedef QSharedPointer<Trigger> TriggerPtr;
typedef QSharedPointer<Field> FieldPtr;
typedef QSharedPointer<Constraint> ConstraintPtr;
typedef QVector<FieldPtr> FieldVector;
typedef QSharedPointer<IndexedColumn> IndexedColumnPtr;
typedef QVector<IndexedColumnPtr> IndexedColumnVector;
typedef QMultiHash<FieldVector, ConstraintPtr> ConstraintMap;
typedef QList<FieldInfo> FieldInfoList;

QStringList fieldVectorToFieldNames(const sqlb::FieldVector& vector);

struct FieldInfo
{
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

    explicit Object(const QString& name): m_name(name), m_temporary(false), m_fullyParsed(false) {}
    virtual ~Object() {}

    virtual Types type() const = 0;
    static QString typeToString(Types type);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setOriginalSql(const QString& original_sql) { m_originalSql = original_sql; }
    QString originalSql() const { return m_originalSql; }

    void setTemporary(bool temp) { m_temporary = temp; }
    bool isTemporary() const { return m_temporary; }

    virtual QString baseTable() const { return QString(); }

    void setFullyParsed(bool fully_parsed) { m_fullyParsed = fully_parsed; }
    bool fullyParsed() const { return m_fullyParsed; }

    virtual FieldInfoList fieldInformation() const { return FieldInfoList(); }

    virtual void clear() {}

    /**
     * @brief Returns the CREATE statement for this object
     * @param ifNotExists If set to true the "IF NOT EXISTS" qualifier will be added to the create statement
     * @return A QString with the CREATE statement.
     */
    virtual QString sql(bool ifNotExists = false) const = 0;

    /**
     * @brief parseSQL Parses the CREATE statement in sSQL.
     * @param sSQL The type of the object.
     * @param sSQL The create statement.
     * @return The parsed database object. The object may be empty if parsing failed.
     */
    static ObjectPtr parseSQL(Object::Types type, const QString& sSQL);

protected:
    QString m_name;
    QString m_originalSql;
    bool m_temporary;
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

    Constraint(const QString& name = QString())
        : m_name(name)
    {
    }
    virtual ~Constraint() {}

    virtual ConstraintTypes type() const = 0;

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    virtual QString toSql(const FieldVector& applyOn) const = 0;

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

    virtual QString toSql(const FieldVector& applyOn) const;

    virtual ConstraintTypes type() const { return ForeignKeyConstraintType; }

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

    virtual QString toSql(const FieldVector& applyOn) const;

    virtual ConstraintTypes type() const { return UniqueConstraintType; }
};

class PrimaryKeyConstraint : public Constraint
{
public:
    PrimaryKeyConstraint() {}

    virtual QString toSql(const FieldVector& applyOn) const;

    virtual ConstraintTypes type() const { return PrimaryKeyConstraintType; }
};

class CheckConstraint : public Constraint
{
public:
    CheckConstraint(const QString& expr = QString())
        : m_expression(expr)
    {
    }

    void setExpression(const QString& expr) { m_expression = expr; }
    QString expression() const { return m_expression; }

    virtual QString toSql(const FieldVector& applyOn) const;

    virtual ConstraintTypes type() const { return CheckConstraintType; }

private:
    QString m_expression;
};

class Field
{
public:
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

    const QString& name() const { return m_name; }
    const QString& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const QString& check() const { return m_check; }
    const QString& defaultValue() const { return m_defaultvalue; }
    bool autoIncrement() const { return m_autoincrement; }
    bool unique() const { return m_unique; }
    const QString& collation() const { return m_collation; }

    static QStringList Datatypes;
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
    virtual ~Table();
    Table& operator=(const Table& rhs);

    virtual Types type() const { return Object::Table; }

    const FieldVector& fields() const { return m_fields; }

    /**
     * @brief Returns the CREATE TABLE statement for this table object
     * @return A QString with the CREATE TABLE object.
     */
    QString sql(bool ifNotExists = false) const;

    void addField(const FieldPtr& f);
    bool removeField(const QString& sFieldName);
    void setFields(const FieldVector& fields);
    void setField(int index, FieldPtr f);
    const FieldPtr& field(int index) const { return m_fields[index]; }
    QStringList fieldNames() const;

    void setRowidColumn(const QString& rowid) {  m_rowidColumn = rowid; }
    const QString& rowidColumn() const { return m_rowidColumn; }
    bool isWithoutRowidTable() const { return m_rowidColumn != "_rowid_"; }

    void setVirtualUsing(const QString& virt_using) { m_virtual = virt_using; }
    QString virtualUsing() const { return m_virtual; }
    bool isVirtual() const { return !m_virtual.isEmpty(); }

    void clear();

    virtual FieldInfoList fieldInformation() const;

    void addConstraint(FieldVector fields, ConstraintPtr constraint);
    void setConstraint(FieldVector fields, ConstraintPtr constraint);
    void removeConstraints(FieldVector fields = FieldVector(), Constraint::ConstraintTypes type = Constraint::NoType); //! Only removes the first constraint, if any
    ConstraintPtr constraint(FieldVector fields = FieldVector(), Constraint::ConstraintTypes type = Constraint::NoType) const;   //! Only returns the first constraint, if any
    QList<ConstraintPtr> constraints(FieldVector fields = FieldVector(), Constraint::ConstraintTypes type = Constraint::NoType) const;
    ConstraintMap allConstraints() const { return m_constraints; }
    void setConstraints(const ConstraintMap& constraints);
    FieldVector& primaryKeyRef();
    const FieldVector& primaryKey() const;

    /**
     * @brief findField Finds a field and returns the index.
     * @param sname
     * @return The field index if the field was found.
     *         -1 if field couldn't be found.
     */
    int findField(const QString& sname) const;

    int findPk() const;

    /**
     * @brief parseSQL Parses the create Table statement in sSQL.
     * @param sSQL The create table statement.
     * @return The table object. The table object may be empty if parsing failed.
     */
    static ObjectPtr parseSQL(const QString& sSQL);
private:
    QStringList fieldList() const;
    bool hasAutoIncrement() const;

private:
    FieldVector m_fields;
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
    virtual ~Index();
    Index& operator=(const Index& rhs);

    virtual Types type() const { return Object::Index; }

    virtual QString baseTable() const { return m_table; }

    void setUnique(bool unique) { m_unique = unique; }
    bool unique() const { return m_unique; }

    void setTable(const QString& table) { m_table = table; }
    const QString& table() const { return m_table; }

    void setWhereExpr(const QString& expr) { m_whereExpr = expr; }
    const QString& whereExpr() const { return m_whereExpr; }

    void setColumns(const IndexedColumnVector& columns);
    const IndexedColumnVector& columns() const { return m_columns; }
    void clearColumns() { m_columns.clear(); }
    void addColumn(const IndexedColumnPtr& c) { m_columns.append(c); }
    bool removeColumn(const QString& name);
    void setColumn(int index, IndexedColumnPtr c) { m_columns[index] = c; }
    IndexedColumnPtr& column(int index) { return m_columns[index]; }
    int findColumn(const QString& name) const;

    void clear();

    /**
     * @brief Returns the CREATE INDEX statement for this index object
     * @return A QString with the CREATE INDEX object.
     */
    QString sql(bool ifNotExists = false) const;

    /**
     * @brief parseSQL Parses the CREATE INDEX statement in sSQL.
     * @param sSQL The create index statement.
     * @return The index object. The index object may be empty if the parsing failed.
     */
    static ObjectPtr parseSQL(const QString& sSQL);

    virtual FieldInfoList fieldInformation() const;

private:
    QStringList columnSqlList() const;

    bool m_unique;
    QString m_table;
    QString m_whereExpr;
    IndexedColumnVector m_columns;
};

class View : public Object
{
public:
    explicit View(const QString& name): Object(name) {}
    virtual ~View();

    virtual Types type() const { return Object::View; }

    QString sql(bool ifNotExists = false) const { /* TODO */ Q_UNUSED(ifNotExists); return m_originalSql; }

    static ObjectPtr parseSQL(const QString& sSQL);

    void clear();

    void addField(const FieldPtr& f);
    void setFields(const FieldVector& fields);
    const FieldPtr& field(int index) const { return m_fields[index]; }
    QStringList fieldNames() const;

    virtual FieldInfoList fieldInformation() const;

private:
    FieldVector m_fields;
};

class Trigger : public Object
{
public:
    explicit Trigger(const QString& name): Object(name) {}
    virtual ~Trigger() {}

    virtual Types type() const { return Object::Trigger; }

    QString sql(bool ifNotExists = false) const { /* TODO */ Q_UNUSED(ifNotExists); return m_originalSql; }

    static ObjectPtr parseSQL(const QString& sSQL);

    virtual QString baseTable() const { return m_table; }

    void setTable(const QString& table) { m_table = table; }
    QString table() const { return m_table; }

private:
    QString m_table;
};

} //namespace sqlb

#endif // SQLITETYPES_H
