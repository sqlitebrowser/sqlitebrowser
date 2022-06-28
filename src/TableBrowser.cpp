#include "AddRecordDialog.h"
#include "Application.h"
#include "ColumnDisplayFormatDialog.h"
#include "CondFormatManager.h"
#include "Data.h"
#include "DbStructureModel.h"
#include "ExportDataDialog.h"
#include "FilterTableHeader.h"
#include "TableBrowser.h"
#include "Settings.h"
#include "sqlitedb.h"
#include "sqlitetablemodel.h"
#include "ui_TableBrowser.h"

#include <QClipboard>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QScrollBar>
#include <QShortcut>
#include <QTextCodec>
#include <QColorDialog>

std::map<sqlb::ObjectIdentifier, BrowseDataTableSettings> TableBrowser::m_settings;
QString TableBrowser::m_defaultEncoding;

TableBrowser::TableBrowser(DBBrowserDB* _db, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TableBrowser),
    gotoValidator(new QIntValidator(0, 0, this)),
    db(_db),
    dbStructureModel(nullptr),
    m_model(nullptr),
    m_adjustRows(false),
    m_columnsResized(false)
{
    ui->setupUi(this);

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set custom placeholder text for global filter field and disable conditional formats
    ui->editGlobalFilter->setPlaceholderText(tr("Filter in any column"));
    ui->editGlobalFilter->setConditionFormatContextMenuEnabled(false);

    // Set up popup menus
    popupNewRecordMenu = new QMenu(this);
    popupNewRecordMenu->addAction(ui->newRecordAction);
    popupNewRecordMenu->addAction(ui->insertValuesAction);
    ui->actionNewRecord->setMenu(popupNewRecordMenu);

    popupSaveFilterAsMenu = new QMenu(this);
    popupSaveFilterAsMenu->addAction(ui->actionFilteredTableExportCsv);
    popupSaveFilterAsMenu->addAction(ui->actionFilteredTableExportJson);
    popupSaveFilterAsMenu->addAction(ui->actionFilterSaveAsView);
    ui->actionSaveFilterAsPopup->setMenu(popupSaveFilterAsMenu);
    qobject_cast<QToolButton*>(ui->browseToolbar->widgetForAction(ui->actionSaveFilterAsPopup))->setPopupMode(QToolButton::InstantPopup);

    popupHeaderMenu = new QMenu(this);
    popupHeaderMenu->addAction(ui->actionShowRowidColumn);
    popupHeaderMenu->addAction(ui->actionFreezeColumns);
    popupHeaderMenu->addAction(ui->actionHideColumns);
    popupHeaderMenu->addAction(ui->actionShowAllColumns);
    popupHeaderMenu->addAction(ui->actionSelectColumn);
    popupHeaderMenu->addSeparator();
    popupHeaderMenu->addAction(ui->actionUnlockViewEditing);
    popupHeaderMenu->addAction(ui->actionBrowseTableEditDisplayFormat);
    popupHeaderMenu->addSeparator();
    popupHeaderMenu->addAction(ui->actionSetTableEncoding);
    popupHeaderMenu->addAction(ui->actionSetAllTablesEncoding);
    popupHeaderMenu->addSeparator();
    popupHeaderMenu->addAction(ui->actionCopyColumnName);

    connect(ui->actionSelectColumn, &QAction::triggered, this, [this]() {
        ui->dataTable->selectColumn(ui->actionBrowseTableEditDisplayFormat->property("clicked_column").toInt());
    });
    connect(ui->actionFreezeColumns, &QAction::triggered, this, [this](bool checked) {
        if(checked)
            freezeColumns(ui->actionBrowseTableEditDisplayFormat->property("clicked_column").toUInt() + 1);
        else
            freezeColumns(0);
    });

    // Set up shortcuts
    QShortcut* dittoRecordShortcut = new QShortcut(QKeySequence("Ctrl+\""), this);
    connect(dittoRecordShortcut, &QShortcut::activated, this, [this]() {
        int currentRow = ui->dataTable->currentIndex().row();
        duplicateRecord(currentRow);
    });

    // Lambda function for keyboard shortcuts for selecting next/previous table in Browse Data tab
    connect(ui->dataTable, &ExtendedTableWidget::switchTable, this, [this](bool next) {
        int index = ui->comboBrowseTable->currentIndex();
        int num_items = ui->comboBrowseTable->count();
        if(next)
        {
            if(++index >= num_items)
                index = 0;
        } else {
            if(--index < 0)
                index = num_items - 1;
        }
        ui->comboBrowseTable->setCurrentIndex(index);
        refresh();
    });

    // This is a workaround needed for QDarkStyleSheet.
    // See https://github.com/ColinDuquesnoy/QDarkStyleSheet/issues/169
    QStyledItemDelegate* styledItemDelegate = new QStyledItemDelegate(ui->comboBrowseTable);
    ui->comboBrowseTable->setItemDelegate(styledItemDelegate);

    // Add the documentation of shortcuts, which aren't otherwise visible in the user interface, to some buttons.
    addShortcutsTooltip(ui->actionRefresh, {QKeySequence(tr("Ctrl+R"))});
    addShortcutsTooltip(ui->actionPrintTable);

    // Set up filters
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::filterChanged, this, &TableBrowser::updateFilter);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::filterFocused, this, [this]() {
        emit prepareForFilter();
    });
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::addCondFormat, this, &TableBrowser::addCondFormatFromFilter);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::allCondFormatsCleared, this, &TableBrowser::clearAllCondFormats);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::condFormatsEdited, this, &TableBrowser::editCondFormats);
    connect(ui->dataTable, &ExtendedTableWidget::editCondFormats, this, &TableBrowser::editCondFormats);
    connect(ui->dataTable, &ExtendedTableWidget::dataAboutToBeEdited, this, &TableBrowser::dataAboutToBeEdited);

    // Set up global filter
    connect(ui->editGlobalFilter, &FilterLineEdit::filterFocused, this, [this]() {
        emit prepareForFilter();
    });
    connect(ui->editGlobalFilter, &FilterLineEdit::delayedTextChanged, this, [this](const QString& value) {
        // Split up filter values
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
        QStringList values = value.trimmed().split(" ", QString::SkipEmptyParts);
#else
        QStringList values = value.trimmed().split(" ", Qt::SkipEmptyParts);
#endif
        std::vector<QString> filters;
        std::copy(values.begin(), values.end(), std::back_inserter(filters));

        ui->actionClearFilters->setEnabled(m_model->filterCount() > 0 || !ui->editGlobalFilter->text().isEmpty());

        // Have they changed?
        BrowseDataTableSettings& settings = m_settings[currentlyBrowsedTableName()];
        if(filters != settings.globalFilters)
        {
            // Set global filters
            m_model->updateGlobalFilter(filters);
            updateRecordsetLabel();

            // Save them
            settings.globalFilters = filters;
            emit projectModified();
        }
    });

    connect(ui->dataTable, &ExtendedTableWidget::doubleClicked, this, &TableBrowser::selectionChangedByDoubleClick);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::sectionClicked, this, &TableBrowser::headerClicked);
    connect(ui->dataTable->filterHeader(), &QHeaderView::sectionDoubleClicked, ui->dataTable, &QTableView::selectColumn);
    connect(ui->dataTable->verticalScrollBar(), &QScrollBar::valueChanged, this, &TableBrowser::updateRecordsetLabel);
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateRecordsetLabel);
    connect(ui->dataTable->verticalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateRecordsetLabel);
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateColumnWidth);
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &TableBrowser::showDataColumnPopupMenu);
    connect(ui->dataTable->verticalHeader(), &QHeaderView::customContextMenuRequested, this, &TableBrowser::showRecordPopupMenu);
    connect(ui->dataTable, &ExtendedTableWidget::openFileFromDropEvent, this, &TableBrowser::requestFileOpen);
    connect(ui->dataTable, &ExtendedTableWidget::selectedRowsToBeDeleted, this, &TableBrowser::deleteRecord);

    connect(ui->dataTable, &ExtendedTableWidget::foreignKeyClicked, this, [this](const sqlb::ObjectIdentifier& table, const std::string& column, const QByteArray& value) {
        // Just select the column that was just clicked instead of selecting an entire range which
        // happens because of the Ctrl and Shift keys.
        ui->dataTable->selectionModel()->select(ui->dataTable->currentIndex(), QItemSelectionModel::ClearAndSelect);

        // Emit the foreign key clicked signal
        emit foreignKeyClicked(table, column, value);
    });

    connect(ui->actionAddDock, &QAction::triggered, this, [this]() {
        emit newDockRequested();
    });
    connect(ui->actionRefresh, &QAction::triggered, this, [this]() {
        db->updateSchema();
        refresh();
    });
    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, [this](const QFont &font) {
        modifyFormat([font](CondFormat& format) { format.setFontFamily(font.family()); });
    });
    connect(ui->fontSizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
        [this](int pointSize) {
        modifyFormat([pointSize](CondFormat& format) { format.setFontPointSize(pointSize); });
    });

    connect(ui->actionFontColor, &QAction::triggered, this, [this]() {
        QColor color = QColorDialog::getColor(QColor(m_model->data(currentIndex(), Qt::ForegroundRole).toString()));
        if(color.isValid())
            modifyFormat([color](CondFormat& format) { format.setForegroundColor(color); });
    });
    connect(ui->actionBackgroundColor, &QAction::triggered, this, [this]() {
        QColor color = QColorDialog::getColor(QColor(m_model->data(currentIndex(), Qt::BackgroundRole).toString()));
        if(color.isValid())
            modifyFormat([color](CondFormat& format) { format.setBackgroundColor(color); });
    });

    connect(ui->actionBold, &QAction::toggled, this, [this](bool checked) {
        modifyFormat([checked](CondFormat& format) { format.setBold(checked); });
    });
    connect(ui->actionItalic, &QAction::toggled, this, [this](bool checked) {
        modifyFormat([checked](CondFormat& format) { format.setItalic(checked); });
    });
    connect(ui->actionUnderline, &QAction::toggled, this, [this](bool checked) {
        modifyFormat([checked](CondFormat& format) { format.setUnderline(checked); });
    });

    connect(ui->actionLeftAlign, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(true);
        ui->actionRightAlign->setChecked(false);
        ui->actionCenter->setChecked(false);
        ui->actionJustify->setChecked(false);
        modifyFormat([](CondFormat& format) { format.setAlignment(CondFormat::AlignLeft); });
    });
    connect(ui->actionRightAlign, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(false);
        ui->actionRightAlign->setChecked(true);
        ui->actionCenter->setChecked(false);
        ui->actionJustify->setChecked(false);
        modifyFormat([](CondFormat& format) { format.setAlignment(CondFormat::AlignRight); });
    });
    connect(ui->actionCenter, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(false);
        ui->actionRightAlign->setChecked(false);
        ui->actionCenter->setChecked(true);
        ui->actionJustify->setChecked(false);
        modifyFormat([](CondFormat& format) { format.setAlignment(CondFormat::AlignCenter); });
    });
    connect(ui->actionJustify, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(false);
        ui->actionRightAlign->setChecked(false);
        ui->actionCenter->setChecked(false);
        ui->actionJustify->setChecked(true);
        modifyFormat([](CondFormat& format) { format.setAlignment(CondFormat::AlignJustify); });
    });

    connect(ui->actionEditCondFormats, &QAction::triggered, this, [this]() { editCondFormats(static_cast<size_t>(currentIndex().column())); });
    connect(ui->actionClearFormat, &QAction::triggered, this, [this]() {
        for (const size_t column : ui->dataTable->selectedCols())
            clearAllCondFormats(column);
        for (const QModelIndex& index : ui->dataTable->selectionModel()->selectedIndexes())
            clearRowIdFormats(index);

    });

    connect(ui->dataTable, &ExtendedTableWidget::currentIndexChanged, this, [this](const QModelIndex &current, const QModelIndex &) {
            // Get cell current format for updating the format toolbar values. Block signals, so the format change is not reapplied.
            QString font_string = m_model->data(current, Qt::FontRole).toString();
            QFont font;
            if(!font_string.isEmpty())
                font.fromString(font_string);

            ui->fontComboBox->blockSignals(true);
            ui->fontComboBox->setCurrentFont(font);
            ui->fontComboBox->blockSignals(false);

            ui->fontSizeBox->blockSignals(true);
            ui->fontSizeBox->setValue(font.pointSize());
            ui->fontSizeBox->blockSignals(false);

            ui->actionBold->blockSignals(true);
            ui->actionBold->setChecked(font.bold());
            ui->actionBold->blockSignals(false);

            ui->actionItalic->blockSignals(true);
            ui->actionItalic->setChecked(font.italic());
            ui->actionItalic->blockSignals(false);

            ui->actionUnderline->blockSignals(true);
            ui->actionUnderline->setChecked(font.underline());
            ui->actionUnderline->blockSignals(false);

            Qt::Alignment align = Qt::Alignment(m_model->data(current, Qt::TextAlignmentRole).toInt());
            ui->actionLeftAlign->blockSignals(true);
            ui->actionLeftAlign->setChecked(align.testFlag(Qt::AlignLeft));
            ui->actionLeftAlign->blockSignals(false);

            ui->actionRightAlign->blockSignals(true);
            ui->actionRightAlign->setChecked(align.testFlag(Qt::AlignRight));
            ui->actionRightAlign->blockSignals(false);

            ui->actionCenter->blockSignals(true);
            ui->actionCenter->setChecked(align.testFlag(Qt::AlignCenter));
            ui->actionCenter->blockSignals(false);

            ui->actionJustify->blockSignals(true);
            ui->actionJustify->setChecked(align.testFlag(Qt::AlignJustify));
            ui->actionJustify->blockSignals(false);
    });

    connect(ui->actionToggleFormatToolbar, &QAction::toggled, ui->formatFrame, &QFrame::setVisible);
    ui->actionToggleFormatToolbar->setChecked(false);
    ui->formatFrame->setVisible(false);

    // Set up find frame
    ui->frameFind->hide();

    QShortcut* shortcutHideFindFrame = new QShortcut(QKeySequence("ESC"), ui->editFindExpression);
    connect(shortcutHideFindFrame, &QShortcut::activated, ui->buttonFindClose, &QToolButton::click);

    QShortcut* shortcutActionFind = new QShortcut(QKeySequence("Ctrl+F"), this, nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
    connect(shortcutActionFind, &QShortcut::activated, ui->actionFind, &QAction::trigger);
    connect(ui->actionFind, &QAction::triggered, this, [this](bool checked) {
       if(checked)
       {
           ui->widgetReplace->hide();
           ui->frameFind->show();
           ui->editFindExpression->setFocus();
           ui->actionReplace->setChecked(false);
       } else {
           ui->buttonFindClose->click();
       }
    });

    QShortcut* shortcutActionReplace = new QShortcut(QKeySequence("Ctrl+H"), this, nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
    connect(shortcutActionReplace, &QShortcut::activated, ui->actionReplace, &QAction::trigger);
    connect(ui->actionReplace, &QAction::triggered, this, [this](bool checked) {
       if(checked)
       {
           ui->widgetReplace->show();
           ui->frameFind->show();
           ui->editFindExpression->setFocus();
           ui->actionFind->setChecked(false);
       } else {
           ui->buttonFindClose->click();
       }
    });

    connect(ui->editFindExpression, &QLineEdit::returnPressed, ui->buttonFindNext, &QToolButton::click);
    connect(ui->editFindExpression, &QLineEdit::textChanged, this, [this]() {
        // When the text has changed but neither Return nor F3 or similar nor any buttons were pressed, we want to include the current
        // cell in the search as well. This makes sure the selected cell does not jump around every time the text is changed but only
        // when the current cell does not match the search expression anymore.
        find(ui->editFindExpression->text(), true, true);
    });
    connect(ui->buttonFindClose, &QToolButton::clicked, this, [this](){
        ui->dataTable->setFocus();
        ui->frameFind->hide();
        ui->actionFind->setChecked(false);
        ui->actionReplace->setChecked(false);
    });
    connect(ui->buttonFindPrevious, &QToolButton::clicked, this, [this](){
        find(ui->editFindExpression->text(), false);
    });
    connect(ui->buttonFindNext, &QToolButton::clicked, this, [this](){
        find(ui->editFindExpression->text(), true);
    });
    connect(ui->buttonReplaceNext, &QToolButton::clicked, this, [this](){
        find(ui->editFindExpression->text(), true, true, ReplaceMode::ReplaceNext);
    });
    connect(ui->buttonReplaceAll, &QToolButton::clicked, this, [this](){
        find(ui->editFindExpression->text(), true, true, ReplaceMode::ReplaceAll);
    });

    // Recreate the model
    if(m_model)
        delete m_model;
    m_model = new SqliteTableModel(*db, this, QString(), true);

    // Connect slots
    connect(m_model, &SqliteTableModel::finishedFetch, this, &TableBrowser::fetchedData);

    // Load initial settings
    reloadSettings();
}

TableBrowser::~TableBrowser()
{
    delete gotoValidator;
    delete ui;
}

void TableBrowser::reset()
{
    // Reset the model
    m_model->reset();

    // Reset the recordset label inside the Browse tab now
    updateRecordsetLabel();

    // Clear filters
    clearFilters();

    // Reset the plot dock model and connection
    emit updatePlot(nullptr, nullptr, nullptr, true);
}

void TableBrowser::resetSharedSettings()
{
    // Remove all stored table information browse data tab
    m_settings.clear();
    m_defaultEncoding = QString();
}

sqlb::ObjectIdentifier TableBrowser::currentlyBrowsedTableName() const
{
    return sqlb::ObjectIdentifier(dbStructureModel->index(ui->comboBrowseTable->currentIndex(),
                                                          DbStructureModel::ColumnSchema,
                                                          ui->comboBrowseTable->rootModelIndex()).data().toString().toStdString(),
                                  ui->comboBrowseTable->currentData(Qt::EditRole).toString().toStdString());  // Use the edit role here to make sure we actually get the
                                                                                                              // table name without the schema bit in front of it.
}

BrowseDataTableSettings& TableBrowser::settings(const sqlb::ObjectIdentifier& object)
{
    return m_settings[object];
}

void TableBrowser::setSettings(const sqlb::ObjectIdentifier& table, const BrowseDataTableSettings& table_settings)
{
    m_settings[table] = table_settings;
}

void TableBrowser::setStructure(QAbstractItemModel* model, const sqlb::ObjectIdentifier& old_table)
{
    dbStructureModel = model;

    ui->comboBrowseTable->blockSignals(true);
    ui->comboBrowseTable->setModel(model);
    ui->comboBrowseTable->setRootModelIndex(dbStructureModel->index(0, 0)); // Show the 'browsable' section of the db structure tree
    ui->comboBrowseTable->blockSignals(false);

    int old_table_index = ui->comboBrowseTable->findText(QString::fromStdString(old_table.toDisplayString()));
    if(old_table_index == -1 && ui->comboBrowseTable->count())      // If the old table couldn't be found anymore but there is another table, select that
        ui->comboBrowseTable->setCurrentIndex(0);
    else if(old_table_index == -1)                                  // If there aren't any tables to be selected anymore, clear the table view
        clear();
    else                                                            // Under normal circumstances just select the old table again
        ui->comboBrowseTable->setCurrentIndex(old_table_index);

    emit currentTableChanged(currentlyBrowsedTableName());
}

QModelIndex TableBrowser::currentIndex() const
{
    return ui->dataTable->currentIndex();
}

void TableBrowser::setEnabled(bool enable)
{
    ui->browseToolbar->setEnabled(enable);
    ui->editGlobalFilter->setEnabled(enable);
    ui->formatFrame->setEnabled(enable);
    ui->frameFind->setEnabled(enable);

    ui->buttonNext->setEnabled(enable);
    ui->buttonPrevious->setEnabled(enable);
    ui->buttonBegin->setEnabled(enable);
    ui->buttonEnd->setEnabled(enable);
    ui->buttonGoto->setEnabled(enable);
    ui->editGoto->setEnabled(enable);

    updateInsertDeleteRecordButton();
}

void TableBrowser::refresh()
{
    // If the list of table names is empty, i.e. there are no tables to browse, clear the view
    // to make sure any left over data is removed and do not add any new information.
    if(ui->comboBrowseTable->model()->rowCount(ui->comboBrowseTable->rootModelIndex()) == 0)
    {
        clear();
        return;
    }

    // Reset the minimum width of the vertical header which could have been modified in updateFilter
    // or in headerClicked.
    ui->dataTable->verticalHeader()->setMinimumWidth(0);

    // Reset the model if it was cleared before
    // Because setting a new model creates a new selection mode, reconnect the slots to its signals.
    if(ui->dataTable->model() == nullptr)
    {
        ui->dataTable->setModel(m_model);
        connect(ui->dataTable->selectionModel(), &QItemSelectionModel::currentChanged, this, &TableBrowser::selectionChanged);
        connect(ui->dataTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](const QItemSelection&, const QItemSelection&) {
            updateInsertDeleteRecordButton();

            const QModelIndexList& sel = ui->dataTable->selectionModel()->selectedIndexes();
            QString statusMessage;
            if (sel.count() > 1) {
                int rows = static_cast<int>(ui->dataTable->rowsInSelection().size());
                statusMessage = tr("%n row(s)", "", rows);
                int columns = static_cast<int>(ui->dataTable->colsInSelection().size());
                statusMessage += tr(", %n column(s)", "", columns);

                if (sel.count() < Settings::getValue("databrowser", "complete_threshold").toInt()) {
                    double sum = 0;
                    double first = m_model->data(sel.first(), Qt::EditRole).toDouble();
                    double min = first;
                    double max = first;
                    for (const QModelIndex& index : sel) {
                        double dblData = m_model->data(index, Qt::EditRole).toDouble();
                        sum += dblData;
                        min = std::min(min, dblData);
                        max = std::max(max, dblData);
                    }
                    statusMessage += tr(". Sum: %1; Average: %2; Min: %3; Max: %4").arg(sum).arg(sum/sel.count()).arg(min).arg(max);
                }
            }
            emit statusMessageRequested(statusMessage);
        });
    }

    // Retrieve the stored data for this table if there is any. If there are no settings for this table,
    // this will insert and return a settings object with default values.
    const sqlb::ObjectIdentifier tablename = currentlyBrowsedTableName();
    const BrowseDataTableSettings& storedData = m_settings[tablename];

    // Set column widths to match contents when no column widths were provided in the stored settings.
    // This needs to be done before setting the query because the data is returned asynchronously and
    // so this information can be needed any time.
    if(storedData.columnWidths.empty())
        m_columnsResized = false;

    // Current table changed
    emit currentTableChanged(tablename);

    // Build query and apply settings
   applyModelSettings(storedData, buildQuery(storedData, tablename));
   applyViewportSettings(storedData, tablename);
   updateRecordsetLabel();
   emit updatePlot(ui->dataTable, m_model, &m_settings[tablename], true);
}

void TableBrowser::clearFilters()
{
    ui->dataTable->filterHeader()->clearFilters();
    ui->editGlobalFilter->clear();
}

void TableBrowser::reloadSettings()
{
    ui->dataTable->reloadSettings();

    ui->browseToolbar->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(Settings::getValue("General", "toolbarStyleBrowse").toInt()));
    m_model->reloadSettings();
}

void TableBrowser::setCurrentTable(const sqlb::ObjectIdentifier& name)
{
    int index = ui->comboBrowseTable->findText(QString::fromStdString(name.toDisplayString()));
    if(index != -1)
        ui->comboBrowseTable->setCurrentIndex(index);
}

void TableBrowser::clear()
{
    // This function unset the model in the data view. So if no model is set at the moment,
    // this indicates that this function has already been called and does not need to
    // executed another time.
    if (!ui->dataTable->model())
        return;

    // Unset the model
    ui->dataTable->setModel(nullptr);

    // Remove any filters, not just the values but the fields too
    if(qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader()))
        qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(0);
    ui->editGlobalFilter->clear();
}

void TableBrowser::updateFilter(size_t column, const QString& value)
{
    // Set minimum width to the vertical header in order to avoid flickering while a filter is being updated.
    ui->dataTable->verticalHeader()->setMinimumWidth(ui->dataTable->verticalHeader()->width());

    // Update the filter in the model and its query
    const std::string column_name = model()->headerData(static_cast<int>(column), Qt::Horizontal, Qt::EditRole).toString().toStdString();
    m_model->updateFilter(column_name, value);

    ui->actionClearFilters->setEnabled(m_model->filterCount() > 0 || !ui->editGlobalFilter->text().isEmpty());

    // Save the new filter settings
    BrowseDataTableSettings& settings = m_settings[currentlyBrowsedTableName()];
    if(value.isEmpty())
    {
        if(settings.filterValues.erase(column_name) > 0)
            emit projectModified();
    } else {
        if (settings.filterValues[column_name] != value) {
            settings.filterValues[column_name] = value;
            emit projectModified();
        }
    }
}

void TableBrowser::addCondFormatFromFilter(size_t column, const QString& value)
{
    QFont font = QFont(Settings::getValue("databrowser", "font").toString());
    font.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());

    // Create automatically a new conditional format with the next serial background color according to the theme and the regular foreground
    // color and font in the settings.
    CondFormat newCondFormat(value, QColor(Settings::getValue("databrowser", "reg_fg_colour").toString()),
                             m_condFormatPalette.nextSerialColor(Palette::appHasDarkTheme()),
                             font,
                             CondFormat::AlignLeft,
                             m_model->encoding());
    addCondFormat(false, column, newCondFormat);
}

void TableBrowser::addCondFormat(bool isRowIdFormat, size_t column, const CondFormat& newCondFormat)
{
    BrowseDataTableSettings& settings = m_settings[currentlyBrowsedTableName()];
    std::vector<CondFormat>& formats = isRowIdFormat ? settings.rowIdFormats[column] : settings.condFormats[column];
    m_model->addCondFormat(isRowIdFormat, column, newCondFormat);

    // Conditionless formats are pushed back and others inserted at the beginning, so they aren't eclipsed.
    if (newCondFormat.filter().isEmpty())
        formats.push_back(newCondFormat);
    else
        formats.insert(formats.begin(), newCondFormat);

    emit projectModified();
}

void TableBrowser::clearAllCondFormats(size_t column)
{
    m_model->setCondFormats(false, column, {});
    m_settings[currentlyBrowsedTableName()].condFormats[column].clear();
    emit projectModified();
}

void TableBrowser::clearRowIdFormats(const QModelIndex index)
{
    std::vector<CondFormat>& rowIdFormats = m_settings[currentlyBrowsedTableName()].rowIdFormats[static_cast<size_t>(index.column())];
    rowIdFormats.erase(std::remove_if(rowIdFormats.begin(), rowIdFormats.end(), [&](const CondFormat& format) {
            return format.filter() == QString("=%1").arg(m_model->data(index.sibling(index.row(), 0)).toString());
            }), rowIdFormats.end());
    m_model->setCondFormats(true, static_cast<size_t>(index.column()), rowIdFormats);

    emit projectModified();
}

void TableBrowser::editCondFormats(size_t column)
{
    CondFormatManager condFormatDialog(m_settings[currentlyBrowsedTableName()].condFormats[column],
                                       m_model->encoding(), this);
    condFormatDialog.setWindowTitle(tr("Conditional formats for \"%1\"").
                                    arg(m_model->headerData(static_cast<int>(column), Qt::Horizontal, Qt::EditRole).toString()));
    if (condFormatDialog.exec()) {
        std::vector<CondFormat> condFormatVector = condFormatDialog.getCondFormats();
        m_model->setCondFormats(false, column, condFormatVector);
        m_settings[currentlyBrowsedTableName()].condFormats[column] = condFormatVector;
        emit projectModified();
    }
}
void TableBrowser::modifySingleFormat(const bool isRowIdFormat, const QString& filter, const QModelIndex refIndex, std::function<void(CondFormat&)> changeFunction)
{
    const size_t column = static_cast<size_t>(refIndex.column());
    BrowseDataTableSettings& settings = m_settings[currentlyBrowsedTableName()];
    std::vector<CondFormat>& formats = isRowIdFormat ? settings.rowIdFormats[column] : settings.condFormats[column];
    auto it = std::find_if(formats.begin(), formats.end(), [&filter](const CondFormat& format) {
            return format.filter() == filter;
        });
    if(it != formats.end()) {
        changeFunction(*it);
        m_model->addCondFormat(isRowIdFormat, column, *it);
    } else {
        // Create a new conditional format based on the current reference index and then modify it as requested using the passed function.
        CondFormat newCondFormat(filter, m_model, refIndex, m_model->encoding());
        changeFunction(newCondFormat);
        addCondFormat(isRowIdFormat, column, newCondFormat);
    }
}

void TableBrowser::modifyFormat(std::function<void(CondFormat&)> changeFunction)
{
    // Modify the conditional formats from entirely selected columns having the current filter value,
    // or modify the row-id formats of selected cells, when the selection does not cover entire columns.
    const std::unordered_set<size_t>& columns = ui->dataTable->selectedCols();
    if (columns.size() > 0) {
        for (size_t column : columns) {
            QString filter;
            const std::string column_name = model()->headerData(static_cast<int>(column), Qt::Horizontal, Qt::EditRole).toString().toStdString();
            if (m_settings[currentlyBrowsedTableName()].filterValues.count(column_name) > 0)
                filter = m_settings[currentlyBrowsedTableName()].filterValues.at(column_name);
            modifySingleFormat(false, filter, currentIndex().sibling(currentIndex().row(), static_cast<int>(column)), changeFunction);
        }
    } else {
        for(const QModelIndex& index : ui->dataTable->selectionModel()->selectedIndexes()) {
            const QString filter = QString("=%1").arg(m_model->data(index.sibling(index.row(), 0)).toString());
            modifySingleFormat(true, filter, index, changeFunction);
        }
    }
    emit projectModified();
}

void TableBrowser::updateRecordsetLabel()
{
    // Get all the numbers, i.e. the number of the first row and the last row as well as the total number of rows
    int from = ui->dataTable->verticalHeader()->visualIndexAt(0) + 1;
    int total = m_model->rowCount();
    int to = from + ui->dataTable->numVisibleRows() - 1;
    if(to < 0)
            to = 0;

    // Adjust visible rows to contents if necessary, and then take the new visible rows, which might have changed.
    if(m_adjustRows) {
        for(int i=from; i<=to; i++)
            ui->dataTable->resizeRowToContents(i);
        from = ui->dataTable->verticalHeader()->visualIndexAt(0) + 1;
        to = from + ui->dataTable->numVisibleRows() - 1;
    }

    // Update the validator of the goto row field
    gotoValidator->setRange(0, total);

    // When there is no query for this table (i.e. no table is selected), there is no row count query either which in turn means
    // that the row count query will never finish. And because of this the row count will be forever unknown. To avoid always showing
    // a misleading "determining row count" text in the UI we set the row count status to complete here for empty queries.
    auto row_count_available = m_model->rowCountAvailable();
    if(m_model->query().empty())
        row_count_available = SqliteTableModel::RowCount::Complete;

    // Update the label showing the current position
    QString txt;
    switch(row_count_available)
    {
    case SqliteTableModel::RowCount::Unknown:
        txt = tr("determining row count...");
        break;
    case SqliteTableModel::RowCount::Partial:
        txt = tr("%1 - %2 of >= %3").arg(from).arg(to).arg(total);
        break;
    case SqliteTableModel::RowCount::Complete:
        txt = tr("%1 - %2 of %3").arg(from).arg(to).arg(total);
        break;
    }
    ui->labelRecordset->setText(txt);

    // Enable editing only for tables or views with editing unlocked for which the row count is already available
    bool is_table_or_unlocked_view = false;
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    if(!current_table.isEmpty() && !m_model->query().empty())
    {
        auto obj = db->getTableByName(current_table);
        is_table_or_unlocked_view = obj && (
                    (obj->isView() && m_model->hasPseudoPk()) ||
                    (!obj->isView()));
    }
    enableEditing(m_model->rowCountAvailable() != SqliteTableModel::RowCount::Unknown && is_table_or_unlocked_view);

    // Show filters unless the table is empty
    const bool needs_filters = total > 0 || m_model->filterCount() > 0;
    FilterTableHeader* header = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
    if(header) {
        if(needs_filters && !header->hasFilters()) {
            generateFilters();
            ui->dataTable->adjustSize();
        } else if(!needs_filters && header->hasFilters()) {
            ui->dataTable->generateFilters(0, false);
        }
    }
}

sqlb::Query TableBrowser::buildQuery(const BrowseDataTableSettings& storedData, const sqlb::ObjectIdentifier& tablename) const
{
    const auto table = db->getTableByName(tablename);

    sqlb::Query query(tablename, table && table->isView());

    // Construct a query from the retrieved settings

    // Sorting
    query.setOrderBy(storedData.sortColumns);
    ui->actionClearSorting->setEnabled(storedData.sortColumns.size() > 0);

    // Filters
    for(auto it=storedData.filterValues.cbegin();it!=storedData.filterValues.cend();++it)
        query.where().insert({it->first, CondFormat::filterToSqlCondition(it->second, m_model->encoding())});

    // Global filter
    for(const auto& f : storedData.globalFilters)
        query.globalWhere().push_back(CondFormat::filterToSqlCondition(f, m_model->encoding()));

    // Display formats
    bool only_defaults = true;
    if(table)
    {
        // When there is at least one custom display format, we have to set all columns for the query explicitly here
        for(size_t i=0; i<table->fields.size(); ++i)
        {
            QString format = contains(storedData.displayFormats, i+1) ? storedData.displayFormats.at(i+1) : QString();
            if(format.size())
            {
                query.selectedColumns().emplace_back(table->fields.at(i).name(), format.toStdString());
                only_defaults = false;
            } else {
                query.selectedColumns().emplace_back(table->fields.at(i).name(), table->fields.at(i).name());
            }
        }
    }
    if(only_defaults)
        query.selectedColumns().clear();

    // Unlock view editing
    query.setRowIdColumn(storedData.unlockViewPk.toStdString());

    return query;
}

void TableBrowser::applyModelSettings(const BrowseDataTableSettings& storedData, const sqlb::Query& query)
{
    // Set query which also resets the model
    m_model->setQuery(query);

    // Regular conditional formats
    for(auto formatIt=storedData.condFormats.cbegin(); formatIt!=storedData.condFormats.cend(); ++formatIt)
        m_model->setCondFormats(false, formatIt->first, formatIt->second);

    // Row Id formats
    for(auto formatIt=storedData.rowIdFormats.cbegin(); formatIt!=storedData.rowIdFormats.cend(); ++formatIt)
        m_model->setCondFormats(true, formatIt->first, formatIt->second);

    // Encoding
    m_model->setEncoding(storedData.encoding);
}

void TableBrowser::applyViewportSettings(const BrowseDataTableSettings& storedData, const sqlb::ObjectIdentifier& tablename)
{
    // Show rowid column. This also takes care of the filters.
    showRowidColumn(storedData.showRowid);

    // Enable editing in general and (un)lock view editing depending on the settings
    unlockViewEditing(!storedData.unlockViewPk.isEmpty() && storedData.unlockViewPk != "_rowid_", storedData.unlockViewPk);

    // Column hidden status
    showAllColumns();
    for(auto hiddenIt=storedData.hiddenColumns.cbegin();hiddenIt!=storedData.hiddenColumns.cend();++hiddenIt)
        hideColumns(hiddenIt->first, hiddenIt->second);

    // Column widths
    std::map<int, int> w = storedData.columnWidths;         // We're working with a copy here because the map can get modified in the process
    for(auto widthIt=w.cbegin();widthIt!=w.cend();++widthIt)
    {
        if(widthIt->first < ui->dataTable->model()->columnCount())
            ui->dataTable->setColumnWidth(widthIt->first, widthIt->second);
    }
    m_columnsResized = !w.empty();

    // Global filters
    QString text;
    for(const auto& f : storedData.globalFilters)
        text += f + " ";
    text.chop(1);
    ui->editGlobalFilter->setText(text);

    // Show/hide some menu options depending on whether this is a table or a view
    const auto table = db->getTableByName(tablename);
    if(!table->isView())
    {
        // Table
        ui->actionUnlockViewEditing->setVisible(false);
        ui->actionShowRowidColumn->setVisible(!table->withoutRowidTable());
    } else {
        // View
        ui->actionUnlockViewEditing->setVisible(true);
        ui->actionShowRowidColumn->setVisible(false);
    }

    // Frozen columns
    freezeColumns(storedData.frozenColumns);
}

void TableBrowser::enableEditing(bool enable_edit)
{
    // Don't enable anything if this is a read only database
    bool edit = enable_edit && !db->readOnly();

    // Apply settings
    ui->dataTable->setEditTriggers(edit ? QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);
    updateInsertDeleteRecordButton();
}

void TableBrowser::showRowidColumn(bool show)
{
    // Disconnect the resized signal from the horizontal header. Otherwise it's resetting the automatic column widths
    disconnect(ui->dataTable->horizontalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateColumnWidth);

    // WORKAROUND
    // Set the opposite hidden/visible status of what we actually want for the rowid column. This is to work around a Qt bug which
    // is present in at least version 5.7.1. The problem is this: when you browse a table/view with n columns, then switch to a table/view
    // with less than n columns, you'll be able to resize the first (hidden!) column by resizing the section to the left of the first visible
    // column. By doing so the table view gets messed up. But even when not resizing the first hidden column, tab-ing through the fields
    // will stop at the not-so-much-hidden rowid column, too. All this can be fixed by this line. I haven't managed to find another workaround
    // or way to fix this yet.
    ui->dataTable->setColumnHidden(0, show);

    // Show/hide rowid column
    ui->dataTable->setColumnHidden(0, !show);
    if(show)
        ui->dataTable->setColumnWidth(0, ui->dataTable->horizontalHeader()->defaultSectionSize());

    // Update checked status of the popup menu action
    ui->actionShowRowidColumn->setChecked(show);

    // Save settings for this table
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    if (m_settings[current_table].showRowid != show) {
        emit projectModified();
        m_settings[current_table].showRowid = show;
    }

    // Update the filter row
    generateFilters();

    // Re-enable signal
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateColumnWidth);

    ui->dataTable->update();
}

void TableBrowser::freezeColumns(size_t columns)
{
    // Update checked status of the popup menu action
    ui->actionFreezeColumns->setChecked(columns != 0);

    // Save settings for this table
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    if (m_settings[current_table].frozenColumns != columns) {
        emit projectModified();
        m_settings[current_table].frozenColumns = columns;
    }

    // Apply settings
    ui->dataTable->horizontalHeader()->blockSignals(true);
    ui->dataTable->setFrozenColumns(columns);
    generateFilters();
    ui->dataTable->horizontalHeader()->blockSignals(false);
}

void TableBrowser::generateFilters()
{
    // Generate a new row of filter line edits
    const auto& settings = m_settings[currentlyBrowsedTableName()];
    ui->dataTable->generateFilters(static_cast<size_t>(m_model->columnCount()), settings.showRowid);

    // Apply the stored filter strings to the new row of line edits
    // Set filters blocking signals for this since the filter is already applied to the browse table model
    FilterTableHeader* filterHeader = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
    bool oldState = filterHeader->blockSignals(true);
    auto obj = db->getTableByName(currentlyBrowsedTableName());
    for(auto filterIt=settings.filterValues.cbegin();filterIt!=settings.filterValues.cend();++filterIt)
        ui->dataTable->setFilter(sqlb::getFieldNumber(obj, filterIt->first) + 1, filterIt->second);
    filterHeader->blockSignals(oldState);

    ui->actionClearFilters->setEnabled(m_model->filterCount() > 0 || !ui->editGlobalFilter->text().isEmpty());
}

void TableBrowser::unlockViewEditing(bool unlock, QString pk)
{
    sqlb::ObjectIdentifier currentTable = currentlyBrowsedTableName();
    sqlb::TablePtr view = db->getTableByName(currentTable);

    // If this isn't a view just unlock editing and return
    if(!view)
    {
        m_model->setPseudoPk(m_model->pseudoPk());
        enableEditing(true);
        return;
    }

    // If the view gets unlocked for editing and we don't have a 'primary key' for this view yet, then ask for one
    if(unlock && pk.isEmpty())
    {
        while(true)
        {
            bool ok;

            QStringList options;
            for(const auto& n : view->fieldNames())
                options.push_back(QString::fromStdString(n));

            // Ask for a PK
            pk = QInputDialog::getItem(this,
                                       qApp->applicationName(),
                                       tr("Please enter a pseudo-primary key in order to enable editing on this view. "
                                          "This should be the name of a unique column in the view."),
                                       options,
                                       0,
                                       false,
                                       &ok);

            // Cancelled?
            if(!ok || pk.isEmpty()) {
                ui->actionUnlockViewEditing->setChecked(false);
                return;
            }

            // Do some basic testing of the input and if the input appears to be good, go on
            if(db->executeSQL("SELECT " + sqlb::escapeIdentifier(pk.toStdString()) + " FROM " + currentTable.toString() + " LIMIT 1;", false, true))
                break;
        }
    } else if(!unlock) {
        // Locking the view is done by unsetting the pseudo-primary key
        pk = "_rowid_";
    }

    // (De)activate editing
    enableEditing(unlock);
    m_model->setPseudoPk({pk.toStdString()});

    // Update checked status of the popup menu action
    ui->actionUnlockViewEditing->blockSignals(true);
    ui->actionUnlockViewEditing->setChecked(unlock);
    ui->actionUnlockViewEditing->blockSignals(false);

    // If the settings didn't change, do not try to reapply them.
    // This avoids an infinite mutual recursion.
    BrowseDataTableSettings& settings = m_settings[currentTable];

    if(settings.unlockViewPk != pk) {
        // Save settings for this table
        settings.unlockViewPk = pk;

        // The filter row might move up one row when we change the query, so we need to regenerate it here
        generateFilters();

        emit projectModified();
    }
}

void TableBrowser::hideColumns(int column, bool hide)
{
    sqlb::ObjectIdentifier tableName = currentlyBrowsedTableName();

    // Select columns to (un)hide
    std::unordered_set<int> columns;
    if(column == -1)
    {
         if(ui->dataTable->selectedCols().size() == 0)
             columns.insert(ui->actionBrowseTableEditDisplayFormat->property("clicked_column").toInt());
         else {
             auto cols = ui->dataTable->selectedCols();
             columns.insert(cols.begin(), cols.end());
         }
    } else {
        columns.insert(column);
    }

    // (Un)hide requested column(s)
    for(int col : columns)
    {
        if(col >= ui->dataTable->model()->columnCount())
            continue;

        ui->dataTable->setColumnHidden(col, hide);
        if(!hide)
            ui->dataTable->setColumnWidth(col, ui->dataTable->horizontalHeader()->defaultSectionSize());
        m_settings[tableName].hiddenColumns[col] = hide;
    }

    // check to see if all the columns are hidden
    bool allHidden = true;
    for(int col = 1; col < ui->dataTable->model()->columnCount(); col++)
    {
        if(!ui->dataTable->isColumnHidden(col))
        {
            allHidden = false;
            break;
        }
    }

    if(allHidden  && ui->dataTable->model()->columnCount() > 1)
        hideColumns(1, false);
    emit projectModified();
}

void TableBrowser::showAllColumns()
{
    for(int col = 1; col < ui->dataTable->model()->columnCount(); col++)
    {
        if(ui->dataTable->isColumnHidden(col))
            hideColumns(col, false);
    }
}

void TableBrowser::updateInsertDeleteRecordButton()
{
    // Update the delete record button to reflect number of selected records

    int rows = 0;

    // If there is no model yet (because e.g. no database file is opened) there is no selection model either. So we need to check for that here
    // in order to avoid null pointer dereferences. If no selection model exists we will just continue as if no row is selected because without a
    // model you could argue there actually is no row to be selected.
    if(ui->dataTable->selectionModel())
    {
        const auto & sel = ui->dataTable->selectionModel()->selectedIndexes();
        if(sel.count())
            rows = sel.last().row() - sel.first().row() + 1;
    }

    // Enable the insert and delete buttons only if the currently browsed table or view is editable. For the delete button we additionally require
    // at least one row to be selected. For the insert button there is an extra rule to disable it when we are browsing a view because inserting
    // into a view isn't supported yet.
    bool isEditable = m_model->isEditable() && !db->readOnly();
    ui->actionNewRecord->setEnabled(isEditable);
    ui->actionDeleteRecord->setEnabled(isEditable && rows != 0);

    if(rows > 1)
        ui->actionDeleteRecord->setText(tr("Delete Records"));
    else
        ui->actionDeleteRecord->setText(tr("Delete Record"));
}

void TableBrowser::duplicateRecord(int currentRow)
{
    auto row = m_model->dittoRecord(currentRow);
    if (row.isValid())
        ui->dataTable->setCurrentIndex(row);
    else
        QMessageBox::warning(this, qApp->applicationName(), db->lastError());
}

void TableBrowser::headerClicked(int logicalindex)
{
    BrowseDataTableSettings& settings = m_settings[currentlyBrowsedTableName()];

    // Set minimum width to the vertical header in order to avoid flickering when sorting.
    ui->dataTable->verticalHeader()->setMinimumWidth(ui->dataTable->verticalHeader()->width());

    // Get the current list of sort columns
    auto& columns = settings.sortColumns;

    // Get the name of the column the user clicked on
    std::string column = model()->headerData(logicalindex, Qt::Horizontal, Qt::EditRole).toString().toStdString();

    // Before sorting, first check if the Control key is pressed. If it is, we want to append this column to the list of sort columns. If it is not,
    // we want to sort only by the new column.
    if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
    {
        // Multi column sorting

        // If the column was control+clicked again, change its sort order.
        // If not already in the sort order, add the column as a new sort column to the list.
        bool present = false;
        for(sqlb::OrderBy& sortedCol : columns) {
            if(sortedCol.expr == column) {
                sortedCol.direction = (sortedCol.direction == sqlb::OrderBy::Ascending ? sqlb::OrderBy::Descending : sqlb::OrderBy::Ascending);
                present = true;
                break;
            }
        }
        if(!present)
            columns.emplace_back(column, sqlb::OrderBy::Ascending);
    } else {
        // Single column sorting

        // If we have exactly one sort column and it is the column which was just clicked, change its sort order.
        // If not, clear the list of sorting columns and replace it by a single new sort column.
        if(columns.size() == 1 && columns.front().expr == column)
        {
            columns.front().direction = (columns.front().direction == sqlb::OrderBy::Ascending ? sqlb::OrderBy::Descending : sqlb::OrderBy::Ascending);
        } else {
            columns.clear();
            columns.emplace_back(column, sqlb::OrderBy::Ascending);
        }
    }

    // Do the actual sorting
    ui->dataTable->sortByColumns(columns);

    ui->actionClearSorting->setEnabled(columns.size() > 0);

    // select the first item in the column so the header is bold
    // we might try to select the last selected item
    ui->dataTable->setCurrentIndex(ui->dataTable->currentIndex().sibling(0, logicalindex));

    emit updatePlot(ui->dataTable, m_model, &m_settings[currentlyBrowsedTableName()], true);

    emit projectModified();
}

void TableBrowser::updateColumnWidth(int section, int /*old_size*/, int new_size)
{
    std::unordered_set<size_t> selectedCols = ui->dataTable->selectedCols();
    sqlb::ObjectIdentifier tableName = currentlyBrowsedTableName();

    if (selectedCols.find(static_cast<size_t>(section)) == selectedCols.end())
    {
        if (m_settings[tableName].columnWidths[section] != new_size) {
            emit projectModified();
            m_settings[tableName].columnWidths[section] = new_size;
        }
    }
    else
    {
        ui->dataTable->blockSignals(true);
        for(size_t col : selectedCols)
        {
            ui->dataTable->setColumnWidth(static_cast<int>(col), new_size);
            if (m_settings[tableName].columnWidths[static_cast<int>(col)] != new_size) {
                emit projectModified();
                m_settings[tableName].columnWidths[static_cast<int>(col)] = new_size;
            }
        }
        ui->dataTable->blockSignals(false);
    }
}

void TableBrowser::showDataColumnPopupMenu(const QPoint& pos)
{
    // Get the index of the column which the user has clicked on and store it in the action. This is sort of hack-ish and it might be the heat in my room
    // but I haven't come up with a better solution so far
    int logical_index = ui->dataTable->horizontalHeader()->logicalIndexAt(pos);
    if(logical_index == -1)     // Don't open the popup menu if the user hasn't clicked on a column header
            return;
    ui->actionBrowseTableEditDisplayFormat->setProperty("clicked_column", logical_index);

    // Calculate the proper position for the context menu and display it
    popupHeaderMenu->exec(ui->dataTable->horizontalHeader()->mapToGlobal(pos));
}

void TableBrowser::showRecordPopupMenu(const QPoint& pos)
{
    int row = ui->dataTable->verticalHeader()->logicalIndexAt(pos);
    if (row == -1)
        return;

    QMenu popupRecordMenu(this);

    // "Delete and duplicate records" can only be done on writable objects
    if(db->getTableByName(currentlyBrowsedTableName()) && !db->readOnly()) {

        // Select the row if it is not already in the selection.
        QModelIndexList rowList = ui->dataTable->selectionModel()->selectedRows();
        bool found = false;
        for (const QModelIndex& index : qAsConst(rowList)) {
            if (row == index.row()) {
                found = true;
                break;
            }
        }
        if (!found)
            ui->dataTable->selectRow(row);

        rowList = ui->dataTable->selectionModel()->selectedRows();

        QString duplicateText = rowList.count() > 1 ? tr("Duplicate records") : tr("Duplicate record");

        QAction* action = new QAction(duplicateText, &popupRecordMenu);
        // Set shortcut for documentation purposes (the actual functional shortcut is not set here)
        action->setShortcut(QKeySequence(tr("Ctrl+\"")));
        popupRecordMenu.addAction(action);

        connect(action, &QAction::triggered, this, [rowList, this]() {
            for (const QModelIndex& index : rowList)
                duplicateRecord(index.row());
        });

        QAction* deleteRecordAction = new QAction(QIcon(":icons/delete_record"), ui->actionDeleteRecord->text(), &popupRecordMenu);
        popupRecordMenu.addAction(deleteRecordAction);

        connect(deleteRecordAction, &QAction::triggered, this, [&]() {
            deleteRecord();
        });

        popupRecordMenu.addSeparator();
    }

    // "Adjust rows" can be done on any object
    QAction* adjustRowHeightAction = new QAction(tr("Adjust rows to contents"), &popupRecordMenu);
    adjustRowHeightAction->setCheckable(true);
    adjustRowHeightAction->setChecked(m_adjustRows);
    popupRecordMenu.addAction(adjustRowHeightAction);

    connect(adjustRowHeightAction, &QAction::toggled, this, [&](bool checked) {
        m_adjustRows = checked;
        refresh();
    });

    popupRecordMenu.exec(ui->dataTable->verticalHeader()->mapToGlobal(pos));
}

void TableBrowser::addRecord()
{
    int row = m_model->rowCount();

    // If table has pseudo_pk, then it must be an editable view. Jump straight to inserting by pop-up dialog.
    if(!m_model->hasPseudoPk() && m_model->insertRow(row))
    {
        selectTableLine(row);
    } else {
        // Error inserting empty row.
        // User has to provide values acomplishing the constraints. Open Add Record Dialog.
        insertValues();
    }
    updateRecordsetLabel();
}

void TableBrowser::insertValues()
{
    std::vector<std::string> pseudo_pk = m_model->hasPseudoPk() ? m_model->pseudoPk() : std::vector<std::string>();
    AddRecordDialog dialog(*db, currentlyBrowsedTableName(), this, pseudo_pk);
    if (dialog.exec())
        refresh();
}

void TableBrowser::deleteRecord()
{
    if(ui->dataTable->selectionModel()->hasSelection())
    {
        // If only filter header is selected
        if(ui->dataTable->selectionModel()->selectedIndexes().isEmpty())
            return;

        while(ui->dataTable->selectionModel()->hasSelection())
        {
            std::set<size_t> row_set = ui->dataTable->rowsInSelection();
            int first_selected_row = static_cast<int>(*row_set.begin());
            int rows_to_remove = 0;
            int previous_row = first_selected_row - 1;

            // Non-contiguous selection: remove only the contiguous
            // rows in the selection in each cycle until the entire
            // selection has been removed.
            for(size_t row : row_set) {
                if(previous_row == static_cast<int>(row - 1))
                    rows_to_remove++;
                else
                    break;
            }

            if(!m_model->removeRows(first_selected_row, rows_to_remove))
            {
                QMessageBox::warning(this, QApplication::applicationName(), tr("Error deleting record:\n%1").arg(db->lastError()));
                break;
            }
        }
    } else {
        QMessageBox::information( this, QApplication::applicationName(), tr("Please select a record first"));
    }
    updateRecordsetLabel();
}

void TableBrowser::navigatePrevious()
{
    int curRow = ui->dataTable->currentIndex().row();
    curRow -= ui->dataTable->numVisibleRows() - 1;
    if(curRow < 0)
        curRow = 0;
    selectTableLine(curRow);
}

void TableBrowser::navigateNext()
{
    int curRow = ui->dataTable->currentIndex().row();
    curRow += ui->dataTable->numVisibleRows() - 1;
    if(curRow >= m_model->rowCount())
        curRow = m_model->rowCount() - 1;
    selectTableLine(curRow);
}

void TableBrowser::navigateBegin()
{
    selectTableLine(0);
}

void TableBrowser::navigateEnd()
{
    selectTableLine(m_model->rowCount()-1);
}

void TableBrowser::navigateGoto()
{
    int row = ui->editGoto->text().toInt();
    if(row <= 0)
        row = 1;
    if(row > m_model->rowCount())
        row = m_model->rowCount();

    selectTableLine(row - 1);
    ui->editGoto->setText(QString::number(row));
}

void TableBrowser::selectTableLine(int lineToSelect)
{
    ui->dataTable->selectTableLine(lineToSelect);
}

void TableBrowser::clearSorting()
{
    // Get the current list of sort columns
    auto& columns = m_settings[currentlyBrowsedTableName()].sortColumns;
    columns.clear();
    // Set cleared vector of sort-by columns
    m_model->sort(columns);

    ui->actionClearSorting->setEnabled(false);
}

void TableBrowser::editDisplayFormat()
{
    // Get the current table name and fetch its table object, then retrieve the fields of that table and look up the index of the clicked table header
    // section using it as the table field array index. Subtract one from the header index to get the column index because in the the first (though hidden)
    // column is always the rowid column. Ultimately, get the column name from the column object
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    size_t field_number = sender()->property("clicked_column").toUInt();
    QString field_name = QString::fromStdString(db->getTableByName(current_table)->fields.at(field_number-1).name());

    // Get the current display format of the field
    QString current_displayformat = m_settings[current_table].displayFormats[field_number];

    // Open the dialog
    ColumnDisplayFormatDialog dialog(*db, current_table, field_name, current_displayformat, this);
    if(dialog.exec())
    {
        // Set the newly selected display format
        QString new_format = dialog.selectedDisplayFormat();
        if(new_format.size())
            m_settings[current_table].displayFormats[field_number] = new_format;
        else
            m_settings[current_table].displayFormats.erase(field_number);
        emit projectModified();

        // Refresh view
        refresh();
    }
}

void TableBrowser::exportCsvFilteredTable()
{
    ExportDataDialog dialog(*db, ExportDataDialog::ExportFormatCsv, this, m_model->customQuery(false));
    dialog.exec();
}

void TableBrowser::exportJsonFilteredTable()
{
    ExportDataDialog dialog(*db, ExportDataDialog::ExportFormatJson, this, m_model->customQuery(false));
    dialog.exec();
}

void TableBrowser::saveFilterAsView()
{
    // Save as view a custom query without rowid
    emit createView(m_model->customQuery(false));
}

void TableBrowser::setTableEncoding(bool forAllTables)
{
    // Get the old encoding
    QString encoding = m_model->encoding();

    // Ask the user for a new encoding
    bool ok;
    QString question;
    QStringList availableCodecs = toStringList(QTextCodec::availableCodecs());
    availableCodecs.removeDuplicates();
    int currentItem = availableCodecs.indexOf(encoding);

    if(forAllTables)
        question = tr("Please choose a new encoding for all tables.");
    else
        question = tr("Please choose a new encoding for this table.");
    encoding = QInputDialog::getItem(this,
                                     tr("Set encoding"),
                                     tr("%1\nLeave the field empty for using the database encoding.").arg(question),
                                     availableCodecs,
                                     currentItem,
                                     true, // editable
                                     &ok);

    // Only set the new encoding if the user clicked the OK button
    if(ok)
    {
        // Check if encoding is valid
        if(!encoding.isEmpty() && !QTextCodec::codecForName(encoding.toUtf8()))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("This encoding is either not valid or not supported."));
            return;
        }

        // Set encoding for current table
        m_model->setEncoding(encoding);

        // Save encoding for this table
        m_settings[currentlyBrowsedTableName()].encoding = encoding;

        // Set default encoding if requested to and change all stored table encodings
        if(forAllTables)
        {
            m_defaultEncoding = encoding;

            for(auto it=m_settings.begin();it!=m_settings.end();++it)
                it->second.encoding = encoding;
        }

        emit projectModified();
    }
}

void TableBrowser::setDefaultTableEncoding()
{
    setTableEncoding(true);
}

void TableBrowser::copyColumnName(){
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    int col_index = ui->actionBrowseTableEditDisplayFormat->property("clicked_column").toInt();
    QString field_name = QString::fromStdString(db->getTableByName(current_table)->fields.at(col_index - 1).name());

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(field_name);
}

void TableBrowser::jumpToRow(const sqlb::ObjectIdentifier& table, std::string column, const QByteArray& value)
{
    // First check if table exists
    sqlb::TablePtr obj = db->getTableByName(table);
    if(!obj)
        return;

    // If no column name is set, assume the primary key is meant
    if(!column.size())
        column = obj->primaryKeyColumns().front().name();

    // If column doesn't exist don't do anything
    auto column_it = sqlb::findField(obj, column);
    if(column_it == obj->fields.end())
        return;

    // Jump to table
    setCurrentTable(table);

    // Set filter
    m_settings[table].filterValues[column_it->name()] = value;
    refresh();
}

static QString replaceInValue(QString value, const QString& find, const QString& replace, Qt::MatchFlags flags)
{
    // Helper function which replaces a string in another string by a third string. It uses regular expressions if told so.
    if(flags.testFlag(Qt::MatchRegExp))
    {
        QRegularExpression reg_exp(find, (flags.testFlag(Qt::MatchCaseSensitive) ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption));
        if(!flags.testFlag(Qt::MatchContains))
        {
#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
            reg_exp.setPattern("\\A(" + reg_exp.pattern() + ")\\Z");
#else
            reg_exp.setPattern(QRegularExpression::anchoredPattern(reg_exp.pattern()));
#endif
        }

        return value.replace(reg_exp, replace);
    } else {
        return value.replace(find, replace, flags.testFlag(Qt::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive);
    }
}

void TableBrowser::find(const QString& expr, bool forward, bool include_first, ReplaceMode replace)
{
    // Reset the colour of the line edit, assuming there is no error.
    ui->editFindExpression->setStyleSheet("");

    // You are not allowed to search for an empty string
    if(expr.isEmpty())
        return;

    // Get the cell from which the search should be started. If there is a selected cell, use that. If there is no selected cell, start at the first cell.
    QModelIndex start;
    if(ui->dataTable->selectionModel()->hasSelection())
        start = ui->dataTable->selectionModel()->selectedIndexes().front();
    else
        start = m_model->index(0, 0);

    // Prepare the match flags with all the search settings
    Qt::MatchFlags flags = Qt::MatchWrap;

    if(ui->checkFindCaseSensitive->isChecked())
        flags |= Qt::MatchCaseSensitive;

    if(ui->checkFindWholeCell->isChecked())
        flags |= Qt::MatchFixedString;
    else
        flags |= Qt::MatchContains;

    if(ui->checkFindRegEx->isChecked())
        flags |= Qt::MatchRegExp;

    // Prepare list of columns to search in. We only search in non-hidden rows
    std::vector<int> column_list;
    sqlb::ObjectIdentifier tableName = currentlyBrowsedTableName();
    if(m_settings[tableName].showRowid)
        column_list.push_back(0);
    for(int i=1;i<m_model->columnCount();i++)
    {
        if(contains(m_settings[tableName].hiddenColumns, i) == false)
            column_list.push_back(i);
        else if(m_settings[tableName].hiddenColumns[i] == false)
            column_list.push_back(i);
    }

    // Are we only searching for text or are we supposed to replace text?
    switch(replace)
    {
    case ReplaceMode::NoReplace: {
        // Perform the actual search using the model class
        const auto match = m_model->nextMatch(start, column_list, expr, flags, !forward, include_first);

        // Select the next match if we found one
        if(match.isValid())
            ui->dataTable->setCurrentIndex(match);

        // Make the expression control red if no results were found
        if(!match.isValid())
            ui->editFindExpression->setStyleSheet("QLineEdit {color: white; background-color: rgb(255, 102, 102)}");
    } break;
    case ReplaceMode::ReplaceNext: {
        // Find the next match
        const auto match = m_model->nextMatch(start, column_list, expr, flags, !forward, include_first);

        // If there was a match, perform the replacement on the cell and select it
        if(match.isValid())
        {
            m_model->setData(match, replaceInValue(match.data(Qt::EditRole).toString(), expr, ui->editReplaceExpression->text(), flags));
            ui->dataTable->setCurrentIndex(match);
        }

        // Make the expression control red if no results were found
        if(!match.isValid())
            ui->editFindExpression->setStyleSheet("QLineEdit {color: white; background-color: rgb(255, 102, 102)}");
    } break;
    case ReplaceMode::ReplaceAll: {
        // Find all matches
        std::set<QModelIndex> all_matches;
        while(true)
        {
            // Find the next match
            const auto match = m_model->nextMatch(start, column_list, expr, flags, !forward, include_first);

            // If there was a match, perform the replacement and continue from that position. If there was no match, stop looking for other matches.
            // Additionally, keep track of all the matches so far in order to avoid running over them again indefinitely, e.g. when replacing "1" by "10".
            if(match.isValid() && all_matches.find(match) == all_matches.end())
            {
                all_matches.insert(match);
                m_model->setData(match, replaceInValue(match.data(Qt::EditRole).toString(), expr, ui->editReplaceExpression->text(), flags));

                // Start searching from the last match onwards in order to not search through the same cells over and over again.
                start = match;
                include_first = false;
            } else {
                break;
            }
        }

        // Make the expression control red if no results were found
        if(!all_matches.empty())
            QMessageBox::information(this, qApp->applicationName(), tr("%1 replacement(s) made.").arg(all_matches.size()));
        else
            ui->editFindExpression->setStyleSheet("QLineEdit {color: white; background-color: rgb(255, 102, 102)}");
    } break;
    }
}

void TableBrowser::fetchedData()
{
    updateRecordsetLabel();

    // Don't resize the columns more than once to fit their contents. This is necessary because the finishedFetch signal of the model
    // is emitted for each loaded prefetch block and we want to avoid column resizes while scrolling down.
    if(m_columnsResized)
        return;
    m_columnsResized = true;

    // Set column widths according to their contents but make sure they don't exceed a certain size
    ui->dataTable->resizeColumnsToContents();
    for(int i = 0; i < m_model->columnCount(); i++)
    {
        if(ui->dataTable->columnWidth(i) > 300)
            ui->dataTable->setColumnWidth(i, 300);
    }
}
