#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>

// Class providing series of colours in a given dark or light side of
// the spectrum.
class Palette
{
public:
    Palette();

    QColor nextSerialColor(bool dark = true);

    static bool appHasDarkTheme();

private:
    int m_lastColourIndex;
};

#endif
