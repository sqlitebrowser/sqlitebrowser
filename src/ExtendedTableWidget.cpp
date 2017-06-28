#include "ExtendedTableWidget.h"
#include "sqlitetablemodel.h"
#include "FilterTableHeader.h"
#include "sqlitetypes.h"
#include "Settings.h"

#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QKeyEvent>
#include <QScrollBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QBuffer>
#include <QMenu>

namespace
{

QList<QStringList> parseClipboard(const QString& clipboard)
{
    QList<QStringList> result;
    result.push_back(QStringList());

    QRegExp re("(\"(?:[^\t\"]+|\"\"[^\"]*\"\")*)\"|(\t|\r?\n)");
    int offset = 0;
    int whitespace_offset = 0;

    while (offset >= 0) {
        QString text;
        int pos = re.indexIn(clipboard, offset);
        if (pos < 0) {
            // insert everything that left
            text = clipboard.mid(whitespace_offset);
            result.last().push_back(text);
            break;
        }

        if (re.pos(2) < 0) {
            offset = pos + re.cap(1).length() + 1;
            continue;
        }

        QString ws = re.cap(2);
        // if two whitespaces in row - that's an empty cell
        if (!(pos - whitespace_offset)) {
            result.last().push_back(QString());
        } else {
            text = clipboard.mid(whitespace_offset, pos - whitespace_offset);
            result.last().push_back(text);
        }

        if (ws.endsWith("\n"))
            // create new row
            result.push_back(QStringList());

        whitespace_offset = offset = pos + ws.length();
    }

    return result;
}

}

ExtendedTableWidget::ExtendedTableWidget(QWidget* parent) :
    QTableView(parent)
{
    setHorizontalScrollMode(ExtendedTableWidget::ScrollPerPixel);
    // Force ScrollPerItem, so scrolling shows all table rows
    setVerticalScrollMode(ExtendedTableWidget::ScrollPerItem);

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vscrollbarChanged(int)));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(cellClicked(QModelIndex)));

    // Set up filter row
    m_tableHeader = new FilterTableHeader(this);
    setHorizontalHeader(m_tableHeader);

    // Set up vertical header context menu
    verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    // Set up table view context menu
    m_contextMenu = new QMenu(this);
    QAction* nullAction = new QAction(tr("Set to NULL"), m_contextMenu);
    QAction* copyAction = new QAction(QIcon(":/icons/copy"), tr("Copy"), m_contextMenu);
    QAction* pasteAction = new QAction(QIcon(":/icons/paste"), tr("Paste"), m_contextMenu);
    m_contextMenu->addAction(nullAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(copyAction);
    m_contextMenu->addAction(pasteAction);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Set up context menu actions
    connect(this, static_cast<void(QTableView::*)(const QPoint&)>(&QTableView::customContextMenuRequested),
            [=](const QPoint& pos)
    {
        // Try to find out whether the current view is editable and (de)activate menu options according to that
        bool editable = editTriggers() != QAbstractItemView::NoEditTriggers;
        nullAction->setEnabled(editable);
        pasteAction->setEnabled(editable);

        // Show menu
        m_contextMenu->popup(viewport()->mapToGlobal(pos));
    });
    connect(nullAction, &QAction::triggered, [&]() {
        foreach(const QModelIndex& index, selectedIndexes())
            model()->setData(index, QVariant());
    });
    connect(copyAction, &QAction::triggered, [&]() {
       copy();
    });
    connect(pasteAction, &QAction::triggered, [&]() {
       paste();
    });
}

void ExtendedTableWidget::reloadSettings()
{
    // Set the new font and font size
    QFont dataBrowserFont(Settings::getValue("databrowser", "font").toString());
    dataBrowserFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    setFont(dataBrowserFont);

    // Set new default row height depending on the font size
    verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height()+10);
}

void ExtendedTableWidget::copy()
{
    QModelIndexList indices = selectionModel()->selectedIndexes();

    // Abort if there's nothing to copy
    if (indices.isEmpty())
        return;
    qSort(indices);

    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    m_buffer.clear();

    // If a single cell is selected, copy it to clipboard
    if (indices.size() == 1) {
        QImage img;
        QVariant data = m->data(indices.first(), Qt::EditRole);

        if (img.loadFromData(data.toByteArray())) { // If it's an image
            qApp->clipboard()->setImage(img);
            return;
        } else {
            QString text = data.toString();
            if (text.isEmpty()) {
                // NULL and empty single-cells are handled via inner buffer
                qApp->clipboard()->clear();
                QByteArrayList lst;
                lst << data.toByteArray();
                m_buffer.push_back(lst);
                return;
            }

            if (text.contains('\n'))
                text = QString("\"%1\"").arg(text);
            qApp->clipboard()->setText(text);
            return;
        }
    }

    // If any of the cells contain binary data - we use inner buffer
    bool containsBinary = false;
    foreach (const QModelIndex& index, indices)
        if (m->isBinary(index)) {
            containsBinary = true;
            break;
        }

    if (containsBinary) {
        qApp->clipboard()->clear();
        // Copy selected data into inner buffer
        int columns = indices.last().column() - indices.first().column() + 1;
        while (!indices.isEmpty()) {
            QByteArrayList lst;
            for (int i = 0; i < columns; ++i) {
                lst << indices.first().data(Qt::EditRole).toByteArray();
                indices.pop_front();
            }
            m_buffer.push_back(lst);
        }

        return;
    }

    QModelIndex first = indices.first();
    QString result;
    int currentRow = 0;

    foreach(const QModelIndex& index, indices) {
        if (first == index) { /* first index */ }
        else if (index.row() != currentRow)
            result.append("\r\n");
        else
            result.append("\t");

        currentRow = index.row();
        QVariant data = index.data(Qt::EditRole);

        // non-NULL data is enquoted, whilst NULL isn't
        if (!data.isNull()) {
            QString text = data.toString();
            text.replace("\"", "\"\"");
            result.append(QString("\"%1\"").arg(text));
        }
    }

    qApp->clipboard()->setText(result);
}

void ExtendedTableWidget::paste()
{
    // Get list of selected items
    QItemSelectionModel* selection = selectionModel();
    QModelIndexList indices = selection->selectedIndexes();

    // Abort if there's nowhere to paste
    if(indices.isEmpty())
        return;

    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // If clipboard contains image - just insert it
    QImage img = qApp->clipboard()->image();
    if (!img.isNull()) {
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");
        buffer.close();

        m->setData(indices.first(), ba);
        return;
    }

    QString clipboard = qApp->clipboard()->text();

    if (clipboard.isEmpty() && !m_buffer.isEmpty()) {
        // If buffer contains something - use it instead of clipboard
        int rows = m_buffer.size();
        int columns = m_buffer.first().size();

        int firstRow = indices.front().row();
        int firstColumn = indices.front().column();

        int lastRow = qMin(firstRow + rows - 1, m->rowCount() - 1);
        int lastColumn = qMin(firstColumn + columns - 1, m->columnCount() - 1);

        int row = firstRow;

        foreach(const QByteArrayList& lst, m_buffer) {
            int column = firstColumn;
            foreach(const QByteArray& ba, lst) {
                m->setData(m->index(row, column), ba);

                column++;
                if (column > lastColumn)
                    break;
            }

            row++;
            if (row > lastRow)
                break;
        }

        return;
    }

    QList<QStringList> clipboardTable = parseClipboard(clipboard);

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
                tr("The content of the clipboard is bigger than the range selected.\nDo you want to insert it anyway?"),
                QMessageBox::Yes|QMessageBox::No);
            if(reply != QMessageBox::Yes)
            {
                return;
            }
        }
    }
    // Here we have positive answer even if cliboard is bigger than selection


    // If last row and column are after table size clamp it
    int lastRow = qMin(firstRow + clipboardRows - 1, m->rowCount() - 1);
    int lastColumn = qMin(firstColumn + clipboardColumns - 1, m->columnCount() - 1);

    int row = firstRow;
    foreach(const QStringList& clipboardRow, clipboardTable)
    {
        int column = firstColumn;
        foreach(const QString& cell, clipboardRow)
        {
            if (cell.isEmpty())
                m->setData(m->index(row, column), QVariant());
            else
            {
                QString text = cell;
                if (QRegExp("\".*\"").exactMatch(text))
                    text = text.mid(1, cell.length() - 2);
                text.replace("\"\"", "\"");
                m->setData(m->index(row, column), text);
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
        return;
    } else if(event->matches(QKeySequence::Paste)) {
        // Call a custom paste method when Ctrl-P is pressed
        paste();
    } else if(event->key() == Qt::Key_Tab && hasFocus() &&
              selectedIndexes().count() == 1 &&
              selectedIndexes().at(0).row() == model()->rowCount()-1 && selectedIndexes().at(0).column() == model()->columnCount()-1) {
        // If the Tab key was pressed while the focus was on the last cell of the last row insert a new row automatically
        model()->insertRow(model()->rowCount());
    } else if ((event->key() == Qt::Key_Delete) || (event->key() == Qt::Key_Backspace)) {
        if(event->modifiers().testFlag(Qt::AltModifier))
        {
            // When pressing Alt+Delete set the value to NULL
            foreach(const QModelIndex& index, selectedIndexes())
                model()->setData(index, QVariant());
        } else {
            // When pressing Delete only set the value to empty string
            foreach(const QModelIndex& index, selectedIndexes())
                model()->setData(index, "");
        }
    } else if(event->modifiers().testFlag(Qt::ControlModifier) && (event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown)) {
        // When pressing Ctrl + Page up/down send a signal indicating the user wants to change the current table
        emit switchTable(event->key() == Qt::Key_PageDown);
        return;
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
    // If Ctrl-Shift is pressed try to jump to the row referenced by the foreign key of the clicked cell
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
