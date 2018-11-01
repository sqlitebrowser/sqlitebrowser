#include "Query.h"

#include <algorithm>

namespace sqlb
{

Query::Query()
{
}

void Query::clear()
{
    m_table.clear();
    m_rowid_column = "rowid";
    m_selected_columns.clear();
    m_where.clear();
    m_sort.clear();
}

std::string Query::buildQuery(bool withRowid) const
{
    // Selector and display formats
    std::string selector;
    if (withRowid)
        selector = sqlb::escapeIdentifier(m_rowid_column) + ",";

    if(m_selected_columns.empty())
    {
        selector += "*";
    } else {
        for(const auto& it : m_selected_columns)
        {
            if (it.original_column != it.selector)
                selector += it.selector + " AS " + sqlb::escapeIdentifier(it.original_column) + ",";
            else
                selector += sqlb::escapeIdentifier(it.original_column) + ",";
        }
        selector.pop_back();
    }

    // Filter
    std::string where;
    if(m_where.size())
    {
        where = "WHERE ";

        for(auto i=m_where.cbegin();i!=m_where.cend();++i)
        {
            const auto it = findSelectedColumnByName(m_column_names.at(i->first));
            std::string column = sqlb::escapeIdentifier(m_column_names.at(i->first));
            if(it != m_selected_columns.cend() && it->selector != column)
                column = it->selector;
            where += column + " " + i->second + " AND ";
        }

        // Remove last ' AND '
        where.erase(where.size() - 5);
    }

    // Sorting
    std::string order_by;
    if(m_sort.size())
    {
        order_by = "ORDER BY ";
        for(const auto& sorted_column : m_sort)
            order_by += sqlb::escapeIdentifier(m_column_names.at(sorted_column.column)) + " "
                    + (sorted_column.direction == sqlb::Ascending ? "ASC" : "DESC") + ",";
        order_by.pop_back();
    }

    return "SELECT " + selector + " FROM " + m_table.toString().toStdString() + " " + where + " " + order_by;
}

std::vector<SelectedColumn>::iterator Query::findSelectedColumnByName(const std::string& name)
{
    return std::find_if(m_selected_columns.begin(), m_selected_columns.end(), [name](const SelectedColumn& c) {
        return name == c.original_column;
    });
}

std::vector<SelectedColumn>::const_iterator Query::findSelectedColumnByName(const std::string& name) const
{
    return std::find_if(m_selected_columns.cbegin(), m_selected_columns.cend(), [name](const SelectedColumn& c) {
        return name == c.original_column;
    });
}

}
