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
    explicit FilterLineEdit(QWidget* parent, std::vector<FilterLineEdit*>* filters = nullptr, size_t columnnum = 0);

    // Override methods for programmatically changing the value of the line edit
    void clear();
    void setText(const QString& text);

    void setConditionFormatContextMenuEnabled(bool enable) { conditional_format = enable; }

private slots:
    void delayedSignalTimerTriggered();

signals:
    void delayedTextChanged(QString text);
    void addFilterAsCondFormat(QString text);
    void clearAllCondFormats();
    void editCondFormats();
    void filterFocused();

protected:
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void setFilterHelper(const QString& filterOperator, const QString& operatorSuffix = QString());

private:
    std::vector<FilterLineEdit*>* filterList;
    size_t columnNumber;
    QTimer* delaySignalTimer;
    QString lastValue;
    bool conditional_format;

private slots:
    void showContextMenu(const QPoint &pos);
};

#endif
