#include "ExtendedTableWidget.h"
#include "sqlitetablemodel.h"
#include "FilterTableHeader.h"
#include "sql/sqlitetypes.h"
#include "Settings.h"
#include "sqlitedb.h"
#include "CondFormat.h"

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
#include <QDateTime>
#include <QLineEdit>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTextDocument>
#include <QCompleter>
#include <QComboBox>
#include <QPainter>
#include <QShortcut>

#include <limits>

using BufferRow = std::vector<QByteArray>;
std::vector<BufferRow> ExtendedTableWidget::m_buffer;
QString ExtendedTableWidget::m_generatorStamp;

namespace
{

std::vector<BufferRow> parseClipboard(QString clipboard)
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
    std::vector<BufferRow> result;
    if(clipboard.isEmpty())
        return result;

    result.push_back(BufferRow());

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
            result.back().push_back(text.toUtf8());
            break;
        }

        if (re.pos(2) < 0) {
            offset = pos + re.cap(1).length() + 1;
            continue;
        }

        QString ws = re.cap(2);
        // if two whitespaces in row - that's an empty cell
        if (!(pos - whitespace_offset)) {
            result.back().push_back(QByteArray());
        } else {
            text = clipboard.mid(whitespace_offset, pos - whitespace_offset);
            if(QRegExp("\".*\"").exactMatch(text))
                text = text.mid(1, text.length() - 2);
            text.replace("\"\"", "\"");
            result.back().push_back(text.toUtf8());
        }

        if (ws.endsWith("\n"))
            // create new row
            result.push_back(BufferRow());

        whitespace_offset = offset = pos + ws.length();
    }

    return result;
}

}

UniqueFilterModel::UniqueFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

bool UniqueFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);
    const std::string value = index.data(Qt::EditRole).toString().toStdString();

    if (!value.empty() && m_uniqueValues.find(value) == m_uniqueValues.end()) {
        const_cast<UniqueFilterModel*>(this)->m_uniqueValues.insert(value);
        return true;
    }
    else
        return false;

}

ExtendedTableWidgetEditorDelegate::ExtendedTableWidgetEditorDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* ExtendedTableWidgetEditorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
{
    emit dataAboutToBeEdited(index);

    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(const_cast<QAbstractItemModel*>(index.model()));
    auto fk = m->getForeignKeyClause(static_cast<size_t>(index.column()-1));

    if(fk) {
        sqlb::ObjectIdentifier foreignTable = sqlb::ObjectIdentifier(m->currentTableName().schema(), fk->table());

        std::string column;
        // If no column name is set, assume the primary key is meant
        if(fk->columns().empty()) {
            sqlb::TablePtr obj = m->db().getTableByName(foreignTable);
            column = obj->primaryKeyColumns().front().name();
        } else
            column = fk->columns().at(0);

        sqlb::TablePtr currentTable = m->db().getTableByName(m->currentTableName());
        QString query = QString("SELECT %1 FROM %2").arg(QString::fromStdString(sqlb::escapeIdentifier(column)), QString::fromStdString(foreignTable.toString()));

        // if the current column of the current table does NOT have not-null constraint,
        // the NULL is united to the query to get the possible values in the combo-box.
        if (!currentTable->fields.at(static_cast<size_t>(index.column())-1).notnull())
            query.append (" UNION SELECT NULL");

        SqliteTableModel* fkModel = new SqliteTableModel(m->db(), parent, m->encoding());
        fkModel->setQuery(query);

        QComboBox* combo = new QComboBox(parent);

        // Complete cache so it is ready when setEditorData is invoked.
        fkModel->completeCache();
        combo->setModel(fkModel);

        return combo;
    } else {

        QLineEdit* editor = new QLineEdit(parent);
        // If the row count is not greater than the complete threshold setting, set a completer of values based on current values in the column.
        if (index.model()->rowCount() <= Settings::getValue("databrowser", "complete_threshold").toInt()) {
            QCompleter* completer = new QCompleter(editor);
            UniqueFilterModel* completerFilter = new UniqueFilterModel(completer);
            // Provide a filter for the source model, so only unique and non-empty values are accepted.
            completerFilter->setSourceModel(const_cast<QAbstractItemModel*>(index.model()));
            completerFilter->setFilterKeyColumn(index.column());
            completer->setModel(completerFilter);
            // Complete on this column, using a popup and case-insensitively.
            completer->setCompletionColumn(index.column());
            completer->setCompletionMode(QCompleter::PopupCompletion);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            editor->setCompleter(completer);
        }
        // Set the maximum length to the highest possible value instead of the default 32768.
        editor->setMaxLength(std::numeric_limits<int>::max());
        return editor;
    }
}

void ExtendedTableWidgetEditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QLineEdit* lineedit = dynamic_cast<QLineEdit*>(editor);
    // Set the data for the editor
    QString data = index.data(Qt::EditRole).toString();

    if(!lineedit) {
        QComboBox* combo = static_cast<QComboBox*>(editor);
        int comboIndex = combo->findText(data);
        if (comboIndex >= 0)
            // if it is valid, adjust the combobox
            combo->setCurrentIndex(comboIndex);
    } else {
        lineedit->setText(data);

        // Put the editor in read only mode if the actual data is larger than the maximum length to avoid accidental truncation of the data
        lineedit->setReadOnly(data.size() > lineedit->maxLength());
    }
}

void ExtendedTableWidgetEditorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    // Only apply the data back to the model if the editor is not in read only mode to avoid accidental truncation of the data
    QLineEdit* lineedit = dynamic_cast<QLineEdit*>(editor);
    if(!lineedit) {
        QComboBox* combo = static_cast<QComboBox*>(editor);
        model->setData(index, combo->currentData(Qt::EditRole), Qt::EditRole);
    } else
        if(!lineedit->isReadOnly())
            model->setData(index, lineedit->text());
}

void ExtendedTableWidgetEditorDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
    editor->setGeometry(option.rect);
}

class ItemBorderDelegate : public QStyledItemDelegate
{
public:
    explicit ItemBorderDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);
        QRect border(option.rect);
        border.translate(border.width() - 1, 0);
        border.setWidth(2);
        painter->fillRect(border, Qt::black);
    }
};


ExtendedTableWidget::ExtendedTableWidget(QWidget* parent) :
    QTableView(parent),
    m_frozen_table_view(qobject_cast<ExtendedTableWidget*>(parent) ? nullptr : new ExtendedTableWidget(this)),
    m_frozen_column_count(0),
    m_item_border_delegate(new ItemBorderDelegate(this))
{
    setHorizontalScrollMode(ExtendedTableWidget::ScrollPerPixel);
    // Force ScrollPerItem, so scrolling shows all table rows
    setVerticalScrollMode(ExtendedTableWidget::ScrollPerItem);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &ExtendedTableWidget::vscrollbarChanged);
    connect(this, &ExtendedTableWidget::clicked, this, &ExtendedTableWidget::cellClicked);

    // Set up filter row
    m_tableHeader = new FilterTableHeader(this);
    setHorizontalHeader(m_tableHeader);

    // Disconnect clicking in header to select column, since we will use it for sorting.
    // Note that, in order to work, this cannot be converted to the standard C++11 format.
    disconnect(m_tableHeader, SIGNAL(sectionPressed(int)),this, SLOT(selectColumn(int)));

    // Set up vertical header context menu
    verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    // Set up table view context menu
    m_contextMenu = new QMenu(this);

    QAction* filterAction = new QAction(QIcon(":/icons/filter"), tr("Use as Exact Filter"), m_contextMenu);
    QAction* containingAction = new QAction(tr("Containing"), m_contextMenu);
    QAction* notContainingAction = new QAction(tr("Not containing"), m_contextMenu);
    QAction* notEqualToAction = new QAction(tr("Not equal to"), m_contextMenu);
    QAction* greaterThanAction = new QAction(tr("Greater than"), m_contextMenu);
    QAction* lessThanAction = new QAction(tr("Less than"), m_contextMenu);
    QAction* greaterEqualAction = new QAction(tr("Greater or equal"), m_contextMenu);
    QAction* lessEqualAction = new QAction(tr("Less or equal"), m_contextMenu);
    QAction* inRangeAction = new QAction(tr("Between this and..."), m_contextMenu);
    QAction* regexpAction = new QAction(tr("Regular expression"), m_contextMenu);
    QAction* condFormatAction = new QAction(QIcon(":/icons/edit_cond_formats"), tr("Edit Conditional Formats..."), m_contextMenu);

    QAction* nullAction = new QAction(QIcon(":/icons/set_to_null"), tr("Set to NULL"), m_contextMenu);
    QAction* cutAction = new QAction(QIcon(":/icons/cut"), tr("Cut"), m_contextMenu);
    QAction* copyAction = new QAction(QIcon(":/icons/copy"), tr("Copy"), m_contextMenu);
    QAction* copyWithHeadersAction = new QAction(QIcon(":/icons/special_copy"), tr("Copy with Headers"), m_contextMenu);
    QAction* copyAsSQLAction = new QAction(QIcon(":/icons/sql_copy"), tr("Copy as SQL"), m_contextMenu);
    QAction* pasteAction = new QAction(QIcon(":/icons/paste"), tr("Paste"), m_contextMenu);
    QAction* printAction = new QAction(QIcon(":/icons/print"), tr("Print..."), m_contextMenu);

    m_contextMenu->addAction(filterAction);
    QMenu* filterMenu = m_contextMenu->addMenu(tr("Use in Filter Expression"));
    filterMenu->addAction(containingAction);
    filterMenu->addAction(notContainingAction);
    filterMenu->addAction(notEqualToAction);
    filterMenu->addAction(greaterThanAction);
    filterMenu->addAction(lessThanAction);
    filterMenu->addAction(greaterEqualAction);
    filterMenu->addAction(lessEqualAction);
    filterMenu->addAction(inRangeAction);
    filterMenu->addAction(regexpAction);
    m_contextMenu->addAction(condFormatAction);

    m_contextMenu->addSeparator();
    m_contextMenu->addAction(nullAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(cutAction);
    m_contextMenu->addAction(copyAction);
    m_contextMenu->addAction(copyWithHeadersAction);
    m_contextMenu->addAction(copyAsSQLAction);
    m_contextMenu->addAction(pasteAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(printAction);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Create and set up delegate
    m_editorDelegate = new ExtendedTableWidgetEditorDelegate(this);
    setItemDelegate(m_editorDelegate);
    connect(m_editorDelegate, &ExtendedTableWidgetEditorDelegate::dataAboutToBeEdited, this, &ExtendedTableWidget::dataAboutToBeEdited);

    // This is only for displaying the shortcut in the context menu.
    // An entry in keyPressEvent is still needed.
    nullAction->setShortcut(QKeySequence(tr("Alt+Del")));
    cutAction->setShortcut(QKeySequence::Cut);
    copyAction->setShortcut(QKeySequence::Copy);
    copyWithHeadersAction->setShortcut(QKeySequence(tr("Ctrl+Shift+C")));
    copyAsSQLAction->setShortcut(QKeySequence(tr("Ctrl+Alt+C")));
    pasteAction->setShortcut(QKeySequence::Paste);
    printAction->setShortcut(QKeySequence::Print);

    // Set up context menu actions
    connect(this, &QTableView::customContextMenuRequested, this,
            [=](const QPoint& pos)
    {
        // Deactivate context menu options if there is no model set
        bool enabled = model();
        filterAction->setEnabled(enabled);
        filterMenu->setEnabled(enabled);
        copyAction->setEnabled(enabled);
        copyWithHeadersAction->setEnabled(enabled);
        copyAsSQLAction->setEnabled(enabled);
        printAction->setEnabled(enabled);
        condFormatAction->setEnabled(enabled);

        // Hide filter actions when there isn't any filters
        bool hasFilters = m_tableHeader->hasFilters();
        filterAction->setVisible(hasFilters);
        filterMenu->menuAction()->setVisible(hasFilters);
        condFormatAction->setVisible(hasFilters);

        // Try to find out whether the current view is editable and (de)activate menu options according to that
        bool editable = editTriggers() != QAbstractItemView::NoEditTriggers;
        nullAction->setEnabled(enabled && editable);
        cutAction->setEnabled(enabled && editable);
        pasteAction->setEnabled(enabled && editable);

        // Show menu
        m_contextMenu->popup(viewport()->mapToGlobal(pos));
    });
    connect(filterAction, &QAction::triggered, this, [&]() {
        useAsFilter(QString ("="));
    });
    connect(containingAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString (""));
        });
    // Use a regular expression for the not containing filter. Simplify this if we ever support the NOT LIKE filter.
    connect(notContainingAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString ("/^((?!"), /* binary */ false, QString (").)*$/"));
        });
    connect(notEqualToAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString ("<>"));
        });
    connect(greaterThanAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString (">"));
        });
    connect(lessThanAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString ("<"));
        });
    connect(greaterEqualAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString (">="));
        });
    connect(lessEqualAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString ("<="));
        });
    connect(inRangeAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString ("~"), /* binary */ true);
        });
    connect(regexpAction, &QAction::triggered, this, [&]() {
            useAsFilter(QString ("/"), /* binary */ false, QString ("/"));
        });
    connect(condFormatAction, &QAction::triggered, this, [&]() {
        emit editCondFormats(currentIndex().column());
    });

    connect(nullAction, &QAction::triggered, this, [&]() {
       setToNull(selectedIndexes());
    });
    connect(copyAction, &QAction::triggered, this, [&]() {
       copy(false, false);
    });
    connect(cutAction, &QAction::triggered, this, &ExtendedTableWidget::cut);
    connect(copyWithHeadersAction, &QAction::triggered, this, [&]() {
       copy(true, false);
    });
    connect(copyAsSQLAction, &QAction::triggered, this, [&]() {
       copy(false, true);
    });
    connect(pasteAction, &QAction::triggered, this, [&]() {
       paste();
    });
    connect(printAction, &QAction::triggered, this, [&]() {
       openPrintDialog();
    });

    // Add spreadsheet shortcuts for selecting entire columns or entire rows.
    QShortcut* selectColumnShortcut = new QShortcut(QKeySequence("Ctrl+Space"), this);
    connect(selectColumnShortcut, &QShortcut::activated, this, [this]() {
        if(!hasFocus() || selectionModel()->selectedIndexes().isEmpty())
            return;
        selectionModel()->select(selectionModel()->selection(), QItemSelectionModel::Select | QItemSelectionModel::Columns);
    });
    QShortcut* selectRowShortcut = new QShortcut(QKeySequence("Shift+Space"), this);
    connect(selectRowShortcut, &QShortcut::activated, this, [this]() {
        if(!hasFocus() || selectionModel()->selectedIndexes().isEmpty())
            return;
        selectionModel()->select(selectionModel()->selection(), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    });

    // Set up frozen columns child widget
    if(m_frozen_table_view)
    {
        // Set up widget
        m_frozen_table_view->setFocusPolicy(Qt::NoFocus);
        m_frozen_table_view->verticalHeader()->hide();
        m_frozen_table_view->setStyleSheet("QTableView { border: none; }"
                                           "QTableView::item:selected{ background-color: " + palette().color(QPalette::Active, QPalette::Highlight).name() + "}");
        m_frozen_table_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_frozen_table_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_frozen_table_view->setVerticalScrollMode(verticalScrollMode());
        viewport()->stackUnder(m_frozen_table_view);
        m_tableHeader->stackUnder(m_frozen_table_view);

        // Keep both widgets in sync
        connect(horizontalHeader(), &QHeaderView::sectionResized, this, &ExtendedTableWidget::updateSectionWidth);
        connect(m_frozen_table_view->horizontalHeader(), &QHeaderView::sectionResized, this, &ExtendedTableWidget::updateSectionWidth);
        connect(verticalHeader(), &QHeaderView::sectionResized, this, &ExtendedTableWidget::updateSectionHeight);
        connect(m_frozen_table_view->verticalHeader(), &QHeaderView::sectionResized, this, &ExtendedTableWidget::updateSectionHeight);
        connect(m_frozen_table_view->verticalScrollBar(), &QAbstractSlider::valueChanged, verticalScrollBar(), &QAbstractSlider::setValue);
        connect(verticalScrollBar(), &QAbstractSlider::valueChanged, m_frozen_table_view->verticalScrollBar(), &QAbstractSlider::setValue);

        // Forward signals from frozen table view widget to the main table view widget
        connect(m_frozen_table_view, &ExtendedTableWidget::doubleClicked, this, &ExtendedTableWidget::doubleClicked);
        connect(m_frozen_table_view->filterHeader(), &FilterTableHeader::sectionClicked, filterHeader(), &FilterTableHeader::sectionClicked);
        connect(m_frozen_table_view->filterHeader(), &QHeaderView::sectionDoubleClicked, filterHeader(), &QHeaderView::sectionDoubleClicked);
        connect(m_frozen_table_view->verticalHeader(), &QHeaderView::sectionResized, verticalHeader(), &QHeaderView::sectionResized);
        connect(m_frozen_table_view->horizontalHeader(), &QHeaderView::customContextMenuRequested, horizontalHeader(), &QHeaderView::customContextMenuRequested);
        connect(m_frozen_table_view->verticalHeader(), &QHeaderView::customContextMenuRequested, verticalHeader(), &QHeaderView::customContextMenuRequested);
        connect(m_frozen_table_view, &ExtendedTableWidget::openFileFromDropEvent, this, &ExtendedTableWidget::openFileFromDropEvent);
        connect(m_frozen_table_view, &ExtendedTableWidget::selectedRowsToBeDeleted, this, &ExtendedTableWidget::selectedRowsToBeDeleted);
        connect(m_frozen_table_view->filterHeader(), &FilterTableHeader::filterChanged, filterHeader(), &FilterTableHeader::filterChanged);
        connect(m_frozen_table_view->filterHeader(), &FilterTableHeader::addCondFormat, filterHeader(), &FilterTableHeader::addCondFormat);
        connect(m_frozen_table_view->filterHeader(), &FilterTableHeader::allCondFormatsCleared, filterHeader(), &FilterTableHeader::allCondFormatsCleared);
        connect(m_frozen_table_view->filterHeader(), &FilterTableHeader::condFormatsEdited, filterHeader(), &FilterTableHeader::condFormatsEdited);
        connect(m_frozen_table_view, &ExtendedTableWidget::editCondFormats, this, &ExtendedTableWidget::editCondFormats);
        connect(m_frozen_table_view, &ExtendedTableWidget::dataAboutToBeEdited, this, &ExtendedTableWidget::dataAboutToBeEdited);
        connect(m_frozen_table_view, &ExtendedTableWidget::foreignKeyClicked, this, &ExtendedTableWidget::foreignKeyClicked);
        connect(m_frozen_table_view, &ExtendedTableWidget::currentIndexChanged, this, &ExtendedTableWidget::currentIndexChanged);
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0) && QT_VERSION < QT_VERSION_CHECK(5, 12, 3)
    // This work arounds QTBUG-73721 and it is applied only for the affected version range.
    setWordWrap(false);
#endif
}

ExtendedTableWidget::~ExtendedTableWidget()
{
    delete m_frozen_table_view;
}

void ExtendedTableWidget::setModel(QAbstractItemModel* item_model)
{
    // Set model
    QTableView::setModel(item_model);

    // Set up frozen table view widget
    if(item_model)
        setFrozenColumns(m_frozen_column_count);
    else
        m_frozen_table_view->hide();
}

void ExtendedTableWidget::reloadSettings()
{
    // We only get the font here to get its metrics. The actual font for the view is set in the model
    QFont dataBrowserFont(Settings::getValue("databrowser", "font").toString());
    dataBrowserFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());

    // Set new default row height depending on the font size
    QFontMetrics fontMetrics(dataBrowserFont);
    verticalHeader()->setDefaultSectionSize(fontMetrics.height()+10);
    if(m_frozen_table_view)
        m_frozen_table_view->reloadSettings();
}

void ExtendedTableWidget::copyMimeData(const QModelIndexList& fromIndices, QMimeData* mimeData, const bool withHeaders, const bool inSQL)
{
    QModelIndexList indices = fromIndices;

    // Remove all indices from hidden columns, because if we don't we might copy data from hidden columns as well which is very
    // unintuitive; especially copying the rowid column when selecting all columns of a table is a problem because pasting the data
    // won't work as expected.
    QMutableListIterator<QModelIndex> it(indices);
    while (it.hasNext()) {
        if (isColumnHidden(it.next().column()))
            it.remove();
    }

    // Abort if there's nothing to copy
    if (indices.isEmpty())
        return;

    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // Clear internal copy-paste buffer
    m_buffer.clear();

    // If a single cell is selected which contains an image, copy it to the clipboard
    if (!inSQL && !withHeaders && indices.size() == 1) {
        QImage img;
        QVariant varData = m->data(indices.first(), Qt::EditRole);

        if (img.loadFromData(varData.toByteArray()))
        {
            // If it's an image, copy the image data to the clipboard
            mimeData->setImageData(img);
            return;
        }
    }

    // If we got here, a non-image cell was or multiple cells were selected, or copy with headers was requested.
    // In this case, we copy selected data into internal copy-paste buffer and then
    // we write a table both in HTML and text formats to the system clipboard.

    // Copy selected data into internal copy-paste buffer
    int last_row = indices.first().row();
    BufferRow lst;
    for(int i=0;i<indices.size();i++)
    {
        if(indices.at(i).row() != last_row)
        {
            m_buffer.push_back(lst);
            lst.clear();
        }
        lst.push_back(indices.at(i).data(Qt::EditRole).toByteArray());
        last_row = indices.at(i).row();
    }
    m_buffer.push_back(lst);

    QString sqlResult;
    QString result;
    QString htmlResult = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">";
    htmlResult.append("<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
    htmlResult.append("<title></title>");

    // The generator-stamp is later used to know whether the data in the system clipboard is still ours.
    // In that case we will give precedence to our internal copy buffer.
    QString now = QDateTime::currentDateTime().toString("YYYY-MM-DDTHH:mm:ss.zzz");
    m_generatorStamp = QString("<meta name=\"generator\" content=\"%1\"><meta name=\"date\" content=\"%2\">").arg(QApplication::applicationName().toHtmlEscaped(), now);
    htmlResult.append(m_generatorStamp);
    // TODO: is this really needed by Excel, since we use <pre> for multi-line cells?
    htmlResult.append("<style type=\"text/css\">br{mso-data-placement:same-cell;}</style></head><body>"
                      "<table border=1 cellspacing=0 cellpadding=2>");

    // Insert the columns in a set, since they could be non-contiguous.
    std::set<int> colsInIndexes, rowsInIndexes;
    for(const QModelIndex & idx : qAsConst(indices)) {
        colsInIndexes.insert(idx.column());
        rowsInIndexes.insert(idx.row());
    }

    int currentRow = indices.first().row();

    const QString fieldSepText = "\t";
#ifdef Q_OS_WIN
    const QString rowSepText = "\r\n";
#else
    const QString rowSepText = "\n";
#endif

    QString sqlInsertStatement = QString("INSERT INTO %1 (").arg(QString::fromStdString(m->currentTableName().toString()));
    // Table headers
    if (withHeaders || inSQL) {
        htmlResult.append("<tr><th>");
        int firstColumn = *colsInIndexes.begin();

        for(int col : colsInIndexes) {
            QByteArray headerText = model()->headerData(col, Qt::Horizontal, Qt::EditRole).toByteArray();
            if (col != firstColumn) {
                result.append(fieldSepText);
                htmlResult.append("</th><th>");
                sqlInsertStatement.append(", ");
            }

            result.append(headerText);
            htmlResult.append(headerText);
            sqlInsertStatement.append(sqlb::escapeIdentifier(headerText));
        }
        result.append(rowSepText);
        htmlResult.append("</th></tr>");
        sqlInsertStatement.append(") VALUES (");
    }

    // Iterate over rows x cols checking if the index actually exists when needed, in order
    // to support non-rectangular selections.
    for(const int row : rowsInIndexes) {
        for(const int column : colsInIndexes) {

            const QModelIndex index = indices.first().sibling(row, column);
            QString style;
            if(indices.contains(index)) {
                QFont font;
                font.fromString(index.data(Qt::FontRole).toString());
                const QString fontStyle(font.italic() ? "italic" : "normal");
                const QString fontWeigth(font.bold() ? "bold" : "normal");
                const QString fontDecoration(font.underline() ? " text-decoration: underline;" : "");
                const QColor bgColor(index.data(Qt::BackgroundRole).toString());
                const QColor fgColor(index.data(Qt::ForegroundRole).toString());
                const Qt::Alignment align(index.data(Qt::TextAlignmentRole).toInt());
                const QString textAlign(CondFormat::alignmentTexts().at(CondFormat::fromCombinedAlignment(align)).toLower());
                style = QString("style=\"font-family: '%1'; font-size: %2pt; font-style: %3; font-weight: %4;%5 "
                                "background-color: %6; color: %7; text-align: %8\"").arg(
                                    font.family().toHtmlEscaped(),
                                    QString::number(font.pointSize()),
                                    fontStyle,
                                    fontWeigth,
                                    fontDecoration,
                                    bgColor.name(),
                                    fgColor.name(),
                                    textAlign);
            }

            // Separators. For first cell, only opening table row tags must be added for the HTML and nothing for the text version.
            if (index.row() == *rowsInIndexes.begin() && index.column() == *colsInIndexes.begin()) {
                htmlResult.append(QString("<tr><td %1>").arg(style));
                sqlResult.append(sqlInsertStatement);
            } else if (index.row() != currentRow) {
                result.append(rowSepText);
                htmlResult.append(QString("</td></tr><tr><td %1>").arg(style));
                sqlResult.append(");" + rowSepText + sqlInsertStatement);
            } else {
                result.append(fieldSepText);
                htmlResult.append(QString("</td><td %1>").arg(style));
                sqlResult.append(", ");
            }

            currentRow = index.row();

            QImage img;
            QVariant bArrdata = indices.contains(index) ? index.data(Qt::EditRole) : QVariant();

            // Table cell data: image? Store it as an embedded image in HTML
            if (!inSQL && img.loadFromData(bArrdata.toByteArray()))
            {
                QByteArray ba;
                QBuffer buffer(&ba);
                buffer.open(QIODevice::WriteOnly);
                img.save(&buffer, "PNG");
                buffer.close();

                QString imageBase64 = ba.toBase64();
                htmlResult.append("<img src=\"data:image/png;base64,");
                htmlResult.append(imageBase64);
                result.append(QString());
                htmlResult.append("\" alt=\"Image\">");
            } else {
                if (bArrdata.isNull()) {
                    sqlResult.append("NULL");
                } else if(!m->isBinary(index)) {
                    QByteArray text = bArrdata.toByteArray();

                    // Table cell data: text
                    if (text.contains('\n') || text.contains('\t'))
                        htmlResult.append("<pre>" + QString(text).toHtmlEscaped() + "</pre>");
                    else
                        htmlResult.append(QString(text).toHtmlEscaped());

                    result.append(text);
                    sqlResult.append(sqlb::escapeString(text));
                } else
                    // Table cell data: binary. Save as BLOB literal in SQL
                    sqlResult.append( "X'" + bArrdata.toByteArray().toHex() + "'" );
            }
        }
    }

    sqlResult.append(");");

    if ( inSQL )
    {
        mimeData->setText(sqlResult);
    } else {
        mimeData->setHtml(htmlResult + "</td></tr></table></body></html>");
        mimeData->setText(result);
    }
}

void ExtendedTableWidget::copy(const bool withHeaders, const bool inSQL )
{
    QMimeData *mimeData = new QMimeData;
    copyMimeData(selectionModel()->selectedIndexes(), mimeData, withHeaders, inSQL);
    qApp->clipboard()->setMimeData(mimeData);
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

    // We're also checking for system clipboard data first. Only if the data in the system clipboard is not ours, we use the system
    // clipboard, otherwise we prefer the internal buffer.  That's because the data in the internal buffer is easier to parse and more
    // accurate, too. However, if we always preferred the internal copy-paste buffer there would be no way to copy data from other
    // applications in here once the internal buffer has been filled.

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


    // If data in system clipboard is ours and the internal copy-paste buffer is filled, use the internal buffer; otherwise parse the
    // system clipboard contents (case for data copied by other application).

    std::vector<BufferRow> clipboardTable;
    std::vector<BufferRow>* source;

    if(mimeClipboard->hasHtml() && mimeClipboard->html().contains(m_generatorStamp) && !m_buffer.empty())
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
    int rows = static_cast<int>(source->size());
    int columns = static_cast<int>(source->front().size());

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
        QByteArray bArrdata = source->front().front();
        for(int row=firstRow;row<firstRow+selectedRows;row++)
        {
            for(int column=firstColumn;column<firstColumn+selectedColumns;column++)
                m->setData(m->index(row, column), bArrdata);
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
    for(const auto& source_row : *source)
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

void ExtendedTableWidget::cut()
{
    const QModelIndexList& indices = selectionModel()->selectedIndexes();
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
    sqlb::TablePtr currentTable = m->db().getTableByName(m->currentTableName());

    copy(false, false);

    // Check if the column in the selection has a NOT NULL constraint, then update with an empty string, else with NULL
    if(currentTable) {
        for(const QModelIndex& index : indices) {
            // Do not process rowid column
            if(index.column() != 0) {
                const size_t indexField = static_cast<size_t>(index.column()-1);
                const sqlb::Field& field = currentTable->fields.at(indexField);
                const QVariant newValue = field.notnull() ? QVariant("") : QVariant();
                // Update aborting in case of any error (to avoid repetitive errors like "Database is locked")
                if(!model()->setData(index, newValue))
                    return;
            }
        }
    }
}

void ExtendedTableWidget::useAsFilter(const QString& filterOperator, bool binary, const QString& operatorSuffix)
{
    QModelIndex index = selectionModel()->currentIndex();
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // Abort if there's nothing to filter
    if (!index.isValid() || !selectionModel()->hasSelection() || m->isBinary(index))
        return;

    QVariant bArrdata = model()->data(index, Qt::EditRole);
    QString value;
    if (bArrdata.isNull())
        value = "NULL";
    else if (bArrdata.toString().isEmpty())
        value = "''";
    else
        value = bArrdata.toString();

    // When Containing filter is requested (empty operator) and the value starts with
    // an operator character, the character is escaped.
    if (filterOperator.isEmpty())
        value.replace(QRegExp("^(<|>|=|/)"), Settings::getValue("databrowser", "filter_escape").toString() + QString("\\1"));

    // If binary operator, the cell data is used as first value and
    // the second value must be added by the user.
    size_t column = static_cast<size_t>(index.column());
    if (binary)
        m_tableHeader->setFilter(column, value + filterOperator);
    else
        m_tableHeader->setFilter(column, filterOperator + value + operatorSuffix);
}

void ExtendedTableWidget::duplicateUpperCell()
{
    const QModelIndex& currentIndex = selectionModel()->currentIndex();
    QModelIndex upperIndex = currentIndex.sibling(currentIndex.row() - 1, currentIndex.column());
    if (upperIndex.isValid()) {
        SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
        // When the data is binary, just copy it, since it cannot be edited inline.
        if (m->isBinary(upperIndex))
            m->setData(currentIndex, m->data(upperIndex, Qt::EditRole), Qt::EditRole);
        else {
            // Open the inline editor and set the value (this mimics the behaviour of LibreOffice Calc)
            edit(currentIndex);
            QLineEdit* editor = qobject_cast<QLineEdit*>(indexWidget(currentIndex));
            editor->setText(upperIndex.data().toString());
        }
    }
}

void ExtendedTableWidget::keyPressEvent(QKeyEvent* event)
{
    // Call a custom copy method when Ctrl-C is pressed
    if(event->matches(QKeySequence::Copy))
    {
        copy(false, false);
        return;
    } else if(event->matches(QKeySequence::Cut)) {
        // Call a custom cut method when Ctrl-X is pressed
        cut();
    } else if(event->matches(QKeySequence::Paste)) {
        // Call a custom paste method when Ctrl-V is pressed
        paste();
    } else if(event->matches(QKeySequence::Print)) {
        openPrintDialog();
    } else if(event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) && (event->key() == Qt::Key_C)) {
        // Call copy with headers when Ctrl-Shift-C is pressed
        copy(true, false);
    } else if(event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::AltModifier) && (event->key() == Qt::Key_C)) {
        // Call copy in SQL format when Ctrl-Alt-C is pressed
        copy(false, true);
    } else if(event->modifiers().testFlag(Qt::ControlModifier) && (event->key() == Qt::Key_Apostrophe)) {
        // Call duplicateUpperCell when Ctrl-' is pressed (this is used by spreadsheets for "Copy Formula from Cell Above")
        duplicateUpperCell();
    } else if(event->key() == Qt::Key_Tab && hasFocus() &&
              selectedIndexes().count() == 1 &&
              selectedIndexes().at(0).row() == model()->rowCount()-1 && selectedIndexes().at(0).column() == model()->columnCount()-1) {
        // If the Tab key was pressed while the focus was on the last cell of the last row insert a new row automatically
        model()->insertRow(model()->rowCount());
    } else if ((event->key() == Qt::Key_Delete) || (event->key() == Qt::Key_Backspace)) {
        // Check if entire rows are selected. We call the selectedRows() method here not only for simplicity reasons but also because it distinguishes between
        // "an entire row is selected" and "all cells of a row are selected", the former is e.g. the case when the row number is clicked, the latter when all cells
        // are selected manually. This is an important distinction (especially when a table has only one column!) to match the users' expectations. Also never
        // delete records when the backspace key was pressed.
        if(event->key() == Qt::Key_Delete && selectionModel()->selectedRows().size())
        {
            // At least on entire row is selected. Because we don't allow completely arbitrary selections (at least at the moment) but only block selections,
            // this means that only entire entire rows are selected. If an entire row is (or multiple entire rows are) selected, we delete that record instead
            // of deleting only the cell contents.

            emit selectedRowsToBeDeleted();
        } else {
            // No entire row is selected. So just set the selected cells to null or empty string depending on the modifier keys

            if(event->modifiers().testFlag(Qt::AltModifier))
            {
                // When pressing Alt+Delete set the value to NULL
                setToNull(selectedIndexes());
            } else {
                // When pressing Delete only set the value to empty string
                for(const QModelIndex& index : selectedIndexes())
                {
                    if(!model()->setData(index, ""))
                        return;
                }
            }
        }
    } else if(event->modifiers().testFlag(Qt::ControlModifier) && (event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown)) {
        // When pressing Ctrl + Page up/down send a signal indicating the user wants to change the current table
        emit switchTable(event->key() == Qt::Key_PageDown);
        return;
    }

    // Discoverability of Ctrl+Shift+Click to follow foreign keys or URLs
    if(event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier))
        QApplication::setOverrideCursor(Qt::PointingHandCursor);

    // This prevents the current selection from being changed when pressing tab to move to the next filter. Note that this is in an 'if' condition,
    // not in an 'else if' because this way, when the tab key was pressed and the focus was on the last cell, a new row is inserted and then the tab
    // key press is processed a second time to move the cursor as well
    if((event->key() != Qt::Key_Tab && event->key() != Qt::Key_Backtab) || hasFocus())
        QTableView::keyPressEvent(event);
}

void ExtendedTableWidget::keyReleaseEvent(QKeyEvent* event)
{

    // Restore the PointingHandCursor override
    if(!event->modifiers().testFlag(Qt::ControlModifier) || !event->modifiers().testFlag(Qt::ShiftModifier))
        QApplication::restoreOverrideCursor();

    QTableView::keyReleaseEvent(event);
}

void ExtendedTableWidget::updateGeometries()
{
    // Call the parent implementation first - it does most of the actual logic
    QTableView::updateGeometries();

    // Update frozen columns view too
    if(m_frozen_table_view)
        m_frozen_table_view->updateGeometries();

    // Check if a model has already been set yet
    if(model())
    {
        // If so and if it is a SqliteTableModel and if the parent implementation of this method decided that a scrollbar is needed, update its maximum value
        SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
        if(m && verticalScrollBar()->maximum())
        {
            verticalScrollBar()->setMaximum(m->rowCount() - numVisibleRows() + 1);
            if(m_frozen_table_view)
                m_frozen_table_view->verticalScrollBar()->setMaximum(verticalScrollBar()->maximum());
        }
    }
}

void ExtendedTableWidget::vscrollbarChanged(int value)
{
    // Cancel if there is no model set yet - this shouldn't happen (because without a model there should be no scrollbar) but just to be sure...
    if(!model())
        return;

    // Fetch more data from the DB if necessary
    const auto nrows = model()->rowCount();
    if(nrows == 0)
        return;

    if(auto * m = dynamic_cast<SqliteTableModel*>(model()))
    {
        int row_begin = std::min(value, nrows - 1);
        int row_end = std::min(value + numVisibleRows(), nrows);
        m->triggerCacheLoad(row_begin, row_end);
    }
}

int ExtendedTableWidget::numVisibleRows() const
{
    if(!isVisible() || !model() || !verticalHeader())
        return 0;

    // Get the row numbers of the rows currently visible at the top and the bottom of the widget
    int row_top = rowAt(0) == -1 ? 0 : rowAt(0);
    int row_bottom = verticalHeader()->visualIndexAt(height()) == -1 ? model()->rowCount() : (verticalHeader()->visualIndexAt(height()) - 1);
    if(horizontalScrollBar()->isVisible())      // Assume the scrollbar covers about one row
        row_bottom--;

    // Calculate the number of visible rows
    return row_bottom - row_top;
}

std::unordered_set<size_t> ExtendedTableWidget::selectedCols() const
{
    std::unordered_set<size_t> selectedCols;
    for(const auto& idx : selectionModel()->selectedColumns())
        selectedCols.insert(static_cast<size_t>(idx.column()));
    return selectedCols;
}

std::unordered_set<size_t> ExtendedTableWidget::colsInSelection() const
{
    std::unordered_set<size_t> colsInSelection;
    for(const QModelIndex & idx : selectedIndexes())
        colsInSelection.insert(static_cast<size_t>(idx.column()));
    return colsInSelection;
}

std::set<size_t> ExtendedTableWidget::rowsInSelection() const
{
    std::set<size_t> rowsInSelection;
    for(const QModelIndex & idx : selectedIndexes())
        rowsInSelection.insert(static_cast<size_t>(idx.row()));
    return rowsInSelection;
}

void ExtendedTableWidget::cellClicked(const QModelIndex& index)
{
    // If Ctrl-Shift is pressed try to jump to the row referenced by the foreign key of the clicked cell
    if(qApp->keyboardModifiers().testFlag(Qt::ControlModifier) && qApp->keyboardModifiers().testFlag(Qt::ShiftModifier) && model())
    {
        SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
        auto fk = m->getForeignKeyClause(static_cast<size_t>(index.column()-1));

        if(fk)
            emit foreignKeyClicked(sqlb::ObjectIdentifier(m->currentTableName().schema(), fk->table()),
                                   fk->columns().size() ? fk->columns().at(0) : "",
                                   m->data(index, Qt::EditRole).toByteArray());
        else {
            // If this column does not have a foreign-key, try to interpret it as a filename/URL and open it in external application.

            // TODO: Qt is doing a contiguous selection when Control+Click is pressed. It should be disabled, but at least moving the
            // current index gives better result.
            setCurrentIndex(index);
            emit requestUrlOrFileOpen(model()->data(index, Qt::EditRole).toString());
        }
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

void ExtendedTableWidget::selectTableLine(int lineToSelect)
{
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // Are there even that many lines?
    if(lineToSelect >= m->rowCount())
        return;

    QApplication::setOverrideCursor( Qt::WaitCursor );
    m->triggerCacheLoad(lineToSelect);

    // Select it
    clearSelection();
    selectRow(lineToSelect);
    scrollTo(currentIndex(), QAbstractItemView::PositionAtTop);
    QApplication::restoreOverrideCursor();
}

void ExtendedTableWidget::selectTableLines(int firstLine, int count)
{
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    int lastLine = firstLine+count-1;
    // Are there even that many lines?
    if(lastLine >= m->rowCount())
        return;

    selectTableLine(firstLine);

    QModelIndex topLeft = m->index(firstLine, 0);
    QModelIndex bottomRight = m->index(lastLine, m->columnCount()-1);

    selectionModel()->select(QItemSelection(topLeft, bottomRight), QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void ExtendedTableWidget::selectAll()
{
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());

    // Fetch all the data if needed and user accepts, then call parent's selectAll()

    QMessageBox::StandardButton answer = QMessageBox::Yes;

    // If we can fetch more data, ask user if they are sure about it.
    if (!m->isCacheComplete()) {

      answer = QMessageBox::question(this, QApplication::applicationName(),
                              tr("<p>Not all data has been loaded. <b>Do you want to load all data before selecting all the rows?</b><p>"
                                 "<p>Answering <b>No</b> means that no more data will be loaded and the selection will not be performed.<br/>"
                                 "Answering <b>Yes</b> might take some time while the data is loaded but the selection will be complete.</p>"
                                 "Warning: Loading all the data might require a great amount of memory for big tables."),
                                     QMessageBox::Yes | QMessageBox::No);

      if (answer == QMessageBox::Yes)
          m->completeCache();
    }
    if (answer == QMessageBox::Yes)
        QTableView::selectAll();
}

void ExtendedTableWidget::openPrintDialog()
{
    QMimeData *mimeData = new QMimeData;
    QModelIndexList indices;

    // Print the selection, if active, or the entire table otherwise.
    // Given that simply clicking over a cell, selects it, one-cell selections are ignored.
    if (selectionModel()->hasSelection() && selectionModel()->selectedIndexes().count() > 1)
        indices = selectionModel()->selectedIndexes();
    else
        for (int row=0; row < model()->rowCount(); row++)
            for (int column=0; column < model()->columnCount(); column++)
                indices << model()->index(row, column);

    // Copy the specified indices content to mimeData for getting the HTML representation of
    // the table with headers. We can then print it using an HTML text document.
    copyMimeData(indices, mimeData, true, false);

    QPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    connect(dialog, &QPrintPreviewDialog::paintRequested, this, [mimeData](QPrinter *previewPrinter) {
        QTextDocument document;
        document.setHtml(mimeData->html());
        document.print(previewPrinter);
    });

    dialog->exec();

    delete dialog;
    delete mimeData;
}

void ExtendedTableWidget::sortByColumns(const std::vector<sqlb::OrderBy>& columns)
{
    // Are there even any columns to sort by?
    if(columns.size() == 0)
        return;

    // We only support sorting for SqliteTableModels with support for multiple and named sort columns
    SqliteTableModel* sqlite_model = dynamic_cast<SqliteTableModel*>(model());
    if(sqlite_model)
        sqlite_model->sort(columns);
}

void ExtendedTableWidget::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    emit currentIndexChanged(current, previous);
}

void ExtendedTableWidget::setToNull(const QModelIndexList& indices)
{
    SqliteTableModel* m = qobject_cast<SqliteTableModel*>(model());
    sqlb::TablePtr currentTable = m->db().getTableByName(m->currentTableName());

    // Check if some column in the selection has a NOT NULL constraint, before trying to update the cells.
    if(currentTable) {
        for(const QModelIndex& index : indices) {
            // Do not process rowid column
            if(index.column() != 0) {
                const size_t indexField = static_cast<size_t>(index.column()-1);
                const sqlb::Field& field = currentTable->fields.at(indexField);
                if(field.notnull()) {
                    QMessageBox::warning(nullptr, qApp->applicationName(),
                                         tr("Cannot set selection to NULL. Column %1 has a NOT NULL constraint.").
                                         arg(QString::fromStdString(field.name())));
                    return;
                }
            }
        }
    }
    for(const QModelIndex& index : indices) {
        // Update aborting in case of any error (to avoid repetitive errors like "Database is locked")
        if(!model()->setData(index, QVariant()))
            return;
    }
}

void ExtendedTableWidget::setFrozenColumns(size_t count)
{
    if(!m_frozen_table_view)
        return;

    m_frozen_column_count = count;

    // Set up frozen table view widget
    m_frozen_table_view->setModel(model());
    m_frozen_table_view->setSelectionModel(selectionModel());
    for(int i=0;i<model()->columnCount();i++)
        m_frozen_table_view->setItemDelegateForColumn(i, nullptr);
    m_frozen_table_view->setItemDelegateForColumn(static_cast<int>(m_frozen_column_count-1), m_item_border_delegate);

    // Only show frozen columns in extra table view and copy column widths
    m_frozen_table_view->horizontalHeader()->blockSignals(true);    // Signals need to be blocked because hiding a column would emit resizedSection
    for(size_t col=0;col<static_cast<size_t>(model()->columnCount());++col)
        m_frozen_table_view->setColumnHidden(static_cast<int>(col), col >= count);
    m_frozen_table_view->horizontalHeader()->blockSignals(false);
    for(int col=0;col<static_cast<int>(count);++col)
        m_frozen_table_view->setColumnWidth(col, columnWidth(col));

    updateFrozenTableGeometry();

    // Only show extra table view when there are frozen columns to see
    if(count)
        m_frozen_table_view->show();
    else
        m_frozen_table_view->hide();
}

void ExtendedTableWidget::generateFilters(size_t number, bool show_rowid)
{
    m_tableHeader->generateFilters(number, m_frozen_column_count);

    if(m_frozen_table_view)
    {
        size_t frozen_columns = std::min(m_frozen_column_count, number);
        m_frozen_table_view->m_tableHeader->generateFilters(frozen_columns, show_rowid ? 0 : 1);
    }
}

void ExtendedTableWidget::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    if(!m_frozen_table_view)
        return;

    if(logicalIndex < static_cast<int>(m_frozen_column_count))
    {
        m_frozen_table_view->setColumnWidth(logicalIndex, newSize);
        setColumnWidth(logicalIndex, newSize);
        updateFrozenTableGeometry();
    }
}

void ExtendedTableWidget::updateSectionHeight(int logicalIndex, int /* oldSize */, int newSize)
{
    if(!m_frozen_table_view)
        return;

    m_frozen_table_view->setRowHeight(logicalIndex, newSize);
}

void ExtendedTableWidget::resizeEvent(QResizeEvent* event)
{
      QTableView::resizeEvent(event);
      updateFrozenTableGeometry();
}

QModelIndex ExtendedTableWidget::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);
    if(!m_frozen_table_view)
        return current;

    int width = 0;
    for(int i=0;i<static_cast<int>(m_frozen_column_count);i++)
        width += m_frozen_table_view->columnWidth(i);

    if(cursorAction == MoveLeft && current.column() > 0 && visualRect(current).topLeft().x() < width)
    {
        const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x() - width;
        horizontalScrollBar()->setValue(newValue);
    }
    return current;
}

void ExtendedTableWidget::scrollTo(const QModelIndex& index, ScrollHint hint)
{
    if(index.column() >= static_cast<int>(m_frozen_column_count))
        QTableView::scrollTo(index, hint);
}

void ExtendedTableWidget::updateFrozenTableGeometry()
{
    if(!m_frozen_table_view)
        return;

    int width = 0;
    for(int i=0;i<static_cast<int>(m_frozen_column_count);i++)
    {
        if(!isColumnHidden(i))
            width += columnWidth(i);
    }

    m_frozen_table_view->setGeometry(verticalHeader()->width() + frameWidth(),
                                     frameWidth(),
                                     width,
                                     viewport()->height() + horizontalHeader()->height());
}

void ExtendedTableWidget::setEditTriggers(QAbstractItemView::EditTriggers editTriggers)
{
    QTableView::setEditTriggers(editTriggers);
    if(m_frozen_table_view)
        m_frozen_table_view->setEditTriggers(editTriggers);
}

void ExtendedTableWidget::setFilter(size_t column, const QString& value)
{
    filterHeader()->setFilter(column, value);
    if(m_frozen_table_view)
        m_frozen_table_view->filterHeader()->setFilter(column, value);
}
