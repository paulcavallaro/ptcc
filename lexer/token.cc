#include "lexer/token.h"

#include "absl/strings/str_cat.h"

#include <ostream>

namespace ptcc {

TokenType Token::Type() { return type_; }

Token Token::NewEOF() { return Token(TokenType::EOF_SENTINEL, ""); }

bool Token::isEOF() { return type_ == TokenType::EOF_SENTINEL; }

Token Token::NewIdentifier(absl::string_view src, IdentifierInfo* info) {
  Token tok(TokenType::IDENTIFIER, src);
  tok.SetIdentifierInfo(info);
  return tok;
}

std::ostream& operator<<(std::ostream& os, const Token& tok) {
  if (tok.type_ == TokenType::EOF_SENTINEL) {
    os << "<EOF>";
  } else if (tok.type_ == TokenType::IDENTIFIER) {
    os << absl::StrCat("<IDENTIFIER: '", tok.src_, "'>");
  } else {
    os << absl::StrCat("<UNKNOWN: '", tok.src_, "'>");
  }
  return os;
}

void Token::SetIdentifierInfo(IdentifierInfo* info) {
  // TODO: Check that type is Identifier
  data_ptr_.ident_ = info;
}

}  // namespace ptcc
