#pragma once
#ifndef SQLITETYPES_H
#define SQLITETYPES_H

#include <QString>
#include <QList>
#include <QStringList>

namespace sqlb {

class Field
{
public:
    Field(const QString& name, const QString& type, bool notnull = false, const QString& check = "")
        : m_name(name), m_type(type), m_notnull(notnull), m_check(check), m_autoincrement(false) {}

    QString toString(const QString& indent = "\t", const QString& sep = "\t") const;

    void setAutoIncrement(bool autoinc) { m_autoincrement = autoinc; }

    bool isText() const;
    bool isInteger() const;

    const QString& name() const { return m_name; }
    const QString& type() const { return m_type; }
    bool notnull() const { return m_notnull; }
    const QString& check() const { return m_check; }
    bool autoIncrement() const { return m_autoincrement; }
private:
    QString m_name;
    QString m_type;
    bool m_notnull;
    QString m_check;
    bool m_autoincrement; //! this is stored here for simplification
};

typedef QList<Field*> FieldList;
class Table
{
public:
    Table(const QString& name): m_name(name) {}
    virtual ~Table();

    const QString& name() const { return m_name; }
    QString sql() const;

    void addField(Field* f);

    bool setPrimaryKey(const FieldList& pk);
    bool setPrimaryKey(Field* pk, bool autoincrement = false);

private:
    QStringList fieldList() const;
    bool hasAutoIncrement() const;

private:
    QString m_name;
    FieldList m_fields;
    FieldList m_primarykey;
};
} //namespace sqlb

#endif // SQLITETYPES_H
