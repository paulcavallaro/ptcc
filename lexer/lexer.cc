#include "lexer/lexer.h"
#include "lexer/token.h"

namespace ptcc {

Lexer::Lexer(absl::string_view text) : text_(text), cur_ptr_(text_.data()) {}

Token Lexer::NextToken() {
LexNextToken:
  // Read current character and advance
  const char cur = *(cur_ptr_++);
  switch (cur) {
    case 0:
      return Token::NewEOF();
    case ' ':
    case '\t':
    case '\v':
      // Horizontal whitespace
      // Munch remaining whitespace
      while (*cur_ptr_ == ' ' || *cur_ptr_ == '\t' || *cur_ptr_ == '\v') {
        ++cur_ptr_;
      }
      goto LexNextToken;
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
      return LexIdentifier();
  }
  return Token::NewEOF();
}

Token Lexer::LexIdentifier() {
  // Already parsed first character of identifier, need to parse the rest
  const char* start = cur_ptr_ - 1;
  switch (*cur_ptr_) {
    // clang-format off
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case '_':
    case '0': case '1': case '2': case '3': case '4': case '5': case '6':
    case '7': case '8': case '9':
      // clang-format on
      cur_ptr_++;
      break;
    default:
      absl::string_view src(start, cur_ptr_ - start);
      IdentifierInfo* info = idents_.MakeIdentifier(src);
      return Token::NewIdentifier(src, info);
  }
  return Token::NewEOF();
}

}  // namespace ptcc
