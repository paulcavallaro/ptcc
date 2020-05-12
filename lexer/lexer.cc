#include "lexer/lexer.h"
#include "lexer/token.h"

namespace ptcc {

Lexer::Lexer(absl::string_view text) : text_(text), cur_ptr_(text_.data()) {}

Token Lexer::NextToken() {
  switch (*cur_ptr_) {
    case 0:
      return Token::EOFToken();
    case ' ':
    case '\t':
    case '\v':
      // Horizontal whitespace
      cur_ptr_ += 1;
  }
  return Token::EOFToken();
}

}  // namespace ptcc
