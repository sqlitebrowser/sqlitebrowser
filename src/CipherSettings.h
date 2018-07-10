#ifndef CIPHERSETTINGS_H
#define CIPHERSETTINGS_H

#include <QString>

class CipherSettings
{
public:
    enum KeyFormats
    {
        Passphrase,
        RawKey
    };

    static const int defaultPageSize = 1024;

    KeyFormats getKeyFormat() const;
    void setKeyFormat(const KeyFormats &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    int getPageSize() const;
    void setPageSize(int value);

    static KeyFormats getKeyFormat(int rawKeyFormat);

private:
    KeyFormats keyFormat;
    QString password;
    int pageSize;
};

#endif // CIPHERSETTINGS_H
