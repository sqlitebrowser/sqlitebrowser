#ifndef PARSER_DRIVER_H
#define PARSER_DRIVER_H

#include <string>
#include "sqlite3_parser.hpp"
#include "../sqlitetypes.h"

// Give Flex the prototype of yylex we want and declare it for the parser
typedef void* yyscan_t;
namespace sqlb { namespace parser { class ParserDriver; } }
#define YY_DECL sqlb::parser::parser::symbol_type yylex(yyscan_t yyscanner, sqlb::parser::ParserDriver& drv)
YY_DECL;

namespace sqlb
{
namespace parser
{

class ParserDriver
{
public:
    ParserDriver();

    void setScannerDebug(bool on) { trace_scanner = on; }
    void setParserDebug(bool on) { trace_parser = on; }

    // Run the parser on string s. Returns 0 on success
    int parse(const std::string& s);

    // Result returned by the parsing process
    sqlb::ObjectPtr result;

    // The token's location used by the scanner
    sqlb::parser::location location;

private:
    // The string to be parsed
    std::string source;

    // Handling the scanner
    void begin_scan();
    void end_scan();

    // Debug flags for both scanner and parser
    bool trace_scanner;
    bool trace_parser;

    // Scanner state
    yyscan_t scanner;

    // Scanner buffer
    typedef struct yy_buffer_state* YY_BUFFER_STATE;
    YY_BUFFER_STATE buffer;
};

}
}

#endif
