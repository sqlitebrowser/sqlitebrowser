#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include "antlr/ASTRefCount.hpp"

#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <QStringList>
#include <QPair>

namespace sqlb {

QString escapeIdentifier(QString id);

class ForeignKeyClause
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

private:
    QString m_table;
    QStringList m_columns;
    QString m_constraint;

    QString m_override;
};

class Field
{
public:
    Field(const QString& name,
          const QString& type,
          bool notnull = false,
          const QString& defaultvalue = "",
          const QString& check = "",
          bool pk = false,
          bool unique = false)
        : m_name(name)
        , m_type(type)
        , m_notnull(notnull)
        , m_check(check)
        , m_defaultvalue(defaultvalue)
        , m_autoincrement(false)
        , m_primaryKey(pk)
        , m_unique(unique)
    {}

    QString toString(const QString& indent = "\t", const QString& sep = "\t") const;

    void setName(const QString& name) { m_name = name; }
    void setType(const QString& type) { m_type = type; }
    void setNotNull(bool notnull = true) { m_notnull = notnull; }
    void setCheck(const QString& check) { m_check = check; }
    void setDefaultValue(const QString& defaultvalue) { m_defaultvalue = defaultvalue; }
    void setAutoIncrement(bool autoinc) { m_autoincrement = autoinc; }
    void setPrimaryKey(bool pk) { m_primaryKey = pk; }
    void setUnique(bool u) { m_unique = u; }
    void setForeignKey(const ForeignKeyClause& key) { m_foreignKey = key; }

    bool isText() const;
    bool isInteger() const;

    const QString& name() const { return m_name; }
    const QString& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const QString& check() const { return m_check; }
    const QString& defaultValue() const { return m_defaultvalue; }
    bool autoIncrement() const { return m_autoincrement; }
    bool primaryKey() const { return m_primaryKey; }
    bool unique() const { return m_unique; }
    const ForeignKeyClause& foreignKey() const { return m_foreignKey; }

    static QStringList Datatypes;
private:
    QString m_name;
    QString m_type;
    bool m_notnull;
    QString m_check;
    QString m_defaultvalue;
    ForeignKeyClause m_foreignKey;   // Even though this information is a table constraint it's easier for accessing and processing to store it here
    bool m_autoincrement; //! this is stored here for simplification
    bool m_primaryKey;
    bool m_unique;
};

typedef QSharedPointer<Field> FieldPtr;
typedef QVector< FieldPtr > FieldVector;
class Table
{
public:
    explicit Table(const QString& name): m_name(name), m_rowidColumn("_rowid_") {}
    virtual ~Table();

    void setName(const QString& name) { m_name = name; }

    const QString& name() const { return m_name; }
    const FieldVector& fields() const { return m_fields; }

    /**
     * @brief Returns the CREATE TABLE statement for this table object
     * @return A QString with the CREATE TABLE object.
     */
    QString sql() const;

    void addField(const FieldPtr& f);
    bool removeField(const QString& sFieldName);
    void setFields(const FieldVector& fields);
    void setField(int index, FieldPtr f) { m_fields[index] = f; }
    QStringList fieldNames() const;
    void setRowidColumn(const QString& rowid) {  m_rowidColumn = rowid; }
    const QString& rowidColumn() const { return m_rowidColumn; }
    bool isWithoutRowidTable() const { return m_rowidColumn != "_rowid_"; }
    void clear();

    /**
     * @brief findField Finds a field and returns the index.
     * @param sname
     * @return The field index if the field was found.
     *         -1 if field couldn't be found.
     */
    int findField(const QString& sname);

    int findPk() const;

    /**
     * @brief parseSQL Parses the create Table statement in sSQL.
     * @param sSQL The create table statement.
     * @return A pair first is a table object, second is a bool indicating
     *         if our modify dialog supports the table.
     *         The table object may be a empty table if parsing failed.
     */
    static QPair<Table, bool> parseSQL(const QString& sSQL);
private:
    QStringList fieldList() const;
    bool hasAutoIncrement() const;

private:
    QString m_name;
    FieldVector m_fields;
    QString m_rowidColumn;
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

    Table table();
    bool modifysupported() const { return m_bModifySupported; }

private:
    void parsecolumn(FieldPtr& f, antlr::RefAST c);

private:
    antlr::RefAST m_root;
    bool m_bModifySupported;
};

} //namespace sqlb

#endif // SQLITETYPES_H
