#ifndef OBJECTIDENTIFIER_H
#define OBJECTIDENTIFIER_H

#include <string>

namespace sqlb {

enum escapeQuoting {
    DoubleQuotes,
    GraveAccents,
    SquareBrackets
};

// Set quoting style for escapeIdentifier
void setIdentifierQuoting(escapeQuoting toQuoting);

// Get currently configured quote char
char getIdentifierQuoteChar();

// Add quotes to an identifier
std::string escapeIdentifier(const std::string& id);

// Object identifier consisting of schema name and object name
class ObjectIdentifier
{
public:
    ObjectIdentifier(const std::string& schema, const std::string& name)
        : m_schema(schema),
          m_name(name)
    {
    }

    ObjectIdentifier()
        : m_schema("main")
    {
    }

    explicit ObjectIdentifier(const std::string& variant)
    {
        // Try to unserialise the parameter first. If that does not work, just assume it's an object name for the main schema
        clear();
        if(!fromSerialised(variant))
            m_name = variant;
    }

    bool operator==(const ObjectIdentifier& rhs) const
    {
        return (rhs.m_schema == m_schema && rhs.m_name == m_name);
    }

    bool operator<(const ObjectIdentifier& rhs) const
    {
        return toDisplayString() < rhs.toDisplayString();
    }

    const std::string& schema() const { return m_schema; }
    const std::string& name() const { return m_name; }
    void setSchema(const std::string& schema) { m_schema = schema; }
    void setName(const std::string& name) { m_name = name; }

    void clear()
    {
        m_schema = "main";
        m_name.clear();
    }

    bool isEmpty() const { return m_name.empty(); }

    // This returns a string which can be used in SQL statements
    std::string toString(bool shortName = false) const
    {
        if(shortName && m_schema == "main")
            return sqlb::escapeIdentifier(m_name);
        else
            return sqlb::escapeIdentifier(m_schema) + "." + sqlb::escapeIdentifier(m_name);
    }

    // This returns a string which can be used in the user interface
    std::string toDisplayString() const
    {
        if(m_schema == "main")
            return m_name;
        else
            return m_schema + "." + m_name;
    }

    std::string toSerialised() const
    {
        return std::to_string(m_schema.size()) + "," + std::to_string(m_name.size()) + ":" + m_schema + m_name;
    }

    bool fromSerialised(const std::string& serialised);

private:
    std::string m_schema;
    std::string m_name;
};

}

#endif
