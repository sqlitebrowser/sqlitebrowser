#ifndef DATA_H
#define DATA_H

#include <QString>

// This returns false if the data in the data parameter contains binary data. If it is text only, the function returns
// true. If the second parameter is specified, it will be used to convert the data from the given encoding to Unicode
// before doing the check. The third parameter can be used to only check the first couple of bytes which speeds up the
// text but makes it less reliable
bool isTextOnly(QByteArray data, const QString& encoding = QString(), bool quickTest = false);

#endif
