#include "ExtendedTableWidget.h"
#include "sqlitetablemodel.h"
#include "FilterTableHeader.h"

#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QKeyEvent>
#include <QScrollBar>
#include <QHeaderView>

ExtendedTableWidget::ExtendedTableWidget(QWidget* parent) :
    QTableView(parent)
{
    setHorizontalScrollMode(ExtendedTableWidget::ScrollPerPixel);

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vscrollbarChanged(int)));

    // Set up filter row
    m_tableHeader = new FilterTableHeader(this);
    setHorizontalHeader(m_tableHeader);
}

void ExtendedTableWidget::copy()
{
    // Get list of selected items
    QItemSelectionModel* selection = selectionModel();
    QModelIndexList indices = selection->selectedIndexes();

    // Abort if there's nothing to copy
    if(indices.size() == 0)
    {
        return;
    } else if(indices.size() == 1) {
        qApp->clipboard()->setText(indices.front().data().toString());
        return;
    }

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
    {
        copy();
    } else if(event->key() == Qt::Key_Tab && hasFocus() &&
              selectedIndexes().count() == 1 &&
              selectedIndexes().at(0).row() == model()->rowCount()-1 && selectedIndexes().at(0).column() == model()->columnCount()-1) {
        // If the Tab key was pressed while the focus was on the last cell of the last row insert a new row automatically
        model()->insertRow(model()->rowCount());
    }

    // This prevents the current selection from being changed when pressing tab to move to the next filter. Note that this is in an 'if' condition,
    // not in an 'else if' because this way, when the tab key was pressed and the focus was on the last cell, a new row is inserted and then the tab
    // key press is processed a second time to move the cursor as well
    if((event->key() != Qt::Key_Tab && event->key() != Qt::Key_Backtab) || hasFocus())
        QTableView::keyPressEvent(event);
}

void ExtendedTableWidget::updateGeometries()
{
    // Call the parent implementation first - it does most of the actual logic
    QTableView::updateGeometries();

    // Check if a model has already been set yet
    if(model())
    {
        // If so and if it is a SqliteTableModel and if the parent implementation of this method decided that a scrollbar is needed, update its maximum value
        SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
        if(m && verticalScrollBar()->maximum())
            verticalScrollBar()->setMaximum(m->totalRowCount() - numVisibleRows() + 1);
    }
}

void ExtendedTableWidget::vscrollbarChanged(int value)
{
    // Cancel if there is no model set yet - this shouldn't happen (because without a model there should be no scrollbar) but just to be sure...
    if(!model())
        return;

    // Fetch more data from the DB if necessary
    if((value + numVisibleRows()) >= model()->rowCount() && model()->canFetchMore(QModelIndex()))
        model()->fetchMore(QModelIndex());
}

int ExtendedTableWidget::numVisibleRows()
{
    // Get the row numbers of the rows currently visible at the top and the bottom of the widget
    int row_top = rowAt(0) == -1 ? 0 : rowAt(0);
    int row_bottom = rowAt(height()) == -1 ? model()->rowCount() : rowAt(height());

    // Calculate the number of visible rows
    return row_bottom - row_top;
}

QSet<int> ExtendedTableWidget::selectedCols()
{
    QSet<int> selectedCols;
    foreach(const QModelIndex & idx, selectedIndexes())
        selectedCols.insert(idx.column());
    return selectedCols;
}
