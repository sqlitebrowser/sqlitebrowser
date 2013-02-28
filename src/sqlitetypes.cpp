#include "sqlitetypes.h"

namespace sqlb {

QString Field::toString(const QString& indent, const QString& sep) const
{
    QString str = indent + m_name + sep + m_type;
    if(m_notnull)
        str += " NOT NULL";
    if(!m_check.isEmpty())
        str += " CHECK(" + m_check + ")";
    if(m_autoincrement)
        str += " PRIMARY KEY AUTOINCREMENT";
    return str;
}

bool Field::isText() const
{
    QString norm = m_type.trimmed().toLower();

    return     norm.startsWith("character")
            || norm.startsWith("varchar")
            || norm.startsWith("varying character")
            || norm.startsWith("nchar")
            || norm.startsWith("native charactar")
            || norm.startsWith("nvarchar")
            || norm == "text"
            || norm == "clob";
}

bool Field::isInteger() const
{
    QString norm = m_type.trimmed().toLower();

    return     norm == "int"
            || norm == "integer"
            || norm == "tinyint"
            || norm == "smallint"
            || norm == "mediumint"
            || norm == "bigint"
            || norm == "unsigned big int"
            || norm == "int2"
            || norm == "int8";
}

Table::~Table()
{
    foreach(Field* f, m_fields) {
        delete f;
    }
    m_fields.clear();
}

void Table::addField(Field *f)
{
    m_fields.append(f);
}

bool Table::setPrimaryKey(const FieldList& pk)
{
    foreach(Field* f, pk) {
        if(!m_fields.contains(f))
            return false;
    }

    m_primarykey = pk;
    return true;
}

bool Table::setPrimaryKey(Field* pk, bool autoincrement)
{
    if(m_fields.contains(pk))
    {
        if(pk->isInteger() && autoincrement)
            pk->setAutoIncrement(true);
        m_primarykey.clear();
        m_primarykey.append(pk);
        return true;
    }

    return false;
}

QStringList Table::fieldList() const
{
    QStringList sl;

    foreach(Field* f, m_fields) {
        sl << f->toString();
    }

    return sl;
}

bool Table::hasAutoIncrement() const
{
    foreach(Field* f, m_fields) {
        if(f->autoIncrement())
            return true;
    }
    return false;
}

QString Table::sql() const
{
    QString sql = QString("CREATE TABLE `%1` (\n").arg(m_name);

    sql += fieldList().join(",\n");

    // primary key
    if( m_primarykey.size() > 0 && !hasAutoIncrement())
    {
        sql += ",\n\tPRIMARY KEY(";
        for(QList<Field*>::ConstIterator it = m_primarykey.constBegin();
            it != m_primarykey.constEnd();
            ++it)
        {
            sql += (*it)->name();
            if(it != --m_primarykey.constEnd())
                sql += ",";
        }
        sql += ")";
    }

    return sql + "\n)";
}
} //namespace sqlb
