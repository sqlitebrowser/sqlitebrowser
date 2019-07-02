#ifndef CIPHERSETTINGS_H
#define CIPHERSETTINGS_H

#include <QString>

class CipherSettings
{
public:
    CipherSettings();

    enum KeyFormats
    {
        Passphrase,
        RawKey
    };

    KeyFormats getKeyFormat() const;
    void setKeyFormat(const KeyFormats &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    int getPageSize() const;
    void setPageSize(int value);

    int getKdfIterations() const { return kdfIterations; }
    void setKdfIterations(int value) { kdfIterations = value; }

    QString getHmacAlgorithm() const { return hmacAlgorithm; }
    void setHmacAlgorithm(const QString &value) { hmacAlgorithm = value; }

    QString getKdfAlgorithm() const { return kdfAlgorithm; }
    void setKdfAlgorithm(const QString &value) { kdfAlgorithm = value; }

    static KeyFormats getKeyFormat(int rawKeyFormat);

private:
    KeyFormats keyFormat;
    QString password;
    int pageSize;
    int kdfIterations;
    QString hmacAlgorithm;
    QString kdfAlgorithm;
};

#endif // CIPHERSETTINGS_H
