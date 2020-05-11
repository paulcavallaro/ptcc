#include "lexer/token.h"

namespace ptcc {

TokenType Token::Type() { return type_; }

Token Token::EOFToken() { return Token(TokenType::EOF_SENTINEL); }

}  // namespace ptcc
