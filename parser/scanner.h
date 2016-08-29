#pragma once

#include <string>

namespace ptcc { namespace parser {

struct Token {
    int m_num{0};
    std::string m_text;
};

} }     // ptc::scanner
