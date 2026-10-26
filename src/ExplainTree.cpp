#include "ExplainTree.h"

#include <QModelIndex>
#include <QRegularExpression>

#include <map>

namespace ExplainTree
{

bool isExplainQueryPlan(const QString& sql)
{
    static const QRegularExpression re("^\\s*EXPLAIN\\s+QUERY\\s+PLAN\\b", QRegularExpression::CaseInsensitiveOption);
    return re.match(sql).hasMatch();
}

std::vector<TreeLine> buildTree(const std::vector<PlanRow>& rows)
{
    const size_t count = rows.size();

    // Find the position of the parent of each row. If an id is used more than once, the first row wins.
    std::map<int, size_t> position;
    for(size_t i = 0; i < count; ++i)
        position.insert({rows[i].id, i});

    const size_t none = count;
    std::vector<size_t> parentOf(count, none);
    std::vector<std::vector<size_t>> children(count);
    for(size_t i = 0; i < count; ++i)
    {
        auto it = position.find(rows[i].parent);
        if(it != position.end() && it->second != i)
        {
            parentOf[i] = it->second;
            children[it->second].push_back(i);
        }
    }

    std::vector<TreeLine> result;
    result.reserve(count);
    std::vector<bool> visited(count, false);

    // Depth-first walk without recursion; the stack holds rows still to be placed, the last one is placed next
    auto walk = [&](size_t root) {
        std::vector<std::pair<size_t, int>> stack;
        stack.push_back({root, 0});
        while(!stack.empty())
        {
            const auto current = stack.back();
            stack.pop_back();
            if(visited[current.first])
                continue;
            visited[current.first] = true;
            result.push_back({current.first, current.second, rows[current.first].detail});

            const auto& kids = children[current.first];
            for(auto it = kids.rbegin(); it != kids.rend(); ++it)
                stack.push_back({*it, current.second + 1});
        }
    };

    for(size_t i = 0; i < count; ++i)
        if(parentOf[i] == none)
            walk(i);

    // Rows which are part of a parent loop are not reachable from any top level row. Don't lose them.
    for(size_t i = 0; i < count; ++i)
        if(!visited[i])
            walk(i);

    return result;
}

std::vector<int> depths(const std::vector<PlanRow>& rows)
{
    std::vector<int> result(rows.size(), 0);
    for(const auto& line : buildTree(rows))
        result[line.row] = line.depth;
    return result;
}

QString indent(const QString& detail, int depth)
{
    if(depth <= 0)
        return detail;

    // Three characters per level, the last one is a branch marker
    return QString(3 * (depth - 1), ' ') + QChar(0x2514) + QChar(0x2500) + ' ' + detail;
}

Delegate::Delegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QString Delegate::indentedText(const QModelIndex& index) const
{
    const QString text = index.data().toString();

    // Only the detail column of a result with the four EXPLAIN QUERY PLAN columns is indented
    const QAbstractItemModel* model = index.model();
    if(!model || model->columnCount() != 4 || index.column() != 3)
        return text;

    // Collect the steps which are available in the model. Rows which are not loaded yet are left out.
    std::vector<PlanRow> rows;
    size_t target = static_cast<size_t>(-1);
    for(int r = 0; r < model->rowCount(); ++r)
    {
        bool idOk = false, parentOk = false;
        const int id = model->index(r, 0).data().toString().toInt(&idOk);
        const int parent = model->index(r, 1).data().toString().toInt(&parentOk);
        if(!idOk || !parentOk)
            continue;
        if(r == index.row())
            target = rows.size();
        rows.push_back({id, parent, QString()});
    }
    if(target >= rows.size())
        return text;

    return indent(text, depths(rows)[target]);
}

void Delegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    option->text = indentedText(index);
}

}
