#include "PageSizeSpinBox.h"

#include <QtCore/qmath.h>

PageSizeSpinBox::PageSizeSpinBox(QWidget* parent) : QSpinBox(parent)
{
    setMinimum(512);
    setMaximum(65536);
    setValue(1024);

    exponent = 10;
}

void PageSizeSpinBox::stepBy(int steps)
{
    switch(steps) {
    case 1:
        exponent++;
        break;
    case -1:
        exponent--;
        break;
    default:
        QSpinBox::stepBy(steps);
        return;
    }

    int newValue = static_cast<int>(qPow(2, exponent));
    setValue(newValue);
}
