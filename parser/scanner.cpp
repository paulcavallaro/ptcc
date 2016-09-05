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
}
} // ptc::scanner
