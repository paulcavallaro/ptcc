#include "parser.h"
#include "grammar.tab.hpp"

namespace ptcc {
namespace parser {

int Parser::check_type(const char *symbol) {
  auto entry = m_symbols[insert(symbol)];
  switch (entry.m_type) {
  default:
    return IDENTIFIER;
  }
}

ssize_t Parser::find(const char *symbol) {
  auto res = m_symtable.find(symbol);
  if (res != m_symtable.end()) {
    return static_cast<size_t>(res->second);
  } else {
    return -1;
  }
}

const SymTableEntry &Parser::get(ssize_t idx) { return m_symbols[idx]; }

ssize_t Parser::insert(const char *symbol) {
  auto res = m_symtable.find(symbol);
  if (res != m_symtable.end()) {
    return static_cast<size_t>(res->second);
  } else {
    auto idx = m_symbols.size();
    m_symbols.emplace_back(symbol, 0);
    m_symtable.emplace(symbol, idx);
    return idx;
  }
}

void Parser::parseTypeSpecifier(Token &out, const int token) {
  switch (token) {
  case VOID:
    out.m_token = token;
    out.m_text = "void";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Void;
    break;
  case CHAR:
    out.m_token = token;
    out.m_text = "char";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Char;
    break;
  case SHORT:
    out.m_token = token;
    out.m_text = "short";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Short;
    break;
  case INT:
    out.m_token = token;
    out.m_text = "int";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Int32;
    break;
  case LONG:
    out.m_token = token;
    out.m_text = "long";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Int64;
    break;
  case FLOAT:
    out.m_token = token;
    out.m_text = "float";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Float;
    break;
  case DOUBLE:
    out.m_token = token;
    out.m_text = "double";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Double;
    break;
  case BOOL:
    out.m_token = token;
    out.m_text = "bool";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Bool;
    break;
  default:
    // TODO(ptc) Handle rest of cases
    out.m_token = token;
    break;
  }
}

void Parser::debugLn(const char *format, ...) {
  if (m_debug) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
  }
}

void Parser::parseReturnStmt(Token &out, const int ret_token,
                             const Token &expr) {
  debugLn("Return Statement: RETURN %s;", expr.m_text.c_str());
  out.m_text = "RETURN " + expr.m_text + ";";
}
}
} // ptcc::parser
