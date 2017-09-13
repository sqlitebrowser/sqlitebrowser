#include "csvparser.h"

#include <QTextStream>
#include <algorithm>

CSVParser::CSVParser(bool trimfields, char16_t fieldseparator, char16_t quotechar)
    : m_bTrimFields(trimfields)
    , m_cFieldSeparator(fieldseparator)
    , m_cQuoteChar(quotechar)
    , m_pCSVProgress(0)
    , m_nBufferSize(4096)
{
}

CSVParser::~CSVParser()
{
    delete m_pCSVProgress;
}

namespace {
inline void addColumn(QVector<QByteArray>& r, QString& field, bool trim)
{
    if(trim)
        r.push_back(field.trimmed().toUtf8());
    else
        r.push_back(field.toUtf8());

    field.clear();
    field.reserve(128);
}

inline bool addRow(CSVParser::csvRowFunction& f, QVector<QByteArray>& r, size_t& rowCount)
{
    if(!f(rowCount, r))
        return false;

    r.clear();
    rowCount++;
    return true;
}
}

CSVParser::ParserResult CSVParser::parse(csvRowFunction insertFunction, QTextStream& stream, size_t nMaxRecords)
{
    ParseStates state = StateNormal;        // State of the parser
    QString sBuffer;                        // Buffer for reading in the file
    QString fieldbuf;                       // Buffer for parsing the current field
    QVector<QByteArray> record;             // Buffer for parsing the current row
    size_t parsedRows = 0;                  // Number of rows parsed so far

    if(m_pCSVProgress)
        m_pCSVProgress->start();

    while(!stream.atEnd())
    {
        sBuffer = stream.read(m_nBufferSize);
        auto sBufferEnd = sBuffer.constEnd();

        for(auto it = sBuffer.constBegin(); it != sBufferEnd; ++it)
        {
            // Get next char
            char16_t c = it->unicode();

            switch(state)
            {
            case StateNormal:
            {
                if(c == m_cFieldSeparator)
                {
                    addColumn(record, fieldbuf, m_bTrimFields);
                }
                else if(c == m_cQuoteChar)
                {
                    state = StateInQuote;
                }
                else if(c == '\r')
                {
                    // look ahead to check for linefeed
                    auto nit = it + 1;

                    // In order to check what the next byte is we must make sure that that byte is already loaded. Assume we're at an m_nBufferSize
                    // boundary but not at the end of the file when we hit a \r character. Now we're going to be at the end of the sBuffer string
                    // because of the m_nBufferSize boundary. But this means that the following check won't work properly because we can't check the
                    // next byte when we really should be able to do so because there's more data coming. To fix this we'll check for this particular
                    // case and, if this is what's happening, we'll just load an extra byte.
                    if(nit == sBufferEnd && !stream.atEnd())
                    {
                        // Load one more byte
                        sBuffer.append(stream.read(1));
                        sBufferEnd = sBuffer.constEnd();

                        // Restore both iterators. sBufferEnd points to the imagined char after the last one in the string. So the extra byte we've
                        // just loaded is the one before that, i.e. the actual last one, and the original last char is the one before that.
                        it = sBufferEnd - 2;
                        nit = sBufferEnd - 1;
                    }

                    // no linefeed, so assume that CR represents a newline
                    if(nit != sBufferEnd && *nit != '\n')
                    {
                        addColumn(record, fieldbuf, m_bTrimFields);

                        if(!addRow(insertFunction, record, parsedRows))
                            return ParserResult::ParserResultError;
                    }
                }
                else if(c == '\n')
                {
                    addColumn(record, fieldbuf, m_bTrimFields);

                    if(!addRow(insertFunction, record, parsedRows))
                        return ParserResult::ParserResultError;
                }
                else
                {
                    fieldbuf.append(c);
                }
            }
            break;
            case StateInQuote:
            {
                if(c == m_cQuoteChar)
                {
                    state = StateEndQuote;
                }
                else
                {
                    fieldbuf.append(c);
                }
            }
            break;
            case StateEndQuote:
            {
                if(c == m_cQuoteChar)
                {
                    state = StateInQuote;
                    fieldbuf.append(c);
                }
                else if(c == m_cFieldSeparator)
                {
                    state = StateNormal;
                    addColumn(record, fieldbuf, m_bTrimFields);
                }
                else if(c == '\n')
                {
                    state = StateNormal;
                    addColumn(record, fieldbuf, m_bTrimFields);

                    if(!addRow(insertFunction, record, parsedRows))
                        return ParserResult::ParserResultError;
                }
                else if(c == '\r')
                {
                    // look ahead to check for linefeed
                    auto nit = it + 1;

                    // See above for details on this.
                    if(nit == sBufferEnd && !stream.atEnd())
                    {
                        sBuffer.append(stream.read(1));
                        sBufferEnd = sBuffer.constEnd();
                        it = sBufferEnd - 2;
                        nit = sBufferEnd - 1;
                    }

                    // no linefeed, so assume that CR represents a newline
                    if(nit != sBufferEnd && *nit != '\n')
                    {
                        addColumn(record, fieldbuf, m_bTrimFields);

                        if(!addRow(insertFunction, record, parsedRows))
                            return ParserResult::ParserResultError;
                    }
                }
                else
                {
                    state = StateNormal;
                    fieldbuf.append(c);
                }
            }
            break;
            }

            if(nMaxRecords > 0 && parsedRows >= nMaxRecords)
                return ParserResult::ParserResultSuccess;
        }

        if(m_pCSVProgress && parsedRows % 100 == 0)
        {
            if(!m_pCSVProgress->update(stream.pos()))
                return ParserResult::ParserResultCancelled;
        }
    }

    if(!record.isEmpty())
    {
        addColumn(record, fieldbuf, m_bTrimFields);

        if(!addRow(insertFunction, record, parsedRows))
            return ParserResult::ParserResultError;
    }

    if(m_pCSVProgress)
        m_pCSVProgress->end();

    return (state == StateNormal) ? ParserResult::ParserResultSuccess : ParserResult::ParserResultError;
}
