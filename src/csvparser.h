#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QChar>
#include <QStringList>
#include <functional>

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
    typedef std::function<bool(size_t, QStringList)> csvRowFunction;

    CSVParser(bool trimfields = true, const QChar& fieldseparator = ',', const QChar& quotechar = '"');
    ~CSVParser();

    enum ParserResult
    {
        ParserResultSuccess,
        ParserResultCancelled,
        ParserResultError
    };

    /*!
     * \brief parse the given stream
     * @param insertFunction A function pointer that is called for each parsed row. It is passed two parameters, the row number and a list of all parsed columns
     *                       in the row. The called function may return false if an error ocurred to stop the import process. Otherwise it should return true.
     * \param stream Stream with the CSV parser
     * \param nMaxRecords Max records too read, -1 if unlimited
     * \return ParserResult value that indicated whether action finished normally, was cancelled or errored.
     */
    ParserResult parse(csvRowFunction insertFunction, QTextStream& stream, qint64 nMaxRecords = -1);

    void setCSVProgress(CSVProgress* csvp) { m_pCSVProgress = csvp; }

private:
    enum ParseStates
    {
        StateNormal,
        StateInQuote,
        StateEndQuote
    };

    inline bool addRow(QStringList& r)
    {
        if(!m_insertFunction(m_iParsedRows, r))
            return false;

        r.clear();
        m_iParsedRows++;
        return true;
    }

private:
    bool m_bTrimFields;
    QChar m_cFieldSeparator;
    QChar m_cQuoteChar;
    CSVProgress* m_pCSVProgress;
    csvRowFunction m_insertFunction;

    qint64 m_iParsedRows;   // Number of rows parsed so far

    size_t m_nBufferSize; //! internal buffer read size
};

#endif
