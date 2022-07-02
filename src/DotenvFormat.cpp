#include "DotenvFormat.h"

#include <QRegularExpression>
#include <QTextStream>

bool DotenvFormat::readEnvFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QTextStream in(&device);

    QString line;

    QRegularExpression keyValueRegex("^\\s*([^=]+)\\s*=\\s*(.*)\\s*$");

    while (in.readLineInto(&line)) {
        QRegularExpressionMatch match = keyValueRegex.match(line);

        if (match.capturedLength() < 3) {
            continue;
        }

        QString key = match.captured(1);
        QString value = match.captured(2);

        map.insert(key, value);
    }

    return true;
}
