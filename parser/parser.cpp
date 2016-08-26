#include "parser.h"
#include "grammar.tab.hpp"

namespace ptcc { namespace parser {

int Parser::check_type(const char* symbol) {
    auto entry = m_symbols[insert(symbol)];
    switch (entry.m_type) {
    default:
        return IDENTIFIER;
    }
}

ssize_t Parser::find(const char* symbol) {
    auto res = m_symtable.find(symbol);
    if (res != m_symtable.end()) {
        return static_cast<size_t>(res->second);
    } else {
        return -1;
    }
}

ssize_t Parser::insert(const char* symbol) {
    auto res = m_symtable.find(symbol);
    if (res != m_symtable.end()) {
        return static_cast<size_t>(res->second);
    } else {
        auto idx = m_symbols.size();
        m_symbols.emplace_back(symbol, 0);
        return idx;
    }
}

} }     // ptcc::parser
