#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <functional>
#include <cstdint>
#include <cstddef>

#include <QString>

class QByteArray;
class QTextStream;

/*!
 * \brief The CSVProgress class
 *
 * This is an abstract class, an implementation of which you can pass to the CSVParser to get progress updates.
 */
class CSVProgress
{
public:
    virtual ~CSVProgress() = default;

    virtual void start() = 0;
    virtual bool update(int64_t pos) = 0;
    virtual void end() = 0;
};

/*
 * This structure represents one parsed column in a CSV row
 */
struct CSVField
{
    // These variables are used internally by the parser. Usually there should be no need to access them from outside the parser
    char* buffer;                   // Start of the field buffer
    uint64_t buffer_length;         // Current length of the buffer content. The content is NOT null-terminated
    uint64_t buffer_max_length;     // Size of the entire buffer

    // These variables point to the parsed contents of the field and can be directly passed to SQLite functions
    char* data;                     // Pointer to the start of the field contents
    uint64_t data_length;           // Length of the field contents
};

/*
 * This structure holds all columns of a single parsed CSV row
 */
struct CSVRow
{
    CSVField* fields;               // Pointer to the field list/the first element in the list
    size_t num_fields;              // Number of fields in the row
    size_t max_num_fields;          // Size of the field list. Used internally by the parser only
};

class CSVParser
{
public:
    using csvRowFunction = std::function<bool(size_t, CSVRow)>;

    explicit CSVParser(bool trimfields = true, char32_t fieldseparator = ',', char32_t quotechar = '"');
    ~CSVParser();

    enum ParserResult
    {
        ParserResultSuccess,
        ParserResultCancelled,
        ParserResultError,
        ParserResultUnexpectedEOF
    };

    /*!
     * \brief parse the given stream
     * @param insertFunction A function pointer that is called for each parsed row. It is passed two parameters, the row number and a list of all parsed columns
     *                       in the row. The called function may return false if an error occurred to stop the import process. Otherwise it should return true.
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
    char m_cFieldSeparator[4];
    char m_cQuoteChar[4];
    int m_iNumExtraBytesFieldSeparator;
    int m_iNumExtraBytesQuoteChar;
    CSVProgress* m_pCSVProgress;

    int64_t m_nBufferSize;        //! internal buffer read size

    bool look_ahead(QTextStream& stream, QByteArray& sBuffer, const char** it, const char** sBufferEnd, char expected);
    bool look_ahead(QTextStream& stream, QByteArray& sBuffer, const char** it, const char** sBufferEnd,
                    const char* expected, int extraBytes);
};

/*!
 * \brief Prepares a value for being written to a CSV file.
 *
 * The value is enclosed in quote characters if it contains the separator, the quote character or a line break. Quote characters inside
 * the value are escaped by doubling them.
 *
 * \param content The value to write.
 * \param separator The field separator. A null character means that there is no field separator.
 * \param quote The quote character. A null character means that quoting has been disabled. Values containing line breaks are quoted
 *              anyway in this case, using a double quote, because the file would be unusable otherwise.
 * \param newline The line break that is written after each record.
 * \return The value ready to be written to the file.
 */
QString quoteCsvField(const QString& content, QChar separator, QChar quote, const QString& newline);

#endif
