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
  void
  parseTypeSpecifierDeclarationSpecifier(Token &out,
                                         const Token &typeSpecifier,
                                         const Token &declarationSpecifiers);

  void parseFunctionDefinition(Token &out, const Token &declSpecifiers,
                               const Token &declarator,
                               const Token *optDeclList,
                               const Token &compoundStmt);

  void parseFunctionDirectDeclarator(Token &out, const Token &directDeclarator,
                                     const Token &parameterTypeList);


  void resetStructDeclaratorList();

  void setDebug(bool debug) { m_debug = debug; }
  void debugLn(const char *format, ...);

private:
  bool m_debug{0};
  std::unordered_map<std::string, size_t> m_symtable;
  std::vector<SymTableEntry> m_symbols;
  std::vector<Token> m_structDeclList;
  TypeSpec m_structDeclType;
  std::vector<FieldDecl> m_structFieldList;
  TypeSpec m_pointerType;
};

// TODO(ptc) rename to something clearer since can also be used for arrays
// or other indirect TypeKinds
void setPtrTo(TypeSpec *ptrType, TypeSpec oType);
}
} // ptcc::parser
