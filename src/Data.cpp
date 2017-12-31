#include "Data.h"

#include <QTextCodec>

bool isTextOnly(QByteArray data, const QString& encoding, bool quickTest)
{
    // Truncate to the first couple of bytes for quick testing
    if(quickTest)
        data = data.left(512);

    // Convert to Unicode if necessary
    if(!encoding.isEmpty())
        data = QTextCodec::codecForName(encoding.toUtf8())->toUnicode(data).toUtf8();

    // Perform check
    return QString(data).toUtf8() == data;
}
