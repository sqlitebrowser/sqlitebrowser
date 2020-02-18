#ifndef QUERY_H
#define QUERY_H

#include "ObjectIdentifier.h"

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
    SortedColumn(size_t column_, SortDirection direction_) :
        column(column_),
        direction(direction_)
    {}

    bool operator==(const SortedColumn& rhs) const
    {
        return column == rhs.column && direction == rhs.direction;
    }

    size_t column;
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
    Query() {}
    explicit Query(const sqlb::ObjectIdentifier& table) :
        m_table(table)
    {}

    void clear();
    std::string buildQuery(bool withRowid) const;
    std::string buildCountQuery() const;

    void setColumNames(const std::vector<std::string>& column_names) { m_column_names = column_names; }
    std::vector<std::string> columnNames() const { return m_column_names; }

    void setTable(const sqlb::ObjectIdentifier& table) { m_table = table; }
    sqlb::ObjectIdentifier table() const { return m_table; }

    void setRowIdColumns(const std::vector<std::string>& rowids) { m_rowid_columns = rowids; }
    std::vector<std::string> rowIdColumns() const { return m_rowid_columns; }
    void setRowIdColumn(const std::string& rowid) { m_rowid_columns = {rowid}; }
    bool hasCustomRowIdColumn() const { return m_rowid_columns.size() != 1 || (m_rowid_columns.at(0) != "rowid" && m_rowid_columns.at(0) != "_rowid_"); }

    const std::vector<SelectedColumn>& selectedColumns() const { return m_selected_columns; }
    std::vector<SelectedColumn>& selectedColumns() { return m_selected_columns; }

    const std::unordered_map<size_t, std::string>& where() const { return m_where; }
    std::unordered_map<size_t, std::string>& where() { return m_where; }

    const std::vector<std::string>& globalWhere() const { return m_global_where; }
    std::vector<std::string>& globalWhere() { return m_global_where; }
    void setGlobalWhere(const std::vector<std::string>& w) { m_global_where = w; }

    const std::vector<SortedColumn>& orderBy() const { return m_sort; }
    std::vector<SortedColumn>& orderBy() { return m_sort; }
    void setOrderBy(const std::vector<SortedColumn>& columns) { m_sort = columns; }

private:
    std::vector<std::string> m_column_names;
    sqlb::ObjectIdentifier m_table;
    std::vector<std::string> m_rowid_columns;
    std::vector<SelectedColumn> m_selected_columns;
    std::unordered_map<size_t, std::string> m_where;    // TODO The two where variables should be merged into a single variable which ...
    std::vector<std::string> m_global_where;            // ... holds some sort of general tree structure for all sorts of where conditions.
    std::vector<SortedColumn> m_sort;

    std::vector<SelectedColumn>::iterator findSelectedColumnByName(const std::string& name);
    std::vector<SelectedColumn>::const_iterator findSelectedColumnByName(const std::string& name) const;
    std::string buildWherePart() const;
};

}

#endif
