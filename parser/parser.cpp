#include "parser.h"
#include "grammar.tab.hpp"

namespace ptcc { namespace parser {

int Parser::check_type(const char*) {
    return IDENTIFIER;
}

} }     // ptcc::parser
