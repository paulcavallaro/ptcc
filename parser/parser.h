#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "scanner.h"

namespace ptcc {
namespace parser {

struct SymTableEntry {
  std::string m_symbol;
  int m_token;
  std::shared_ptr<TypeSpec> m_type;
};

struct Parser {
  int check_type(const char *text);
  ssize_t find(const std::string &symbol);
  ssize_t insert(const SymTableEntry);
  ssize_t overwrite(const SymTableEntry);
  const SymTableEntry &get(ssize_t);

  void parseTypeSpecifier(Token &out, const int token);
  void parseTypedefTypeSpec(Token &out, const Token &typeDef);
  void parseTypeQualifier(Token &out, const int token);
  void parseTypeQualifierList(Token &out, const Token &tQual);
  void parseTypeQualifierBaseDeclarationSpecifier(Token &out,
                                                  const Token &typeQualifier);
  void parseTypeQualifierDeclarationSpecifier(Token &out,
                                              const Token &typeQualifier,
                                              const Token &declSpecs);

  void parseSpecifierQualifierListSpecifier(Token &out, const Token &specifier,
                                            const Token *list);
  void parseSpecifierQualifierListQualifier(Token &out, const Token &qualifier,
                                            const Token *list);
  void parseReturnStmt(Token &out, const int ret_token, const Token &expr);
  void parseExprStmt(Token &out, const Token &expr);

  void parseDirectDeclaratorId(Token &out, const Token &id);
  void parseDirectDeclaratorArray(Token &out, const Token &declarator);

  void parseStructDeclarator(Token &out, const Token &structDecl,
                             const Token *optStructDeclList);
  void parseStructDeclaration(Token &out, const Token &specQualList,
                              const Token &structDeclList);
  void parseStructDeclarationList(Token &out, const Token &structDecl,
                                  const Token *optStructDeclList);
  void parseStructUnionSpecifier(Token &out, const Token &structOrUnion,
                                 const Token &id, const Token &structDeclList);
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
  void parseTypeSpecifierBaseDeclarationSpecifier(Token &out,
                                                  const Token &typeSpecifier);
  void
  parseTypeSpecifierDeclarationSpecifier(Token &out, const Token &typeSpecifier,
                                         const Token &declarationSpecifiers);

  void createNewEnumConstant(Token &out, const Token &id);
  void parseEnumeratorListBase(Token &out, const Token &enumerator);
  void parseEnumeratorList(Token &out, const Token &enumeratorList,
                           const Token &enumerator);
  void parseEnumerator(Token &out, const Token &enumConstant,
                       const Token *optionalConstExpression);
  void parseEnumSpecifier(Token &out, const Token &enumeratorList,
                          const Token *optionalEnumId);
  void parseEnumSpecifierNoEnums(Token &out, const Token &enumId);

  void parseFunctionDefinition(Token &out, const Token &declSpecifiers,
                               const Token &declarator,
                               const Token *optDeclList,
                               const Token &compoundStmt);

  void parseFunctionDirectDeclarator(Token &out, const Token &directDeclarator,
                                     const Token &parameterTypeList);
  void parseParameterTypeList(Token &out, const Token &parameterList);
  void parseParameterListBase(Token &out, const Token &parameterDecl);
  void parseParameterList(Token &out, const Token &parameterList,
                          const Token &parameterDecl);
  void parseParameterDeclarator(Token &out, const Token &declSpecifiers,
                                const Token &declarator);
  void parseParameterAbstractDeclarator(Token &out, const Token &declSpecifiers,
                                        const Token &abstractDeclarator);
  void parseParameterDeclSpecs(Token &out, const Token &declSpecifiers);
  void parseTranslationUnitBase(Token &out, const Token &externalDecl);
  void parseTranslationUnitAdd(Token &out, const Token &externalDecl);
  void parseExternalDeclarationFunc(Token &out,
                                    const Token &functionDefinition);
  void parseExternalDeclarationDecl(Token &out, const Token &declaration);

  void parseInitDeclarator(Token &out, const Token &declarator,
                           const Token *optInitializer);
  void parseInitDeclaratorListAdd(Token &out, const Token &initDeclaratorList,
                                  const Token &initDeclarator);
  void parseInitDeclaratorListBase(Token &out, const Token &initDeclarator);

  void setDebug(bool debug) { m_debug = debug; }
  void debugLn(const char *format, ...);

private:
  bool m_debug{0};
  std::unordered_map<std::string, size_t> m_symtable;
  std::vector<SymTableEntry> m_symbols;

  TypeSpec mergeTypes(TypeSpec type, TypeSpec listType);
  TypeSpec coalesceTypes(const std::vector<TypeSpec> types);
};

// TODO(ptc) rename to something clearer since can also be used for arrays
// or other indirect TypeKinds
void setPtrTo(TypeSpec *ptrType, TypeSpec oType);
}
} // ptcc::parser
