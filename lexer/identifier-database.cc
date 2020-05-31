#include "lexer/identifier-database.h"
#include "lexer/token-type.h"

#include <utility>

namespace ptcc {

constexpr std::array<std::pair<absl::string_view, TokenType>, 43> kKeywords = {{
    {"auto", TokenType::AUTO},
    {"break", TokenType::BREAK},
    {"case", TokenType::CASE},
    {"char", TokenType::CHAR},
    {"const", TokenType::CONST},
    {"continue", TokenType::CONTINUE},
    {"default", TokenType::DEFAULT},
    {"do", TokenType::DO},
    {"double", TokenType::DO},
    {"else", TokenType::ELSE},
    {"enum", TokenType::ENUM},
    {"extern", TokenType::EXTERN},
    {"float", TokenType::FLOAT},
    {"for", TokenType::FOR},
    {"goto", TokenType::GOTO},
    {"if", TokenType::IF},
    {"inline", TokenType::INLINE},
    {"int", TokenType::INT},
    {"long", TokenType::LONG},
    {"register", TokenType::REGISTER},
    {"restrict", TokenType::RESTRICT},
    {"return", TokenType::RETURN},
    {"short", TokenType::SHORT},
    {"signed", TokenType::SIGNED},
    {"sizeof", TokenType::SIZEOF},
    {"static", TokenType::STATIC},
    {"struct", TokenType::STRUCT},
    {"switch", TokenType::SWITCH},
    {"typedef", TokenType::TYPEDEF},
    {"union", TokenType::UNION},
    {"unsigned", TokenType::UNSIGNED},
    {"void", TokenType::VOID},
    {"volatile", TokenType::VOLATILE},
    {"while", TokenType::WHILE},
    {"_Alignas", TokenType::_ALIGNAS},
    {"_Atomic", TokenType::_ATOMIC},
    {"_Bool", TokenType::_BOOL},
    {"_Complex", TokenType::_COMPLEX},
    {"_Generic", TokenType::_GENERIC},
    {"_Imaginary", TokenType::_IMAGINARY},
    {"_Noreturn", TokenType::_NORETURN},
    {"_Static_assert", TokenType::_STATIC_ASSERT},
    {"_Thread_local", TokenType::_THREAD_LOCAL},
}};

IdentifierDatabase::IdentifierDatabase() {
  idents_.reserve(kKeywords.size() * 2);
  for (auto a : kKeywords) {
    idents_.emplace(a.first, IdentifierInfo(a.second));
  }
};

IdentifierInfo* IdentifierDatabase::MakeIdentifier(absl::string_view ident) {
  auto it = idents_.find(ident);
  if (it != idents_.end()) {
    return &it->second;
  }
  it = idents_.emplace_hint(it, ident, IdentifierInfo{});
  return &it->second;
}

}  // namespace ptcc
