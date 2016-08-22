#include <string>

namespace ptcc { namespace parser {

struct ScannerToken {
    int m_num{0};
    std::string m_text;
};

struct Parser {
    void check_type(const char* text) {}
};

} }     // ptc::scanner
