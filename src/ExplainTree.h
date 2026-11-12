#ifndef EXPLAINTREE_H
#define EXPLAINTREE_H

#include <QString>
#include <QStyledItemDelegate>
#include <vector>

// Helpers for presenting the result of EXPLAIN QUERY PLAN as a tree. SQLite returns one row per plan step with the
// columns (id, parent, notused, detail) where parent refers to the id of the enclosing step (0 for top level steps).
namespace ExplainTree
{

struct PlanRow
{
    int id;
    int parent;
    QString detail;
};

struct TreeLine
{
    size_t row;     // Index of the step in the input vector
    int depth;      // Nesting level, 0 for top level steps
    QString detail;
};

// Returns true if the statement is an EXPLAIN QUERY PLAN statement.
bool isExplainQueryPlan(const QString& sql);

// Sorts the steps into depth-first order (siblings keep their input order) and assigns each a depth. Steps whose
// parent can't be found are treated as top level steps. Every input step is returned exactly once.
std::vector<TreeLine> buildTree(const std::vector<PlanRow>& rows);

// Returns the nesting depth of each step in the order of the input rows.
std::vector<int> depths(const std::vector<PlanRow>& rows);

// Returns the detail text prefixed with indentation for the given depth.
QString indent(const QString& detail, int depth);

// Item delegate for the detail column of an EXPLAIN QUERY PLAN result (id, parent, notused, detail). It shows the
// text indented by the nesting depth of the row without changing the model data.
class Delegate : public QStyledItemDelegate
{
public:
    explicit Delegate(QObject* parent = nullptr);

    QString indentedText(const QModelIndex& index) const;

    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
};

}

#endif
