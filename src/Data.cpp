#include "Data.h"

#include <QBuffer>
#include <QDateTime>
#include <QImageReader>
#include <QLocale>
#include <QTextCodec>
#include <QFile>
#include <QRegularExpression>

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
    QByteArray::size_type testSize = quickTest? std::min(QByteArray::size_type(512), data.size()) : data.size();

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

bool isTextOnlyFile(const QString& fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.read(512);
        return !data.isEmpty() && isTextOnly(data);
    } else {
        return false;
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

void removeCommentsFromQuery(QString& query)
{
    // Store the current size so we can easily check later if the string has been changed
    int oldSize = query.size();

    // This implements a simple state machine to strip the query from comments
    QChar quote;
    for(int i=0;i<query.size();i++)
    {
        // Are we in quote state?
        if(quote.isNull())
        {
            // We are currently not in quote state

            // So are we starting a quote?
            if((query.at(i) == '\'' || query.at(i) == '\"' || query.at(i) == '[') && (i == 0 || query.at(i-1) != '\\'))
            {
                // Quoted text is beginning. Switch to the quote state

                quote = query.at(i);
            } else if(query.at(i) == '-' && i+1 < query.size() && query.at(i+1) == '-') {
                // This is an end of line comment. Remove anything till the end of the line or the end of the string if this is the last line

                int pos_next_line_break = query.indexOf('\n', i);
                if(pos_next_line_break == -1)
                    query = query.left(i);
                else
                    query.remove(i, pos_next_line_break - i);       // The \n is left in intentionally
            } else if(query.at(i) == '/' && i+1 < query.size() && query.at(i+1) == '*') {
                // This is a block comment. Remove anything till the end of the block or the end of the string if the block is not closed
                int pos_end_comment = query.indexOf("*/", i);
                if(pos_end_comment == -1)
                    query = query.left(i);
                else
                    query.remove(i, pos_end_comment - i + 2);       // Add 2 to include the */
            }
        } else {
            // We are currently in quote state

            // If this is the closing quote character, switch back to normal state
            if((query.at(i) == quote) && (i == 0 || query.at(i-1) != '\\'))
                quote = QChar(0);
        }
    }

    query = query.trimmed();

    if (oldSize != query.size()) {
        // Remove multiple line breaks that might have been created by deleting comments till the end of the line but not including the line break
        query.replace(QRegularExpression("\\n+"), "\n");

        // Also remove any remaining whitespace at the end of each line
        query.replace(QRegularExpression("[ \t]+\n"), "\n");
    }
}
