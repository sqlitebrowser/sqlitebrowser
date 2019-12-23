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

QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> TableBrowser::m_settings;
QString TableBrowser::m_defaultEncoding;

TableBrowser::TableBrowser(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TableBrowser),
    gotoValidator(new QIntValidator(0, 0, this)),
    db(nullptr),
    dbStructureModel(nullptr),
    m_model(nullptr),
    m_adjustRows(false),
    m_columnsResized(false)
{
    ui->setupUi(this);

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set custom placeholder text for global filter field and disable conditional formats
    ui->editGlobalFilter->setPlaceholderText(tr("Filter in all columns"));
    ui->editGlobalFilter->setConditionFormatContextMenuEnabled(false);

    // Set up popup menus
    popupNewRecordMenu = new QMenu(this);
    popupNewRecordMenu->addAction(ui->newRecordAction);
    popupNewRecordMenu->addAction(ui->insertValuesAction);
    ui->actionNewRecord->setMenu(popupNewRecordMenu);

    popupSaveFilterAsMenu = new QMenu(this);
    popupSaveFilterAsMenu->addAction(ui->actionFilteredTableExportCsv);
    popupSaveFilterAsMenu->addAction(ui->actionFilterSaveAsView);
    ui->actionSaveFilterAsPopup->setMenu(popupSaveFilterAsMenu);
    qobject_cast<QToolButton*>(ui->browseToolbar->widgetForAction(ui->actionSaveFilterAsPopup))->setPopupMode(QToolButton::InstantPopup);

    popupHeaderMenu = new QMenu(this);
    popupHeaderMenu->addAction(ui->actionShowRowidColumn);
    popupHeaderMenu->addAction(ui->actionHideColumns);
    popupHeaderMenu->addAction(ui->actionShowAllColumns);
    popupHeaderMenu->addAction(ui->actionSelectColumn);
    popupHeaderMenu->addSeparator();
    popupHeaderMenu->addAction(ui->actionUnlockViewEditing);
    popupHeaderMenu->addAction(ui->actionBrowseTableEditDisplayFormat);
    popupHeaderMenu->addSeparator();
    popupHeaderMenu->addAction(ui->actionSetTableEncoding);
    popupHeaderMenu->addAction(ui->actionSetAllTablesEncoding);

    connect(ui->actionSelectColumn, &QAction::triggered, [this]() {
        ui->dataTable->selectColumn(ui->actionBrowseTableEditDisplayFormat->property("clicked_column").toInt());
      });

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
    connect(ui->dataTable->filterHeader(), &FilterTableHeader::addCondFormat, this, &TableBrowser::addCondFormatFromFilter);
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
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::sectionResized, this, &TableBrowser::updateColumnWidth);
    connect(ui->dataTable->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &TableBrowser::showDataColumnPopupMenu);
    connect(ui->dataTable->verticalHeader(), &QHeaderView::customContextMenuRequested, this, &TableBrowser::showRecordPopupMenu);
    connect(ui->dataTable, &ExtendedTableWidget::openFileFromDropEvent, this, &TableBrowser::requestFileOpen);
    connect(ui->dataTable, &ExtendedTableWidget::selectedRowsToBeDeleted, this, &TableBrowser::deleteRecord);

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
            QFont font;
            font.fromString(m_model->data(current, Qt::FontRole).toString());
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

    connect(ui->actionFind, &QAction::triggered, [this](bool checked) {
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
    connect(ui->actionReplace, &QAction::triggered, [this](bool checked) {
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
}

TableBrowser::~TableBrowser()
{
    delete gotoValidator;
    delete ui;
}

void TableBrowser::init(DBBrowserDB* _db)
{
    db = _db;

    if(m_model)
        delete m_model;
    m_model = new SqliteTableModel(*db, this);

    connect(m_model, &SqliteTableModel::finishedFetch, this, &TableBrowser::fetchedData);

}

void TableBrowser::reset()
{
    // Reset the model
    if(m_model)
        m_model->reset();

    // Remove all stored table information browse data tab
    m_settings.clear();
    m_defaultEncoding = QString();

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
    return m_settings[object];
}

void TableBrowser::setSettings(const sqlb::ObjectIdentifier& table, const BrowseDataTableSettings& table_settings)
{
    m_settings[table] = table_settings;
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
        clear();
    else                                                            // Under normal circumstances just select the old table again
        ui->comboBrowseTable->setCurrentIndex(old_table_index);
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

void TableBrowser::updateTable()
{
    // Remove the model-view link if the table name is empty in order to remove any data from the view
    if(ui->comboBrowseTable->model()->rowCount(ui->comboBrowseTable->rootModelIndex()) == 0)
    {
        clear();
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
    ui->dataTable->setModel(m_model);
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
    // Search stored table settings for this table
    bool storedDataFound = m_settings.contains(tablename);

    // Set new table
    if(!storedDataFound)
    {
        // No stored settings found.

        // Set table name and apply default display format settings
        m_model->setQuery(sqlb::Query(tablename));

        // There aren't any information stored for this table yet, so use some default values

        // Hide rowid column. Needs to be done before the column widths setting because of the workaround in there
        showRowidColumn(false);

        // Unhide all columns by default
        on_actionShowAllColumns_triggered();

        // Enable editing in general, but lock view editing
        unlockViewEditing(false);

        // Prepare for setting an initial column width based on the content.
        m_columnsResized = false;

        // Encoding
        m_model->setEncoding(m_defaultEncoding);

        // Global filter
        ui->editGlobalFilter->clear();

        updateRecordsetLabel();

        // Plot
        emit updatePlot(ui->dataTable, m_model, &m_settings[tablename], true);

        // The filters can be left empty as they are
    } else {
        // Stored settings found. Retrieve them and assemble a query from them.
        BrowseDataTableSettings storedData = m_settings[tablename];
        sqlb::Query query(tablename);

        // Sorting
        query.setOrderBy(storedData.query.orderBy());

        // Filters
        for(auto it=storedData.filterValues.constBegin();it!=storedData.filterValues.constEnd();++it)
            query.where().insert({it.key(), CondFormat::filterToSqlCondition(it.value(), m_model->encoding())});

        // Global filter
        for(const auto& f : storedData.globalFilters)
            query.globalWhere().push_back(CondFormat::filterToSqlCondition(f, m_model->encoding()));

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
        m_model->setQuery(query);

        // There is information stored for this table, so extract it and apply it
        applySettings(storedData);

        updateRecordsetLabel();

        // Plot
        emit updatePlot(ui->dataTable, m_model, &m_settings[tablename], false);
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
    m_model->reloadSettings();
}

void TableBrowser::setCurrentTable(const sqlb::ObjectIdentifier& name)
{
    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(QString::fromStdString(name.toDisplayString())));
}

void TableBrowser::clear()
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

void TableBrowser::updateFilter(size_t column, const QString& value)
{
    // Set minimum width to the vertical header in order to avoid flickering while a filter is being updated.
    ui->dataTable->verticalHeader()->setMinimumWidth(ui->dataTable->verticalHeader()->width());

    m_model->updateFilter(column, value);
    BrowseDataTableSettings& settings = m_settings[currentlyBrowsedTableName()];
    if(value.isEmpty() && settings.filterValues.remove(static_cast<int>(column)) > 0)
    {
        emit projectModified();
    } else {
        if (settings.filterValues[static_cast<int>(column)] != value) {
            settings.filterValues[static_cast<int>(column)] = value;
            emit projectModified();
        }
    }

    updateRecordsetLabel();

    // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
    applySettings(settings, true);
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
    // Conditionless formats are pushed back and others inserted at the begining, so they aren't eclipsed.
    if (newCondFormat.filter().isEmpty())
        formats.push_back(newCondFormat);
    else
        formats.insert(formats.begin(), newCondFormat);
}

void TableBrowser::clearAllCondFormats(size_t column)
{
    std::vector<CondFormat> emptyCondFormatVector = std::vector<CondFormat>();
    m_model->setCondFormats(false, column, emptyCondFormatVector);
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
            const QString& filter = m_settings[currentlyBrowsedTableName()].filterValues.value(static_cast<int>(column));
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
    int to = ui->dataTable->verticalHeader()->visualIndexAt(ui->dataTable->height()) - 1;
    if (to == -2)
        to = total;

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

    enableEditing(m_model->rowCountAvailable() != SqliteTableModel::RowCount::Unknown);
}

void TableBrowser::applySettings(const BrowseDataTableSettings& storedData, bool skipFilters)
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
    m_columnsResized = true;

    // Filters
    if(!skipFilters)
    {
        // Set filters blocking signals, since the filter is already applied to the browse table model
        FilterTableHeader* filterHeader = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
        bool oldState = filterHeader->blockSignals(true);
        for(auto filterIt=storedData.filterValues.constBegin();filterIt!=storedData.filterValues.constEnd();++filterIt)
            filterHeader->setFilter(static_cast<size_t>(filterIt.key()), filterIt.value());

        // Regular conditional formats
        for(auto formatIt=storedData.condFormats.constBegin(); formatIt!=storedData.condFormats.constEnd(); ++formatIt)
            m_model->setCondFormats(false, formatIt.key(), formatIt.value());

        // Row Id formats
        for(auto formatIt=storedData.rowIdFormats.constBegin(); formatIt!=storedData.rowIdFormats.constEnd(); ++formatIt)
            m_model->setCondFormats(true, formatIt.key(), formatIt.value());

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
    m_model->setEncoding(storedData.encoding);
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
    if (m_settings[current_table].showRowid != show) {
        emit projectModified();
        m_settings[current_table].showRowid = show;
    }

    // Update the filter row
    if(!skipFilters)
        qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(static_cast<size_t>(m_model->columnCount()), show);

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
        m_model->setPseudoPk(m_model->pseudoPk());
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
        // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
        applySettings(settings);
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

    // Abort if there is more than one column selected because this tells us that the user pretty sure wants to do a range selection
    // instead of sorting data. But restore before the sort indicator automatically changed by Qt so it still indicates the last
    // use sort action.
    // This check is disabled when the Control key is pressed. This is done because we use the Control key for sorting by multiple columns and
    // Qt seems to pretty much always select multiple columns when the Control key is pressed.
    if(!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) && ui->dataTable->selectionModel()->selectedColumns().count() > 1) {
        applySettings(settings);
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

    emit updatePlot(ui->dataTable, m_model, &m_settings[currentlyBrowsedTableName()], true);

    // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
    applySettings(settings);

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
    if(db->getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Types::Table && !db->readOnly()) {

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

        QAction* action = new QAction(duplicateText, &popupRecordMenu);
        // Set shortcut for documentation purposes (the actual functional shortcut is not set here)
        action->setShortcut(QKeySequence(tr("Ctrl+\"")));
        popupRecordMenu.addAction(action);

        connect(action, &QAction::triggered, [rowList, this]() {
            for (const QModelIndex& index : rowList)
                duplicateRecord(index.row());
        });

        QAction* deleteRecordAction = new QAction(QIcon(":icons/delete_record"), ui->actionDeleteRecord->text(), &popupRecordMenu);
        popupRecordMenu.addAction(deleteRecordAction);

        connect(deleteRecordAction, &QAction::triggered, [&]() {
            deleteRecord();
        });

        popupRecordMenu.addSeparator();
    }

    // "Adjust rows" can be done on any object
    QAction* adjustRowHeightAction = new QAction(tr("Adjust rows to contents"), &popupRecordMenu);
    adjustRowHeightAction->setCheckable(true);
    adjustRowHeightAction->setChecked(m_adjustRows);
    popupRecordMenu.addAction(adjustRowHeightAction);

    connect(adjustRowHeightAction, &QAction::toggled, [&](bool checked) {
        m_adjustRows = checked;
        if(m_adjustRows)
            ui->dataTable->resizeRowsToContents();
        else
            updateTable();
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
}

void TableBrowser::insertValues()
{
    std::vector<std::string> pseudo_pk = m_model->hasPseudoPk() ? m_model->pseudoPk() : std::vector<std::string>();
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
            if(!m_model->removeRows(first_selected_row, selected_rows_count))
            {
                QMessageBox::warning(this, QApplication::applicationName(), tr("Error deleting record:\n%1").arg(db->lastError()));
                break;
            }
        }

        if(old_row > m_model->rowCount())
            old_row = m_model->rowCount();
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

void TableBrowser::on_actionClearFilters_triggered()
{
    ui->dataTable->filterHeader()->clearFilters();
}

void TableBrowser::on_actionClearSorting_triggered()
{
    // Get the current list of sort columns
    auto& columns = m_settings[currentlyBrowsedTableName()].query.orderBy();
    columns.clear();
    // Set cleared vector of sort-by columns
    m_model->sort(columns);
}

void TableBrowser::editDisplayFormat()
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
            m_settings[current_table].displayFormats.remove(field_number);
        emit projectModified();

        // Refresh view
        updateTable();
    }
}

void TableBrowser::exportFilteredTable()
{
    ExportDataDialog dialog(*db, ExportDataDialog::ExportFormatCsv, this, m_model->customQuery(false));
    dialog.exec();
}

void TableBrowser::saveFilterAsView()
{
    if (m_model->filterCount() > 0)
        // Save as view a custom query without rowid
        emit createView(m_model->customQuery(false));
    else
        QMessageBox::information(this, qApp->applicationName(), tr("There is no filter set for this table. View will not be created."));
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
                it.value().encoding = encoding;
        }

        emit projectModified();
    }
}

void TableBrowser::setDefaultTableEncoding()
{
    setTableEncoding(true);
}

void TableBrowser::jumpToRow(const sqlb::ObjectIdentifier& table, std::string column, const QByteArray& value)
{
    // First check if table exists
    sqlb::TablePtr obj = db->getObjectByName<sqlb::Table>(table);
    if(!obj)
        return;

    // If no column name is set, assume the primary key is meant
    if(!column.size())
        column = obj->primaryKey()->columnList().front();

    // If column doesn't exist don't do anything
    auto column_index = sqlb::findField(obj, column);
    if(column_index == obj->fields.end())
        return;

    // Jump to table
    setCurrentTable(table);

    // Set filter
    ui->dataTable->filterHeader()->setFilter(static_cast<size_t>(column_index-obj->fields.begin()+1), QString("=") + value);
    updateTable();
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

    // You are not allowed to search for an ampty string
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
        if(m_settings[tableName].hiddenColumns.contains(i) == false)
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

    // Adjust row height to contents. This has to be done each time new data is fetched.
    if(m_adjustRows)
        ui->dataTable->resizeRowsToContents();

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
