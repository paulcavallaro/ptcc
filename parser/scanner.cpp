#include "scanner.h"

#include <queue>

namespace ptcc {
namespace parser {

std::string kindToString(const TypeKind kind) {
  switch (kind) {
  case TypeKind::Void:
    return "void";
  case TypeKind::Int:
    return "int";
  case TypeKind::Short:
    return "long";
  case TypeKind::Long:
    return "long";
  case TypeKind::Signed:
    return "signed";
  case TypeKind::Undecided:
    return "<undecided>";
  case TypeKind::Unsigned:
    return "unsigned";
  case TypeKind::Double:
    return "double";
  case TypeKind::Float:
    return "float";
  case TypeKind::Enum:
    return "enum";
  case TypeKind::Char:
    return "char";
  case TypeKind::Bool:
    return "bool";
  case TypeKind::Struct:
    return "struct";
  case TypeKind::Union:
    return "union";
  case TypeKind::Array:
    return "array";
  case TypeKind::Pointer:
    return "*";
  case TypeKind::TypeDef:
    return "typedef";
  }
  return "<UNKNOWN>";
}

std::string typeQualToString(const TypeQual qual) {
  switch (qual) {
  case TypeQual::Const:
    return "const";
  case TypeQual::Restrict:
    return "restrict";
  case TypeQual::Volatile:
    return "volatile";
  case TypeQual::Atomic:
    return "_Atomic";
  }
  return "<UNKNOWN>";
}

std::string pointerTypeToString(const TypeSpec tSpec) {
  std::queue<TypeSpec> typeQ;
  // TODO(ptc) actually write this properly, pretty sure this is just incorrect
  typeQ.push(tSpec);
  std::string ret;
  while (!typeQ.empty()) {
    auto type = typeQ.front();
    if (type.m_otype) {
      typeQ.push(*type.m_otype);
    }
    switch (type.m_kind) {
    case TypeKind::Pointer:
      for (auto &tQual : type.m_quals) {
        ret = " " + typeQualToString(tQual) + ret;
      }
      ret = "*" + ret;
      break;
    case TypeKind::Array:
      ret = ret + "[]";
      break;
    default:
      std::string tyQuals;
      for (auto &tQual : type.m_quals) {
        tyQuals = typeQualToString(tQual) + " " + tyQuals;
      }
      ret = tyQuals + kindToString(type.m_kind) + ret;
      break;
    }
    typeQ.pop();
  }
  return ret;
}

std::string specToString(const TypeSpec typeSpec) {
  switch (typeSpec.m_kind) {
  case TypeKind::Pointer:
    return pointerTypeToString(typeSpec);
  case TypeKind::Array:
    return pointerTypeToString(typeSpec);
    break;
  default:
    return kindToString(typeSpec.m_kind);
  }
}

std::string tyQualToString(const TypeQual tyQual) {
  switch (tyQual) {
  case TypeQual::Const:
    return "const";
  case TypeQual::Restrict:
    return "restrict";
  case TypeQual::Volatile:
    return "volatile";
  case TypeQual::Atomic:
    return "_Atomic";
  }
  return "<UNKNOWN>";
}

std::string toString(const Token token) {
  std::string ret = "struct Token {\n";
  ret += "\tm_token = " + std::to_string(token.m_token) + "\n";
  ret += "\tm_text = '" + token.m_text + "'\n";
  ret += "\tm_id = '" + token.m_id + "'\n";
  ret += "\tm_expr = " + toString(token.m_expr) + "\n";
  ret += "\tm_constant = " + toString(token.m_constant) + "\n";
  ret += "\tm_type = " + toString(token.m_type) + "\n";
  ret += "\tm_struct = " + toString(token.m_struct) + "\n";
  ret += "\tm_declSpecs = " + toString(token.m_declSpecs) + "\n";
  ret += "\tm_fieldDecls = " + toString(token.m_fieldDecls) + "\n";
  ret += "\tm_typeQuals = " + toString(token.m_typeQuals) + "\n";
  ret += "\tm_paramDecls = " + toString(token.m_paramDecls) + "\n";
  ret += "\tm_structDeclList = " + toString(token.m_structDeclList) + "\n";
  ret += "}";
  return ret;
}

std::string toString(const ParameterDecl paramDecl) {
  std::string ret = "struct ParameterDecl {\n";
  ret += "\tm_id = " + paramDecl.m_id + "\n";
  ret += "\tm_declSpecs = " + toString(paramDecl.m_declSpecs) + "\n";
  ret += "\tm_type = " + toString(paramDecl.m_type) + "\n";
  ret += "}";
  return ret;
}

std::string toString(const TypeQual tyQual) {
  switch (tyQual) {
  case TypeQual::Const:
    return "TypeQual::Const";
  case TypeQual::Restrict:
    return "TypeQual::Restrict";
  case TypeQual::Volatile:
    return "TypeQual::Volatile";
  case TypeQual::Atomic:
    return "TypeQual::Atomic";
  }
  return "<UNKNOWN TypeQual>";
}

std::string toString(const DeclarationSpecifiers declSpecs) {
  std::string ret = "struct DeclarationSpecifiers {\n";
  ret += "\tm_storageClassSpecs = " + toString(declSpecs.m_storageClassSpecs) +
         "\n";
  ret += "\tm_typeSpecs = " + toString(declSpecs.m_typeSpecs) + "\n";
  ret += "}";
  return ret;
}

std::string toString(const StorageClassSpecifier storageClassSpec) {
  switch (storageClassSpec) {
  case StorageClassSpecifier::Typedef:
    return "StorageClassSpecifier::Typedef";
  case StorageClassSpecifier::Extern:
    return "StorageClassSpecifier::Extern";
  case StorageClassSpecifier::Static:
    return "StorageClassSpecifier::Static";
  case StorageClassSpecifier::ThreadLocal:
    return "StorageClassSpecifier::ThreadLocal";
  case StorageClassSpecifier::Auto:
    return "StorageClassSpecifier::Auto";
  case StorageClassSpecifier::Register:
    return "StorageClassSpecifier::Register";
  }
  return "<UNKNOWN StorageClassSpecifier>";
}

std::string toString(const StructUnionDecl structUnionDecl) {
  std::string ret = "struct StructUnionDecl {\n";
  ret += "\tm_name = '" + structUnionDecl.m_name + "'\n";
  ret += "\tm_members = '" + toString(structUnionDecl.m_members) + "'\n";
  ret += "}";
  return ret;
}

std::string toString(const FieldDecl fieldDecl) {
  std::string ret = "struct FieldDecl {\n";
  ret += "\tm_type = " + toString(fieldDecl.m_type) + "\n";
  ret += "\tm_name = " + fieldDecl.m_name + "\n";
  ret += "}";
  return ret;
}

std::string toString(const TypeSpec typeSpec) { return specToString(typeSpec); }

std::string toString(const ConstantType constType) {
  switch (constType) {
  case ConstantType::Int32:
    return "ConstantType::Int32";
  case ConstantType::Int64:
    return "ConstantType::Int64";
  case ConstantType::Uint32:
    return "ConstantType::Uint32";
  case ConstantType::Uint64:
    return "ConstantType::Uint64";
  case ConstantType::Float:
    return "ConstantType::Float";
  case ConstantType::Double:
    return "ConstantType::Double";
  case ConstantType::Enum:
    return "ConstantType::Enum";
  }
  return "<UNKOWN ConstantType>";
}

std::string toString(const Constant constant) {
  std::string ret = "struct Constant {\n";
  ret += "\tm_type = " + toString(constant.m_type) + "\n";
  ret += "\tm_text = " + constant.m_text + "\n";
  ret += "\tm_ivalue = " + std::to_string(constant.m_ivalue) + "\n";
  ret += "\tm_dvalue = " + std::to_string(constant.m_dvalue) + "\n";
  ret += "}";
  return ret;
}

std::string toString(const ExprType exprType) {
  switch (exprType) {
  case ExprType::Constant:
    return "ExprType::Constant";
  case ExprType::Identifier:
    return "ExprType::Identifier";
  case ExprType::String:
    return "ExprType::String";
  case ExprType::BinaryExpr:
    return "ExprType::BinaryExpr";
  }
  return "<UNKNOWN ExprType>";
}

std::string toString(const Expression expr) {
  std::string ret = "struct Expression {\n";
  ret += "\t m_type = " + toString(expr.m_type) + "\n";
  ret += "\t m_text =" + expr.m_text + "\n";
  ret += "\t m_token =" + std::to_string(expr.m_token) + "\n";
  ret += "}";
  return ret;
}

template <typename T> std::string toString(const std::shared_ptr<T> token) {
  if (token) {
    return toString(*token);
  }
  return "std::shared_ptr { nullptr }";
}

template <typename T> std::string toString(const std::vector<T> vec) {
  if (vec.size() == 0) {
    return "std::vector { }";
  }
  std::string ret = "std::vector {\n";
  for (auto item : vec) {
    ret += "\t" + toString(item) + "\n";
  }
  ret += "}";
  return ret;
}
}
} // ptc::scanner
