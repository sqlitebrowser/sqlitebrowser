#ifndef DOTENVFORMAT_H
#define DOTENVFORMAT_H

#include <QIODevice>
#include <QSettings>

class DotenvFormat
{
public:
    static bool readEnvFile(QIODevice &device, QSettings::SettingsMap &map);
};

#endif // DOTENVFORMAT_H
