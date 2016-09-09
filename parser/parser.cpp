#include "parser.h"
#include "grammar.tab.hpp"

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
}

void Parser::parseSpecifierQualifierListSpecifier(Token &out,
                                                  const Token &specifier,
                                                  const Token *list) {
  debugLn("Entering parseSpecifierQualifierListSpecifier");
  assert(specifier.m_type);
  m_typeSpecs.push_back(*specifier.m_type);
  if (list) {
    // TODO(ptc) maybe need to do something here
  }
}

void Parser::parseSpecifierQualifierListQualifier(Token &out,
                                                  const Token &qualifier,
                                                  const Token *list) {
  debugLn("Entering parseSpecifierQualifierListQualifier");
  assert(qualifier.m_typeQuals.size() == 1);
  m_typeQuals.push_back(qualifier.m_typeQuals[0]);
  if (list) {
    // TODO(ptc) maybe need to do something here
  }
}

void Parser::resetSpecifierQualifierList() {
  m_typeSpecs.clear();
  m_typeQuals.clear();
}

void Parser::parsePointerTyQual(Token &out, const Token &tyQualList,
                                const Token *pointer) {
  debugLn("Entering parsePointerTyQual");
  out.m_token = '*';
  out.m_text = "*";
  out.m_type =
      std::make_shared<TypeSpec>(TypeSpec{.m_kind = TypeKind::Pointer});
  out.m_type->m_quals = tyQualList.m_typeQuals;
  for (auto &tyQual : tyQualList.m_typeQuals) {
    out.m_text += " ";
    out.m_text += tyQualToString(tyQual);
  }
  if (pointer) {
    out.m_type->m_otype = pointer->m_type;
    out.m_text += pointer->m_text;
  }
  debugLn("Pointer Type Qualifier of %s", out.m_text.c_str());
}

void Parser::parsePointer(Token &out, const Token *pointer) {
  out.m_token = '*';
  out.m_text = "*";
  out.m_type =
      std::make_shared<TypeSpec>(TypeSpec{.m_kind = TypeKind::Pointer});
  if (pointer) {
    out.m_type->m_otype = pointer->m_type;
    out.m_text = out.m_text + pointer->m_text;
  }
  debugLn("Pointer of %s", out.m_text.c_str());
}

void Parser::parsePointerDeclarator(Token &out, const Token &pointer,
                                    const Token &directDecl) {
  // TODO(ptc) flesh out pointer + direct delcarator
  out.m_text = pointer.m_text + directDecl.m_text;
  out.m_type = pointer.m_type;
}

void Parser::parseDirectDeclarator(Token &out, const Token &directDecl) {
  // TODO(ptc) flesh out pointer + direct delcarator
  out.m_text = directDecl.m_text;
}

void Parser::parseTypeQualifier(Token &out, const int token) {
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
}

void Parser::parseDirectDeclaratorArray(Token &out, const Token &declarator) {
  debugLn("Direct Declarator of Direct Declarator Array []: %s",
          declarator.m_text.c_str());
  // TODO(ptc) implement this
  out = declarator;
  out.m_text = declarator.m_text + "[]";
}

void Parser::parseStructDeclarator(Token &out, const Token &structDecl) {
  debugLn("Struct Declarator List Item: %s", structDecl.m_text.c_str());
  m_structDeclList.push_back(structDecl);
}

void Parser::parseStructDeclaration(Token &out, const Token &specQualList,
                                    const Token &structDeclList) {
  // Type specifiers and qualifiers should be in parser local variables
  // m_typeQuals + m_typeSpecs
  assert(!(m_typeQuals.empty() && m_typeSpecs.empty()));

  // TODO(ptc) In the future we should do merging of type specifiers such as
  // "long long" or "long int" or "unsigned int", but for now just take the
  // first type specifier as the type
  if (m_typeSpecs.empty()) {
    debugLn("Can't handle empty typeSpecs!");
    assert(false);
  }
  m_structDeclType = m_typeSpecs[0];
  m_structDeclType.m_quals = m_typeQuals;

  std::string names;
  for (int i = 0; i < m_structDeclList.size(); i++) {
    auto &declarator = m_structDeclList[i];
    if (i != 0) {
      names = names + " " + declarator.m_text;
    } else {
      names = declarator.m_text;
    }
  }
  debugLn("Struct Declaration of Specifier Qualifier List + Struct "
          "Declarator List: %s %s",
          specQualList.m_text.c_str(), names.c_str());
}

void Parser::parseStructDeclarationList(Token &out, const Token &structDecl) {
  debugLn("Struct Declaration List Item: %s", structDecl.m_text.c_str());
  for (auto &declarator : m_structDeclList) {
    FieldDecl field;
    if (!declarator.m_type) {
      field.m_type = m_structDeclType;
    } else {
      // Declarators can encode extra information about the type that the
      // declared
      // thing references, either as a pointer, array, etc.
      switch (declarator.m_type->m_kind) {
      // TODO(ptc) codify and expand how this will work for all types not just
      // the basic
      // pointer type
      case TypeKind::Pointer:
        field.m_type = *declarator.m_type;
        field.m_type.m_otype = std::make_shared<TypeSpec>(m_structDeclType);
        break;
      case TypeKind::Array:
        field.m_type = *declarator.m_type;
        field.m_type.m_otype = std::make_shared<TypeSpec>(m_structDeclType);
        break;
      default:
        debugLn("Encountered unhandled declarator m_type!");
        field.m_type = *declarator.m_type;
        break;
      }
    }
    field.m_name = declarator.m_text;
    m_structFieldList.push_back(field);
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

void Parser::parseReturnStmt(Token &out, const int ret_token,
                             const Token &expr) {
  debugLn("Return Statement: RETURN %s;", expr.m_text.c_str());
  out.m_text = "RETURN " + expr.m_text + ";";
}
}
} // ptcc::parser
