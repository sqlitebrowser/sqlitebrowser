#ifndef FILTERTABLEHEADER_H
#define FILTERTABLEHEADER_H

#include <QHeaderView>
#include <QList>

class QLineEdit;
class QTableView;
class FilterLineEdit;

class FilterTableHeader : public QHeaderView
{
    Q_OBJECT

public:
    explicit FilterTableHeader(QTableView* parent = nullptr);
    QSize sizeHint() const override;
    bool hasFilters() const {return (filterWidgets.count() > 0);}
    
public slots:
    void generateFilters(int number, bool showFirst = false);
    void adjustPositions();
    void clearFilters();
    void setFilter(int column, const QString& value);

signals:
    void filterChanged(int column, QString value);
    void addCondFormat(int column, QString filter);
    void clearAllCondFormats(int column);
    void editCondFormats(int column);

protected:
    void updateGeometries() override;

private slots:
    void inputChanged(const QString& new_value);
    void addFilterAsCondFormat(const QString& filter);
    void clearAllCondFormats();
    void editCondFormats();

private:
    QList<FilterLineEdit*> filterWidgets;
};

#endif
