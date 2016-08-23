#pragma once

#include <string>

namespace ptcc { namespace parser {

struct ScannerToken {
    int m_num{0};
    std::string m_text;
};

} }     // ptc::scanner
