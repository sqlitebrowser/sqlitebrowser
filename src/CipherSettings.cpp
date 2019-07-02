#include "CipherSettings.h"

CipherSettings::CipherSettings()
    : keyFormat(Passphrase),
      pageSize(0),
      kdfIterations(0)
{
}

CipherSettings::KeyFormats CipherSettings::getKeyFormat() const
{
    return keyFormat;
}

void CipherSettings::setKeyFormat(const KeyFormats &value)
{
    keyFormat = value;
}

QString CipherSettings::getPassword() const
{
    if(keyFormat == Passphrase)
    {
        QString tempPassword = password;

        tempPassword.replace("'", "''");

        return QString("'%1'").arg(tempPassword);
    } else {
        // Remove the '0x' part at the beginning
        return QString("\"x'%1'\"").arg(password.mid(2));
    }
}

void CipherSettings::setPassword(const QString &value)
{
    password = value;
}

int CipherSettings::getPageSize() const
{
    return pageSize;
}

void CipherSettings::setPageSize(int value)
{
    pageSize = value;
}

CipherSettings::KeyFormats CipherSettings::getKeyFormat(int rawKeyFormat)
{
    return static_cast<CipherSettings::KeyFormats>(rawKeyFormat);
}
