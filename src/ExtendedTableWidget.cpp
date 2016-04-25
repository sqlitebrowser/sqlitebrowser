#include "ExtendedTableWidget.h"
#include "sqlitetablemodel.h"
#include "FilterTableHeader.h"
#include "sqlitetypes.h"

#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QKeyEvent>
#include <QScrollBar>
#include <QHeaderView>
#include <QMessageBox>

ExtendedTableWidget::ExtendedTableWidget(QWidget* parent) :
    QTableView(parent)
{
    setHorizontalScrollMode(ExtendedTableWidget::ScrollPerPixel);

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vscrollbarChanged(int)));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(cellClicked(QModelIndex)));

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

void ExtendedTableWidget::paste()
{
    QString clipboard = qApp->clipboard()->text();

    // Get list of selected items
    QItemSelectionModel* selection = selectionModel();
    QModelIndexList indices = selection->selectedIndexes();

    // Abort if there's nowhere to paste
    if(indices.size() == 0)
    {
        return;
    }


    // Find out end of line character
    QString endOfLine;
    if(clipboard.endsWith('\n'))
    {
        if(clipboard.endsWith("\r\n"))
        {
            endOfLine = "\r\n";
        }
        else
        {
            endOfLine = "\n";
        }
    }
    else if(clipboard.endsWith('\r'))
    {
        endOfLine = "\r";
    }
    else
    {
        // Have only one cell, so there is no line break at end
        endOfLine = "\n";
    }

    // Unpack cliboard, assuming that it is rectangular
    QList<QStringList> clipboardTable;
    QStringList cr = clipboard.split(endOfLine);
    foreach(const QString& r, cr)
    {
        // Usually last splited line is empty
        if(!r.isEmpty())
        {
            clipboardTable.push_back(r.split("\t"));
        }
    }

    int clipboardRows = clipboardTable.size();
    int clipboardColumns = clipboardTable.front().size();


    // Sort the items by row, then by column
    qSort(indices);

    // Starting from assumption that selection is rectangular, and then first index is upper-left corner and last is lower-right.
    int firstRow = indices.front().row();
    int selectedRows = indices.back().row() - firstRow + 1;
    int firstColumn = indices.front().column();
    int selectedColumns = indices.back().column() - firstColumn + 1;


    // If not selected only one cell then check does selection match cliboard dimensions
    if(selectedRows != 1 || selectedColumns != 1)
    {
        if(selectedRows != clipboardRows || selectedColumns != clipboardColumns)
        {
            // Ask user is it sure about this
            QMessageBox::StandardButton reply = QMessageBox::question(this, QApplication::applicationName(),
                tr("The content of clipboard is bigger than the range selected.\nDo you want to insert it anyway?"),
                QMessageBox::Yes|QMessageBox::No);
            if(reply != QMessageBox::Yes)
            {
                return;
            }
        }
    }
    // Here we have positive answer even if cliboard is bigger than selection


    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
    // If last row and column are after table size clamp it
    int lastRow = qMin(firstRow + clipboardRows - 1, m->rowCount() - 1);
    int lastColumn = qMin(firstColumn + clipboardColumns - 1, m->columnCount() - 1);

    int row = firstRow;
    foreach(const QStringList& clipboardRow, clipboardTable)
    {
        int column = firstColumn;
        foreach(const QString& cell, clipboardRow)
        {
            if(cell.startsWith('"') && cell.endsWith('"'))
            {
                QString unquatedCell = cell.mid(1, cell.length()-2);
                m->setData(m->index(row, column), unquatedCell);
            }
            else
            {
                m->setData(m->index(row, column), cell);
            }

            column++;
            if(column> lastColumn)
            {
                break;
            }
        }

        row++;
        if(row > lastRow)
        {
            break;
        }
    }

}

void ExtendedTableWidget::keyPressEvent(QKeyEvent* event)
{
    // Call a custom copy method when Ctrl-C is pressed
    if(event->matches(QKeySequence::Copy))
    {
        copy();
    // Call a custom paste method when Ctrl-P is pressed
    } else if(event->matches(QKeySequence::Paste))
    {
        paste();
    } else if(event->key() == Qt::Key_Tab && hasFocus() &&
              selectedIndexes().count() == 1 &&
              selectedIndexes().at(0).row() == model()->rowCount()-1 && selectedIndexes().at(0).column() == model()->columnCount()-1) {
        // If the Tab key was pressed while the focus was on the last cell of the last row insert a new row automatically
        model()->insertRow(model()->rowCount());
    } else if(event->key() == Qt::Key_Delete) {
        if(event->modifiers().testFlag(Qt::AltModifier))
        {
            // When pressing Alt+Delete set the value to NULL
            foreach(const QModelIndex& index, selectedIndexes())
                model()->setData(index, QString());
        } else {
            // When pressing Delete only set the value to empty string
            foreach(const QModelIndex& index, selectedIndexes())
                model()->setData(index, "");
        }
    } else if(event->key() == Qt::Key_Return && selectedIndexes().count() == 1 && state() != QTableView::EditingState) {
        // When hitting the return key simulate a double click. This way you can change the focus to the editor dock when pressing the
        // return key for advanced editing, just like a double click would open the edit dialog
        emit doubleClicked(selectedIndexes().at(0));
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

void ExtendedTableWidget::cellClicked(const QModelIndex& index)
{
    // If Alt key is pressed try to jump to the row referenced by the foreign key of the clicked cell
    if(qApp->keyboardModifiers().testFlag(Qt::ControlModifier) && qApp->keyboardModifiers().testFlag(Qt::ShiftModifier) && model())
    {
        SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
        sqlb::ForeignKeyClause fk = m->getForeignKeyClause(index.column()-1);

        if(fk.isSet())
            emit foreignKeyClicked(fk.table(), fk.columns().size() ? fk.columns().at(0) : "", m->data(index, Qt::EditRole).toByteArray());
    }
}

void ExtendedTableWidget::dragEnterEvent(QDragEnterEvent* event)
{
    event->accept();
}

void ExtendedTableWidget::dragMoveEvent(QDragMoveEvent* event)
{
    event->accept();
}

void ExtendedTableWidget::dropEvent(QDropEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    model()->dropMimeData(event->mimeData(), Qt::CopyAction, index.row(), index.column(), QModelIndex());
    event->acceptProposedAction();
}
