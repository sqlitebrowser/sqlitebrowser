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
            break;
        case 1:
            return QColor(255, 66, 14);
            break;
        case 2:
            return QColor(255, 211, 32);
            break;
        case 3:
            return QColor(87, 157, 28);
            break;
        case 4:
            return QColor(126, 0, 33);
            break;
        case 5:
            return QColor(131, 202, 255);
            break;
        case 6:
            return QColor(49, 64, 4);
            break;
        case 7:
            return QColor(174, 207, 0);
            break;
        case 8:
            return QColor(75, 31, 111);
            break;
        case 9:
            return QColor(255, 149, 14);
            break;
        case 10:
            return QColor(197, 00, 11);
            break;
        case 11:

            // Since this is the last colour in our table, reset the counter back
            // to the first colour
            m_lastColourIndex = 0;

            return QColor(0, 132, 209);
            break;
        default:
            // NOTE: This shouldn't happen!
            m_lastColourIndex = 0;
            return QColor(0, 0, 0);
            break;
        }
    } else {
        // TODO: review the bright colours
        switch(m_lastColourIndex++)
        {
        case 0:
            return QColor(Qt::yellow);
            break;
        case 1:
            return QColor(Qt::cyan);
            break;
        case 2:
            return QColor(Qt::green);
            break;
        case 3:
            return QColor(Qt::magenta);
            break;
        case 4:
            return QColor(Qt::lightGray);
            break;
        case 5:
            return QColor("beige");
            break;
        case 6:
            return QColor("lightblue");
            break;
        case 7:
            return QColor("turquoise");
            break;
        case 8:
            return QColor("mediumspringgreen");
            break;
        case 9:
            return QColor("lightskyblue");
            break;
        case 10:
            return QColor("moccasin");
            break;
        case 11:

            // Since this is the last colour in our table, reset the counter back
            // to the first colour
            m_lastColourIndex = 0;

            return QColor("pink");
            break;
        default:
            // NOTE: This shouldn't happen!
            return QColor(0, 0, 0);
            break;
        }
    }
}

bool Palette::appHasDarkTheme()
{
    QColor backgroundColour = QPalette().color(QPalette::Active, QPalette::Base);
    QColor foregroundColour = QPalette().color(QPalette::Active, QPalette::Text);
    return backgroundColour.value() < foregroundColour.value();
}
