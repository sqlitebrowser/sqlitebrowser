#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QChar>
#include <QVector>
#include <QStringList>

class QTextStream;

/*!
 * \brief The CSVProgress class
 *
 * This is an abstract class, an implementation of which you can pass to the CSVParser to get progress updates.
 */
class CSVProgress
{
public:
    virtual ~CSVProgress() { }

    virtual void start() = 0;
    virtual bool update(size_t pos) = 0;
    virtual void end() = 0;
};

class CSVParser
{
public:
    typedef QVector<QStringList> TCSVResult;

    CSVParser(bool trimfields = true, const QChar& fieldseparator = ',', const QChar& quotechar = '"');
    ~CSVParser();

    /*!
     * \brief parse the given stream
     * \param stream Stream with the CSV parser
     * \param nMaxRecords Max records too read, -1 if unlimited
     * \return True if parsing worked.
     */
    bool parse(QTextStream& stream, qint64 nMaxRecords = -1);

    /*!
     * \brief csv
     * \return The parse result
     */
    const TCSVResult& csv() const { return m_vCSVData; }

    /*!
     * \brief columns
     * \return Number of columns parsed
     */
    size_t columns() const { return m_nColumns; }

    void setCSVProgress(CSVProgress* csvp) { m_pCSVProgress = csvp; }

private:
    enum ParseStates
    {
        StateNormal,
        StateInQuote,
        StateEndQuote
    };

    inline void addRow(QStringList& r)
    {
        m_vCSVData.append(r);
        m_nColumns = std::max<size_t>(r.size(), m_nColumns);
        r.clear();
    }

private:
    bool m_bTrimFields;
    QChar m_cFieldSeparator;
    QChar m_cQuoteChar;
    CSVProgress* m_pCSVProgress;

    TCSVResult m_vCSVData;
    size_t m_nColumns;

    size_t m_nBufferSize; //! internal buffer read size
};

#endif // CSVPARSER_H
