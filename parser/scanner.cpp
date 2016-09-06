#include "scanner.h"

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
  default:
    return "Unknown";
  }
}

std::string specToString(const TypeSpec typeSpec) {
  return kindToString(typeSpec.m_kind);
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
