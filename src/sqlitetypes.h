#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include "antlr/ASTRefCount.hpp"

#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <QStringList>

namespace sqlb {

class Field
{
public:
    Field(const QString& name,
          const QString& type,
          bool notnull = false,
          const QString& defaultvalue = "",
          const QString& check = "",
          bool pk = false)
        : m_name(name)
        , m_type(type)
        , m_notnull(notnull)
        , m_check(check)
        , m_defaultvalue(defaultvalue)
        , m_autoincrement(false)
        , m_primaryKey(pk)
    {}

    QString toString(const QString& indent = "\t", const QString& sep = "\t") const;

    void setName(const QString& name) { m_name = name; }
    void setType(const QString& type) { m_type = type; }
    void setNotNull(bool notnull = true) { m_notnull = notnull; }
    void setCheck(const QString& check) { m_check = check; }
    void setDefaultValue(const QString& defaultvalue) { m_defaultvalue = defaultvalue; }
    void setAutoIncrement(bool autoinc) { m_autoincrement = autoinc; }
    void setPrimaryKey(bool pk) { m_primaryKey = pk; }

    bool isText() const;
    bool isInteger() const;

    const QString& name() const { return m_name; }
    const QString& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const QString& check() const { return m_check; }
    const QString& defaultValue() const { return m_defaultvalue; }
    bool autoIncrement() const { return m_autoincrement; }
    bool primaryKey() const { return m_primaryKey; }

    static QStringList Datatypes;
private:
    QString m_name;
    QString m_type;
    bool m_notnull;
    QString m_check;
    QString m_defaultvalue;
    bool m_autoincrement; //! this is stored here for simplification
    bool m_primaryKey;
};

typedef QSharedPointer<Field> FieldPtr;
typedef QVector< FieldPtr > FieldVector;
class Table
{
public:
    Table(const QString& name): m_name(name), m_rowidColumn("rowid") {}
    virtual ~Table();

    void setName(const QString& name) { m_name = name; }

    const QString& name() const { return m_name; }
    const FieldVector& fields() const { return m_fields; }

    /**
     * @brief Creates an empty insert statement.
     * @return An sqlite conform INSERT INTO statement with empty values. (NULL,'',0)
     */
    QString emptyInsertStmt() const;

    /**
     * @brief Returns the CREATE TABLE statement for this table object
     * @return A QString with the CREATE TABLE object.
     */
    QString sql() const;

    void addField(const FieldPtr& f);
    bool removeField(const QString& sFieldName);
    void setFields(const FieldVector& fields);
    void setField(int index, FieldPtr f) { m_fields[index] = f; }
    void setRowidColumn(const QString& rowid) {  m_rowidColumn = rowid; }
    QString rowidColumn() const { return m_rowidColumn; }
    void clear();

    /**
     * @brief findField Finds a field and returns the index.
     * @param sname
     * @return The field index if the field was found.
     *         -1 if field coulnd't be found.
     */
    int findField(const QString& sname);

    int findPk() const;

    static Table parseSQL(const QString& sSQL);
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
    CreateTableWalker(antlr::RefAST r) : m_root(r) {}

    Table table();

private:
    void parsecolumn(FieldPtr& f, antlr::RefAST c);

private:
    antlr::RefAST m_root;
};

} //namespace sqlb

#endif // SQLITETYPES_H
