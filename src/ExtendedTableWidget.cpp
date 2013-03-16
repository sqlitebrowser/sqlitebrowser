#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QKeyEvent>
#include "ExtendedTableWidget.h"

ExtendedTableWidget::ExtendedTableWidget(QWidget* parent) :
    QTableView(parent)
{
}

void ExtendedTableWidget::copy()
{
    // Get list of selected items
    QItemSelectionModel* selection = selectionModel();
    QModelIndexList indices = selection->selectedIndexes();

    // Abort if there's nothing to copy
    if(indices.size() == 0)
        return;

    // Sort the items by row, then by column
    qSort(indices);

    // Go through all the items...
    QString result;
    QModelIndex prev = indices.front();
    indices.removeFirst();
    foreach(QModelIndex index, indices)
    {
        // Add the content of this cell to the clipboard string
        result.append(QString("\"%1\"").arg(prev.data().toString()));

        // If this is a new row add a line break, if not add a tab for cell separation
        if(index.row() != prev.row())
            result.append("\r\n");
        else
            result.append("\t");

        prev = index;
    }
    result.append(QString("\"%1\"\r\n").arg(indices.last().data().toString()));      // And the last cell

    // And finally add it to the clipboard
    qApp->clipboard()->setText(result);
}

void ExtendedTableWidget::keyPressEvent(QKeyEvent* event)
{
    // Call a custom copy method when Ctrl-C is pressed
    if(event->matches(QKeySequence::Copy))
        copy();
    else
        QTableView::keyPressEvent(event);
}
