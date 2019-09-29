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

#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QScrollBar>
#include <QShortcut>
#include <QTextCodec>
#include <QColorDialog>

QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> TableBrowser::browseTableSettings;
QString TableBrowser::defaultBrowseTableEncoding;

TableBrowser::TableBrowser(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TableBrowser),
    gotoValidator(new QIntValidator(0, 0, this)),
    db(nullptr),
    dbStructureModel(nullptr),
    m_browseTableModel(nullptr)
{
    ui->setupUi(this);

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set custom placeholder text for global filter field and disable conditional formats
    ui->editGlobalFilter->setPlaceholderText(tr("Filter in all columns"));
    ui->editGlobalFilter->setConditionFormatContextMenuEnabled(false);

    // Set uo popup menus
    popupNewRecordMenu = new QMenu(this);
    popupNewRecordMenu->addAction(ui->newRecordAction);
    popupNewRecordMenu->addAction(ui->insertValuesAction);
    ui->actionNewRecord->setMenu(popupNewRecordMenu);

    popupSaveFilterAsMenu = new QMenu(this);
    popupSaveFilterAsMenu->addAction(ui->actionFilteredTableExportCsv);
    popupSaveFilterAsMenu->addAction(ui->actionFilterSaveAsView);
    ui->actionSaveFilterAsPopup->setMenu(popupSaveFilterAsMenu);
    qobject_cast<QToolButton*>(ui->browseToolbar->widgetForAction(ui->actionSaveFilterAsPopup))->setPopupMode(QToolButton::InstantPopup);

    popupBrowseDataHeaderMenu = new QMenu(this);
    popupBrowseDataHeaderMenu->addAction(ui->actionShowRowidColumn);
    popupBrowseDataHeaderMenu->addAction(ui->actionHideColumns);
    popupBrowseDataHeaderMenu->addAction(ui->actionShowAllColumns);
    popupBrowseDataHeaderMenu->addSeparator();
    popupBrowseDataHeaderMenu->addAction(ui->actionUnlockViewEditing);
    popupBrowseDataHeaderMenu->addAction(ui->actionBrowseTableEditDisplayFormat);
    popupBrowseDataHeaderMenu->addSeparator();
    popupBrowseDataHeaderMenu->addAction(ui->actionSetTableEncoding);
    popupBrowseDataHeaderMenu->addAction(ui->actionSetAllTablesEncoding);

    // Set up shortcuts
    QShortcut* dittoRecordShortcut = new QShortcut(QKeySequence("Ctrl+\""), this);
    connect(dittoRecordShortcut, &QShortcut::activated, [this]() {
        int currentRow = ui->dataTable->currentIndex().row();
        duplicateRecord(currentRow);
    });

    // Lambda function for keyboard shortcuts for selecting next/previous table in Browse Data tab
    connect(ui->dataTable, &ExtendedTableWidget::switchTable, [this](bool next) {
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
        updateTable();
    });

    // Add the documentation of shortcuts, which aren't otherwise visible in the user interface, to some buttons.
    addShortcutsTooltip(ui->actionRefresh, {QKeySequence(tr("Ctrl+R"))});
    addShortcutsTooltip(ui->actionPrintTable);

    // Set up filters
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::filterChanged, this, &TableBrowser::updateFilter);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::addCondFormat, this, &TableBrowser::addCondFormat);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::allCondFormatsCleared, this, &TableBrowser::clearAllCondFormats);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::condFormatsEdited, this, &TableBrowser::editCondFormats);
    connect(ui->dataTable, &ExtendedTableWidget::editCondFormats, this, &TableBrowser::editCondFormats);

    // Set up global filter
    connect(ui->editGlobalFilter, &FilterLineEdit::delayedTextChanged, this, [this](const QString& value) {
        // Split up filter values
        QStringList values = value.trimmed().split(" ", QString::SkipEmptyParts);
        std::vector<QString> filters;
        for(const auto& s : values)
            filters.push_back(s);

        // Have they changed?
        BrowseDataTableSettings& settings = browseTableSettings[currentlyBrowsedTableName()];
        if(filters != settings.globalFilters)
        {
            // Set global filters
            m_browseTableModel->updateGlobalFilter(filters);
            updateRecordsetLabel();

            // Save them
            settings.globalFilters = filters;
            emit projectModified();
        }
    });

    connect(ui->dataTable, &ExtendedTableWidget::doubleClicked, this, &TableBrowser::selectionChangedByDoubleClick);
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::sectionClicked, this, &TableBrowser::browseTableHeaderClicked);
    connect(ui->dataTable->filterHeader(), &QHeaderView::sectionDoubleClicked, ui->dataTable, &QTableView::selectColumn);
    connect(ui->dataTable->verticalScrollBar(), &QScrollBar::valueChanged, this, &TableBrowser::updateRecordsetLabel);
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateBrowseDataColumnWidth);
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &TableBrowser::showDataColumnPopupMenu);
    connect(ui->dataTable->verticalHeader(), &QHeaderView::customContextMenuRequested, this, &TableBrowser::showRecordPopupMenu);
    connect(ui->dataTable, &ExtendedTableWidget::openFileFromDropEvent, this, &TableBrowser::requestFileOpen);
    connect(ui->dataTable, &ExtendedTableWidget::selectedRowsToBeDeleted, this, &TableBrowser::deleteRecord);

    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, [this](const QFont &font) {
        modifyColumnFormat(ui->dataTable->colsInSelection(), [font](CondFormat& format) { format.setFontFamily(font.family()); });
    });
    connect(ui->fontSizeBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int pointSize) {
        modifyColumnFormat(ui->dataTable->colsInSelection(), [pointSize](CondFormat& format) { format.setFontPointSize(pointSize); });
    });

    connect(ui->actionFontColor, &QAction::triggered, this, [this]() {
        QColor color = QColorDialog::getColor(QColor(m_browseTableModel->data(currentIndex(), Qt::ForegroundRole).toString()));
        if(color.isValid())
            modifyColumnFormat(ui->dataTable->colsInSelection(), [color](CondFormat& format) { format.setForegroundColor(color); });
    });
    connect(ui->actionBackgroundColor, &QAction::triggered, this, [this]() {
        QColor color = QColorDialog::getColor(QColor(m_browseTableModel->data(currentIndex(), Qt::BackgroundRole).toString()));
        if(color.isValid())
            modifyColumnFormat(ui->dataTable->colsInSelection(), [color](CondFormat& format) { format.setBackgroundColor(color); });
    });

    connect(ui->actionBold, &QAction::toggled, this, [this](bool checked) {
        modifyColumnFormat(ui->dataTable->colsInSelection(), [checked](CondFormat& format) { format.setBold(checked); });
    });
    connect(ui->actionItalic, &QAction::toggled, this, [this](bool checked) {
        modifyColumnFormat(ui->dataTable->colsInSelection(), [checked](CondFormat& format) { format.setItalic(checked); });
    });
    connect(ui->actionUnderline, &QAction::toggled, this, [this](bool checked) {
        modifyColumnFormat(ui->dataTable->colsInSelection(), [checked](CondFormat& format) { format.setUnderline(checked); });
    });

    connect(ui->actionLeftAlign, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(true);
        ui->actionRightAlign->setChecked(false);
        ui->actionCenter->setChecked(false);
        ui->actionJustify->setChecked(false);
        modifyColumnFormat(ui->dataTable->colsInSelection(), [](CondFormat& format) { format.setAlignment(CondFormat::AlignLeft); });
    });
    connect(ui->actionRightAlign, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(false);
        ui->actionRightAlign->setChecked(true);
        ui->actionCenter->setChecked(false);
        ui->actionJustify->setChecked(false);
        modifyColumnFormat(ui->dataTable->colsInSelection(), [](CondFormat& format) { format.setAlignment(CondFormat::AlignRight); });
    });
    connect(ui->actionCenter, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(false);
        ui->actionRightAlign->setChecked(false);
        ui->actionCenter->setChecked(true);
        ui->actionJustify->setChecked(false);
        modifyColumnFormat(ui->dataTable->colsInSelection(), [](CondFormat& format) { format.setAlignment(CondFormat::AlignCenter); });
    });
    connect(ui->actionJustify, &QAction::triggered, this, [this]() {
        ui->actionLeftAlign->setChecked(false);
        ui->actionRightAlign->setChecked(false);
        ui->actionCenter->setChecked(false);
        ui->actionJustify->setChecked(true);
        modifyColumnFormat(ui->dataTable->colsInSelection(), [](CondFormat& format) { format.setAlignment(CondFormat::AlignJustify); });
    });

    connect(ui->actionEditCondFormats, &QAction::triggered, this, [this]() { editCondFormats(currentIndex().column()); });
    connect(ui->actionClearFormat, &QAction::triggered, this, [this]() {
        for (int column : ui->dataTable->colsInSelection())
            clearAllCondFormats(column);
    });

    connect(ui->dataTable, &ExtendedTableWidget::currentChanged, this, [this](const QModelIndex &current, const QModelIndex &) {
            // Get cell current format for updating the format toolbar values. Block signals, so the format change is not reapplied.
            QFont font;
            font.fromString(m_browseTableModel->data(current, Qt::FontRole).toString());
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

            Qt::Alignment align = Qt::Alignment(m_browseTableModel->data(current, Qt::TextAlignmentRole).toInt());
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

    connect(ui->actionFind, &QAction::triggered, [this](bool checked) {
       if(checked)
       {
           ui->frameFind->show();
           ui->editFindExpression->setFocus();
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
    });
    connect(ui->buttonFindPrevious, &QToolButton::clicked, this, [this](){
        find(ui->editFindExpression->text(), false);
    });
    connect(ui->buttonFindNext, &QToolButton::clicked, this, [this](){
        find(ui->editFindExpression->text(), true);
    });
}

TableBrowser::~TableBrowser()
{
    delete gotoValidator;
    delete ui;
}

void TableBrowser::init(DBBrowserDB* _db)
{
    db = _db;

    if(m_browseTableModel)
        delete m_browseTableModel;
    m_browseTableModel = new SqliteTableModel(*db, this, static_cast<std::size_t>(Settings::getValue("db", "prefetchsize").toUInt()));

    connect(m_browseTableModel, &SqliteTableModel::finishedFetch, this, &TableBrowser::updateRecordsetLabel);
}

void TableBrowser::reset()
{
    // Reset the model
    if(m_browseTableModel)
        m_browseTableModel->reset();

    // Remove all stored table information browse data tab
    browseTableSettings.clear();
    defaultBrowseTableEncoding = QString();

    // Reset the recordset label inside the Browse tab now
    updateRecordsetLabel();

    // Clear filters
    clearFilters();

    // Reset the plot dock model and connection
    emit updatePlot(nullptr, nullptr, nullptr, true);
}

sqlb::ObjectIdentifier TableBrowser::currentlyBrowsedTableName() const
{
    return sqlb::ObjectIdentifier(ui->comboBrowseTable->model()->data(dbStructureModel->index(ui->comboBrowseTable->currentIndex(),
                                                                                              DbStructureModel::ColumnSchema,
                                                                                              ui->comboBrowseTable->rootModelIndex())).toString().toStdString(),
                                  ui->comboBrowseTable->currentData(Qt::EditRole).toString().toStdString());  // Use the edit role here to make sure we actually get the
                                                                                                              // table name without the schema bit in front of it.
}

BrowseDataTableSettings& TableBrowser::settings(const sqlb::ObjectIdentifier& object)
{
    return browseTableSettings[object];
}

void TableBrowser::setSettings(const sqlb::ObjectIdentifier& table, const BrowseDataTableSettings& table_settings)
{
    browseTableSettings[table] = table_settings;
}

void TableBrowser::setStructure(QAbstractItemModel* model, const QString& old_table)
{
    dbStructureModel = model;
    ui->comboBrowseTable->setModel(model);

    ui->comboBrowseTable->setRootModelIndex(dbStructureModel->index(0, 0)); // Show the 'browsable' section of the db structure tree
    int old_table_index = ui->comboBrowseTable->findText(old_table);
    if(old_table_index == -1 && ui->comboBrowseTable->count())      // If the old table couldn't be found anymore but there is another table, select that
        ui->comboBrowseTable->setCurrentIndex(0);
    else if(old_table_index == -1)                                  // If there aren't any tables to be selected anymore, clear the table view
        clearTableBrowser();
    else                                                            // Under normal circumstances just select the old table again
        ui->comboBrowseTable->setCurrentIndex(old_table_index);
}

QModelIndex TableBrowser::currentIndex() const
{
    return ui->dataTable->currentIndex();
}

void TableBrowser::setEnabled(bool enable)
{
    ui->buttonNext->setEnabled(enable);
    ui->buttonPrevious->setEnabled(enable);
    ui->buttonBegin->setEnabled(enable);
    ui->buttonEnd->setEnabled(enable);
    ui->buttonGoto->setEnabled(enable);
    ui->editGoto->setEnabled(enable);
    ui->actionSaveFilterAsPopup->setEnabled(enable);
    ui->actionClearFilters->setEnabled(enable);
    ui->actionClearSorting->setEnabled(enable);
    ui->actionRefresh->setEnabled(enable);
    ui->actionPrintTable->setEnabled(enable);
    ui->editGlobalFilter->setEnabled(enable);
    ui->actionFind->setEnabled(enable);

    updateInsertDeleteRecordButton();
}

void TableBrowser::updateTable()
{
    // Remove the model-view link if the table name is empty in order to remove any data from the view
    if(ui->comboBrowseTable->model()->rowCount(ui->comboBrowseTable->rootModelIndex()) == 0)
    {
        clearTableBrowser();
        return;
    }

    // Restore default value that could have been modified in updateFilter or browseTableHeaderClicked
    ui->dataTable->verticalHeader()->setMinimumWidth(0);

    // Get current table name
    sqlb::ObjectIdentifier tablename = currentlyBrowsedTableName();

    // Set model
    bool reconnectSelectionSignals = false;
    if(ui->dataTable->model() == nullptr)
        reconnectSelectionSignals = true;
    ui->dataTable->setModel(m_browseTableModel);
    if(reconnectSelectionSignals)
    {
        connect(ui->dataTable->selectionModel(), &QItemSelectionModel::currentChanged, this, &TableBrowser::selectionChanged);
        connect(ui->dataTable->selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection&, const QItemSelection&) {
            updateInsertDeleteRecordButton();

            const QModelIndexList& sel = ui->dataTable->selectionModel()->selectedIndexes();
            QString statusMessage;
            if (sel.count() > 1) {
                int rows = sel.last().row() - sel.first().row() + 1;
                statusMessage = tr("%n row(s)", "", rows);
                int columns = sel.last().column() - sel.first().column() + 1;
                statusMessage += tr(", %n column(s)", "", columns);

                if (sel.count() < Settings::getValue("databrowser", "complete_threshold").toInt()) {
                    double sum = 0;
                    double first = m_browseTableModel->data(sel.first(), Qt::EditRole).toDouble();
                    double min = first;
                    double max = first;
                    for (const QModelIndex& index : sel) {
                        double dblData = m_browseTableModel->data(index, Qt::EditRole).toDouble();
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
    // Search stored table settings for this table
    bool storedDataFound = browseTableSettings.contains(tablename);

    // Set new table
    if(!storedDataFound)
    {
        // No stored settings found.

        // Set table name and apply default display format settings
        m_browseTableModel->setQuery(sqlb::Query(tablename));

        // There aren't any information stored for this table yet, so use some default values

        // Hide rowid column. Needs to be done before the column widths setting because of the workaround in there
        showRowidColumn(false);

        // Unhide all columns by default
        on_actionShowAllColumns_triggered();

        // Enable editing in general, but lock view editing
        unlockViewEditing(false);

        // Column widths
        for(int i=1;i<m_browseTableModel->columnCount();i++)
            ui->dataTable->setColumnWidth(i, ui->dataTable->horizontalHeader()->defaultSectionSize());

        // Encoding
        m_browseTableModel->setEncoding(defaultBrowseTableEncoding);

        // Global filter
        ui->editGlobalFilter->clear();

        updateRecordsetLabel();

        // Plot
        emit updatePlot(ui->dataTable, m_browseTableModel, &browseTableSettings[tablename], true);

        // The filters can be left empty as they are
    } else {
        // Stored settings found. Retrieve them and assemble a query from them.
        BrowseDataTableSettings storedData = browseTableSettings[tablename];
        sqlb::Query query(tablename);

        // Sorting
        query.setOrderBy(storedData.query.orderBy());

        // Filters
        for(auto it=storedData.filterValues.constBegin();it!=storedData.filterValues.constEnd();++it)
            query.where().insert({it.key(), CondFormat::filterToSqlCondition(it.value(), m_browseTableModel->encoding()).toStdString()});

        // Global filter
        for(const auto& f : storedData.globalFilters)
            query.globalWhere().push_back(CondFormat::filterToSqlCondition(f, m_browseTableModel->encoding()).toStdString());

        // Display formats
        bool only_defaults = true;
        if(db->getObjectByName(tablename))
        {
            const sqlb::FieldInfoList& tablefields = db->getObjectByName(tablename)->fieldInformation();
            for(size_t i=0; i<tablefields.size(); ++i)
            {
                QString format = storedData.displayFormats[static_cast<int>(i)+1];
                if(format.size())
                {
                    query.selectedColumns().emplace_back(tablefields.at(i).name, format.toStdString());
                    only_defaults = false;
                } else {
                    query.selectedColumns().emplace_back(tablefields.at(i).name, tablefields.at(i).name);
                }
            }
        }
        if(only_defaults)
            query.selectedColumns().clear();

        // Unlock view editing
        query.setRowIdColumn(storedData.unlockViewPk.toStdString());

        // Apply query
        m_browseTableModel->setQuery(query);

        // There is information stored for this table, so extract it and apply it
        applyBrowseTableSettings(storedData);

        updateRecordsetLabel();

        // Plot
        emit updatePlot(ui->dataTable, m_browseTableModel, &browseTableSettings[tablename], false);
    }

    // Show/hide menu options depending on whether this is a table or a view
    if(db->getObjectByName(currentlyBrowsedTableName()) && db->getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Table)
    {
        // Table
        sqlb::TablePtr table = db->getObjectByName<sqlb::Table>(currentlyBrowsedTableName());
        ui->actionUnlockViewEditing->setVisible(false);
        ui->actionShowRowidColumn->setVisible(!table->withoutRowidTable());
    } else {
        // View
        ui->actionUnlockViewEditing->setVisible(true);
        ui->actionShowRowidColumn->setVisible(false);
    }

    updateInsertDeleteRecordButton();
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
    m_browseTableModel->setChunkSize(static_cast<std::size_t>(Settings::getValue("db", "prefetchsize").toUInt()));
}

void TableBrowser::setCurrentTable(const sqlb::ObjectIdentifier& name)
{
    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(QString::fromStdString(name.toDisplayString())));
}

void TableBrowser::clearTableBrowser()
{
    if (!ui->dataTable->model())
        return;

    ui->dataTable->setModel(nullptr);
    if(qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader()))
        qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(0);

    ui->editGlobalFilter->blockSignals(true);
    ui->editGlobalFilter->clear();
    ui->editGlobalFilter->blockSignals(false);
}

void TableBrowser::updateFilter(int column, const QString& value)
{
    // Set minimum width to the vertical header in order to avoid flickering while a filter is being updated.
    ui->dataTable->verticalHeader()->setMinimumWidth(ui->dataTable->verticalHeader()->width());

    m_browseTableModel->updateFilter(column, value);
    BrowseDataTableSettings& settings = browseTableSettings[currentlyBrowsedTableName()];
    if(value.isEmpty() && settings.filterValues.remove(column) > 0)
    {
        emit projectModified();
    } else {
        if (settings.filterValues[column] != value) {
            settings.filterValues[column] = value;
            emit projectModified();
        }
    }

    updateRecordsetLabel();

    // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
    applyBrowseTableSettings(settings, true);
}

void TableBrowser::addCondFormat(int column, const QString& value)
{
    QFont font = QFont(Settings::getValue("databrowser", "font").toString());
    font.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());

    // Create automatically a new conditional format with the next serial background color according to the theme and the regular foreground
    // color and font in the settings.
    CondFormat newCondFormat(value, QColor(Settings::getValue("databrowser", "reg_fg_colour").toString()),
                             m_condFormatPalette.nextSerialColor(Palette::appHasDarkTheme()),
                             font,
                             CondFormat::AlignLeft,
                             m_browseTableModel->encoding());
    m_browseTableModel->addCondFormat(column, newCondFormat);
    browseTableSettings[currentlyBrowsedTableName()].condFormats[column].push_back(newCondFormat);
}

void TableBrowser::clearAllCondFormats(int column)
{
    std::vector<CondFormat> emptyCondFormatVector = std::vector<CondFormat>();
    m_browseTableModel->setCondFormats(column, emptyCondFormatVector);
    browseTableSettings[currentlyBrowsedTableName()].condFormats[column].clear();
    emit projectModified();
}

void TableBrowser::editCondFormats(int column)
{
    CondFormatManager condFormatDialog(browseTableSettings[currentlyBrowsedTableName()].condFormats[column],
                                       m_browseTableModel->encoding(), this);
    condFormatDialog.setWindowTitle(tr("Conditional formats for \"%1\"").
                                    arg(m_browseTableModel->headerData(column, Qt::Horizontal).toString()));
    if (condFormatDialog.exec()) {
        std::vector<CondFormat> condFormatVector = condFormatDialog.getCondFormats();
        m_browseTableModel->setCondFormats(column, condFormatVector);
        browseTableSettings[currentlyBrowsedTableName()].condFormats[column] = condFormatVector;
        emit projectModified();
    }
}

void TableBrowser::modifyColumnFormat(std::unordered_set<int> columns, std::function<void(CondFormat&)> changeFunction)
{
    for (int column : columns) {
        std::vector<CondFormat>& columnFormats = browseTableSettings[currentlyBrowsedTableName()].condFormats[column];

        auto it = std::find_if(columnFormats.begin(), columnFormats.end(), [](const CondFormat& format) {
                return format.sqlCondition().isEmpty();
            });
        if(it != columnFormats.end()) {
            changeFunction(*it);
            m_browseTableModel->addCondFormat(column, *it);
        } else {
            // Create a new conditional format based on defaults and then modify it as requested using the passed function.
            // Alignment is get from the current column since the default is different from text and numbers.
            QFont font = QFont(Settings::getValue("databrowser", "font").toString());
            font.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
            Qt::Alignment align = Qt::Alignment(m_browseTableModel->data(currentIndex().sibling(currentIndex().row(), column),
                                                                         Qt::TextAlignmentRole).toInt());

            CondFormat newCondFormat(QString(""), QColor(Settings::getValue("databrowser", "reg_fg_colour").toString()),
                                     QColor(Settings::getValue("databrowser", "reg_bg_colour").toString()),
                                     font,
                                     CondFormat::fromCombinedAlignment(align),
                                     m_browseTableModel->encoding());
            changeFunction(newCondFormat);
            m_browseTableModel->addCondFormat(column, newCondFormat);
            browseTableSettings[currentlyBrowsedTableName()].condFormats[column].push_back(newCondFormat);
        }
    }
    emit projectModified();
}

void TableBrowser::updateRecordsetLabel()
{
    // Get all the numbers, i.e. the number of the first row and the last row as well as the total number of rows
    int from = ui->dataTable->verticalHeader()->visualIndexAt(0) + 1;
    int total = m_browseTableModel->rowCount();
    int to = ui->dataTable->verticalHeader()->visualIndexAt(ui->dataTable->height()) - 1;
    if (to == -2)
        to = total;

    // Update the validator of the goto row field
    gotoValidator->setRange(0, total);

    // When there is no query for this table (i.e. no table is selected), there is no row count query either which in turn means
    // that the row count query will never finish. And because of this the row count will be forever unknown. To avoid always showing
    // a misleading "determining row count" text in the UI we set the row count status to complete here for empty queries.
    auto row_count_available = m_browseTableModel->rowCountAvailable();
    if(m_browseTableModel->query().isEmpty())
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

    enableEditing(m_browseTableModel->rowCountAvailable() != SqliteTableModel::RowCount::Unknown);
}

void TableBrowser::applyBrowseTableSettings(const BrowseDataTableSettings& storedData, bool skipFilters)
{
    // We don't want to pass storedData by reference because the functions below would change the referenced data in their original
    // place, thus modifiying the data this function can use. To have a static description of what the view should look like we want
    // a copy here.

    // Show rowid column. Needs to be done before the column widths setting because of the workaround in there and before the filter setting
    // because of the filter row generation.
    showRowidColumn(storedData.showRowid, skipFilters);

    // Enable editing in general and (un)lock view editing depending on the settings
    unlockViewEditing(!storedData.unlockViewPk.isEmpty(), storedData.unlockViewPk);

    // Column hidden status
    on_actionShowAllColumns_triggered();
    for(auto hiddenIt=storedData.hiddenColumns.constBegin();hiddenIt!=storedData.hiddenColumns.constEnd();++hiddenIt)
        hideColumns(hiddenIt.key(), hiddenIt.value());

    // Column widths
    for(auto widthIt=storedData.columnWidths.constBegin();widthIt!=storedData.columnWidths.constEnd();++widthIt)
        ui->dataTable->setColumnWidth(widthIt.key(), widthIt.value());

    // Filters
    if(!skipFilters)
    {
        // Set filters blocking signals, since the filter is already applied to the browse table model
        FilterTableHeader* filterHeader = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
        bool oldState = filterHeader->blockSignals(true);
        for(auto filterIt=storedData.filterValues.constBegin();filterIt!=storedData.filterValues.constEnd();++filterIt)
            filterHeader->setFilter(static_cast<size_t>(filterIt.key()), filterIt.value());

        // Conditional formats
        for(auto formatIt=storedData.condFormats.constBegin(); formatIt!=storedData.condFormats.constEnd(); ++formatIt)
            m_browseTableModel->setCondFormats(formatIt.key(), formatIt.value());

      filterHeader->blockSignals(oldState);

      ui->editGlobalFilter->blockSignals(true);
      QString text;
      for(const auto& f : storedData.globalFilters)
          text += f + " ";
      text.chop(1);
      ui->editGlobalFilter->setText(text);
      ui->editGlobalFilter->blockSignals(false);
    }

    // Encoding
    m_browseTableModel->setEncoding(storedData.encoding);
}

void TableBrowser::enableEditing(bool enable_edit)
{
    // Don't enable anything if this is a read only database
    bool edit = enable_edit && !db->readOnly();

    // Apply settings
    ui->dataTable->setEditTriggers(edit ? QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);
    updateInsertDeleteRecordButton();
}

void TableBrowser::showRowidColumn(bool show, bool skipFilters)
{
    // Block all signals from the horizontal header. Otherwise the QHeaderView::sectionResized signal causes us trouble
    ui->dataTable->horizontalHeader()->blockSignals(true);

    // WORKAROUND
    // Set the opposite hidden/visible status of what we actually want for the rowid column. This is to work around a Qt bug which
    // is present in at least version 5.7.1. The problem is this: when you browse a table/view with n colums, then switch to a table/view
    // with less than n columns, you'll be able to resize the first (hidden!) column by resizing the section to the left of the first visible
    // column. By doing so the table view gets messed up. But even when not resizing the first hidden column, tab-ing through the fields
    // will stop at the not-so-much-hidden rowid column, too. All this can be fixed by this line. I haven't managed to find another workaround
    // or way to fix this yet.
    ui->dataTable->setColumnHidden(0, show);

    // Show/hide rowid column
    ui->dataTable->setColumnHidden(0, !show);

    // Update checked status of the popup menu action
    ui->actionShowRowidColumn->setChecked(show);

    // Save settings for this table
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    if (browseTableSettings[current_table].showRowid != show) {
        emit projectModified();
        browseTableSettings[current_table].showRowid = show;
    }

    // Update the filter row
    if(!skipFilters)
        qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(static_cast<size_t>(m_browseTableModel->columnCount()), show);

    // Re-enable signals
    ui->dataTable->horizontalHeader()->blockSignals(false);

    ui->dataTable->update();
}

void TableBrowser::unlockViewEditing(bool unlock, QString pk)
{
    sqlb::ObjectIdentifier currentTable = currentlyBrowsedTableName();

    if(currentTable.isEmpty())
        return;

    // If this isn't a view just unlock editing and return
    if(db->getObjectByName(currentTable) && db->getObjectByName(currentTable)->type() != sqlb::Object::View)
    {
        m_browseTableModel->setPseudoPk(m_browseTableModel->pseudoPk());
        enableEditing(true);
        return;
    }

    sqlb::ViewPtr obj = db->getObjectByName<sqlb::View>(currentTable);

    // If the view gets unlocked for editing and we don't have a 'primary key' for this view yet, then ask for one
    if(unlock && pk.isEmpty())
    {
        while(true)
        {
            bool ok;

            QStringList options;
            for(const auto& n : obj->fieldNames())
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
            if(db->executeSQL(QString("SELECT %1 FROM %2 LIMIT 1;").arg(sqlb::escapeIdentifier(pk)).arg(QString::fromStdString(currentTable.toString())), false, true))
                break;
        }
    } else if(!unlock) {
        // Locking the view is done by unsetting the pseudo-primary key
        pk = "_rowid_";
    }

    // (De)activate editing
    enableEditing(unlock);
    m_browseTableModel->setPseudoPk({pk.toStdString()});

    // Update checked status of the popup menu action
    ui->actionUnlockViewEditing->blockSignals(true);
    ui->actionUnlockViewEditing->setChecked(unlock);
    ui->actionUnlockViewEditing->blockSignals(false);

    // If the settings didn't change, do not try to reapply them.
    // This avoids an infinite mutual recursion.
    BrowseDataTableSettings& settings = browseTableSettings[currentTable];

    if(settings.unlockViewPk != pk) {
        // Save settings for this table
        settings.unlockViewPk = pk;
        // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
        applyBrowseTableSettings(settings);
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
        ui->dataTable->setColumnHidden(col, hide);
        if(!hide)
            ui->dataTable->setColumnWidth(col, ui->dataTable->horizontalHeader()->defaultSectionSize());
        browseTableSettings[tableName].hiddenColumns[col] = hide;
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

void TableBrowser::on_actionShowAllColumns_triggered()
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

    // NOTE: We're assuming here that the selection is always contiguous, i.e. that there are never two selected
    // rows with a non-selected row in between.
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
    bool isEditable = m_browseTableModel->isEditable() && !db->readOnly();
    ui->actionNewRecord->setEnabled(isEditable);
    ui->actionDeleteRecord->setEnabled(isEditable && rows != 0);

    if(rows > 1)
        ui->actionDeleteRecord->setText(tr("Delete Records"));
    else
        ui->actionDeleteRecord->setText(tr("Delete Record"));
}

void TableBrowser::duplicateRecord(int currentRow)
{
    auto row = m_browseTableModel->dittoRecord(currentRow);
    if (row.isValid())
        ui->dataTable->setCurrentIndex(row);
    else
        QMessageBox::warning(this, qApp->applicationName(), db->lastError());
}

void TableBrowser::browseTableHeaderClicked(int logicalindex)
{
    BrowseDataTableSettings& settings = browseTableSettings[currentlyBrowsedTableName()];

    // Abort if there is more than one column selected because this tells us that the user pretty sure wants to do a range selection
    // instead of sorting data. But restore before the sort indicator automatically changed by Qt so it still indicates the last
    // use sort action.
    // This check is disabled when the Control key is pressed. This is done because we use the Control key for sorting by multiple columns and
    // Qt seems to pretty much always select multiple columns when the Control key is pressed.
    if(!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) && ui->dataTable->selectionModel()->selectedColumns().count() > 1) {
        applyBrowseTableSettings(settings);
        return;
    }

    // Set minimum width to the vertical header in order to avoid flickering when sorting.
    ui->dataTable->verticalHeader()->setMinimumWidth(ui->dataTable->verticalHeader()->width());

    // Get the current list of sort columns
    auto& columns = settings.query.orderBy();

    // Before sorting, first check if the Control key is pressed. If it is, we want to append this column to the list of sort columns. If it is not,
    // we want to sort only by the new column.
    if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
    {
        // Multi column sorting

        // If the column was control+clicked again, change its sort order.
        // If not already in the sort order, add the column as a new sort column to the list.
        bool present = false;
        for(sqlb::SortedColumn& sortedCol : columns) {

            if(sortedCol.column == static_cast<size_t>(logicalindex)) {
                sortedCol.direction = (sortedCol.direction == sqlb::Ascending ? sqlb::Descending : sqlb::Ascending);
                present = true;
                break;
            }
        }
        if(!present)
            columns.emplace_back(logicalindex, sqlb::Ascending);
    } else {
        // Single column sorting

        // If we have exactly one sort column and it is the column which was just clicked, change its sort order.
        // If not, clear the list of sorting columns and replace it by a single new sort column.
        if(columns.size() == 1 && columns.front().column == static_cast<size_t>(logicalindex))
        {
            columns.front().direction = (columns.front().direction == sqlb::Ascending ? sqlb::Descending : sqlb::Ascending);
        } else {
            columns.clear();
            columns.emplace_back(logicalindex, sqlb::Ascending);
        }
    }

    // Do the actual sorting
    ui->dataTable->sortByColumns(columns);

    // select the first item in the column so the header is bold
    // we might try to select the last selected item
    ui->dataTable->setCurrentIndex(ui->dataTable->currentIndex().sibling(0, logicalindex));

    emit updatePlot(ui->dataTable, m_browseTableModel, &browseTableSettings[currentlyBrowsedTableName()], true);

    // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
    applyBrowseTableSettings(settings);

    emit projectModified();
}

void TableBrowser::updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size)
{
    std::unordered_set<int> selectedCols = ui->dataTable->selectedCols();
    sqlb::ObjectIdentifier tableName = currentlyBrowsedTableName();

    if (selectedCols.find(section) == selectedCols.end())
    {
        if (browseTableSettings[tableName].columnWidths[section] != new_size) {
            emit projectModified();
            browseTableSettings[tableName].columnWidths[section] = new_size;
        }
    }
    else
    {
        ui->dataTable->blockSignals(true);
        for(int col : selectedCols)
        {
            ui->dataTable->setColumnWidth(col, new_size);
            if (browseTableSettings[tableName].columnWidths[col] != new_size) {
                emit projectModified();
                browseTableSettings[tableName].columnWidths[col] = new_size;
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
    popupBrowseDataHeaderMenu->exec(ui->dataTable->horizontalHeader()->mapToGlobal(pos));
}

void TableBrowser::showRecordPopupMenu(const QPoint& pos)
{
    if(!(db->getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Types::Table && !db->readOnly()))
        return;

    int row = ui->dataTable->verticalHeader()->logicalIndexAt(pos);
    if (row == -1)
        return;

    // Select the row if it is not already in the selection.
    QModelIndexList rowList = ui->dataTable->selectionModel()->selectedRows();
    bool found = false;
    for (QModelIndex index : rowList) {
        if (row == index.row()) {
            found = true;
            break;
        }
    }
    if (!found)
        ui->dataTable->selectRow(row);

    rowList = ui->dataTable->selectionModel()->selectedRows();

    QString duplicateText = rowList.count() > 1 ? tr("Duplicate records") : tr("Duplicate record");

    QMenu popupRecordMenu(this);
    QAction* action = new QAction(duplicateText, &popupRecordMenu);
    // Set shortcut for documentation purposes (the actual functional shortcut is not set here)
    action->setShortcut(QKeySequence(tr("Ctrl+\"")));
    popupRecordMenu.addAction(action);

    connect(action, &QAction::triggered, [&]() {
            for (QModelIndex index : rowList) {
                duplicateRecord(index.row());
            }
    });

    QAction* deleteRecordAction = new QAction(QIcon(":icons/delete_record"), ui->actionDeleteRecord->text(), &popupRecordMenu);
    popupRecordMenu.addAction(deleteRecordAction);

    connect(deleteRecordAction, &QAction::triggered, [&]() {
            deleteRecord();
    });

    popupRecordMenu.exec(ui->dataTable->verticalHeader()->mapToGlobal(pos));
}

void TableBrowser::addRecord()
{
    int row = m_browseTableModel->rowCount();

    // If table has pseudo_pk, then it must be an editable view. Jump straight to inserting by pop-up dialog.
    if(!m_browseTableModel->hasPseudoPk() && m_browseTableModel->insertRow(row))
    {
        selectTableLine(row);
    } else {
        // Error inserting empty row.
        // User has to provide values acomplishing the constraints. Open Add Record Dialog.
        insertValues();
    }
}

void TableBrowser::insertValues()
{
    std::vector<std::string> pseudo_pk = m_browseTableModel->hasPseudoPk() ? m_browseTableModel->pseudoPk() : std::vector<std::string>();
    AddRecordDialog dialog(*db, currentlyBrowsedTableName(), this, pseudo_pk);
    if (dialog.exec())
        updateTable();
}

void TableBrowser::deleteRecord()
{
    if(ui->dataTable->selectionModel()->hasSelection())
    {
        // If only filter header is selected
        if(ui->dataTable->selectionModel()->selectedIndexes().isEmpty())
            return;

        int old_row = ui->dataTable->currentIndex().row();
        while(ui->dataTable->selectionModel()->hasSelection())
        {
            int first_selected_row = ui->dataTable->selectionModel()->selectedIndexes().first().row();
            int last_selected_row = ui->dataTable->selectionModel()->selectedIndexes().last().row();
            int selected_rows_count = last_selected_row - first_selected_row + 1;
            if(!m_browseTableModel->removeRows(first_selected_row, selected_rows_count))
            {
                QMessageBox::warning(this, QApplication::applicationName(), tr("Error deleting record:\n%1").arg(db->lastError()));
                break;
            }
        }

        if(old_row > m_browseTableModel->rowCount())
            old_row = m_browseTableModel->rowCount();
        selectTableLine(old_row);
    } else {
        QMessageBox::information( this, QApplication::applicationName(), tr("Please select a record first"));
    }
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
    if(curRow >= m_browseTableModel->rowCount())
        curRow = m_browseTableModel->rowCount() - 1;
    selectTableLine(curRow);
}

void TableBrowser::navigateBegin()
{
    selectTableLine(0);
}

void TableBrowser::navigateEnd()
{
    selectTableLine(m_browseTableModel->rowCount()-1);
}

void TableBrowser::navigateGoto()
{
    int row = ui->editGoto->text().toInt();
    if(row <= 0)
        row = 1;
    if(row > m_browseTableModel->rowCount())
        row = m_browseTableModel->rowCount();

    selectTableLine(row - 1);
    ui->editGoto->setText(QString::number(row));
}

void TableBrowser::selectTableLine(int lineToSelect)
{
    ui->dataTable->selectTableLine(lineToSelect);
}

void TableBrowser::on_actionClearFilters_triggered()
{
    ui->dataTable->filterHeader()->clearFilters();
}

void TableBrowser::on_actionClearSorting_triggered()
{
    // Get the current list of sort columns
    auto& columns = browseTableSettings[currentlyBrowsedTableName()].query.orderBy();
    columns.clear();
    // Set cleared vector of sort-by columns
    m_browseTableModel->sort(columns);
}

void TableBrowser::editDataColumnDisplayFormat()
{
    // Get the current table name and fetch its table object, then retrieve the fields of that table and look up the index of the clicked table header
    // section using it as the table field array index. Subtract one from the header index to get the column index because in the the first (though hidden)
    // column is always the rowid column. Ultimately, get the column name from the column object
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    int field_number = sender()->property("clicked_column").toInt();
    QString field_name;
    if (db->getObjectByName(current_table)->type() == sqlb::Object::Table)
        field_name = QString::fromStdString(db->getObjectByName<sqlb::Table>(current_table)->fields.at(static_cast<size_t>(field_number)-1).name());
    else
        field_name = QString::fromStdString(db->getObjectByName<sqlb::View>(current_table)->fieldNames().at(static_cast<size_t>(field_number)-1));
    // Get the current display format of the field
    QString current_displayformat = browseTableSettings[current_table].displayFormats[field_number];

    // Open the dialog
    ColumnDisplayFormatDialog dialog(*db, current_table, field_name, current_displayformat, this);
    if(dialog.exec())
    {
        // Set the newly selected display format
        QString new_format = dialog.selectedDisplayFormat();
        if(new_format.size())
            browseTableSettings[current_table].displayFormats[field_number] = new_format;
        else
            browseTableSettings[current_table].displayFormats.remove(field_number);
        emit projectModified();

        // Refresh view
        updateTable();
    }
}

void TableBrowser::exportFilteredTable()
{
    ExportDataDialog dialog(*db, ExportDataDialog::ExportFormatCsv, this, m_browseTableModel->customQuery(false));
    dialog.exec();
}

void TableBrowser::saveFilterAsView()
{
    if (m_browseTableModel->filterCount() > 0)
        // Save as view a custom query without rowid
        emit createView(m_browseTableModel->customQuery(false));
    else
        QMessageBox::information(this, qApp->applicationName(), tr("There is no filter set for this table. View will not be created."));
}

void TableBrowser::browseDataSetTableEncoding(bool forAllTables)
{
    // Get the old encoding
    QString encoding = m_browseTableModel->encoding();

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
        m_browseTableModel->setEncoding(encoding);

        // Save encoding for this table
        browseTableSettings[currentlyBrowsedTableName()].encoding = encoding;

        // Set default encoding if requested to and change all stored table encodings
        if(forAllTables)
        {
            defaultBrowseTableEncoding = encoding;

            for(auto it=browseTableSettings.begin();it!=browseTableSettings.end();++it)
                it.value().encoding = encoding;
        }

        emit projectModified();
    }
}

void TableBrowser::browseDataSetDefaultTableEncoding()
{
    browseDataSetTableEncoding(true);
}

void TableBrowser::jumpToRow(const sqlb::ObjectIdentifier& table, QString column, const QByteArray& value)
{
    // First check if table exists
    sqlb::TablePtr obj = db->getObjectByName<sqlb::Table>(table);
    if(!obj)
        return;

    // If no column name is set, assume the primary key is meant
    if(!column.size())
        column = QString::fromStdString(obj->primaryKey()->columnList().front());

    // If column doesn't exist don't do anything
    auto column_index = sqlb::findField(obj, column.toStdString());
    if(column_index == obj->fields.end())
        return;

    // Jump to table
    setCurrentTable(table);

    // Set filter
    ui->dataTable->filterHeader()->setFilter(static_cast<size_t>(column_index-obj->fields.begin()+1), QString("=") + value);
    updateTable();
}

void TableBrowser::find(const QString& expr, bool forward, bool include_first)
{
    // Get the cell from which the search should be started. If there is a selected cell, use that. If there is no selected cell, start at the first cell.
    QModelIndex start;
    if(ui->dataTable->selectionModel()->hasSelection())
        start = ui->dataTable->selectionModel()->selectedIndexes().front();
    else
        start = m_browseTableModel->index(0, 0);

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
    if(browseTableSettings[tableName].showRowid)
        column_list.push_back(0);
    for(int i=1;i<m_browseTableModel->columnCount();i++)
    {
        if(browseTableSettings[tableName].hiddenColumns.contains(i) == false)
            column_list.push_back(i);
        else if(browseTableSettings[tableName].hiddenColumns[i] == false)
            column_list.push_back(i);
    }

    // Perform the actual search using the model class
    const auto match = m_browseTableModel->nextMatch(start, column_list, expr, flags, !forward, include_first);

    // Select the next match if we found one
    if(match.isValid()) {
        ui->dataTable->setCurrentIndex(match);
        ui->dataTable->scrollTo(match);
    }
    // Make the expression control red if no results were found
    if(match.isValid() || expr == "")
        ui->editFindExpression->setStyleSheet("");
    else
        ui->editFindExpression->setStyleSheet("QLineEdit {color: white; background-color: rgb(255, 102, 102)}");
}
