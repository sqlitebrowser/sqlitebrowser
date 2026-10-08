#include "../sqlitedb.h"

namespace sqlb
{

QString escapeString(const QString& literal)
{
    return QString::fromStdString(escapeString(literal.toStdString()));
}

}
