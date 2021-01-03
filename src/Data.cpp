#include "Data.h"

#include <QBuffer>
#include <QDateTime>
#include <QImageReader>
#include <QLocale>
#include <QTextCodec>

#include <algorithm>

// Note that these aren't all possible BOMs. But they are probably the most common ones.
// The size is needed at least for the ones with character zero in them.
static const QByteArray bom3("\xEF\xBB\xBF", 3);
static const QByteArray bom2a("\xFE\xFF", 2);
static const QByteArray bom2b("\xFF\xFE", 2);
static const QByteArray bom4a("\x00\x00\xFE\xFF", 4);
static const QByteArray bom4b("\xFF\xFE\x00\x00", 4);

bool isTextOnly(QByteArray data, const QString& encoding, bool quickTest)
{
    // If the data starts with a Unicode BOM, we can use detection provided by QTextCodec.
    if(startsWithBom(data)) {
        QTextCodec *codec = encoding.isEmpty()? QTextCodec::codecForName("UTF-8") : QTextCodec::codecForName(encoding.toUtf8());
        QTextCodec *detectedCodec = QTextCodec::codecForUtfText(data, nullptr);
        return detectedCodec == codec;
    }

    // Truncate to the first few bytes for quick testing
    int testSize = quickTest? std::min(512, data.size()) : data.size();

    // If the quick test has been requested and we have to truncate the string, we have to use
    // an approach where truncated multibyte characters are not interpreted as invalid characters.
    if(quickTest && data.size() > testSize) {

        // We can assume that the default encoding (UTF-8) and all the ISO-8859
        // cannot contain character zero.
        // This has to be checked explicitly because toUnicode() is using zero as
        // a terminator for these encodings.
        if((encoding.isEmpty() || encoding.startsWith("ISO-8859")) && data.contains('\0'))
            return false;

        QTextCodec::ConverterState state;
        QTextCodec *codec = encoding.isEmpty()? QTextCodec::codecForName("UTF-8") : QTextCodec::codecForName(encoding.toUtf8());
        codec->toUnicode(data.constData(), testSize, &state);
        return state.invalidChars == 0;
    } else {
        // Convert to Unicode if necessary
        data = decodeString(data, encoding);
        QString text (data);
        bool isPrintable = true;
        for(QChar character : text) {
            if(character.isNonCharacter() || (character.category() == QChar::Other_Control && !character.isSpace())) {
                isPrintable = false;
                break;
            }
        }
        // Perform check
        return isPrintable && text.toUtf8() == data;
    }
}

bool containsRightToLeft(const QString& text) {

    for(QChar ch : text) {
        switch(ch.direction()) {
        case QChar::DirR:
        case QChar::DirAL:
        case QChar::DirRLE:
        case QChar::DirRLO:
        case QChar::DirRLI:
            return true;
        default:
            break;
        }
    }
    return false;
}

bool startsWithBom(const QByteArray& data)
{
    if(data.startsWith(bom3) ||
            data.startsWith(bom2a) || data.startsWith(bom2b) ||
            data.startsWith(bom4a) || data.startsWith(bom4b))
    return true;
    else
        return false;
}

QByteArray removeBom(QByteArray& data)
{
    if(data.startsWith(bom3))
    {
        QByteArray bom = data.left(3);
        data.remove(0, 3);
        return bom;
    } else if(data.startsWith(bom2a) || data.startsWith(bom2b)) {
        QByteArray bom = data.left(2);
        data.remove(0, 2);
        return bom;
    } else if(data.startsWith(bom4a) || data.startsWith(bom4b)) {
        QByteArray bom = data.left(4);
        data.remove(0, 4);
        return bom;
    } else {
        return QByteArray();
    }
}

QString isImageData(const QByteArray& data)
{
    // Check if it's an image. First do a quick test by calling canRead() which only checks the first couple of bytes or so. Only if
    // that returned true, do a more sophisticated test of the data. This way we get both, good performance and proper data checking.
    QBuffer imageBuffer(const_cast<QByteArray*>(&data));
    QImageReader readerBuffer(&imageBuffer);
    QString imageFormat = readerBuffer.format();
    if(readerBuffer.canRead() && !readerBuffer.read().isNull())
        return imageFormat;
    else
        return QString();
}

QStringList toStringList(const QList<QByteArray>& list) {
    QStringList strings;
    for (const QByteArray &item : list) {
        strings.append(QString::fromUtf8(item));
    }
    return strings;
}

QByteArray encodeString(const QByteArray& str, const QString& encoding)
{
    if(encoding.isEmpty())
        return str;
    else
        return QTextCodec::codecForName(encoding.toUtf8())->fromUnicode(str);
}

QByteArray decodeString(const QByteArray& str, const QString& encoding)
{
    if(encoding.isEmpty())
        return str;
    else
        return QTextCodec::codecForName(encoding.toUtf8())->toUnicode(str).toUtf8();
}

QString humanReadableSize(unsigned long byteCount)
{
    static const std::vector<QString> units = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};

    double size = static_cast<double>(byteCount);
    for(const QString& unit : units)
    {
        if(size < 1024.0)
            return QString::number(size, 'f', 2) + " " + unit;

        size /= 1024.0;
    }

    return QString::number(size, 'f', 2) + " YiB";
}

QString isoDateTimeStringToLocalDateTimeString(const QString& date_string)
{
    return QLocale::system().toString(QDateTime::fromString(date_string, Qt::ISODate).toLocalTime(), QLocale::ShortFormat);
}
