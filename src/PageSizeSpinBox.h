#ifndef POWEROFTWOSPINBOX_H
#define POWEROFTWOSPINBOX_H

#include <QSpinBox>

class PageSizeSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    PageSizeSpinBox(QWidget* parent = Q_NULLPTR);
public slots:
    void stepBy(int steps);
private:
    int exponent;
};

#endif // POWEROFTWOSPINBOX_H
