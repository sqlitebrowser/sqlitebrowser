#include "ObjectIdentifier.h"

#include <regex>

namespace sqlb {

static escapeQuoting customQuoting = DoubleQuotes;

void setIdentifierQuoting(escapeQuoting toQuoting)
{
    customQuoting = toQuoting;
}

std::string escapeIdentifier(const std::string& id)
{
    switch(customQuoting) {
    case GraveAccents:
        return '`' + std::regex_replace(id, std::regex("\\`"), std::string("``")) + '`';
    case SquareBrackets:
        // There aren't any escaping possibilities for square brackets inside the identifier,
        // so we rely on the user to not enter these characters when this kind of quoting is
        // selected.
        return '[' + id + ']';
    case DoubleQuotes:
    default:
        // This may produce a 'control reaches end of non-void function' warning if the
        // default branch is removed, even though we have covered all possibilities in the
        // switch statement.
        return '"' + std::regex_replace(id, std::regex("\\\""), std::string("\"\"")) + '"';
    }
}

bool ObjectIdentifier::fromSerialised(const std::string& serialised)
{
    auto pos_comma = serialised.find(",");
    auto pos_colon = serialised.find(":");
    if(pos_comma == serialised.npos || pos_colon == serialised.npos)
        return false;

    size_t size_schema, size_name;
    size_schema = std::stoul(serialised.substr(0, pos_comma));
    size_name = std::stoul(serialised.substr(pos_comma+1, pos_colon-pos_comma));
    if(pos_colon + size_schema + size_name + 1 != serialised.size())
        return false;

    m_schema = serialised.substr(pos_colon + 1, size_schema);
    m_name = serialised.substr(pos_colon + size_schema + 1, size_name);

    return true;
}

}
