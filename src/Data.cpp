#include "Data.h"

#include <QTextCodec>

bool isTextOnly(QByteArray data, const QString& encoding, bool quickTest)
{
    // If the data starts with a Unicode BOM, we always assume it is text
    if(startsWithBom(data))
        return true;

    // Truncate to the first couple of bytes for quick testing
    if(quickTest)
        data = data.left(512);

    // Convert to Unicode if necessary
    if(!encoding.isEmpty())
        data = QTextCodec::codecForName(encoding.toUtf8())->toUnicode(data).toUtf8();

    // Perform check
    return QString(data).toUtf8() == data;
}

bool startsWithBom(const QByteArray& data)
{
    // Note that these aren't all possible BOMs. But they are probably the most common ones.

    if(data.startsWith("\xEF\xBB\xBF") ||
            data.startsWith("\xFE\xFF") || data.startsWith("\xFF\xFE") ||
            data.startsWith("\x00\x00\xFE\xFF") || data.startsWith("\xFF\xFE\x00\x00"))
        return true;
    else
        return false;
}

QByteArray removeBom(QByteArray& data)
{
    if(data.left(3) == QByteArray("\xEF\xBB\xBF"))
    {
        QByteArray bom = data.left(3);
        data.remove(0, 3);
        return bom;
    } else if(data.left(2) == QByteArray("\xFE\xFF") || data.left(2) == QByteArray("\xFF\xFE")) {
        QByteArray bom = data.left(2);
        data.remove(0, 2);
        return bom;
    } else if(data.left(4) == QByteArray("\x00\x00\xFE\xFF") || data.left(4) == QByteArray("\xFF\xFE\x00\x00")) {
        QByteArray bom = data.left(4);
        data.remove(0, 4);
        return bom;
    } else {
        return QByteArray();
    }
}
