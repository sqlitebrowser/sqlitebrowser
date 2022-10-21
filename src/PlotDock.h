#ifndef PLOTDOCK_H
#define PLOTDOCK_H

#include "Palette.h"

#include <QDialog>
#include <QVariant>

#include <vector>

class QMenu;
class QPrinter;
class QTreeWidgetItem;
class QCPAxis;

class SqliteTableModel;
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
        PlotColumnY1 = 2,
        PlotColumnY2 = 3,
        PlotColumnType = 4,
    };

    Ui::PlotDock* ui;

    SqliteTableModel* m_currentPlotModel;
    BrowseDataTableSettings* m_currentTableSettings;
    QMenu* m_contextMenu;
    bool m_showLegend;
    bool m_stackedBars;
    bool m_fixedFormat;
    Palette m_graphPalette;
    std::vector<QCPAxis *> yAxes;
    std::vector<int> PlotColumnY;

    /*!
     * \brief guessdatatype try to parse the first 10 rows and decide the datatype
     * \param model model to check the data
     * \param column index of the column to check
     * \return the guessed datatype
     */
    QVariant::Type guessDataType(SqliteTableModel* model, int column) const;
    void adjustBars();
    void adjustAxisFormat();

private slots:
    void columnItemChanged(QTreeWidgetItem* item, int column);
    void columnItemDoubleClicked(QTreeWidgetItem* item, int column);
    void savePlot();
    void lineTypeChanged(int index);
    void pointShapeChanged(int index);
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
