#include "PlotDock.h"
#include "ui_PlotDock.h"
#include "Settings.h"
#include "sqlitetablemodel.h"
#include "FileDialog.h"
#include "MainWindow.h"     // Just for BrowseDataTableSettings, not for the actual main window class

PlotDock::PlotDock(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::PlotDock),
      m_currentPlotModel(nullptr),
      m_currentTableSettings(nullptr)
{
    ui->setupUi(this);

    // Init widgets
    ui->treePlotColumns->setSelectionMode(QAbstractItemView::NoSelection);

    // Restore state
    ui->splitterForPlot->restoreState(Settings::getValue("PlotDock", "splitterSize").toByteArray());
    ui->comboLineType->setCurrentIndex(Settings::getValue("PlotDock", "lineType").toInt());
    ui->comboPointShape->setCurrentIndex(Settings::getValue("PlotDock", "pointShape").toInt());

    // Connect signals
    connect(ui->treePlotColumns, &QTreeWidget::itemChanged, this, &PlotDock::on_treePlotColumns_itemChanged);
    connect(ui->plotWidget, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->plotWidget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->plotWidget, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // Enable: click on items to select them, Ctrl+Click for multi-selection, mouse-wheel for zooming and mouse drag for
    // changing the visible range.
    // Select one axis for zoom and drag applying only to that orientation.
    ui->plotWidget->setInteractions(QCP::iSelectPlottables | QCP::iMultiSelect | QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectAxes);
    ui->plotWidget->setSelectionRectMode(QCP::srmNone);

    QShortcut* shortcutCopy = new QShortcut(QKeySequence::Copy, ui->plotWidget, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutCopy, SIGNAL(activated()), this, SLOT(copy()));

    ui->plotWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Set up context menu
    m_contextMenu = new QMenu(this);
    QAction* copyAction = new QAction(QIcon(":/icons/copy"), tr("Copy"), m_contextMenu);
    copyAction->setShortcut(shortcutCopy->key());
    m_contextMenu->addAction(copyAction);

    connect(copyAction, &QAction::triggered, [&]() {
       copy();
    });

    connect(ui->plotWidget, &QTableView::customContextMenuRequested,
            [=](const QPoint& pos) {
        // Show menu
        m_contextMenu->popup(ui->plotWidget->mapToGlobal(pos));
    });

}

PlotDock::~PlotDock()
{
    // Save state
    Settings::setValue("PlotDock", "splitterSize", ui->splitterForPlot->saveState());
    Settings::setValue("PlotDock", "lineType", ui->comboLineType->currentIndex());
    Settings::setValue("PlotDock", "pointShape", ui->comboPointShape->currentIndex());

    // Finally, delete all widgets
    delete ui;
}

void PlotDock::updatePlot(SqliteTableModel* model, BrowseDataTableSettings* settings, bool update, bool keepOrResetSelection)
{
    // Each column has an id that we use internally, starting from 0. However, at the beginning of the columns list we want to add
    // the virtual 'Row #' column which needs a separate unique id for internal use. This id is defined here as -1 in a 16bit integer.
    const unsigned int RowNumId = 0xFFFF;

    // add columns to x/y selection tree widget
    if(update)
    {
        // Store pointer to the current browse table settings in the main window
        m_currentTableSettings = settings;

        // disable tree plot columns item changed updates
        ui->treePlotColumns->blockSignals(true);

        m_currentPlotModel = model;

        // save current selected columns, so we can restore them after the update
        QString sItemX; // selected X column
        QMap<QString, PlotSettings> mapItemsY; // selected Y columns with color

        if(keepOrResetSelection)
        {
            // Store the currently selected plot columns to restore them later
            for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
            {
                QTreeWidgetItem* item = ui->treePlotColumns->topLevelItem(i);
                if(item->checkState(PlotColumnX) == Qt::Checked)
                    sItemX = item->text(PlotColumnField);

                if(item->checkState(PlotColumnY) == Qt::Checked)
                    mapItemsY[item->text(PlotColumnField)] = PlotSettings(0, 0, item->backgroundColor(PlotColumnY), item->checkState(PlotColumnY) == Qt::Checked);
            }
        } else {
            // Get the plot columns to select from the stored browse table information
            sItemX = m_currentTableSettings->plotXAxis;

            mapItemsY = m_currentTableSettings->plotYAxes;
        }

        ui->treePlotColumns->clear();

        if(model)
        {
            model->waitForFetchingFinished();

            // Add each column with a supported data type to the column selection view
            for(int i=0;i<model->columnCount();++i)
            {
                QVariant::Type columntype = guessDataType(model, i);
                if(columntype != QVariant::String && columntype != QVariant::Invalid)
                {
                    QTreeWidgetItem* columnitem = new QTreeWidgetItem(ui->treePlotColumns);
                    // maybe i make this more complicated than i should
                    // but store the model column index in the first 16 bit and the type
                    // in the other 16 bits
                    uint itemdata = 0;
                    itemdata = i << 16;
                    itemdata |= columntype;
                    columnitem->setData(PlotColumnField, Qt::UserRole, itemdata);
                    columnitem->setText(PlotColumnField, model->headerData(i, Qt::Horizontal).toString());

                    // restore previous check state
                    if(mapItemsY.contains(columnitem->text(PlotColumnField)))
                    {
                        columnitem->setCheckState(PlotColumnY, mapItemsY[columnitem->text(PlotColumnField)].active ? Qt::Checked : Qt::Unchecked);
                        columnitem->setBackgroundColor(PlotColumnY, mapItemsY[columnitem->text(PlotColumnField)].colour);
                    } else {
                        columnitem->setCheckState(PlotColumnY, Qt::Unchecked);
                    }

                    if(sItemX == columnitem->text(PlotColumnField))
                        columnitem->setCheckState(PlotColumnX, Qt::Checked);
                    else
                        columnitem->setCheckState(PlotColumnX, Qt::Unchecked);
                }
            }
            ui->treePlotColumns->resizeColumnToContents(PlotColumnField);

            // Add a row number column at the beginning of the column list, but only when there were (other) columns added
            if(ui->treePlotColumns->topLevelItemCount())
            {
                QTreeWidgetItem* columnitem = new QTreeWidgetItem(ui->treePlotColumns);

                // Just set all bits in the user role information field here to somehow indicate what column this is
                uint itemdata = -1;
                columnitem->setData(PlotColumnField, Qt::UserRole, itemdata);
                columnitem->setText(PlotColumnField, tr("Row #"));

                // restore previous check state
                if(mapItemsY.contains(columnitem->text(PlotColumnField)))
                {
                    columnitem->setCheckState(PlotColumnY, mapItemsY[columnitem->text(PlotColumnField)].active ? Qt::Checked : Qt::Unchecked);
                    columnitem->setBackgroundColor(PlotColumnY, mapItemsY[columnitem->text(PlotColumnField)].colour);
                } else {
                    columnitem->setCheckState(PlotColumnY, Qt::Unchecked);
                }
                if(sItemX == columnitem->text(PlotColumnField))
                    columnitem->setCheckState(PlotColumnX, Qt::Checked);
                else
                    columnitem->setCheckState(PlotColumnX, Qt::Unchecked);

                ui->treePlotColumns->takeTopLevelItem(ui->treePlotColumns->indexOfTopLevelItem(columnitem));
                ui->treePlotColumns->insertTopLevelItem(0, columnitem);
            }
        }

        ui->plotWidget->yAxis->setLabel("Y");
        ui->plotWidget->xAxis->setLabel("X");
        ui->treePlotColumns->blockSignals(false);
    }

    // search for the x axis select
    QTreeWidgetItem* xitem = nullptr;
    for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
    {
        xitem = ui->treePlotColumns->topLevelItem(i);
        if(xitem->checkState(PlotColumnX) == Qt::Checked)
            break;

        xitem = nullptr;
    }

    QStringList yAxisLabels;

    // Clear graphs and axis labels
    ui->plotWidget->clearPlottables();
    ui->plotWidget->xAxis->setLabel(QString());
    ui->plotWidget->yAxis->setLabel(QString());

    if(xitem)
    {
        // regain the model column index and the datatype
        // leading 16 bit are column index, the other 16 bit are the datatype
        // right now datatype is only important for X axis (date, non date)
        uint xitemdata = xitem->data(PlotColumnField, Qt::UserRole).toUInt();
        int x = xitemdata >> 16;
        int xtype = xitemdata & (uint)0xFF;

        // check if we have a x axis with datetime data
        if(xtype == QVariant::DateTime)
        {
            QSharedPointer<QCPAxisTickerDateTime> ticker(new QCPAxisTickerDateTime);
            ticker->setDateTimeFormat("yyyy-MM-dd");
            ui->plotWidget->xAxis->setTicker(ticker);
        } else {
            QSharedPointer<QCPAxisTickerFixed> ticker(new QCPAxisTickerFixed);
            ticker->setTickStepStrategy(QCPAxisTicker::tssReadability);
            ticker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
            ui->plotWidget->xAxis->setTicker(ticker);
        }

        // add graph for each selected y axis
        for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem* item = ui->treePlotColumns->topLevelItem(i);
            if(item->checkState((PlotColumnY)) == Qt::Checked)
            {
                // regain the model column index and the datatype
                // leading 16 bit are column index
                uint itemdata = item->data(0, Qt::UserRole).toUInt();
                int column = itemdata >> 16;

                bool isSorted = true;

                // prepare the data vectors for qcustomplot
                // possible improvement might be a QVector subclass that directly
                // access the model data, to save memory, we are copying here
                QVector<double> xdata(model->rowCount()), ydata(model->rowCount()), tdata(model->rowCount());
                for(int i = 0; i < model->rowCount(); ++i)
                {
                    tdata[i] = i;
                    // convert x type axis if it's datetime
                    if(xtype == QVariant::DateTime)
                    {
                        QString s = model->data(model->index(i, x)).toString();
                        QDateTime d = QDateTime::fromString(s, Qt::ISODate);
                        xdata[i] = d.toMSecsSinceEpoch() / 1000.0;
                    } else {
                        // Get the x value for this point. If the selected column is -1, i.e. the row number, just use the current row number from the loop
                        // instead of retrieving some value from the model.
                        if(x == RowNumId)
                            xdata[i] = i+1;

                        else
                            xdata[i] = model->data(model->index(i, x)).toDouble();
                    }

                    if (i != 0)
                        isSorted &= (xdata[i-1] <= xdata[i]);

                    // Get the y value for this point. If the selected column is -1, i.e. the row number, just use the current row number from the loop
                    // instead of retrieving some value from the model.
                    QVariant pointdata;
                    if(column == RowNumId)
                        pointdata = i+1;
                    else
                        pointdata = model->data(model->index(i, column), Qt::EditRole);

                    if(pointdata.isNull())
                        ydata[i] = qQNaN();
                    else
                        ydata[i] = pointdata.toDouble();
                }
                // WARN: ssDot is removed
                int shapeIdx = ui->comboPointShape->currentIndex();
                if (shapeIdx > 0) shapeIdx += 1;
                QCPScatterStyle scatterStyle = QCPScatterStyle(static_cast<QCPScatterStyle::ScatterShape>(shapeIdx), 5);

                QCPAbstractPlottable* plottable;
                // When it is already sorted by x, we draw a graph.
                // When it is not sorted by x, we draw a curve, so the order selected by the user in the table or in the query is
                // respected.  In this case the line will have loops and only None and Line is supported as line style.
                // TODO: how to make the user aware of this without disturbing.
                if (isSorted) {
                    QCPGraph* graph = ui->plotWidget->addGraph();
                    plottable = graph;
                    graph->setData(xdata, ydata, /*alreadySorted*/ true);
                    // set some graph styles not supported by the abstract plottable
                    graph->setLineStyle((QCPGraph::LineStyle) ui->comboLineType->currentIndex());
                    graph->setScatterStyle(scatterStyle);

                } else {
                    QCPCurve* curve = new QCPCurve(ui->plotWidget->xAxis, ui->plotWidget->yAxis);
                    plottable = curve;
                    curve->setData(tdata, xdata, ydata, /*alreadySorted*/ true);
                    // set some curve styles not supported by the abstract plottable
                    if (ui->comboLineType->currentIndex() == QCPCurve::lsNone)
                        curve->setLineStyle(QCPCurve::lsNone);
                    else
                        curve->setLineStyle(QCPCurve::lsLine);
                    curve->setScatterStyle(scatterStyle);
                }

                plottable->setPen(QPen(item->backgroundColor(PlotColumnY)));
                plottable->setSelectable (QCP::stDataRange);

                // gather Y label column names
                if(column == RowNumId)
                    yAxisLabels << tr("Row #");
                else
                    yAxisLabels << model->headerData(column, Qt::Horizontal).toString();
            }
        }

        ui->plotWidget->rescaleAxes(true);

        // set axis labels
        if(x == RowNumId)
            ui->plotWidget->xAxis->setLabel(tr("Row #"));
        else
            ui->plotWidget->xAxis->setLabel(model->headerData(x, Qt::Horizontal).toString());
        ui->plotWidget->yAxis->setLabel(yAxisLabels.join("|"));
    }
    ui->plotWidget->replot();

    // Warn user if not all data has been fetched and hint about the button for loading all the data
    if (ui->plotWidget->graphCount() > 0 && model->canFetchMore()) {
        ui->buttonLoadAllData->setEnabled(true);
        ui->buttonLoadAllData->setStyleSheet("QToolButton {color: white; background-color: rgb(255, 102, 102)}");
        ui->buttonLoadAllData->setToolTip(tr("Load all data and redraw plot.\n"
                                             "Warning: not all data has been fetched from the table yet due to the partial fetch mechanism."));
        QToolTip::showText(ui->buttonLoadAllData->mapToGlobal(QPoint(0, 0)), ui->buttonLoadAllData->toolTip());
    } else {
        ui->buttonLoadAllData->setEnabled(false);
        ui->buttonLoadAllData->setStyleSheet("");
        ui->buttonLoadAllData->setToolTip(tr("Load all data and redraw plot"));
    }
}

void PlotDock::on_treePlotColumns_itemChanged(QTreeWidgetItem* changeitem, int column)
{
    // disable change updates, or we get unwanted redrawing and weird behavior
    ui->treePlotColumns->blockSignals(true);

    // make sure only 1 X axis is selected
    if(column == PlotColumnX)
    {
        for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem* item = ui->treePlotColumns->topLevelItem(i);
            if(item->checkState(column) == Qt::Checked && item != changeitem)
            {
                item->setCheckState(column, Qt::Unchecked);
            }
        }

        // Save settings for this table
        if(m_currentTableSettings)
        {
            if(changeitem->checkState(column) == Qt::Checked)
                m_currentTableSettings->plotXAxis = changeitem->text(PlotColumnField);
            else
                m_currentTableSettings->plotXAxis = QString();
        }
    } else if(column == PlotColumnY) {
        // Save check state of this column
        if(m_currentTableSettings)
        {
            PlotSettings& plot_settings = m_currentTableSettings->plotYAxes[changeitem->text(PlotColumnField)];
            plot_settings.active = (changeitem->checkState(column) == Qt::Checked);
        }

        if(changeitem->checkState(column) == Qt::Checked)
        {
            // Generate a default colour if none isn't set yet
            QColor colour = changeitem->backgroundColor(column);
            if(!colour.isValid())
            {
                static int last_colour_index = 0;
                switch(last_colour_index++)
                {
                case 0:
                    colour = QColor(0, 69, 134);
                    break;
                case 1:
                    colour = QColor(255, 66, 14);
                    break;
                case 2:
                    colour = QColor(255, 211, 32);
                    break;
                case 3:
                    colour = QColor(87, 157, 28);
                    break;
                case 4:
                    colour = QColor(126, 0, 33);
                    break;
                case 5:
                    colour = QColor(131, 202, 255);
                    break;
                case 6:
                    colour = QColor(49, 64, 4);
                    break;
                case 7:
                    colour = QColor(174, 207, 0);
                    break;
                case 8:
                    colour = QColor(75, 31, 111);
                    break;
                case 9:
                    colour = QColor(255, 149, 14);
                    break;
                case 10:
                    colour = QColor(197, 00, 11);
                    break;
                case 11:
                    colour = QColor(0, 132, 209);

                    // Since this is the last colour in our table, reset the counter back
                    // to the first colour
                    last_colour_index = 0;
                    break;
                default:
                    // NOTE: This shouldn't happen!
                    colour = QColor(0, 0, 0);
                    break;
                }
            }

            // Set colour
            changeitem->setBackgroundColor(column, colour);

            // Save settings for this table
            if(m_currentTableSettings)
            {
                PlotSettings& plot_settings = m_currentTableSettings->plotYAxes[changeitem->text(PlotColumnField)];
                plot_settings.colour = colour;
                plot_settings.lineStyle = ui->comboLineType->currentIndex();
                plot_settings.pointShape = (ui->comboPointShape->currentIndex() > 0 ? (ui->comboPointShape->currentIndex()+1) : ui->comboPointShape->currentIndex());
            }
        }
    }

    ui->treePlotColumns->blockSignals(false);

    updatePlot(m_currentPlotModel, m_currentTableSettings, false);
}

void PlotDock::on_treePlotColumns_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    // disable change updates, or we get unwanted redrawing and weird behavior
    ui->treePlotColumns->blockSignals(true);

    if(column == PlotColumnY)
    {
        // On double click open the colordialog
        QColorDialog colordialog(this);
        QColor curbkcolor = item->backgroundColor(column);
        QColor precolor = !curbkcolor.isValid() ? (Qt::GlobalColor)(qrand() % 13 + 5) : curbkcolor;
        QColor color = colordialog.getColor(precolor, this, tr("Choose an axis color"));
        if(color.isValid())
        {
            item->setCheckState(column, Qt::Checked);
            item->setBackgroundColor(column, color);

            // Save settings for this table
            if(m_currentTableSettings)
            {
                PlotSettings& plot_settings = m_currentTableSettings->plotYAxes[item->text(PlotColumnField)];
                plot_settings.active = (item->checkState(column) == Qt::Checked);
                plot_settings.colour = color;
                plot_settings.lineStyle = ui->comboLineType->currentIndex();
                plot_settings.pointShape = (ui->comboPointShape->currentIndex() > 0 ? (ui->comboPointShape->currentIndex()+1) : ui->comboPointShape->currentIndex());
            }
        } else {
            item->setCheckState(column, Qt::Unchecked);

            // Save settings for this table
            if(m_currentTableSettings)
                m_currentTableSettings->plotYAxes.remove(item->text(PlotColumnField));
        }
    }

    ui->treePlotColumns->blockSignals(false);

    updatePlot(m_currentPlotModel, m_currentTableSettings, false);
}

void PlotDock::on_butSavePlot_clicked()
{
    QString fileName = FileDialog::getSaveFileName(this,
                                                    tr("Choose a filename to save under"),
                                                    tr("PNG(*.png);;JPG(*.jpg);;PDF(*.pdf);;BMP(*.bmp);;All Files(*)")
                                                    );
    if(!fileName.isEmpty())
    {
        if(fileName.endsWith(".png", Qt::CaseInsensitive))
        {
            ui->plotWidget->savePng(fileName);
        }
        else if(fileName.endsWith(".jpg", Qt::CaseInsensitive))
        {
            ui->plotWidget->saveJpg(fileName);
        }
        else if(fileName.endsWith(".pdf", Qt::CaseInsensitive))
        {
            ui->plotWidget->savePdf(fileName);
        }
        else if(fileName.endsWith(".bmp", Qt::CaseInsensitive))
        {
            ui->plotWidget->saveBmp(fileName);
        }
        else
        {
            fileName += ".png";
            ui->plotWidget->savePng(fileName);
        }
    }
}

void PlotDock::on_comboLineType_currentIndexChanged(int index)
{
    Q_ASSERT(index >= QCPGraph::lsNone &&
             index <= QCPGraph::lsImpulse);

    bool hasCurves = (ui->plotWidget->plottableCount() > ui->plotWidget->graphCount());
    QCPGraph::LineStyle lineStyle = (QCPGraph::LineStyle) index;
    if (lineStyle > QCPGraph::lsLine && hasCurves) {
        QMessageBox::warning(this, qApp->applicationName(),
                             tr("There are curves in this plot and the selected line style can only be applied to graphs sorted by X. "
                                "Either sort the table or query by X to remove curves or select one of the styles supported by curves: "
                                "None or Line."));
        return;
    }
    for (int i = 0, ie = ui->plotWidget->graphCount(); i < ie; ++i)
    {
        QCPGraph * graph = ui->plotWidget->graph(i);
        if (graph)
            graph->setLineStyle(lineStyle);
    }
    // We have changed the style only for graphs, but not for curves.
    // If there are any in the plot, we have to update it completely in order to apply the new style
    if (hasCurves)
        updatePlot(m_currentPlotModel, m_currentTableSettings, false);
    else
        ui->plotWidget->replot();

    // Save settings for this table
    if(m_currentTableSettings)
    {
        QMap<QString, PlotSettings>& graphs = m_currentTableSettings->plotYAxes;
        auto it = graphs.begin();
        while(it != graphs.end())
        {
            it.value().lineStyle = lineStyle;
            ++it;
        }
    }
}

void PlotDock::on_comboPointShape_currentIndexChanged(int index)
{
    // WARN: because ssDot point shape is removed
    if (index > 0) index += 1;
    Q_ASSERT(index >= QCPScatterStyle::ssNone &&
             index <  QCPScatterStyle::ssPixmap);

    bool hasCurves = (ui->plotWidget->plottableCount() > ui->plotWidget->graphCount());
    QCPScatterStyle::ScatterShape shape = (QCPScatterStyle::ScatterShape) index;
    for (int i = 0, ie = ui->plotWidget->graphCount(); i < ie; ++i)
    {
        QCPGraph * graph = ui->plotWidget->graph(i);
        if (graph)
            graph->setScatterStyle(QCPScatterStyle(shape, 5));
    }
    // We have changed the style only for graphs, but not for curves.
    // If there are any in the plot, we have to update it completely in order to apply the new style
    if (hasCurves)
        updatePlot(m_currentPlotModel, m_currentTableSettings, false);
    else
        ui->plotWidget->replot();

    // Save settings for this table
    if(m_currentTableSettings)
    {
        QMap<QString, PlotSettings>& graphs = m_currentTableSettings->plotYAxes;
        auto it = graphs.begin();
        while(it != graphs.end())
        {
            it.value().pointShape = shape;
            ++it;
        }
    }
}

QVariant::Type PlotDock::guessDataType(SqliteTableModel* model, int column)
{
    QVariant::Type type = QVariant::Invalid;
    for(int i = 0; i < std::min(10, model->rowCount()) && type != QVariant::String; ++i)
    {
        QVariant data = model->data(model->index(i, column), Qt::EditRole);
        if(data.isNull() || data.convert(QVariant::Double))
        {
            type = QVariant::Double;
        } else {
            QString s = model->data(model->index(i, column)).toString();
            QDate d = QDate::fromString(s, Qt::ISODate);
            if(d.isValid())
                type = QVariant::DateTime;
            else
                type = QVariant::String;
        }
    }

    return type;
}

void PlotDock::fetchAllData()
{
    if(m_currentPlotModel)
    {
        // Show progress dialog because fetching all data might take some time
        QProgressDialog progress(tr("Fetching all data..."),
                                 tr("Cancel"), m_currentPlotModel->rowCount(), m_currentPlotModel->totalRowCount());
        progress.setWindowModality(Qt::ApplicationModal);
        progress.show();
        qApp->processEvents();

        // Make sure all data is loaded
        while(m_currentPlotModel->canFetchMore())
        {
            // Fetch the next bunch of data
            m_currentPlotModel->fetchMore();

            // Update the progress dialog and stop loading data when the cancel button was pressed
            progress.setValue(m_currentPlotModel->rowCount());
            qApp->processEvents();
            if(progress.wasCanceled())
                break;
        }

        // Update plot
        updatePlot(m_currentPlotModel, m_currentTableSettings);
    }
}

void PlotDock::selectionChanged()
{

    for (QCPAbstractPlottable* plottable : ui->plotWidget->selectedPlottables()) {

        for (QCPDataRange dataRange : plottable->selection().dataRanges()) {

            int index = dataRange.begin();
            if (dataRange.length() != 0) {
                emit pointsSelected(index, dataRange.length());
                break;
            }

        }
    }

}
void PlotDock::mousePress()
{
    // Allow user to reset the plot
    ui->buttonLoadAllData->setEnabled(true);

    // if an axis (or axis labels) is selected, only allow the direction of that axis to be dragged
    // if no axis (or axis labels) is selected, both directions may be dragged
    if (ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxisLabel))
        ui->plotWidget->axisRect()->setRangeDrag(ui->plotWidget->xAxis->orientation());
    else if (ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
             ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
             ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxisLabel))
        ui->plotWidget->axisRect()->setRangeDrag(ui->plotWidget->yAxis->orientation());
    else
        ui->plotWidget->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
}

void PlotDock::mouseWheel()
{
    // Allow user to reset the plot
    ui->buttonLoadAllData->setEnabled(true);

    // if an axis (or axis labels) is selected, only allow the direction of that axis to be zoomed
    // if no axis (or axis labels) is selected, both directions may be zoomed
    if (ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        ui->plotWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxisLabel))
        ui->plotWidget->axisRect()->setRangeZoom(ui->plotWidget->xAxis->orientation());
    else if (ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
             ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
             ui->plotWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxisLabel))
        ui->plotWidget->axisRect()->setRangeZoom(ui->plotWidget->yAxis->orientation());
    else
        ui->plotWidget->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}

void PlotDock::copy()
{
    QApplication::clipboard()->setPixmap(ui->plotWidget->toPixmap());
}
