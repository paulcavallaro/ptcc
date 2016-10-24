#include "parser.h"
#include "parser/grammar.tab.hpp"

#include <assert.h>

namespace ptcc {
namespace parser {

int Parser::check_type(const char *symbol) {
  // TODO(ptc) fix this whole mess, just default to adding as IDENTIFIER if not
  // already defined
  auto entry = m_symbols[insert(symbol, IDENTIFIER)];
  switch (entry.m_type) {
  case IDENTIFIER:
    debugLn("Checking type of: %s, Found: IDENTIFIER", symbol);
    return IDENTIFIER;
  case TYPEDEF_NAME:
    debugLn("Checking type of: %s, Found: TYPEDEF_NAME", symbol);
    return TYPEDEF_NAME;
  default:
    debugLn("Checking type of: %s, Not Found, Default To: IDENTIFIER", symbol);
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

ssize_t Parser::insert(const char *symbol, int token) {
  auto res = m_symtable.find(symbol);
  if (res != m_symtable.end()) {
    return static_cast<size_t>(res->second);
  } else {
    auto idx = m_symbols.size();
    m_symbols.emplace_back(symbol, token);
    m_symtable.emplace(symbol, idx);
    return idx;
  }
}

ssize_t Parser::overwrite(const char *symbol, int token) {
  auto res = m_symtable.find(symbol);
  if (res != m_symtable.end()) {
    auto idx = static_cast<size_t>(res->second);
    m_symbols[idx].m_type = token;
    return idx;
  } else {
    auto idx = m_symbols.size();
    m_symbols.emplace_back(symbol, token);
    m_symtable.emplace(symbol, idx);
    return idx;
  }
}

void Parser::parseTypeQualifierList(Token &out, Token &tQual) {
  debugLn("Entering parseTypeQualifierList out.m_typeQuals.size() = %lu",
          out.m_typeQuals.size());
  out.m_typeQuals = {};
  switch (tQual.m_token) {
  case CONST:
    out.m_typeQuals.push_back(TypeQual::Const);
    break;
  case RESTRICT:
    out.m_typeQuals.push_back(TypeQual::Restrict);
    break;
  case VOLATILE:
    out.m_typeQuals.push_back(TypeQual::Volatile);
    break;
  case ATOMIC:
    out.m_typeQuals.push_back(TypeQual::Atomic);
    break;
  default:
    debugLn("ERROR - Unknown Type Qualifier");
    break;
  }
  debugLn("Exiting parseTypeQualifierList out.m_typeQuals.size() = %lu",
          out.m_typeQuals.size());
}

void Parser::parseSpecifierQualifierListSpecifier(Token &out,
                                                  const Token &specifier,
                                                  const Token *list) {
  debugLn("Entering parseSpecifierQualifierListSpecifier");
  assert(specifier.m_type);
  if (!list) {
    debugLn("No list, setting out = specifier");
    out = specifier;
  } else {
    debugLn("List exists");
    // If there is a list I believe the further list has to be a pointer type
    if (list->m_type) {
      debugLn("List TYPE exists");
      // TODO(ptc) fill this out
      assert(false);
      assert(list->m_type->m_kind == TypeKind::Pointer);
      // If the list is a pointer type then we need to traverse down and set the
      // last unset
      // o_type
    } else {
      // Just a bunch of type qualifiers, let's augment our type from the
      // specifier
      // then
      debugLn("No list type, just a bunch of type qualifiers so augmenting our "
              "specifier with them");
      out = specifier;
      assert(!list->m_typeQuals.empty());
      out.m_type->m_quals = list->m_typeQuals;
    }
  }
}

void Parser::parseSpecifierQualifierListQualifier(Token &out,
                                                  const Token &qualifier,
                                                  const Token *list) {
  debugLn("Entering parseSpecifierQualifierListQualifier");
  assert(qualifier.m_typeQuals.size() == 1);
  if (!list) {
    debugLn("No list, just setting out = qualifier");
    out = qualifier;
  } else {
    debugLn("List exists");
    if (list->m_type) {
      debugLn(
          "List TYPE exists, just adding our qualifier to the list type then");
      out = *list;
      out.m_type->m_quals.push_back(qualifier.m_typeQuals[0]);
      debugLn("New type is: %s", pointerTypeToString(*out.m_type).c_str());
    } else {
      debugLn("No list type, just a bunch of type qualifiers so augmenting our "
              "m_tyQuals with them as well");
      out.m_typeQuals = list->m_typeQuals;
      out.m_typeQuals.push_back(qualifier.m_typeQuals[0]);
    }
  }
}

bool isIndirectTypeKind(TypeKind kind) {
  return kind == TypeKind::Pointer || kind == TypeKind::Array;
}

void setPtrTo(TypeSpec *ptrType, TypeSpec oType) {
  assert(ptrType != nullptr);
  assert(isIndirectTypeKind(ptrType->m_kind));
  while (isIndirectTypeKind(ptrType->m_kind) && ptrType->m_otype &&
         isIndirectTypeKind(ptrType->m_otype->m_kind)) {
    ptrType = ptrType->m_otype.get();
  }
  ptrType->m_otype = std::make_shared<TypeSpec>(oType);
}

void Parser::parsePointerTyQual(Token &out, const Token &tyQualList,
                                const Token *pointer) {
  out = Token{};
  debugLn("Entering parsePointerTyQual, pointer=%lu", pointer);
  out.m_token = '*';
  out.m_text = "*";
  debugLn("tyQualList.m_typeQuals.size() = %lu", tyQualList.m_typeQuals.size());
  if (!pointer) {
    out.m_type =
        std::make_shared<TypeSpec>(TypeSpec{.m_kind = TypeKind::Pointer});
    out.m_type->m_quals = tyQualList.m_typeQuals;
  } else {
    debugLn("Pointer exists as well! Calling setPtrTo to set to pointer");
    out.m_type = pointer->m_type;
    setPtrTo(out.m_type.get(), TypeSpec{.m_kind = TypeKind::Pointer,
                                        .m_quals = tyQualList.m_typeQuals});
  }
  debugLn("Pointer Type Qualifier of %s",
          pointerTypeToString(*out.m_type).c_str());
}

void Parser::parsePointer(Token &out, const Token *pointer) {
  out = Token{};
  debugLn("Entering parsePointer, pointer=%lu", pointer);
  out.m_token = '*';
  out.m_text = "*";
  if (pointer) {
    debugLn("Calling setPtrTo");
    out.m_type = pointer->m_type;
    setPtrTo(out.m_type.get(), TypeSpec{.m_kind = TypeKind::Pointer});
    out.m_text = out.m_text + pointer->m_text;
  } else {
    out.m_type =
        std::make_shared<TypeSpec>(TypeSpec{.m_kind = TypeKind::Pointer});
  }
  debugLn("Pointer of %s", pointerTypeToString(*out.m_type).c_str());
}

void Parser::parsePointerDeclarator(Token &out, const Token &pointer,
                                    const Token &directDecl) {
  // TODO(ptc) flesh out pointer + direct delcarator
  debugLn("Entering parsePointerDeclarator");
  out.m_text = pointer.m_text + directDecl.m_text;
  out.m_id = directDecl.m_id;
  out.m_type = pointer.m_type;
  debugLn("Pointer Delcarator Type Of %s",
          pointerTypeToString(*out.m_type).c_str());
}

void Parser::parseDirectDeclarator(Token &out, const Token &directDecl) {
  // TODO(ptc) flesh out pointer + direct delcarator
  out = directDecl;
}

void Parser::parseTypeQualifier(Token &out, const int token) {
  debugLn("Entering parseTypeQualifier, out.m_typeQuals.size() = %lu",
          out.m_typeQuals.size());
  switch (token) {
  case CONST:
    out.m_token = CONST;
    out.m_text = "const";
    out.m_typeQuals.push_back(TypeQual::Const);
    break;
  case RESTRICT:
    out.m_token = RESTRICT;
    out.m_text = "restrict";
    out.m_typeQuals.push_back(TypeQual::Restrict);
    break;
  case VOLATILE:
    out.m_token = VOLATILE;
    out.m_text = "volatile";
    out.m_typeQuals.push_back(TypeQual::Volatile);
    break;
  case ATOMIC:
    out.m_token = ATOMIC;
    out.m_text = "_Atomic";
    out.m_typeQuals.push_back(TypeQual::Atomic);
    break;
  default:
    debugLn("ERROR - Unknown Type Qualifier");
    break;
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
  out.m_id = id.m_text;
}

void Parser::parseDirectDeclaratorArray(Token &out, const Token &declarator) {
  debugLn("Direct Declarator of Direct Declarator Array []: %s",
          declarator.m_text.c_str());
  // TODO(ptc) implement this
  out.m_id = declarator.m_id;
  out.m_text = declarator.m_text + "[]";
  out.m_type = std::make_shared<TypeSpec>(TypeSpec{.m_kind = TypeKind::Array});
  if (declarator.m_type) {
    setPtrTo(out.m_type.get(), *declarator.m_type);
  }
}

void Parser::parseStructDeclarator(Token &out, const Token &structDecl) {
  debugLn("Struct Declarator List Item: type=%s, id=%s",
          structDecl.m_type ? specToString(*structDecl.m_type).c_str()
                            : "Unknown",
          structDecl.m_id.c_str());

  m_structDeclList.push_back(structDecl);
}

void Parser::parseStructDeclaration(Token &out, const Token &specQualList,
                                    const Token &structDeclList) {
  debugLn("Entering parseStructDeclaration");
  // Type specifiers and qualifiers should be in the specQualList m_typeQuals
  // and m_type variables
  assert(!(specQualList.m_typeQuals.empty() && !specQualList.m_type));

  // TODO(ptc) In the future we should do merging of type specifiers such as
  // "long long" or "long int" or "unsigned int", but for now just take the
  // first type specifier as the type
  if (!specQualList.m_type) {
    debugLn("Can't handle empty type in specifier qualifier list!");
    assert(false);
  }

  // TODO(ptc) Have to merge the type from both specQualList and the
  // structDeclList to arrive at the type for each declarator
  out.m_fieldDecls = {};
  for (int i = 0; i < m_structDeclList.size(); i++) {
    auto &declarator = m_structDeclList[i];
    // TODO(ptc) This works for pointer types, probably will have to modify it
    // for other complex array types
    FieldDecl decl;
    decl.m_name = declarator.m_id;
    if (declarator.m_type) {
      decl.m_type = *declarator.m_type;
      setPtrTo(&decl.m_type, *specQualList.m_type);
    } else {
      decl.m_type = *specQualList.m_type;
    }
    out.m_fieldDecls.push_back(decl);
    debugLn("Struct Declaration of type = %s, name = %s",
            specToString(decl.m_type).c_str(), decl.m_name.c_str());
  }
}

void Parser::parseStructDeclarationList(Token &out, const Token &structDecl) {
  debugLn("Struct Declaration List Item: %s", structDecl.m_text.c_str());
  for (auto &fieldDecl : structDecl.m_fieldDecls) {
    m_structFieldList.push_back(fieldDecl);
  }
}

void Parser::parseStructSpecifier(Token &out, const Token &id,
                                  const Token &structDeclList) {
  debugLn("Struct Specifier struct %s with fields:", id.m_text.c_str());
  std::string fields;
  for (auto &fieldDecl : m_structFieldList) {
    debugLn("\tField named %s of type %s", fieldDecl.m_name.c_str(),
            specToString(fieldDecl.m_type).c_str());
    fields +=
        "\t" + specToString(fieldDecl.m_type) + " " + fieldDecl.m_name + ";\n";
  }
  out.m_token = STRUCT;
  out.m_text = "struct " + id.m_text + " {\n" + fields + "}";
  out.m_type = std::make_shared<TypeSpec>();
  out.m_type->m_kind = TypeKind::Struct;
  // TODO(ptc) fix up usage of m_type->m_struct + m_struct in Token
  out.m_type->m_struct =
      std::make_shared<StructDecl>(id.m_text, m_structFieldList);

  // Reset parser state
  m_structFieldList.clear();
}

void Parser::resetStructDeclaratorList() {
  debugLn("Resetting the Struct Declarator List");
  m_structDeclList.clear();
  m_structDeclType = TypeSpec{.m_kind = TypeKind::Void};
}

void Parser::parseStorageClassSpecifier(Token &out, const int token) {
  switch (token) {
  case EXTERN:
    out.m_token = EXTERN;
    out.m_text = "extern";
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_storageClassSpecs.push_back(
        StorageClassSpecifier::Extern);
    break;
  case STATIC:
    out.m_token = STATIC;
    out.m_text = "static";
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_storageClassSpecs.push_back(
        StorageClassSpecifier::Static);
    break;
  case THREAD_LOCAL:
    out.m_token = THREAD_LOCAL;
    out.m_text = "_Thread_local";
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_storageClassSpecs.push_back(
        StorageClassSpecifier::ThreadLocal);
    break;
  case AUTO:
    out.m_token = AUTO;
    out.m_text = "auto";
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_storageClassSpecs.push_back(StorageClassSpecifier::Auto);
    break;
  case REGISTER:
    out.m_token = REGISTER;
    out.m_text = "register";
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_storageClassSpecs.push_back(
        StorageClassSpecifier::Register);
    break;
  case TYPEDEF:
    out.m_token = TYPEDEF;
    out.m_text = "typedef";
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_storageClassSpecs.push_back(
        StorageClassSpecifier::Typedef);
    break;
  default:
    debugLn("ERROR - Unknown Storage Class Specifier %d", token);
    return;
  }
  debugLn("Storage Class Specifier: %s", out.m_text.c_str());
}

void Parser::parseStructTypeSpecifier(Token &out,
                                      const Token &structSpecifier) {
  out = structSpecifier;
}

void Parser::parseUnionTypeSpecifier(Token &out, const Token &unionSpecifier) {
  out = unionSpecifier;
}

void Parser::parseStorageClassDeclarationSpecifiers(
    Token &out, const Token &storageClassSpecifier,
    const Token &declarationSpecifiers) {
  debugLn("Entering parseStorageClassDeclarationSpecifiers");
  out = declarationSpecifiers;
  // TODO(ptc) handle other declaration_specifiers
  debugLn("Entering parseStorageClassDeclarationSpecifiers "
          "storageClassSpecifier.m_declSpecs: %d, out.m_declSpecs: %d",
          storageClassSpecifier.m_declSpecs.get(), out.m_declSpecs.get());
  if (!out.m_declSpecs) {
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
  }
  if (storageClassSpecifier.m_declSpecs) {
    debugLn("Entering parseStorageClassDeclarationSpecifiers -- Modifying "
            "m_declSpecs");
    for (auto &storageClassSpec :
         storageClassSpecifier.m_declSpecs->m_storageClassSpecs) {
      out.m_declSpecs->m_storageClassSpecs.push_back(storageClassSpec);
    }
  }
}

void Parser::parseTypeSpecifierDeclarationSpecifier(
    Token &out, const Token &typeSpecifier,
    const Token &declarationSpecifiers) {
  debugLn("Entering parseTypeSpecifierDeclarationSpecifier %s",
          typeSpecifier.m_text.c_str());
  out.m_declSpecs = declarationSpecifiers.m_declSpecs;
  if (!out.m_declSpecs) {
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
  }
  // Should have a type from the TypeSpecifier
  if (typeSpecifier.m_type) {
    assert(typeSpecifier.m_type);
    out.m_declSpecs->m_typeSpecs.push_back(*typeSpecifier.m_type);
  } else {
    // TODO(ptc) still need to flesh out type_specifier parsing so that
    // all routes will return a type
  }
  // TODO(ptc) still need to pass through some information here regard to text
  // span and whatnot
}

void Parser::parseDeclarationFromDeclarationSpecifiers(
    Token &out, const Token &declarationSpecifiers,
    const Token *initDeclaratorList) {
  debugLn("Entering parseDeclarationFromDeclarationSpecifiers");
  // TODO(ptc) make this much broader, for now just add possible typedef
  // declarations into the symbol table
  if (declarationSpecifiers.m_declSpecs) {
    debugLn("parseDeclarationFromDeclarationSpecifiers: m_declSpecs not null");
    bool isTypedef = false;
    for (auto &storageClassSpec :
         declarationSpecifiers.m_declSpecs->m_storageClassSpecs) {
      if (storageClassSpec == StorageClassSpecifier::Typedef) {
        isTypedef = true;
        break;
      }
    }
    if (isTypedef) {
      debugLn("parseDeclarationFromDeclarationSpecifiers: isTypedef");
      if (declarationSpecifiers.m_type &&
          declarationSpecifiers.m_type->m_struct) {
        // Have a typedef of a struct, add the typedef name to the symbol table
        // TODO(ptc) fix up how we insert into this symbol table
        if (initDeclaratorList) {
          debugLn("parseDeclarationFromDeclarationSpecifiers: is "
                  "initDeclaratorList");
          debugLn("Adding typedef name of %s",
                  initDeclaratorList->m_text.c_str());
          overwrite(initDeclaratorList->m_text.c_str(), TYPEDEF_NAME);
        }
      } else {
        if (initDeclaratorList) {
          debugLn("parseDeclarationFromDeclarationSpecifiers: is "
                  "initDeclaratorList and not declarationSpecifiers.m_type");
          debugLn("Adding typedef name of %s",
                  initDeclaratorList->m_text.c_str());
          overwrite(initDeclaratorList->m_text.c_str(), TYPEDEF_NAME);
        }
      }
    }
  }
}

void Parser::parseParameterDeclarator(Token &out, const Token &declSpecifiers,
                                      const Token &declarator) {
  // TODO(ptc) still need to do more
  debugLn("Entering parseParameterDeclarator");
  out.m_declSpecs = declSpecifiers.m_declSpecs;
  if (out.m_declSpecs) {
    debugLn("parseParameterDeclarator: declSpecifiers has m_declSpecs");
  }
  out.m_id = declarator.m_id;
  debugLn("parseParameterDeclarator: id=%s", out.m_id.c_str());
  if (declarator.m_type) {
    debugLn("parseParameterDeclarator: declarator has m_type");
    out.m_type = declarator.m_type;
  }
}

void Parser::parseParameterAbstractDeclarator(Token &out,
                                              const Token &declSpecifiers,
                                              const Token &abstractDeclarator) {
  // TODO(ptc)
  debugLn("Entering parseParameterAbstractDeclarator");
}

void Parser::parseParameterDeclSpecs(Token &out, const Token &declSpecifiers) {
  // TODO(ptc)
  debugLn("Entering parseParameterDeclSpecs");
}

void Parser::parseParameterListBase(Token &out, const Token &parameterDecl) {
  // TODO(ptc)
  debugLn("Entering parseParameterListBase");
}

void Parser::parseParameterList(Token &out, const Token &parameterList,
                                const Token &parameterDecl) {
  // TODO(ptc)
  debugLn("Entering parseParameterList");
}

void Parser::parseParameterTypeList(Token &out, const Token &parameterList) {
  // TODO(ptc)
  debugLn("Entering parseParameterTypeList");
}

void Parser::parseFunctionDirectDeclarator(Token &out,
                                           const Token &directDeclarator,
                                           const Token &parameterTypeList) {
  // TODO(ptc)
  debugLn("Entering parseFunctionDirectDeclarator");
}

void Parser::parseFunctionDefinition(Token &out, const Token &declSpecifiers,
                                     const Token &declarator,
                                     const Token *optDeclList,
                                     const Token &compoundStmt) {
  // TODO(ptc)
  debugLn("Entering parseFunctionDefinition");
}

void Parser::parseReturnStmt(Token &out, const int ret_token,
                             const Token &expr) {
  // TODO(ptc)
  debugLn("Return Statement: RETURN %s;", expr.m_text.c_str());
  out.m_text = "RETURN " + expr.m_text + ";";
}
}
} // ptcc::parser
