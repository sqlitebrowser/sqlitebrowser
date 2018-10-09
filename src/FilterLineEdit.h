#ifndef FILTERLINEEDIT_H
#define FILTERLINEEDIT_H

#include <QLineEdit>
#include <QList>

class QTimer;
class QKeyEvent;

class FilterLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit FilterLineEdit(QWidget* parent, QList<FilterLineEdit*>* filters, int columnnum);

    // Override methods for programatically changing the value of the line edit
    void clear();
    void setText(const QString& text);

private slots:
    void delayedSignalTimerTriggered();

signals:
    void delayedTextChanged(QString text);

protected:
    void keyReleaseEvent(QKeyEvent* event) override;
    void setFilterHelper(const QString& filterOperator);

private:
    QList<FilterLineEdit*>* filterList;
    int columnNumber;
    QTimer* delaySignalTimer;
    QString lastValue;

private slots:
    void showContextMenu(const QPoint &pos);
};

#endif
