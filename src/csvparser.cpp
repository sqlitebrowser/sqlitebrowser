#include "csvparser.h"

#include <QTextStream>
#include <algorithm>

CSVParser::CSVParser(bool trimfields, const QChar& fieldseparator, const QChar& quotechar)
    : m_bTrimFields(trimfields)
    , m_cFieldSeparator(fieldseparator)
    , m_cQuoteChar(quotechar)
    , m_pCSVProgress(0)
    , m_nColumns(0)
    , m_nBufferSize(4096)
{
}

CSVParser::~CSVParser()
{
    delete m_pCSVProgress;
}

namespace {
inline void addColumn(QStringList& r, QString& field, bool trim)
{
    if(trim)
        r << field.trimmed();
    else
        r << field;
    field.clear();
}
}

bool CSVParser::parse(QTextStream& stream, qint64 nMaxRecords)
{
    m_vCSVData.clear();
    m_nColumns = 0;
    ParseStates state = StateNormal;
    QString fieldbuf;
    QStringList record;

    if(m_pCSVProgress)
        m_pCSVProgress->start();

    while(!stream.atEnd())
    {
        QString sBuffer = stream.read(m_nBufferSize);

        for(QString::iterator it = sBuffer.begin(); it != sBuffer.end(); ++it)
        {
            QChar c = *it;
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
                    QString::iterator nit = it + 1;

                    // In order to check what the next byte is we must make sure that that byte is already loaded. Assume we're at an m_nBufferSize
                    // boundary but not at the end of the file when we hit a \r character. Now we're going to be at the end of the sBuffer string
                    // because of the m_nBufferSize boundary. But this means that the following check won't work properly because we can't check the
                    // next byte when we really should be able to do so because there's more data coming. To fix this we'll check for this particular
                    // case and, if this is what's happening, we'll just load an extra byte.
                    if(nit == sBuffer.end() && !stream.atEnd())
                    {
                        // Load one more byte
                        sBuffer.append(stream.read(1));

                        // Restore both iterators. sBuffer.end() points to the imagined char after the last one in the string. So the extra byte we've
                        // just loaded is the one before that, i.e. the actual last one, and the original last char is the one before that.
                        it = sBuffer.end() - 2;
                        nit = sBuffer.end() - 1;
                    }

                    // no linefeed, so assume that CR represents a newline
                    if(nit != sBuffer.end() && *nit != '\n')
                    {
                        addColumn(record, fieldbuf, m_bTrimFields);

                        addRow(record);
                    }
                }
                else if(c == '\n')
                {
                    addColumn(record, fieldbuf, m_bTrimFields);

                    addRow(record);
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

                    addRow(record);
                }
                else if(c == '\r')
                {
                    // look ahead to check for linefeed
                    QString::iterator nit = it + 1;

                    // See above for details on this.
                    if(nit == sBuffer.end() && !stream.atEnd())
                    {
                        sBuffer.append(stream.read(1));
                        it = sBuffer.end() - 2;
                        nit = sBuffer.end() - 1;
                    }

                    // no linefeed, so assume that CR represents a newline
                    if(nit != sBuffer.end() && *nit != '\n')
                    {
                        addColumn(record, fieldbuf, m_bTrimFields);

                        addRow(record);
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

            if(nMaxRecords != -1 && m_vCSVData.size() >= nMaxRecords)
                return true;
        }

        if(m_pCSVProgress && m_vCSVData.size() % 100 == 0)
        {
            if(!m_pCSVProgress->update(stream.pos()))
                return false;
        }
    }

    if(!record.isEmpty())
    {
        addColumn(record, fieldbuf, m_bTrimFields);

        addRow(record);
    }

    if(m_pCSVProgress)
        m_pCSVProgress->end();

    return state == StateNormal;
}
