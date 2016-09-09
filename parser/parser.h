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
  ssize_t insert(const char *symbol, int token);
  ssize_t overwrite(const char *symbol, int token);
  const SymTableEntry &get(ssize_t);

  void parseTypeSpecifier(Token &out, const int token);
  void parseTypeQualifier(Token &out, const int token);
  void parseTypeQualifierList(Token &out, Token &tQual);

  void parseSpecifierQualifierListSpecifier(Token &out, const Token &specifier,
                                            const Token *list);
  void parseSpecifierQualifierListQualifier(Token &out, const Token &qualifier,
                                            const Token *list);
  void resetSpecifierQualifierList();

  void parseReturnStmt(Token &out, const int ret_token, const Token &expr);
  void parseExprStmt(Token &out, const Token &expr);

  void parseDirectDeclaratorId(Token &out, const Token &id);
  void parseDirectDeclaratorArray(Token &out, const Token &declarator);

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

  void parseStorageClassSpecifier(Token &out, const int token);
  void parseStructTypeSpecifier(Token &out, const Token &structSpecifier);
  void parseUnionTypeSpecifier(Token &out, const Token &unionSpecifier);

  void parseDeclarationFromDeclarationSpecifiers(
      Token &out, const Token &declarationSpecifiers,
      const Token *initDeclaratorList = nullptr);
  void
  parseStorageClassDeclarationSpecifiers(Token &out,
                                         const Token &storageClassSpecifiers,
                                         const Token &declarationSpecifiers);

  void resetStructDeclaratorList();

  void setDebug(bool debug) { m_debug = debug; }

private:
  void debugLn(const char *format, ...);

  bool m_debug{0};
  std::unordered_map<std::string, size_t> m_symtable;
  std::vector<SymTableEntry> m_symbols;
  std::vector<Token> m_structDeclList;
  TypeSpec m_structDeclType;
  std::vector<FieldDecl> m_structFieldList;
  std::vector<TypeQual> m_typeQuals;
  std::vector<TypeSpec> m_typeSpecs;
  TypeSpec m_pointerType;
};
}
} // ptcc::parser
