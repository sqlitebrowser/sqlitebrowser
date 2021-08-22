#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QByteArray>

// This returns false if the data in the data parameter contains binary data. If it is text only, the function returns
// true. If the second parameter is specified, it will be used to convert the data from the given encoding to Unicode
// before doing the check. The third parameter can be used to only check the first couple of bytes which speeds up the
// text but makes it less reliable
bool isTextOnly(QByteArray data, const QString& encoding = QString(), bool quickTest = false);

// Determine if file is a text file reading first 512 bytes and calling isTextOnly().
bool isTextOnlyFile(const QString& fileName);

// This returns true if text contains some character whose direction is right-to-left.
bool containsRightToLeft(const QString& text);

// This function returns true if the data in the data parameter starts with a Unicode BOM. Otherwise it returns false.
bool startsWithBom(const QByteArray& data);

// This function checks if the data in the data parameter starts with a Unicode BOM. If so, the BOM is removed from the
// byte array and passed back to the caller separately as the return value of the function. If the data does not start
// with a BOM an empty byte array is returned and the original data is not modified.
QByteArray removeBom(QByteArray& data);

// Check if a byte array contains an image. Returns the name of the image format for images or a null string for non-image data.
QString isImageData(const QByteArray& data);

QStringList toStringList(const QList<QByteArray>& list);

QByteArray encodeString(const QByteArray& str, const QString& encoding);

QByteArray decodeString(const QByteArray& str, const QString& encoding);

QString humanReadableSize(unsigned long byteCount);

QString isoDateTimeStringToLocalDateTimeString(const QString& date_string);

// Helper function for removing all comments from a SQL query
void removeCommentsFromQuery(QString& query);

#endif
