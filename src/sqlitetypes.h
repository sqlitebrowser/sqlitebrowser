#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include "antlr/ASTRefCount.hpp"

#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <QStringList>
#include <QPair>
#include <QMultiHash>

namespace sqlb {

QString escapeIdentifier(QString id);

class Object;
class Table;
class Index;
class Field;
class Constraint;
class IndexedColumn;
typedef QSharedPointer<Object> ObjectPtr;
typedef QSharedPointer<Table> TablePtr;
typedef QSharedPointer<Index> IndexPtr;
typedef QSharedPointer<Field> FieldPtr;
typedef QSharedPointer<Constraint> ConstraintPtr;
typedef QVector<FieldPtr> FieldVector;
typedef QSharedPointer<IndexedColumn> IndexedColumnPtr;
typedef QVector<IndexedColumnPtr> IndexedColumnVector;

class Object
{
public:
    enum ObjectTypes
    {
        Table,
        Index,
        View,
        Trigger
    };

    explicit Object(const QString& name): m_name(name), m_temporary(false) {}
    virtual ~Object() {}

    virtual ObjectTypes type() const = 0;

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setOriginalSql(const QString& original_sql) { m_originalSql = original_sql; }
    QString originalSql() const { return m_originalSql; }

    void setTemporary(bool temp) { m_temporary = temp; }
    bool isTemporary() const { return m_temporary; }

    virtual QString baseTable() const { return QString(); }

    virtual void clear() {}

    /**
     * @brief Returns the CREATE statement for this object
     * @return A QString with the CREATE statement.
     */
    virtual QString sql() const = 0;

    /**
     * @brief parseSQL Parses the CREATE statement in sSQL.
     * @param sSQL The type of the object.
     * @param sSQL The create statement.
     * @return A pair. The first part is the parsed database object, the second a bool indicating if our
     *         parser fully understood the statement. The object may be empty if parsing failed.
     */
    static QPair<ObjectPtr, bool> parseSQL(Object::ObjectTypes type, const QString& sSQL);

protected:
    QString m_name;
    QString m_originalSql;
    bool m_temporary;
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

class Field
{
public:
    Field(const QString& name,
          const QString& type,
          bool notnull = false,
          const QString& defaultvalue = "",
          const QString& check = "",
          bool unique = false)
        : m_name(name)
        , m_type(type)
        , m_notnull(notnull)
        , m_check(check)
        , m_defaultvalue(defaultvalue)
        , m_autoincrement(false)
        , m_unique(unique)
    {}

    QString toString(const QString& indent = "\t", const QString& sep = "\t") const;

    void setName(const QString& name) { m_name = name; }
    void setType(const QString& type) { m_type = type; }
    void setNotNull(bool notnull = true) { m_notnull = notnull; }
    void setCheck(const QString& check) { m_check = check; }
    void setDefaultValue(const QString& defaultvalue) { m_defaultvalue = defaultvalue; }
    void setAutoIncrement(bool autoinc) { m_autoincrement = autoinc; }
    void setUnique(bool u) { m_unique = u; }

    bool isText() const;
    bool isInteger() const;

    const QString& name() const { return m_name; }
    const QString& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const QString& check() const { return m_check; }
    const QString& defaultValue() const { return m_defaultvalue; }
    bool autoIncrement() const { return m_autoincrement; }
    bool unique() const { return m_unique; }

    static QStringList Datatypes;
private:
    QString m_name;
    QString m_type;
    bool m_notnull;
    QString m_check;
    QString m_defaultvalue;
    bool m_autoincrement; //! this is stored here for simplification
    bool m_unique;
};

typedef QMultiHash<FieldVector, ConstraintPtr> ConstraintMap;

class Table : public Object
{
public:
    explicit Table(const QString& name): Object(name), m_rowidColumn("_rowid_") {}
    virtual ~Table();

    virtual ObjectTypes type() const { return Object::Table; }

    const FieldVector& fields() const { return m_fields; }

    /**
     * @brief Returns the CREATE TABLE statement for this table object
     * @return A QString with the CREATE TABLE object.
     */
    QString sql() const;

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
     * @return A pair first is a table object, second is a bool indicating
     *         if our modify dialog supports the table.
     *         The table object may be a empty table if parsing failed.
     */
    static QPair<ObjectPtr, bool> parseSQL(const QString& sSQL);
private:
    QStringList fieldList() const;
    bool hasAutoIncrement() const;

private:
    FieldVector m_fields;
    QString m_rowidColumn;
    ConstraintMap m_constraints;
    QString m_virtual;
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
        , m_bModifySupported(true)
    {}

    TablePtr table();
    bool modifysupported() const { return m_bModifySupported; }

private:
    void parsecolumn(Table* table, antlr::RefAST c);

private:
    antlr::RefAST m_root;
    bool m_bModifySupported;
};

QStringList fieldVectorToFieldNames(const sqlb::FieldVector& vector);

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

    virtual ObjectTypes type() const { return Object::Index; }

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
    QStringList columnSqlList() const;

    void clear();

    /**
     * @brief Returns the CREATE INDEX statement for this index object
     * @return A QString with the CREATE INDEX object.
     */
    QString sql() const;

    /**
     * @brief parseSQL Parses the CREATE INDEX statement in sSQL.
     * @param sSQL The create index statement.
     * @return A pair. The first part is an index object, the second a bool indicating
     *         if our parser fully understood the statement.
     *         The index object may be if parsing failed.
     */
    static QPair<ObjectPtr, bool> parseSQL(const QString& sSQL);

private:
    bool m_unique;
    QString m_table;
    QString m_whereExpr;
    IndexedColumnVector m_columns;
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
        , m_bModifySupported(true)
    {}

    IndexPtr index();
    bool modifysupported() const { return m_bModifySupported; }

private:
    void parsecolumn(Index* index, antlr::RefAST c);

private:
    antlr::RefAST m_root;
    bool m_bModifySupported;
};

} //namespace sqlb

#endif // SQLITETYPES_H
