#ifndef PLOTDOCK_H
#define PLOTDOCK_H

#include "Palette.h"

#include <QDialog>
#include <QVariant>
#include <QMenu>

class SqliteTableModel;
class QTreeWidgetItem;
class QPrinter;
struct BrowseDataTableSettings;

namespace Ui {
class PlotDock;
}

class PlotDock : public QDialog
{
    Q_OBJECT

public:
    explicit PlotDock(QWidget* parent = nullptr);
    ~PlotDock() override;

    struct PlotSettings
    {
        int lineStyle;
        int pointShape;
        QColor colour;
        bool active;

        PlotSettings() :
            lineStyle(0),
            pointShape(0),
            active(false)
        {}

        PlotSettings(int _lineStyle, int _pointShape, QColor _colour, bool _active) :
            lineStyle(_lineStyle),
            pointShape(_pointShape),
            colour(_colour),
            active(_active)
        {}

        friend QDataStream& operator<<(QDataStream& stream, const PlotDock::PlotSettings& object)
        {
            stream << object.lineStyle;
            stream << object.pointShape;
            stream << object.colour;
            stream << object.active;

            return stream;
        }
        friend QDataStream& operator>>(QDataStream& stream, PlotDock::PlotSettings& object)
        {
            stream >> object.lineStyle;
            stream >> object.pointShape;
            stream >> object.colour;

            if(!stream.atEnd())
                stream >> object.active;

            return stream;
        }
    };

public slots:
    void updatePlot(SqliteTableModel* model, BrowseDataTableSettings* settings = nullptr, bool update = true, bool keepOrResetSelection = true);
    void fetchAllData();
    void resetPlot();
    void reject() override;

signals:
    void pointsSelected(int firstIndex, int count);

private:
    enum PlotColumns
    {
        PlotColumnField = 0,
        PlotColumnX = 1,
        PlotColumnY = 2,
        PlotColumnType = 3,
    };

    Ui::PlotDock* ui;

    SqliteTableModel* m_currentPlotModel;
    BrowseDataTableSettings* m_currentTableSettings;
    QMenu* m_contextMenu;
    bool m_showLegend;
    bool m_stackedBars;
    Palette m_graphPalette;

    /*!
     * \brief guessdatatype try to parse the first 10 rows and decide the datatype
     * \param model model to check the data
     * \param column index of the column to check
     * \return the guessed datatype
     */
    QVariant::Type guessDataType(SqliteTableModel* model, int column);
    void adjustBars();

private slots:
    void on_treePlotColumns_itemChanged(QTreeWidgetItem* item, int column);
    void on_treePlotColumns_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_butSavePlot_clicked();
    void on_comboLineType_currentIndexChanged(int index);
    void on_comboPointShape_currentIndexChanged(int index);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void copy();
    void toggleLegendVisible(bool visible);
    void toggleStackedBars(bool stacked);
    void openPrintDialog();
    void renderPlot(QPrinter* printer);
};

#endif
