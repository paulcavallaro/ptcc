#include "lexer/lexer.h"
#include "lexer/token.h"

namespace ptcc {

Lexer::Lexer(absl::string_view text)
    : text_(text), cur_pos_(0), cur_ptr_(text_.data()) {}

Token Lexer::NextToken() {
  if (cur_pos_ >= text_.length()) return Token::EOFToken();
  return Token::EOFToken();
}

}  // namespace ptcc
