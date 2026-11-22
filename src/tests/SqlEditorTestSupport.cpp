#include "../sqlitedb.h"

namespace sqlb
{

// The same wrapper sqlitedb.cpp provides. It is repeated here so the editor tests do not have to link the whole database code.
QString escapeIdentifier(const QString& id)
{
    return QString::fromStdString(escapeIdentifier(id.toStdString()));
}

}
