#include "parser.h"
#include "parser/grammar.tab.hpp"

#include <assert.h>
#include <cstdarg>

#include "glog/logging.h"

namespace ptcc {
namespace parser {

// Parser::check_type will search m_symbols for a TYPEDEF_NAME and if not found
// will return IDENTIFIER
int Parser::check_type(const char *symbol) {
  auto idx = find(symbol);
  if (idx == -1) {
    debugLn("Checking type of: %s, Not Found, Default To IDENTIFIER", symbol);
    return IDENTIFIER;
  }
  auto entry = get(idx);
  switch (entry.m_token) {
  case IDENTIFIER:
    debugLn("Checking type of: %s, Found: IDENTIFIER", symbol);
    return IDENTIFIER;
  case TYPEDEF_NAME:
    debugLn("Checking type of: %s, Found: TYPEDEF_NAME", symbol);
    return TYPEDEF_NAME;
  default:
    debugLn("Checking type of: %s, Found: %d", symbol, entry.m_token);
    return entry.m_token;
  }
}

ssize_t Parser::find(const std::string &symbol) {
  auto res = m_symtable.find(symbol);
  if (res != m_symtable.end()) {
    return static_cast<size_t>(res->second);
  } else {
    return -1;
  }
}

const SymTableEntry &Parser::get(ssize_t idx) { return m_symbols[idx]; }

ssize_t Parser::insert(const SymTableEntry entry) {
  auto res = m_symtable.find(entry.m_symbol);
  if (res != m_symtable.end()) {
    return static_cast<size_t>(res->second);
  } else {
    auto idx = m_symbols.size();
    m_symtable.emplace(entry.m_symbol, idx);
    m_symbols.emplace_back(std::move(entry));
    return idx;
  }
}

ssize_t Parser::overwrite(const SymTableEntry entry) {
  auto res = m_symtable.find(entry.m_symbol);
  if (res != m_symtable.end()) {
    auto idx = static_cast<size_t>(res->second);
    m_symbols[idx] = std::move(entry);
    return idx;
  } else {
    auto idx = m_symbols.size();
    m_symtable.emplace(entry.m_symbol, idx);
    m_symbols.emplace_back(std::move(entry));
    return idx;
  }
}

void Parser::parseTypeQualifierList(Token &out, const Token &tQual) {
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

TypeSpec Parser::mergeTypes(TypeSpec type, TypeSpec listType) {
  /*

    From the spec:

    At least one type specifier shall be given in the declaration specifiers in
    each declaration, and in the specifier-qualifier list in each struct
    declaration and type name. Each list of type specifiers shall be one of the
    following multisets (delimited by commas, when there is more than one
    multiset per item); the type specifiers may occur in any order, possibly
    intermixed with the other declaration specifiers.

    — void
    — char
    — signed char
    — unsigned char
    — short, signed short, short int, or signed short int — unsigned short, or
    unsigned short int
    — int, signed, or signed int
    — unsigned, or unsigned int
    — long, signed long, long int, or signed long int
    — unsigned long, or unsigned long int
    — long long, signed long long, long long int, or signed long long int
    — unsigned long long, or unsigned long long int
    — float
    — double
    — long double
    — _Bool
    — float _Complex
    — double _Complex
    — long double _Complex
    — atomic type specifier
    — struct or union specifier
    — enum specifier
    — typedef name

    This function  has to deal with when we have two or more types in a
    specifier qualifier list, so we have to check if the two types are in this
    insanity.

   */

  switch (type.m_kind) {
  case TypeKind::Struct:
  case TypeKind::TypeDef:
  case TypeKind::Pointer:
  case TypeKind::Array:
  case TypeKind::Union:
  case TypeKind::Void:
    // These are all errors
    assert(false);
    break;
  case TypeKind::Int:
  case TypeKind::Long:
  case TypeKind::Char:
  case TypeKind::Signed:
  case TypeKind::Unsigned:
    // TODO(ptc)
    // Have to actually implement this, but just throw away one of the types for
    // now to
    // see how far we can get processing a file
    return type;
    assert(false);
    break;
  default:
    // TODO(ptc)
    assert(false);
    break;
  }
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
    if (list->m_type) {
      debugLn("List TYPE exists");
      // These are types like `long long` or `long int`
      out.m_type = std::make_shared<TypeSpec>(
          mergeTypes(*specifier.m_type, *list->m_type));
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
  out.m_declSpecs = pointer.m_declSpecs;
  debugLn("Pointer Declarator Id=%s Type Of %s", out.m_id.c_str(),
          pointerTypeToString(*out.m_type).c_str());
}

void Parser::parseDirectDeclarator(Token &out, const Token &directDecl) {
  // TODO(ptc) flesh out pointer + direct delcarator
  debugLn("Entering parseDirectDeclarator id=%s", directDecl.m_id.c_str());
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
  debugLn("Entering parseTypeSpecifier, token = %i, out.m_text = %s", token,
          out.m_text.c_str());
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
    out.m_type->m_kind = TypeKind::Int;
    break;
  case LONG:
    out.m_token = token;
    out.m_text = "long";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Long;
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
  case UNSIGNED:
    out.m_token = token;
    out.m_text = "unsigned";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Unsigned;
    break;
  case SIGNED:
    out.m_token = token;
    out.m_text = "unsigned";
    out.m_type = std::make_shared<TypeSpec>();
    out.m_type->m_kind = TypeKind::Unsigned;
    break;
  default:
    // TODO(ptc) Handle rest of cases
    assert(false);
  }
}

void Parser::parseTypedefTypeSpec(Token &out, const Token &typeDef) {
  // Look up the aliased type in the symbol table and reference it from m_otype
  auto otype = get(find(typeDef.m_text)).m_type;
  out.m_type = std::make_shared<TypeSpec>(
      TypeSpec{.m_kind = TypeKind::TypeDef,
               .m_quals = {},
               .m_otype = get(find(typeDef.m_text)).m_type,
               .m_struct = nullptr,
               .m_typedefName = typeDef.m_text});
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

void Parser::parseStructDeclarator(Token &out, const Token &structDecl,
                                   const Token *optStructDeclList) {
  debugLn("Struct Declarator List Item: type=%s, id=%s",
          structDecl.m_type ? specToString(*structDecl.m_type).c_str()
                            : "Unknown",
          structDecl.m_id.c_str());

  if (optStructDeclList) {
    out.m_structDeclList = std::move(optStructDeclList->m_structDeclList);
  }
  out.m_structDeclList.push_back(structDecl);
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
  for (size_t i = 0; i < structDeclList.m_structDeclList.size(); i++) {
    auto &declarator = structDeclList.m_structDeclList[i];
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

void Parser::parseStructDeclarationList(Token &out, const Token &structDecl,
                                        const Token *optStructDeclList) {
  debugLn("Struct Declaration List Item: %s", structDecl.m_text.c_str());
  if (optStructDeclList) {
    out.m_fieldDecls = optStructDeclList->m_fieldDecls;
    for (auto &fieldDecl : structDecl.m_fieldDecls) {
      out.m_fieldDecls.push_back(std::move(fieldDecl));
    }
  } else {
    out.m_fieldDecls = structDecl.m_fieldDecls;
  }
}

void Parser::parseStructUnionSpecifier(Token &out, const Token &structOrUnion,
                                       const Token &id,
                                       const Token &structDeclList) {
  debugLn("%s Specifier %s with fields:", structOrUnion.m_text.c_str(),
          id.m_text.c_str());
  std::string fields;
  for (auto &fieldDecl : structDeclList.m_fieldDecls) {
    debugLn("\tField named %s of type %s", fieldDecl.m_name.c_str(),
            specToString(fieldDecl.m_type).c_str());
    fields +=
        "\t" + specToString(fieldDecl.m_type) + " " + fieldDecl.m_name + ";\n";
  }
  out.m_token = structOrUnion.m_token;
  out.m_type = std::make_shared<TypeSpec>();
  if (structOrUnion.m_token == STRUCT) {
    out.m_text = "struct " + id.m_text + " {\n" + fields + "}";
    out.m_type->m_kind = TypeKind::Struct;
  } else if (structOrUnion.m_token == UNION) {
    out.m_text = "union " + id.m_text + " {\n" + fields + "}";
    out.m_type->m_kind = TypeKind::Union;
  }
  // TODO(ptc) fix up usage of m_type->m_struct + m_struct in Token
  out.m_type->m_struct =
      std::make_shared<StructUnionDecl>(id.m_text, structDeclList.m_fieldDecls);
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
          "storageClassSpecifier.m_declSpecs: %p, out.m_declSpecs: %p",
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

void Parser::parseTypeQualifierBaseDeclarationSpecifier(
    Token &out, const Token &typeQualifier) {
  debugLn("Entering parseTypeQualifierBaseDeclarationSpecifier %s",
          typeQualifier.m_text.c_str());
  assert(typeQualifier.m_typeQuals.size() == 1);
  TypeSpec tySpec{.m_kind = TypeKind::Undecided,
                  .m_quals = typeQualifier.m_typeQuals};
  out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
  out.m_declSpecs->m_typeSpecs.push_back(tySpec);
}

void Parser::parseTypeQualifierDeclarationSpecifier(Token &out,
                                                    const Token &typeQualifier,
                                                    const Token &declSpecs) {
  debugLn("Entering parseTypeQualifierDeclarationSpecifier %s",
          typeQualifier.m_text.c_str());
  assert(typeQualifier.m_typeQuals.size() == 1);
  if (declSpecs.m_declSpecs) {
    out.m_declSpecs = declSpecs.m_declSpecs;
    // Look at back of declSpecs.m_declSpecs->m_typeSpecs to see if there
    // is a TypeSpec we can modify
    if (declSpecs.m_declSpecs->m_typeSpecs.size() > 0) {
      out.m_declSpecs->m_typeSpecs.back().m_quals = typeQualifier.m_typeQuals;
    } else {
      // This shouldn't happen, but we haven't finished implementing all of
      // declaration_specifiers so it could happen in theory
      debugLn("parseTypeQualifierDeclarationSpecifier: "
              "declSpecs.m_declSpecs->m_typeSpecs is EMPTY!");
    }
  } else {
    TypeSpec tySpec{.m_kind = TypeKind::Undecided,
                    .m_quals = typeQualifier.m_typeQuals};
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
    out.m_declSpecs->m_typeSpecs.push_back(tySpec);
  }
}

void Parser::parseTypeSpecifierBaseDeclarationSpecifier(
    Token &out, const Token &typeSpecifier) {
  debugLn("Entering parseTypeSpecifierBaseDeclarationSpecifier %s",
          typeSpecifier.m_text.c_str());
  // TODO(ptc) Not sure why we have to re-initialize tokens ...
  out = Token{};
  out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
  assert(typeSpecifier.m_type);
  out.m_text = typeSpecifier.m_text;
  out.m_declSpecs->m_typeSpecs.push_back(*typeSpecifier.m_type);
}

void Parser::parseTypeSpecifierDeclarationSpecifier(
    Token &out, const Token &typeSpecifier,
    const Token &declarationSpecifiers) {
  debugLn("Entering parseTypeSpecifierDeclarationSpecifier %s",
          typeSpecifier.m_text.c_str());
  out = Token{};
  out.m_text = declarationSpecifiers.m_text;
  out.m_declSpecs = declarationSpecifiers.m_declSpecs;
  if (!out.m_declSpecs) {
    // TODO(ptc) Remove this once we actually handle parsing all the possible
    // constructions of declaration_specifier, namely:
    // alignment_specifier and function_specifier
    out.m_declSpecs = std::make_shared<DeclarationSpecifiers>();
  }
  // Should have a type from the TypeSpecifier
  assert(typeSpecifier.m_type);
  out.m_declSpecs->m_typeSpecs.push_back(*typeSpecifier.m_type);
  out.m_text = typeSpecifier.m_text + " " + out.m_text;
  debugLn("Token Output of parseTypeSpecifierDeclarationSpecifier\n%s",
          toString(out).c_str());
  // TODO(ptc) still need to pass through some information here regard to text
  // span and whatnot
}

void Parser::parseDeclarationFromDeclarationSpecifiers(
    Token &out, const Token &declarationSpecifiers,
    const Token *initDeclaratorList) {
  debugLn("Entering parseDeclarationFromDeclarationSpecifiers");
  debugLn(
      "parseTypeSpecifierDeclarationSpecifier: declarationSpecifiers = \n%s",
      toString(declarationSpecifiers).c_str());
  debugLn("parseTypeSpecifierDeclarationSpecifier: initDeclaratorList = \n%s",
          initDeclaratorList ? toString(*initDeclaratorList).c_str()
                             : "nullptr");
  // TODO(ptc) Have to keep this if (declarationSpecifiers.m_declSpecs) check
  // because we do not handle all the declaration_specifier constructions,
  // namely: alignment_specifier and function_specifier
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
    // TODO(ptc) make this much broader, for now just add possible typedef
    // declarations into the symbol table
    if (isTypedef) {
      debugLn("parseDeclarationFromDeclarationSpecifiers: isTypedef");
      assert(!declarationSpecifiers.m_type);
      if (initDeclaratorList) {
        // Have a typedef of some type, add the typedef name to the symbol
        // table with the corresponding type
        debugLn("parseDeclarationFromDeclarationSpecifiers: is "
                "initDeclaratorList");
        debugLn("Adding typedef name of %s", initDeclaratorList->m_id.c_str());
        assert(!declarationSpecifiers.m_declSpecs->m_typeSpecs.empty());
        auto type =
            coalesceTypes(declarationSpecifiers.m_declSpecs->m_typeSpecs);
        SymTableEntry entry{
            .m_symbol = initDeclaratorList->m_id,
            .m_token = TYPEDEF_NAME,
            .m_type = std::make_shared<TypeSpec>(type),
        };
        overwrite(entry);
      }
    }
  }
}

TypeSpec Parser::coalesceTypes(const std::vector<TypeSpec> types) {
  // TODO(ptc) this is similar to mergeTypes and should be combined into on
  // thing
  // also it should be fully implemented
  assert(!types.empty());
  if (types.size() == 1) {
    return types[0];
  } else {
    // TODO(ptc): lolwut remove this code and implement this properly
    return types[types.size() - 1];
  }
}

void Parser::createNewEnumConstant(Token &out, const Token &id) {
  out.m_id = id.m_text;
  out.m_token = ENUMERATION_CONSTANT;
  SymTableEntry entry{
      .m_symbol = id.m_text, .m_token = ENUMERATION_CONSTANT, .m_type = nullptr,
  };
  overwrite(entry);
}

void Parser::parseEnumeratorListBase(Token &out, const Token &enumerator) {
  // TODO(ptc)
}

void Parser::parseEnumeratorList(Token &out, const Token &enumeratorList,
                                 const Token &enumerator) {
  // TODO(ptc)
}

void Parser::parseEnumerator(Token &out, const Token &enumConstant,
                             const Token *optionalConstExpression) {
  // TODO(ptc)
}

void Parser::parseEnumSpecifier(Token &out, const Token &enumeratorList,
                                const Token *optionalEnumId) {
  // TODO(ptc)
  out.m_type = std::make_shared<TypeSpec>();
}

void Parser::parseEnumSpecifierNoEnums(Token &out, const Token &enumId) {
  // TODO(ptc)
  out.m_type = std::make_shared<TypeSpec>();
}

void Parser::parseParameterDeclarator(Token &out, const Token &declSpecifiers,
                                      const Token &declarator) {
  debugLn("Entering parseParameterDeclarator");
  ParameterDecl paramDecl;
  paramDecl.m_declSpecs = declSpecifiers.m_declSpecs;
  if (declSpecifiers.m_declSpecs) {
    debugLn("parseParameterDeclarator: declSpecifiers has m_declSpecs");
  } else {
    debugLn(
        "parseParameterDeclarator: declSpecifiers DOESN'T have m_declSpecs");
  }
  paramDecl.m_id = declarator.m_id;
  debugLn("parseParameterDeclarator: id=%s", declarator.m_id.c_str());
  if (declSpecifiers.m_type) {
    debugLn("parseParameterDeclarator: declSpecifiers has m_type of %s",
            pointerTypeToString(*declSpecifiers.m_type).c_str());
    paramDecl.m_type = declSpecifiers.m_type;
  } else {
    debugLn("parseParameterDeclarator: declSpecifiers DOESN'T HAVE m_type");
    // TODO(ptc) need to take the below code snippet and translate it to here,
    // basically we need to do the same merging we do in parseStructDeclaration
    // of the declarator.m_type (in case it's a pointer) and the specifier
    // qualifier list here, in this case with the declarator.m_type + the
    // declSpecifiers.m_declSpecs

    /*

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

    */

    if (declSpecifiers.m_declSpecs) {
      if (declSpecifiers.m_declSpecs->m_typeSpecs.size() != 0) {
        debugLn("parseParameterDeclarator: declSpecifiers's m_declSpecs has %d "
                "m_typeSpecs",
                declSpecifiers.m_declSpecs->m_typeSpecs.size());
        if (declSpecifiers.m_declSpecs->m_typeSpecs.size() > 1) {
          debugLn("More than one m_typeSpecs, not sure what to do...");
        } else {
          if (declarator.m_type) {
            paramDecl.m_type = declarator.m_type;
            setPtrTo(paramDecl.m_type.get(),
                     declSpecifiers.m_declSpecs->m_typeSpecs[0]);
          } else {
            paramDecl.m_type = std::make_shared<TypeSpec>(
                declSpecifiers.m_declSpecs->m_typeSpecs[0]);
          }
        }
      } else {
        debugLn("parseParameterDeclarator: declSpecifiers's m_declSpecs "
                "DOESN'T HAVE m_typeSpecs");
      }
    }
  }

  out.m_paramDecls = {};
  out.m_paramDecls.push_back(std::move(paramDecl));
}

void Parser::parseParameterAbstractDeclarator(Token &out,
                                              const Token &declSpecifiers,
                                              const Token &abstractDeclarator) {
  debugLn("Entering parseParameterAbstractDeclarator");
  // TODO(ptc)
  out.m_paramDecls = {};
  out.m_paramDecls.emplace_back();
}

void Parser::parseParameterDeclSpecs(Token &out, const Token &declSpecifiers) {
  debugLn("Entering parseParameterDeclSpecs");
  // Pass an empty declarator to parseParameterDeclarator
  Token emptyDeclarator;
  return parseParameterDeclarator(out, declSpecifiers, emptyDeclarator);
}

void Parser::parseParameterListBase(Token &out, const Token &parameterDecl) {
  debugLn("Entering parseParameterListBase");
  debugLn("parameterDecl.m_paramDecls.size() == %d",
          parameterDecl.m_paramDecls.size());
  assert(parameterDecl.m_paramDecls.size() == 1);
  out.m_paramDecls = parameterDecl.m_paramDecls;
}

void Parser::parseParameterList(Token &out, const Token &parameterList,
                                const Token &parameterDecl) {
  debugLn("Entering parseParameterList");
  debugLn("parameterDecl.m_paramDecls.size() == %d",
          parameterDecl.m_paramDecls.size());
  assert(parameterDecl.m_paramDecls.size() == 1);
  out.m_paramDecls = parameterList.m_paramDecls;
  out.m_text = parameterList.m_text + ", " + parameterDecl.m_text;
  out.m_paramDecls.push_back(parameterDecl.m_paramDecls[0]);
}

void Parser::parseParameterTypeList(Token &out, const Token &parameterList) {
  // TODO(ptc) handling ellipsis, but skip for now
  debugLn("Entering parseParameterTypeList");
  out.m_paramDecls = parameterList.m_paramDecls;
  out.m_text = parameterList.m_text;
}

void Parser::parseFunctionDirectDeclarator(Token &out,
                                           const Token &directDeclarator,
                                           const Token &parameterTypeList) {
  // TODO(ptc)
  debugLn("Entering parseFunctionDirectDeclarator");
  debugLn("Function named %s with parameters:", directDeclarator.m_id.c_str());
  for (auto paramDecl : parameterTypeList.m_paramDecls) {
    if (paramDecl.m_type) {
      debugLn("\t%s %s", pointerTypeToString(*paramDecl.m_type).c_str(),
              paramDecl.m_id.size() ? paramDecl.m_id.c_str() : "<unnamed>");
    } else {
      debugLn("\t %s (paramDecl.m_type is nullptr)",
              paramDecl.m_id.size() ? paramDecl.m_id.c_str() : "<unnamed>");
    }
  }
  debugLn("toString(ParameterTypeList) = %s",
          toString(parameterTypeList).c_str());
}

void Parser::parseFunctionDefinition(Token &out, const Token &declSpecifiers,
                                     const Token &declarator,
                                     const Token *optDeclList,
                                     const Token &compoundStmt) {
  // TODO(ptc)
  debugLn("Entering parseFunctionDefinition");
  /* If optDeclList is not nullptr, then it's handling the unusual syntax:

     void foo(a, b)
       int a, b;

   */
  if (optDeclList) {
    // TODO(ptc) Handle this craziness later
    return;
  }
  debugLn(toString(declSpecifiers).c_str());
  debugLn(toString(declarator).c_str());
  debugLn(toString(compoundStmt).c_str());
}

void Parser::parseInitDeclaratorListAdd(Token &out,
                                        const Token &initDeclaratorList,
                                        const Token &initDeclarator) {
  // TODO(ptc)
  debugLn("Entering parseInitDeclaratorListAdd initDeclaratorList = %s, "
          "initDeclarator = %s",
          toString(initDeclaratorList).c_str(),
          toString(initDeclarator).c_str());
}

void Parser::parseInitDeclaratorListBase(Token &out,
                                         const Token &initDeclarator) {
  // TODO(ptc)
  debugLn("Entering parseInitDeclaratorListBase initDeclarator = %s",
          toString(initDeclarator).c_str());
  out = initDeclarator;
}

void Parser::parseInitDeclarator(Token &out, const Token &declarator,
                                 const Token *optInitializer) {
  // TODO(ptc)
  debugLn("Entering parseInitDeclarator declarator = %s",
          toString(declarator).c_str());
  if (optInitializer) {
    debugLn("optInitializer = %s", toString(*optInitializer).c_str());
  }
  out = declarator;
}

void Parser::parseExternalDeclarationFunc(Token &out,
                                          const Token &functionDefinition) {
  // TODO(ptc)
  debugLn("Entering parseExternalDeclarationFunc functionDefinition = %s",
          toString(functionDefinition).c_str());
  out = functionDefinition;
}

void Parser::parseExternalDeclarationDecl(Token &out,
                                          const Token &declaration) {
  // TODO(ptc)
  debugLn("Entering parseExternalDeclarationDecl declaration = %s",
          toString(declaration).c_str());
  out = declaration;
}

void Parser::parseTranslationUnitBase(Token &out, const Token &externalDecl) {
  // TODO(ptc)
  debugLn("Entering parseTranslationUnitBase externalDecl = %s",
          toString(externalDecl).c_str());
}

void Parser::parseTranslationUnitAdd(Token &out, const Token &externalDecl) {
  // TODO(ptc)
  debugLn("Entering parseTranslationUnitAdd externalDecl = %s",
          toString(externalDecl).c_str());
}

void Parser::parseReturnStmt(Token &out, const int ret_token,
                             const Token &expr) {
  // TODO(ptc)
  LOG(INFO) << "Return statement encountered";
  debugLn("Return Statement: RETURN %s;", expr.m_text.c_str());
  out.m_text = "RETURN " + expr.m_text + ";";
}
}
} // ptcc::parser
