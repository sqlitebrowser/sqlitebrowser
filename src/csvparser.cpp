#include "csvparser.h"

#include <QTextStream>

CSVParser::CSVParser(bool trimfields, char fieldseparator, char quotechar)
    : m_bTrimFields(trimfields)
    , m_cFieldSeparator(fieldseparator)
    , m_cQuoteChar(quotechar)
    , m_pCSVProgress(nullptr)
    , m_nBufferSize(4096)
{
}

CSVParser::~CSVParser()
{
    delete m_pCSVProgress;
}

namespace {
// This function adds a character to an existing field structure. If necessary, it extends the buffer size.
inline void addChar(CSVField* field, char c)
{
    // Increase buffer size if it is too small
    if(field->buffer_length >= field->buffer_max_length)
    {
        field->buffer_max_length += 64;
        field->buffer = static_cast<char*>(realloc(field->buffer, field->buffer_max_length));
    }

    // Add char to the end of the buffer and increase length by one
    field->buffer[field->buffer_length++] = c;
}

// This function increases the size of the field list of an existing row. However, it only does so if the field list is currently full.
inline void increaseRowSize(CSVRow& r)
{
    // Check if field list is full
    if(r.num_fields >= r.max_num_fields)
    {
        // Increase field list size
        r.max_num_fields += 5;
        r.fields = static_cast<CSVField*>(realloc(r.fields, r.max_num_fields*sizeof(CSVField)));

        // Initialise the newly created field structures
        for(size_t i=r.num_fields;i<r.max_num_fields;i++)
        {
            r.fields[i].buffer = nullptr;
            r.fields[i].buffer_length = 0;
            r.fields[i].buffer_max_length = 0;
        }
    }
}

// This function adds a parsed field to a row structure. It returns a pointer to a new field object which can be used for the storing the
// contents of the next field. The function doesn't "add" the field to the row but assumes that the field has been constructed in-place.
// All it does is finish the field structure and update the row structure to take the new field into account.
inline CSVField* addColumn(CSVRow& r, CSVField* field, bool trim)
{
    // Make the parsed data start and length identical with the buffer start and length
    field->data = field->buffer;
    field->data_length = field->buffer_length;

    // If we have to trim the field, do this by manipulating the data start and data length variables
    if(trim)
    {
        // Check for trailing spaces and omit them
        while(field->data_length && isspace(*field->data))
        {
            field->data++;
            field->data_length--;
        }

        // Check for pending spaces and omit them
        while(field->data_length && isspace(field->data[field->data_length-1]))
            field->data_length--;
    }

    // We assume here that the field object has been constructed in-place. So all we need to do for adding it to the row structure
    // is increasing the field count by one to make sure the newly constructed field object is used.
    r.num_fields++;

    // Clear field buffer for next use
    field->buffer_length = 0;

    // Increase field list size if it is too small
    increaseRowSize(r);

    // Return pointer to the next field
    return &r.fields[r.num_fields];
}

// This function takes a parsed CSV row and hands it back to the caller of the CSV parser. It returns a null pointer if the parsing should be
// aborted, otherwise it returns a pointer to a new field object that can be used for storing the contents of the first field of the next row.
inline CSVField* addRow(CSVParser::csvRowFunction& f, CSVRow& r, size_t& rowCount)
{
    // Call row function
    if(!f(rowCount, r))
        return nullptr;

    // Reset the field list by setting the field count to 0. No need to deconstruct anything else.
    r.num_fields = 0;

    // Increase row count by one, as we're now starting to parse the next row
    rowCount++;

    // Return a pointer to the first field in the row object because we're starting with the first field of the next row now
    return r.fields;
}
}

CSVParser::ParserResult CSVParser::parse(csvRowFunction insertFunction, QTextStream& stream, size_t nMaxRecords)
{
    ParseStates state = StateNormal;        // State of the parser
    QByteArray sBuffer;                     // Buffer for reading the file
    CSVRow record;                          // Buffer for parsing the current row
    size_t parsedRows = 0;                  // Number of rows parsed so far
    CSVField* field;                        // Buffer for parsing the current field

    if(m_pCSVProgress)
        m_pCSVProgress->start();

    // Initialise row buffer and get pointer to the first field
    record = { nullptr, 0, 0 };
    increaseRowSize(record);
    field = record.fields;

    // Make sure all buffers are freed when we're done here
    class FieldBufferDealloc
    {
    public:
        explicit FieldBufferDealloc(CSVRow& row) : m_row(row) {}
        ~FieldBufferDealloc()
        {
            for(size_t i=0;i<m_row.max_num_fields;i++)
                free(m_row.fields[i].buffer);
            free(m_row.fields);
        }
    private:
        CSVRow& m_row;
    };
    FieldBufferDealloc dealloc(record);

    qint64 bufferPos = 0;
    while(!stream.atEnd())
    {
        sBuffer = stream.read(m_nBufferSize).toUtf8();
        bufferPos += sBuffer.length();
        auto sBufferEnd = sBuffer.constEnd();

        for(auto it = sBuffer.constBegin(); it != sBufferEnd; ++it)
        {
            // Get next char
            char c = *it;

            switch(state)
            {
            case StateNormal:
            {
                if(c == m_cFieldSeparator)
                {
                    field = addColumn(record, field, m_bTrimFields);
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
                        addColumn(record, field, m_bTrimFields);

                        if(!(field = addRow(insertFunction, record, parsedRows)))
                            return ParserResult::ParserResultError;
                    }
                }
                else if(c == '\n')
                {
                    addColumn(record, field, m_bTrimFields);

                    if(!(field = addRow(insertFunction, record, parsedRows)))
                        return ParserResult::ParserResultError;
                }
                else
                {
                    addChar(field, c);
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
                    addChar(field, c);
                }
            }
            break;
            case StateEndQuote:
            {
                if(c == m_cQuoteChar)
                {
                    state = StateInQuote;
                    addChar(field, c);
                }
                else if(c == m_cFieldSeparator)
                {
                    state = StateNormal;
                    field = addColumn(record, field, m_bTrimFields);
                }
                else if(c == '\n')
                {
                    state = StateNormal;
                    addColumn(record, field, m_bTrimFields);

                    if(!(field = addRow(insertFunction, record, parsedRows)))
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
                        addColumn(record, field, m_bTrimFields);

                        if(!(field = addRow(insertFunction, record, parsedRows)))
                            return ParserResult::ParserResultError;
                    }
                }
                else
                {
                    state = StateNormal;
                    addChar(field, c);
                }
            }
            break;
            }

            if(nMaxRecords > 0 && parsedRows >= nMaxRecords)
                return ParserResult::ParserResultSuccess;
        }

        if(m_pCSVProgress && parsedRows % 100 == 0)
        {
            if(!m_pCSVProgress->update(bufferPos))
                return ParserResult::ParserResultCancelled;
        }
    }

    if(record.num_fields)
    {
        addColumn(record, field, m_bTrimFields);

        if(!(field = addRow(insertFunction, record, parsedRows)))
            return ParserResult::ParserResultError;
    }

    if(m_pCSVProgress)
        m_pCSVProgress->end();

    return (state == StateNormal) ? ParserResult::ParserResultSuccess : ParserResult::ParserResultError;
}
