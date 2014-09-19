#ifndef FILTERTABLEHEADER_H
#define FILTERTABLEHEADER_H

#include <QHeaderView>
#include <QList>

class QLineEdit;
class QTableView;

class FilterTableHeader : public QHeaderView
{
    Q_OBJECT

public:
    explicit FilterTableHeader(QTableView* parent = 0);
    virtual QSize sizeHint() const;
    
public slots:
    void generateFilters(int number, bool bKeepValues = false);
    void adjustPositions();

signals:
    void filterChanged(int column, QString value);

protected:
    virtual void updateGeometries();

private slots:
    void inputChanged(const QString& new_value);

private:
    QList<QLineEdit*> filterWidgets;
};

#endif
