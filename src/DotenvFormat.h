#ifndef DOTENVFORMAT_H
#define DOTENVFORMAT_H

#include <QSettings>

class QIODevice;

class DotenvFormat
{
public:
    static bool readEnvFile(QIODevice &device, QSettings::SettingsMap &map);
};

#endif // DOTENVFORMAT_H
