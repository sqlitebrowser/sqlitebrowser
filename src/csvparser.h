#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QVector>
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
    virtual bool update(qint64 pos) = 0;
    virtual void end() = 0;
};

class CSVParser
{
public:
    typedef std::function<bool(size_t, QVector<QByteArray>)> csvRowFunction;

    CSVParser(bool trimfields = true, char16_t fieldseparator = ',', char16_t quotechar = '"');
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
     * \param nMaxRecords Max records too read, 0 if unlimited
     * \return ParserResult value that indicated whether action finished normally, was cancelled or errored.
     */
    ParserResult parse(csvRowFunction insertFunction, QTextStream& stream, size_t nMaxRecords = 0);

    void setCSVProgress(CSVProgress* csvp) { m_pCSVProgress = csvp; }

private:
    enum ParseStates
    {
        StateNormal,
        StateInQuote,
        StateEndQuote
    };

private:
    bool m_bTrimFields;
    char16_t m_cFieldSeparator;
    char16_t m_cQuoteChar;
    CSVProgress* m_pCSVProgress;

    qint64 m_nBufferSize;   //! internal buffer read size
};

#endif
