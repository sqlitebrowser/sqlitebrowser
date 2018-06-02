#ifndef CIPHERSETTINGS_H
#define CIPHERSETTINGS_H

class CipherSettings
{
public:
    enum KeyFormats
    {
        Passphrase,
        RawKey
    };

    static const int defaultPageSize = 1024;
};

#endif // CIPHERSETTINGS_H
