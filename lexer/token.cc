#include "lexer/token.h"

namespace ptcc {

TokenType Token::Type() { return type_; }

Token Token::NewEOF() { return Token(TokenType::EOF_SENTINEL); }

void Token::SetIdentifierInfo(IdentifierInfo* info) {
  // TODO: Check that type is Identifier
  data_ptr_.ident_ = info;
}

}  // namespace ptcc
