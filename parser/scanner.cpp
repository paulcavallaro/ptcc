#include "scanner.h"

#include <queue>

namespace ptcc {
namespace parser {

std::string kindToString(const TypeKind kind) {
  switch (kind) {
  case TypeKind::Void:
    return "void";
  case TypeKind::Int64:
    return "int64";
  case TypeKind::Int32:
    return "int32";
  case TypeKind::Double:
    return "double";
  case TypeKind::Float:
    return "float";
  case TypeKind::Char:
    return "char";
  case TypeKind::Bool:
    return "bool";
  case TypeKind::Struct:
    return "struct";
  default:
    return "Unknown";
  }
}

std::string typeQualToString(const TypeQual qual) {
  switch (qual) {
  case TypeQual::Const:
    return "const";
  case TypeQual::Restrict:
    return "restruct";
  case TypeQual::Volatile:
    return "volatile";
  case TypeQual::Atomic:
    return "_Atomic";
  }
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
      ret = "[]" + ret;
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
    // TODO(ptc) write an array to string function
    return "UNIMPLEMENTED ARRAY specToString";
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
}
}
} // ptc::scanner
