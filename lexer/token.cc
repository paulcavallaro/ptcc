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
    case TokenType::ARROW:
      return os << absl::StrCat("<ARROW: '", tok.src_, "'>");
    case TokenType::IDENTIFIER:
      return os << absl::StrCat("<IDENTIFIER: '", tok.src_, "'>");
    case TokenType::DOT:
      return os << absl::StrCat("<DOT: '", tok.src_, "'>");
    case TokenType::ELLIPSIS:
      return os << absl::StrCat("<ELLIPSIS: '", tok.src_, "'>");
    case TokenType::STRING_LITERAL:
      return os << absl::StrCat("<STRING_LITERAL: ", tok.src_, " >");
    case TokenType::L_BRACKET:
      return os << absl::StrCat("<L_BRACKET: '", tok.src_, "'>");
    case TokenType::R_BRACKET:
      return os << absl::StrCat("<R_BRACKET: '", tok.src_, "'>");
    case TokenType::COMMA:
      return os << absl::StrCat("<COMMA: '", tok.src_, "'>");
    case TokenType::TILDE:
      return os << absl::StrCat("<TILDE: '", tok.src_, "'>");
    case TokenType::SEMICOLON:
      return os << absl::StrCat("<SEMICOLON: '", tok.src_, "'>");
    case TokenType::MINUS:
      return os << absl::StrCat("<MINUS: '", tok.src_, "'>");
    case TokenType::MINUSMINUS:
      return os << absl::StrCat("<MINUSMINUS: '", tok.src_, "'>");
    case TokenType::PLUS:
      return os << absl::StrCat("<PLUS: '", tok.src_, "'>");
    case TokenType::PLUSPLUS:
      return os << absl::StrCat("<PLUSPLUS: '", tok.src_, "'>");
    case TokenType::L_PAREN:
      return os << absl::StrCat("<L_PAREN: '", tok.src_, "'>");
    case TokenType::R_PAREN:
      return os << absl::StrCat("<R_PAREN: '", tok.src_, "'>");
    case TokenType::L_CURLY_BRACE:
      return os << absl::StrCat("<L_CURLY_BRACE: '", tok.src_, "'>");
    case TokenType::R_CURLY_BRACE:
      return os << absl::StrCat("<R_CURLY_BRACE: '", tok.src_, "'>");
    case TokenType::NUMERIC_CONSTANT:
      return os << absl::StrCat("<NUMERIC_CONSTANT: '", tok.src_, "'>");
    case TokenType::STAR:
      return os << absl::StrCat("<STAR: '", tok.src_, "'>");
    case TokenType::CONST:
      return os << absl::StrCat("<CONST: '", tok.src_, "'>");
    case TokenType::CHAR:
      return os << absl::StrCat("<CHAR: '", tok.src_, "'>");
    case TokenType::AND:
      return os << absl::StrCat("<AND: '", tok.src_, "'>");
    case TokenType::ANDAND:
      return os << absl::StrCat("<ANDAND: '", tok.src_, "'>");
    case TokenType::CHARACTER_CONSTANT:
      return os << absl::StrCat("<CHARACTER_CONSTANT: ", tok.src_, " >");
    case TokenType::EQ:
      return os << absl::StrCat("<EQ: '", tok.src_, "'>");
    case TokenType::EQEQ:
      return os << absl::StrCat("<EQEQ: '", tok.src_, "'>");
    case TokenType::LT:
      return os << absl::StrCat("<LT: '", tok.src_, "'>");
    case TokenType::LTEQ:
      return os << absl::StrCat("<LTEQ: '", tok.src_, "'>");
    case TokenType::GT:
      return os << absl::StrCat("<GT: '", tok.src_, "'>");
    case TokenType::GTEQ:
      return os << absl::StrCat("<GTEQ: '", tok.src_, "'>");
    case TokenType::VOID:
      return os << absl::StrCat("<VOID: '", tok.src_, "'>");
    case TokenType::IF:
      return os << absl::StrCat("<IF: '", tok.src_, "'>");
    case TokenType::ELSE:
      return os << absl::StrCat("<ELSE: '", tok.src_, "'>");
    case TokenType::INT:
      return os << absl::StrCat("<INT: '", tok.src_, "'>");
    case TokenType::RETURN:
      return os << absl::StrCat("<RETURN: '", tok.src_, "'>");
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
