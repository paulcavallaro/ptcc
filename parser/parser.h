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

  void parseTypeSpecifier(Token &out, const int token);
  void parseTypeQualifier(Token &out, const int token);
  void parseTypeQualifierList(Token &out, Token &tQual);
  void parseReturnStmt(Token &out, const int ret_token, const Token &expr);
  void parseExprStmt(Token &out, const Token &expr);
  void parseDirectDeclaratorId(Token &out, const Token &id);
  void parseStructDeclarator(Token &out, const Token &structDecl);
  void parseStructDeclaration(Token &out, const Token &specQualList,
                              const Token &structDeclList);
  void parseStructDeclarationList(Token &out, const Token &structDecl);
  void parseStructSpecifier(Token &out, const Token &id,
                            const Token &structDeclList);
  void parseDirectDeclarator(Token &out, const Token &directDecl);
  void parsePointerDeclarator(Token &out, const Token &pointer,
                              const Token &directDecl);
  void parsePointer(Token &out, const Token *pointer);
  void parsePointerTyQual(Token &out, const Token &tyQualList,
                          const Token *pointer);

  void resetStructDeclaratorList();

  void setDebug(bool debug) { m_debug = debug; }

private:
  void debugLn(const char *format, ...);

  bool m_debug{0};
  std::unordered_map<std::string, size_t> m_symtable;
  std::vector<SymTableEntry> m_symbols;
  std::vector<std::string> m_structDeclList;
  TypeSpec m_structDeclType;
  std::vector<FieldDecl> m_structFieldList;
  std::vector<TypeQual> m_typeQuals;
  TypeSpec m_pointerType;
};
}
} // ptcc::parser
