#include "ExtendedTableWidget.h"
#include "sqlitetablemodel.h"
#include "FilterTableHeader.h"
#include "sqlitetypes.h"
#include "Settings.h"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QKeySequence>
#include <QKeyEvent>
#include <QScrollBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QBuffer>
#include <QMenu>

QList<QByteArrayList> ExtendedTableWidget::m_buffer;

namespace
{

QList<QByteArrayList> parseClipboard(QString clipboard)
{
    // Remove trailing line break from the clipboard text. This is necessary because some applications append an extra
    // line break to the clipboard contents which we would then interpret as regular data, setting the first field of the
    // first row after the paste area to NULL. One problem here is that this breaks for those cases where an empty line at
    // the end of the selection is explicitly copied and the originating application doesn't add an extra line break. However,
    // there are two reasons for favoring this way: 1) Spreadsheet applications seem to add an extra line break and they are
    // probably the main source for pasted data, 2) Having to manually delete on extra field seems to be less problematic than
    // having one extra field deleted without any warning.
    if(clipboard.endsWith("\n"))
        clipboard.chop(1);
    if(clipboard.endsWith("\r"))
        clipboard.chop(1);

    // Make sure there is some data in the clipboard
    QList<QByteArrayList> result;
    if(clipboard.isEmpty())
        return result;

    result.push_back(QByteArrayList());

    QRegExp re("(\"(?:[^\t\"]+|\"\"[^\"]*\"\")*)\"|(\t|\r?\n)");
    int offset = 0;
    int whitespace_offset = 0;

    while (offset >= 0) {
        QString text;
        int pos = re.indexIn(clipboard, offset);
        if (pos < 0) {
            // insert everything that left
            text = clipboard.mid(whitespace_offset);
            if(QRegExp("\".*\"").exactMatch(text))
                text = text.mid(1, text.length() - 2);
            text.replace("\"\"", "\"");
            result.last().push_back(text.toUtf8());
            break;
        }

        if (re.pos(2) < 0) {
            offset = pos + re.cap(1).length() + 1;
            continue;
        }

        QString ws = re.cap(2);
        // if two whitespaces in row - that's an empty cell
        if (!(pos - whitespace_offset)) {
            result.last().push_back(QByteArray());
        } else {
            text = clipboard.mid(whitespace_offset, pos - whitespace_offset);
            if(QRegExp("\".*\"").exactMatch(text))
                text = text.mid(1, text.length() - 2);
            text.replace("\"\"", "\"");
            result.last().push_back(text.toUtf8());
        }

        if (ws.endsWith("\n"))
            // create new row
            result.push_back(QByteArrayList());

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
    QAction* copyWithHeadersAction = new QAction(QIcon(":/icons/special_copy"), tr("Copy with Headers"), m_contextMenu);
    QAction* pasteAction = new QAction(QIcon(":/icons/paste"), tr("Paste"), m_contextMenu);
    QAction* filterAction = new QAction(tr("Use as Filter"), m_contextMenu);
    m_contextMenu->addAction(filterAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(nullAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(copyAction);
    m_contextMenu->addAction(copyWithHeadersAction);
    m_contextMenu->addAction(pasteAction);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // This is only for displaying the shortcut in the context menu.
    // An entry in keyPressEvent is still needed.
    nullAction->setShortcut(QKeySequence(tr("Alt+Del")));
    copyAction->setShortcut(QKeySequence::Copy);
    copyWithHeadersAction->setShortcut(QKeySequence(tr("Ctrl+Shift+C")));
    pasteAction->setShortcut(QKeySequence::Paste);

    // Set up context menu actions
    connect(this, &QTableView::customContextMenuRequested,
            [=](const QPoint& pos)
    {
        // Deactivate context menu options if there is no model set
        bool enabled = model();
        filterAction->setEnabled(enabled);
        copyAction->setEnabled(enabled);
        copyWithHeadersAction->setEnabled(enabled);

        // Try to find out whether the current view is editable and (de)activate menu options according to that
        bool editable = editTriggers() != QAbstractItemView::NoEditTriggers;
        nullAction->setEnabled(enabled && editable);
        pasteAction->setEnabled(enabled && editable);

        // Show menu
        m_contextMenu->popup(viewport()->mapToGlobal(pos));
    });
    connect(filterAction, &QAction::triggered, [&]() {
        useAsFilter();
    });
    connect(nullAction, &QAction::triggered, [&]() {
        for(const QModelIndex& index : selectedIndexes())
            model()->setData(index, QVariant());
    });
    connect(copyAction, &QAction::triggered, [&]() {
       copy(false);
    });
    connect(copyWithHeadersAction, &QAction::triggered, [&]() {
       copy(true);
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

void ExtendedTableWidget::copy(const bool withHeaders)
{
    QModelIndexList indices = selectionModel()->selectedIndexes();

    // Remove all indices from hidden columns, because if we don't we might copy data from hidden columns as well which is very
    // unintuitive; especially copying the rowid column when selecting all columns of a table is a problem because pasting the data
    // won't work as expected.
    for(int i=indices.size()-1;i>=0;)
    {
        if(isColumnHidden(indices.at(i).column()))
            indices.removeAt(i);
        else
            i--;
    }

    // Abort if there's nothing to copy
    if (indices.isEmpty())
        return;

    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // Clear internal copy-paste buffer
    m_buffer.clear();

    // If a single cell is selected, copy it to clipboard
    if (!withHeaders && indices.size() == 1) {
        QImage img;
        QVariant data = m->data(indices.first(), Qt::EditRole);

        if (img.loadFromData(data.toByteArray()))
        {
            // If it's an image, copy the image data to the clipboard
            qApp->clipboard()->setImage(img);
            return;
        } else {
            // It it's not an image, check if it's an empty field
            QString text = data.toString();
            if (text.isEmpty())
            {
                // The field is either NULL or empty. Those are are handled via the internal copy-paste buffer
                qApp->clipboard()->setText(QString());      // Calling clear() alone doesn't seem to work on all systems
                qApp->clipboard()->clear();
                m_buffer.push_back(QByteArrayList{data.toByteArray()});
                return;
            }

            // The field isn't empty. Quote data as needed and copy it to the clipboard
            qApp->clipboard()->setText(escapeCopiedData(text));
            return;
        }
    }

    // If any of the selected cells contains binary data, we use the internal copy-paste buffer
    bool containsBinary = false;
    for(const QModelIndex& index : indices)
    {
        if (m->isBinary(index)) {   // TODO: Should we check for NULL values, too?
            containsBinary = true;
            break;
        }
    }

    if (containsBinary)
    {
        // Make sure to clear the system clipboard, so it's not used when pasting
        qApp->clipboard()->setText(QString());      // Calling clear() alone doesn't seem to work on all systems
        qApp->clipboard()->clear();

        // Copy selected data into internal copy-paste buffer
        int columns = indices.last().column() - indices.first().column() + 1;       // Make sure the layout of the internal buffer is rectangular
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

    // If we got here, there are multiple selected cells, none of which contains binary data.
    // In this case, write a table both in HTML and text formats to clipboard

    QString result;
    QString htmlResult = "<html><header><style>br{mso-data-placement:same-cell;}</style></header><body><table>";
    int currentRow = indices.first().row();

    const QString fieldSepHtml = "</td><td>";
    const QString rowSepHtml = "</td></tr><tr><td>";
    const QString fieldSepText = "\t";
    const QString rowSepText = "\r\n";

    // Table headers
    if (withHeaders) {
        htmlResult.append("<tr><th>");
        int firstColumn = indices.front().column();
        for(int i = firstColumn; i <= indices.back().column(); i++) {
            QString headerText = model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
            if (i != firstColumn) {
                result.append(fieldSepText);
                htmlResult.append("</th><th>");
            }

            result.append(escapeCopiedData(headerText));
            htmlResult.append(headerText);
        }
        result.append(rowSepText);
        htmlResult.append("</th></tr>");
    }

    // Table data rows
    for(const QModelIndex& index : indices) {
        // Separators. For first cell, only opening table row tags must be added for the HTML and nothing for the text version.
        if (indices.first() == index)
            htmlResult.append("<tr><td>");
        else if (index.row() != currentRow) {
            result.append(rowSepText);
            htmlResult.append(rowSepHtml);
        } else {
            result.append(fieldSepText);
            htmlResult.append(fieldSepHtml);
        }
        currentRow = index.row();
        QString text = index.data(Qt::EditRole).toByteArray();

        // Table cell data
        if (text.contains('\n') || text.contains('\t'))
          htmlResult.append("<pre>" + text.toHtmlEscaped() + "</pre>");
        else
          htmlResult.append(text.toHtmlEscaped());

        result.append(escapeCopiedData(text));
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setHtml(htmlResult + "</td></tr></table></body></html>");
    mimeData->setText(result);
    qApp->clipboard()->setMimeData(mimeData);
}

QString ExtendedTableWidget::escapeCopiedData(QString data) const
{
    // Empty string is enquoted in plain text format, whilst NULL isn't
    // We also quote the data when there are line breaks in the text, again for spreadsheet compatability.
    // We also need to quote when there are tabs in the string (another option would be to replace the tabs by spaces, that's what
    // LibreOffice seems to be doing here).

    if(data.isNull())
        return data;

    if(data.isEmpty() || data.contains('\n') || data.contains('\t') || data.contains('"'))
    {
        data.replace("\"", "\"\"");
        return QString("\"%1\"").arg(data);
    } else {
        return data;
    }
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

    // We're also checking for system clipboard data first. Only if there is no data in the system clipboard we're falling back to the internal buffer.
    // That's a bit unfortunate because the data in the internal buffer is easier to parse and more accurate, too. However, if we always preferred the
    // internal copy-paste buffer there would be no way to copy data from other applications in here once the internal buffer has been filled.

    // If clipboard contains an image and no text, just insert the image
    const QMimeData* mimeClipboard = qApp->clipboard()->mimeData();
    if (mimeClipboard->hasImage() && !mimeClipboard->hasText()) {
        QImage img = qApp->clipboard()->image();
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");       // We're always converting the image format to PNG here. TODO: Is that correct?
        buffer.close();

        m->setData(indices.first(), ba);
        return;
    }

    // Get the clipboard text
    QString clipboard = qApp->clipboard()->text();

    // If there is no text but the internal copy-paste buffer is filled, use the internal buffer; otherwise parse the system clipboard contents
    QList<QByteArrayList> clipboardTable;
    QList<QByteArrayList>* source;
    if(clipboard.isEmpty() && !m_buffer.isEmpty())
    {
        source = &m_buffer;
    } else {
        clipboardTable = parseClipboard(clipboard);
        source = &clipboardTable;
    }

    // Stop here if there's nothing to paste
    if(!source->size())
        return;

    // Starting from assumption that selection is rectangular, and then first index is upper-left corner and last is lower-right.
    int rows = source->size();
    int columns = source->first().size();

    int firstRow = indices.front().row();
    int firstColumn = indices.front().column();
    int selectedRows = indices.back().row() - firstRow + 1;
    int selectedColumns = indices.back().column() - firstColumn + 1;

    // If last row and column are after table size, clamp it
    int lastRow = qMin(firstRow + rows - 1, m->rowCount() - 1);
    int lastColumn = qMin(firstColumn + columns - 1, m->columnCount() - 1);

    // Special case: if there is only one cell of data to be pasted, paste it into all selected fields
    if(rows == 1 && columns == 1)
    {
        QByteArray data = source->first().first();
        for(int row=firstRow;row<firstRow+selectedRows;row++)
        {
            for(int column=firstColumn;column<firstColumn+selectedColumns;column++)
                m->setData(m->index(row, column), data);
        }
        return;
    }

    // If more than one cell was selected, check if the selection matches the cliboard dimensions
    if(selectedRows != rows || selectedColumns != columns)
    {
        // Ask user if they are sure about this
        if(QMessageBox::question(this, QApplication::applicationName(),
                                 tr("The content of the clipboard is bigger than the range selected.\nDo you want to insert it anyway?"),
                                 QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        {
            // If the user doesn't want to paste the clipboard data anymore, stop now
            return;
        }
    }

    // If we get here, we can definitely start pasting: either the ranges match in their size or the user agreed to paste anyway

    // Copy the data cell by cell and as-is from the source buffer to the table
    int row = firstRow;
    for(const QByteArrayList& source_row : *source)
    {
        int column = firstColumn;
        for(const QByteArray& source_cell : source_row)
        {
            m->setData(m->index(row, column), source_cell);

            column++;
            if (column > lastColumn)
                break;
        }

        row++;
        if (row > lastRow)
            break;
    }
}

void ExtendedTableWidget::useAsFilter()
{
    QModelIndex index = selectionModel()->currentIndex();
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // Abort if there's nothing to filter
    if (!index.isValid() || !selectionModel()->hasSelection() || m->isBinary(index))
        return;

    QVariant data = model()->data(index, Qt::EditRole);

    if (data.isNull())
        m_tableHeader->setFilter(index.column(), "=NULL");
    else if (data.toString().isEmpty())
        m_tableHeader->setFilter(index.column(), "=''");
    else
        m_tableHeader->setFilter(index.column(), "=" + data.toString());
}

void ExtendedTableWidget::keyPressEvent(QKeyEvent* event)
{
    // Call a custom copy method when Ctrl-C is pressed
    if(event->matches(QKeySequence::Copy))
    {
        copy(false);
        return;
    } else if(event->matches(QKeySequence::Paste)) {
        // Call a custom paste method when Ctrl-V is pressed
        paste();
    } else if(event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) && (event->key() == Qt::Key_C)) {
        // Call copy with headers when Ctrl-Shift-C is pressed
        copy(true);
    } else if(event->key() == Qt::Key_Tab && hasFocus() &&
              selectedIndexes().count() == 1 &&
              selectedIndexes().at(0).row() == model()->rowCount()-1 && selectedIndexes().at(0).column() == model()->columnCount()-1) {
        // If the Tab key was pressed while the focus was on the last cell of the last row insert a new row automatically
        model()->insertRow(model()->rowCount());
    } else if ((event->key() == Qt::Key_Delete) || (event->key() == Qt::Key_Backspace)) {
        if(event->modifiers().testFlag(Qt::AltModifier))
        {
            // When pressing Alt+Delete set the value to NULL
            for(const QModelIndex& index : selectedIndexes())
                model()->setData(index, QVariant());
        } else {
            // When pressing Delete only set the value to empty string
            for(const QModelIndex& index : selectedIndexes())
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
    for(const QModelIndex & idx : selectedIndexes())
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
            emit foreignKeyClicked(sqlb::ObjectIdentifier(m->currentTableName().schema(), fk.table()),
                                   fk.columns().size() ? fk.columns().at(0) : "",
                                   m->data(index, Qt::EditRole).toByteArray());
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
    
    if (!index.isValid())
    {
        if (event->mimeData()->hasUrls() && event->mimeData()->urls().first().isLocalFile())
            emit openFileFromDropEvent(event->mimeData()->urls().first().toLocalFile());
        return;
    }
    
    model()->dropMimeData(event->mimeData(), Qt::CopyAction, index.row(), index.column(), QModelIndex());
    event->acceptProposedAction();
}
