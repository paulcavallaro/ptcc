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
      break;
      // clang-format off
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K':    /*'L'*/case 'M': case 'N':
    case 'O': case 'P': case 'Q':    /*'R'*/case 'S': case 'T':    /*'U'*/
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't':    /*'u'*/
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case '_':
      // clang-format on
      // 6.4.2 Identifiers
      break;
  }
  return Token::EOFToken();
}

}  // namespace ptcc
