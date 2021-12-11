#include "CipherSettings.h"
#include "sqlitedb.h"

CipherSettings::CipherSettings()
    : keyFormat(Passphrase),
      pageSize(0),
      kdfIterations(0),
      plaintextHeaderSize(0)
{
}

std::string CipherSettings::getPassword() const
{
    if(keyFormat == Passphrase)
    {
        return sqlb::escapeString(password);
    } else {
        // Remove the '0x' part at the beginning
        if (password.length() > 2) {
            return "\"x'" + password.substr(2) + "'\"";
        }

        return "''";
    }
}

CipherSettings::KeyFormats CipherSettings::getKeyFormat(int rawKeyFormat)
{
    return static_cast<CipherSettings::KeyFormats>(rawKeyFormat);
}
