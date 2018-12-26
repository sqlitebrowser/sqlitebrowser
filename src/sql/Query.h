#ifndef QUERY_H
#define QUERY_H

#include "sqlitetypes.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace sqlb
{

enum SortDirection
{
    Ascending,
    Descending
};

struct SortedColumn
{
    SortedColumn(int column_, SortDirection direction_) :
        column(column_),
        direction(direction_)
    {}

    int column;
    SortDirection direction;
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
    std::string buildCountQuery() const;

    void setColumNames(const std::vector<std::string>& column_names) { m_column_names = column_names; }
    std::vector<std::string> columnNames() const { return m_column_names; }

    void setTable(const sqlb::ObjectIdentifier& table) { m_table = table; }
    sqlb::ObjectIdentifier table() const { return m_table; }

    void setRowIdColumn(const std::string& rowid) { m_rowid_column = rowid; }
    std::string rowIdColumn() const { return m_rowid_column; }
    bool hasCustomRowIdColumn() const { return m_rowid_column != "rowid" && m_rowid_column != "_rowid_"; }

    const std::vector<SelectedColumn>& selectedColumns() const { return m_selected_columns; }
    std::vector<SelectedColumn>& selectedColumns() { return m_selected_columns; }

    const std::unordered_map<int, std::string>& where() const { return m_where; }
    std::unordered_map<int, std::string>& where() { return m_where; }

    const std::vector<SortedColumn>& orderBy() const { return m_sort; }
    std::vector<SortedColumn>& orderBy() { return m_sort; }

private:
    std::vector<std::string> m_column_names;
    sqlb::ObjectIdentifier m_table;
    std::string m_rowid_column;
    std::vector<SelectedColumn> m_selected_columns;
    std::unordered_map<int, std::string> m_where;
    std::vector<SortedColumn> m_sort;

    std::vector<SelectedColumn>::iterator findSelectedColumnByName(const std::string& name);
    std::vector<SelectedColumn>::const_iterator findSelectedColumnByName(const std::string& name) const;
    std::string buildWherePart() const;
};

}

#endif
