#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "scanner.h"

namespace ptcc {
namespace parser {

struct SymTableEntry {
  SymTableEntry(std::string symbol, int type)
      : m_symbol(symbol), m_type(type) {}

  std::string m_symbol;
  int m_type;
};

struct Parser {
  int check_type(const char *text);
  ssize_t find(const char *symbol);
  ssize_t insert(const char *symbol);
  const SymTableEntry &get(ssize_t);

  void parseTypeSpecifier(Token &out, int token);

private:
  std::unordered_map<std::string, size_t> m_symtable;
  std::vector<SymTableEntry> m_symbols;
};
}
} // ptcc::parser
