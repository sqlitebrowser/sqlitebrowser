#ifndef PLOTDOCK_H
#define PLOTDOCK_H

#include <QDialog>
#include <QVariant>

class SqliteTableModel;
class QTreeWidgetItem;
struct BrowseDataTableSettings;

namespace Ui {
class PlotDock;
}

class PlotDock : public QDialog
{
    Q_OBJECT

public:
    explicit PlotDock(QWidget* parent = 0);
    ~PlotDock();

    struct PlotSettings
    {
        int lineStyle;
        int pointShape;
        QColor colour;

        friend QDataStream& operator<<(QDataStream& stream, const PlotDock::PlotSettings& object)
        {
            stream << object.lineStyle;
            stream << object.pointShape;
            stream << object.colour;

            return stream;
        }
        friend QDataStream& operator>>(QDataStream& stream, PlotDock::PlotSettings& object)
        {
            stream >> object.lineStyle;
            stream >> object.pointShape;
            stream >> object.colour;

            return stream;
        }
    };

public slots:
    void updatePlot(SqliteTableModel* model, BrowseDataTableSettings* settings = nullptr, bool update = true, bool keepOrResetSelection = true);
    void fetchAllData();

private:
    enum PlotColumns
    {
        PlotColumnField = 0,
        PlotColumnX = 1,
        PlotColumnY = 2,
        PlotColumnDummy = 3
    };

    Ui::PlotDock* ui;

    SqliteTableModel* m_currentPlotModel;
    BrowseDataTableSettings* m_currentTableSettings;

    /*!
     * \brief guessdatatype try to parse the first 10 rows and decide the datatype
     * \param model model to check the data
     * \param column index of the column to check
     * \return the guessed datatype
     */
    QVariant::Type guessDataType(SqliteTableModel* model, int column);

private slots:
    void on_treePlotColumns_itemChanged(QTreeWidgetItem* item, int column);
    void on_treePlotColumns_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_butSavePlot_clicked();
    void on_comboLineType_currentIndexChanged(int index);
    void on_comboPointShape_currentIndexChanged(int index);
};

#endif
