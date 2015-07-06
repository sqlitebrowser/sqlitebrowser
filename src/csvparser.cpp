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
