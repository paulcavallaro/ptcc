#include "lexer/lexer.h"
#include "lexer/token.h"

namespace ptcc {

Lexer::Lexer(absl::Cord text) : text_(text) {}

Token Lexer::NextToken() { return Token(TokenType::AUTO); }

}  // namespace ptcc
