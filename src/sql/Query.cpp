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
    m_rowid_column = "_rowid_";
    m_selected_columns.clear();
    m_where.clear();
    m_sort.clear();
}

std::string Query::buildWherePart() const
{
    std::string where;
    if(m_where.size())
    {
        where = "WHERE ";

        for(auto i = m_where.cbegin(); i != m_where.cend(); ++i)
        {
            const auto it = findSelectedColumnByName(m_column_names.at(static_cast<size_t>(i->first)));
            std::string column = sqlb::escapeIdentifier(m_column_names.at(static_cast<size_t>(i->first)));
            if(it != m_selected_columns.cend() && it->selector != column)
                column = it->selector;
            where += column + " " + i->second + " AND ";
        }

        // Remove last ' AND '
        where.erase(where.size() - 5);
    }
    return where;
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
    std::string where = buildWherePart();

    // Sorting
    std::string order_by;
    for(const auto& sorted_column : m_sort)
    {
        if(static_cast<size_t>(sorted_column.column) < m_column_names.size())
            order_by += sqlb::escapeIdentifier(m_column_names.at(static_cast<size_t>(sorted_column.column)))
					+ " "
                    + (sorted_column.direction == sqlb::Ascending ? "ASC" : "DESC") + ",";
    }
    if(order_by.size())
    {
        order_by.pop_back();
        order_by = "ORDER BY " + order_by;
    }

    return "SELECT " + selector + " FROM " + m_table.toString().toStdString() + " " + where + " " + order_by;
}

std::string Query::buildCountQuery() const
{
    // Build simplest count query for this (filtered) table
    return "SELECT COUNT(*) FROM " + m_table.toString().toStdString() + " " + buildWherePart();
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
