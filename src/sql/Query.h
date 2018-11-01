#ifndef QUERY_H
#define QUERY_H

#include "sqlitetypes.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace sqlb
{

struct SortedColumn
{
    SortedColumn(const std::string& column_, const std::string& direction_) :
        column(column_),
        direction(direction_)
    {}

    std::string column;
    std::string direction;
};

struct SelectedColumn
{
    SelectedColumn(const std::string& original_column_, const std::string& selector_) :
        original_column(original_column_),
        selector(selector_)
    {}

    std::string original_column;
    std::string selector;
};

class Query
{
public:
    Query();
    Query(const sqlb::ObjectIdentifier& table) :
        m_table(table)
    {}

    void clear();
    std::string buildQuery(bool withRowid) const;

    void setTable(const sqlb::ObjectIdentifier& table) { m_table = table; }
    sqlb::ObjectIdentifier table() const { return m_table; }

    void setRowIdColumn(const std::string& rowid) { m_rowid_column = rowid; }
    std::string rowIdColumn() const { return m_rowid_column; }

    const std::vector<SelectedColumn>& selectedColumns() const { return m_selected_columns; }
    std::vector<SelectedColumn>& selectedColumns() { return m_selected_columns; }

    const std::unordered_map<std::string, std::string>& where() const { return m_where; }
    std::unordered_map<std::string, std::string>& where() { return m_where; }

    const std::vector<SortedColumn>& orderBy() const { return m_sort; }
    std::vector<SortedColumn>& orderBy() { return m_sort; }

private:
    sqlb::ObjectIdentifier m_table;
    std::string m_rowid_column;
    std::vector<SelectedColumn> m_selected_columns;
    std::unordered_map<std::string, std::string> m_where;
    std::vector<SortedColumn> m_sort;

    std::vector<SelectedColumn>::iterator findSelectedColumnByName(const std::string& name);
    std::vector<SelectedColumn>::const_iterator findSelectedColumnByName(const std::string& name) const;
};

}

#endif
