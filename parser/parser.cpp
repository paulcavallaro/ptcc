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

void Parser::parseExprStmt(Token &out, const Token &expr) {
  debugLn("Expression Statement of Expression: %s", expr.m_text.c_str());
  out = expr;
}

void Parser::parseDirectDeclaratorId(Token &out, const Token &id) {
  debugLn("Direct Declarator of IDENTIFIER: %s", id.m_text.c_str());
  out = id;
}

void Parser::parseStructDeclarator(Token &out, const Token &structDecl) {
  debugLn("Struct Declarator List Item: %s", structDecl.m_text.c_str());
  m_structDeclList.push_back(structDecl.m_text);
}

void Parser::parseStructDeclaration(Token &out, const Token &specQualList,
                                    const Token &structDeclList) {
  m_structDeclType = *specQualList.m_type;

  std::string names;
  for (int i = 0; i < m_structDeclList.size(); i++) {
    auto &field = m_structDeclList[i];
    if (i != 0) {
      names = names + " " + field;
    } else {
      names = field;
    }
  }
  debugLn("Struct Declaration of Specifier Qualifier List + Struct "
          "Declarator List: %s %s",
          specQualList.m_text.c_str(), names.c_str());
}

void Parser::parseStructDeclarationList(Token &out, const Token &structDecl) {
  debugLn("Struct Declaration List Item: %s", structDecl.m_text.c_str());
  for (auto &name : m_structDeclList) {
    FieldDecl field;
    field.m_type = m_structDeclType;
    field.m_name = name;
    m_structFieldList.push_back(field);
  }
}

void Parser::parseStructSpecifier(Token &out, const Token &id,
                                  const Token &structDeclList) {
  debugLn("Struct Specifier struct %s with fields:", id.m_text.c_str());
  for (auto &fieldDecl : m_structFieldList) {
    debugLn("\tField named %s of type %s", fieldDecl.m_name.c_str(),
            specToString(fieldDecl.m_type).c_str());
  }
}

void Parser::resetStructDeclaratorList() {
  debugLn("Resetting the Struct Declarator List");
  m_structDeclList.clear();
  m_structDeclType = TypeSpec{.m_kind = TypeKind::Void};
}

void Parser::parseReturnStmt(Token &out, const int ret_token,
                             const Token &expr) {
  debugLn("Return Statement: RETURN %s;", expr.m_text.c_str());
  out.m_text = "RETURN " + expr.m_text + ";";
}
}
} // ptcc::parser
