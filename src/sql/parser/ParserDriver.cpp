#include "ParserDriver.h"

namespace sqlb
{
namespace parser
{

ParserDriver::ParserDriver() :
    result(nullptr),
    trace_scanner(false),
    trace_parser(false),
    scanner(nullptr),
    buffer(nullptr)
{
}

int ParserDriver::parse(const std::string& s)
{
    source = s;
    begin_scan();

    sqlb::parser::parser parse(scanner, *this);
    parse.set_debug_level(trace_parser);
    int res = parse();

    end_scan();
    return res;
}

}
}
