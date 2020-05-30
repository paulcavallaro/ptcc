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
  switch (tok.type_) {
    case TokenType::EOF_SENTINEL:
      return os << "<EOF>";
    case TokenType::IDENTIFIER:
      return os << absl::StrCat("<IDENTIFIER: '", tok.src_, "'>");
    case TokenType::COMMA:
      return os << absl::StrCat("<COMMA: '", tok.src_, "'>");
    case TokenType::SEMICOLON:
      return os << absl::StrCat("<SEMICOLON: '", tok.src_, "'>");
    case TokenType::PLUS:
      return os << absl::StrCat("<PLUS: '", tok.src_, "'>");
    case TokenType::L_PAREN:
      return os << absl::StrCat("<L_PAREN: '", tok.src_, "'>");
    case TokenType::R_PAREN:
      return os << absl::StrCat("<R_PAREN: '", tok.src_, "'>");
    case TokenType::L_CURLY_BRACE:
      return os << absl::StrCat("<L_CURLY_BRACE: '", tok.src_, "'>");
    case TokenType::R_CURLY_BRACE:
      return os << absl::StrCat("<R_CURLY_BRACE: '", tok.src_, "'>");
    default:
      os << absl::StrCat("<UNKNOWN: '", tok.src_, "'>");
  }
  return os;
}

void Token::SetIdentifierInfo(IdentifierInfo* info) {
  // TODO: Check that type is Identifier
  data_ptr_.ident_ = info;
}

}  // namespace ptcc
