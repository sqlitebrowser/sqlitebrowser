#ifndef FILTERLINEEDIT_H
#define FILTERLINEEDIT_H

#include <QLineEdit>
#include <vector>

class QTimer;
class QKeyEvent;

class FilterLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit FilterLineEdit(QWidget* parent, std::vector<FilterLineEdit*>* filters, size_t columnnum);

    // Override methods for programatically changing the value of the line edit
    void clear();
    void setText(const QString& text);

private slots:
    void delayedSignalTimerTriggered();

signals:
    void delayedTextChanged(QString text);
    void addFilterAsCondFormat(QString text);
    void clearAllCondFormats();
    void editCondFormats();

protected:
    void keyReleaseEvent(QKeyEvent* event) override;
    void setFilterHelper(const QString& filterOperator, const QString& operatorSuffix = "");

private:
    std::vector<FilterLineEdit*>* filterList;
    size_t columnNumber;
    QTimer* delaySignalTimer;
    QString lastValue;

private slots:
    void showContextMenu(const QPoint &pos);
};

#endif
