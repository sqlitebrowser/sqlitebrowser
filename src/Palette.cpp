#include "Palette.h"

#include <QPalette>

Palette::Palette()
    : m_lastColourIndex(0)
{
}

QColor Palette::nextSerialColor(bool dark)
{
    if (dark) {
        switch(m_lastColourIndex++)
        {
        case 0:
            return QColor(0, 69, 134);
        case 1:
            return QColor(255, 66, 14);
        case 2:
            return QColor(255, 211, 32);
        case 3:
            return QColor(87, 157, 28);
        case 4:
            return QColor(126, 0, 33);
        case 5:
            return QColor(131, 202, 255);
        case 6:
            return QColor(49, 64, 4);
        case 7:
            return QColor(174, 207, 0);
        case 8:
            return QColor(75, 31, 111);
        case 9:
            return QColor(255, 149, 14);
        case 10:
            return QColor(197, 00, 11);
        case 11:

            // Since this is the last colour in our table, reset the counter back
            // to the first colour
            m_lastColourIndex = 0;

            return QColor(0, 132, 209);
        default:
            // NOTE: This shouldn't happen!
            m_lastColourIndex = 0;
            return QColor(0, 0, 0);
        }
    } else {
        // TODO: review the bright colours
        switch(m_lastColourIndex++)
        {
        case 0:
            return QColor(Qt::yellow);
        case 1:
            return QColor(Qt::cyan);
        case 2:
            return QColor(Qt::green);
        case 3:
            return QColor(Qt::magenta);
        case 4:
            return QColor(Qt::lightGray);
        case 5:
            return QColor("beige");
        case 6:
            return QColor("lightblue");
        case 7:
            return QColor("turquoise");
        case 8:
            return QColor("mediumspringgreen");
        case 9:
            return QColor("lightskyblue");
        case 10:
            return QColor("moccasin");
        case 11:

            // Since this is the last colour in our table, reset the counter back
            // to the first colour
            m_lastColourIndex = 0;

            return QColor("pink");
        default:
            // NOTE: This shouldn't happen!
            return QColor(0, 0, 0);
        }
    }
}

bool Palette::appHasDarkTheme()
{
    QColor backgroundColour = QPalette().color(QPalette::Active, QPalette::Base);
    QColor foregroundColour = QPalette().color(QPalette::Active, QPalette::Text);
    return backgroundColour.value() < foregroundColour.value();
}
