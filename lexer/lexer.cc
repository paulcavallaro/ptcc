#include "lexer/lexer.h"
#include "lexer/token.h"

namespace ptcc {

namespace {

bool isHexChar(const char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

bool isOctalDigit(const char c) { return c >= '0' && c <= '7'; }

}  // namespace

Lexer::Lexer(absl::string_view text) : text_(text), cur_ptr_(text_.data()) {}

Token Lexer::NextToken() {
LexNextToken:
  // Read current character and advance
  const char cur = *(cur_ptr_++);
  switch (cur) {
    case '\0':
      return Token::NewEOF();
    case '\n':
      // Vertical whitespace
      // TODO(line numbering and continuation)
      goto LexNextToken;
    case ' ':
    case '\t':
    case '\v':
      // Horizontal whitespace
      // Munch remaining whitespace
      while (*cur_ptr_ == ' ' || *cur_ptr_ == '\t' || *cur_ptr_ == '\v') {
        ++cur_ptr_;
      }
      goto LexNextToken;
    case '=':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::EQEQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::EQ, absl::string_view(cur_ptr_ - 1, 1));
    case '*':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::STAR_EQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::STAR, absl::string_view(cur_ptr_ - 1, 1));
    case ';':
      // 6.4.6 Punctuators
      return Token(TokenType::SEMICOLON, absl::string_view(cur_ptr_ - 1, 1));
    case '!':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::NOT_EQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::NOT, absl::string_view(cur_ptr_ - 1, 1));
    case '<':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::LTEQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      if (*cur_ptr_ == '<') {
        cur_ptr_++;
        if (*cur_ptr_ == '=') {
          cur_ptr_++;
          return Token(TokenType::L_SHIFT_EQ,
                       absl::string_view(cur_ptr_ - 3, 3));
        }
        return Token(TokenType::L_SHIFT, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::LT, absl::string_view(cur_ptr_ - 1, 1));
    case '>':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::GTEQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      if (*cur_ptr_ == '>') {
        cur_ptr_++;
        if (*cur_ptr_ == '=') {
          cur_ptr_++;
          return Token(TokenType::R_SHIFT_EQ,
                       absl::string_view(cur_ptr_ - 3, 3));
        }
        return Token(TokenType::R_SHIFT, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::GT, absl::string_view(cur_ptr_ - 1, 1));
    case '#':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '#') {
        cur_ptr_++;
        return Token(TokenType::HASH_HASH, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::HASH, absl::string_view(cur_ptr_ - 1, 1));
    case '|':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::BITWISE_OR_EQ,
                     absl::string_view(cur_ptr_ - 2, 2));
      }
      if (*cur_ptr_ == '|') {
        cur_ptr_++;
        return Token(TokenType::LOGICAL_OR, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::BITWISE_OR, absl::string_view(cur_ptr_ - 1, 1));
    case '?':
      // 6.4.6 Punctuators
      return Token(TokenType::Q_MARK, absl::string_view(cur_ptr_ - 1, 1));
    case ':':
      // 6.4.6 Punctuators
      return Token(TokenType::COLON, absl::string_view(cur_ptr_ - 1, 1));
    case '^':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::CARET_EQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::CARET, absl::string_view(cur_ptr_ - 1, 1));
    case '%':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::MOD_EQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::MOD, absl::string_view(cur_ptr_ - 1, 1));
    case '&':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '&') {
        cur_ptr_++;
        return Token(TokenType::LOGICAL_AND,
                     absl::string_view(cur_ptr_ - 2, 2));
      }
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::BITWISE_AND_EQ,
                     absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::BITWISE_AND, absl::string_view(cur_ptr_ - 1, 1));
    case '+':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '+') {
        cur_ptr_++;
        return Token(TokenType::PLUSPLUS, absl::string_view(cur_ptr_ - 2, 2));
      }
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::PLUS_EQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::PLUS, absl::string_view(cur_ptr_ - 1, 1));
    case '-':
      // 6.4.6 Punctuators
      switch (*cur_ptr_) {
        case '-':
          cur_ptr_++;
          return Token(TokenType::MINUSMINUS,
                       absl::string_view(cur_ptr_ - 2, 2));
        case '>':
          cur_ptr_++;
          return Token(TokenType::ARROW, absl::string_view(cur_ptr_ - 2, 2));
        case '=':
          cur_ptr_++;
          return Token(TokenType::MINUS_EQ, absl::string_view(cur_ptr_ - 2, 2));
        default:
          return Token(TokenType::MINUS, absl::string_view(cur_ptr_ - 1, 1));
      }
    case ',':
      // 6.4.6 Punctuators
      return Token(TokenType::COMMA, absl::string_view(cur_ptr_ - 1, 1));
    case '(':
      // 6.4.6 Punctuators
      return Token(TokenType::L_PAREN, absl::string_view(cur_ptr_ - 1, 1));
    case ')':
      // 6.4.6 Punctuators
      return Token(TokenType::R_PAREN, absl::string_view(cur_ptr_ - 1, 1));
    case '{':
      // 6.4.6 Punctuators
      return Token(TokenType::L_CURLY_BRACE,
                   absl::string_view(cur_ptr_ - 1, 1));
    case '}':
      // 6.4.6 Punctuators
      return Token(TokenType::R_CURLY_BRACE,
                   absl::string_view(cur_ptr_ - 1, 1));
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
    case 'u':
      if (*cur_ptr_ == '\'') {
        cur_ptr_++;
        // TODO: Signal that this is a multi-byte character constant
        return LexCharacterConstant();
      }
      if (*cur_ptr_ == '"') {
        cur_ptr_++;
        // TODO: Signal that this is a multi-byte string literal
        return LexStringLiteral();
      }
      if (*cur_ptr_ == '8') {
        cur_ptr_++;
        if (*cur_ptr_ == '"') {
          // TODO: Signal that this is a UTF-8 string literal
          cur_ptr_++;
          return LexStringLiteral();
        }
        cur_ptr_--;
      }
      return LexIdentifier();
    case 'U':
      if (*cur_ptr_ == '\'') {
        cur_ptr_++;
        // TODO: Signal that this is a multi-byte character constant
        return LexCharacterConstant();
      }
      if (*cur_ptr_ == '"') {
        cur_ptr_++;
        // TODO: Signal that this is a multi-byte string literal
        return LexStringLiteral();
      }
      return LexIdentifier();
    case 'L':
      if (*cur_ptr_ == '"') {
        cur_ptr_++;
        // TODO: Signal that this is a multi-byte string literal
        return LexStringLiteral();
      }
      return LexIdentifier();
      // clang-format off
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      // clang-format on
      // 6.4.4.1 Integer Constants
      // 6.4.4.2 Floating Constants
      return LexNumericConstant();
    case '\'':
      // 6.4.4.4 Character Contants
      return LexCharacterConstant();
    case '"':
      // 6.4.5 String Literals
      return LexStringLiteral();
    case '[':
      // 6.4.6 Punctuators
      return Token(TokenType::L_BRACKET, absl::string_view(cur_ptr_ - 1, 1));
    case ']':
      // 6.4.6 Punctuators
      return Token(TokenType::R_BRACKET, absl::string_view(cur_ptr_ - 1, 1));
    case '~':
      // 6.4.6 Punctuators
      return Token(TokenType::TILDE, absl::string_view(cur_ptr_ - 1, 1));
    case '.':
      // 6.4.6 Punctuators
      if (*cur_ptr_ == '.' && *(cur_ptr_ + 1) == '.') {
        cur_ptr_ += 2;
        return Token(TokenType::ELLIPSIS, absl::string_view(cur_ptr_ - 3, 3));
      }
      if (*cur_ptr_ >= '0' && *cur_ptr_ <= '9') {
        return LexNumericConstant();
      }
      return Token(TokenType::DOT, absl::string_view(cur_ptr_ - 1, 1));
    case '/':
      // 6.4.6 Punctuators
      // Handle single-line comments
      if (*cur_ptr_ == '/') {
        cur_ptr_++;
        LexSingleLineComment();
        goto LexNextToken;
      }
      if (*cur_ptr_ == '*') {
        cur_ptr_++;
        LexMultiLineComment();
        goto LexNextToken;
      }
      if (*cur_ptr_ == '=') {
        cur_ptr_++;
        return Token(TokenType::DIV_EQ, absl::string_view(cur_ptr_ - 2, 2));
      }
      return Token(TokenType::DIV, absl::string_view(cur_ptr_ - 1, 1));
  }
  return Token::NewEOF();
}

Token Lexer::LexStringLiteral() {
  // TODO: Handle escape sequences
  const char* start = cur_ptr_ - 1;
  while (*cur_ptr_ != '\0' && *cur_ptr_ != '\n' && *cur_ptr_ != '"') {
    cur_ptr_++;
  }
  if (*cur_ptr_ == '\0') {
    return Token(TokenType::STRING_LITERAL,
                 absl::string_view(start, cur_ptr_ - start));
  }
  if (*cur_ptr_ == '\n') {
    absl::string_view src(start, cur_ptr_ - start);
    cur_ptr_++;
    return Token(TokenType::STRING_LITERAL, src);
  }
  cur_ptr_++;
  absl::string_view src(start, cur_ptr_ - start);
  return Token(TokenType::STRING_LITERAL, src);
}

void Lexer::LexSingleLineComment() {
  // TODO: Handle other newline schemes.
  while (*cur_ptr_ != '\0' && *cur_ptr_ != '\n') cur_ptr_++;
}

void Lexer::LexMultiLineComment() {
  // TODO: Handle multi-line comments
  while (true) {
    while (*cur_ptr_ != '\0' && *cur_ptr_ != '*') cur_ptr_++;
    if (*cur_ptr_ == '\0') return;
    cur_ptr_++;  // Skip over '*'
    if (*cur_ptr_ == '/') {
      cur_ptr_++;
      return;
    }
  }
}

Token Lexer::LexCharacterConstant() {
  // Already parsed at least one character of the constant already
  const char* start = cur_ptr_ - 1;
  // TODO: Actually write up proper character constant parsing
  while (true) {
    while (*cur_ptr_ != '\'' && *cur_ptr_ != '\0' && *cur_ptr_ != '\\') {
      cur_ptr_++;
    }
    if (*cur_ptr_ == '\'') {
      cur_ptr_++;
      break;
    }
    if (*cur_ptr_ == '0') {
      break;
    }
    if (*cur_ptr_ == '\\') {
      // Escape Sequence
      cur_ptr_++;
      if (*cur_ptr_ == '\'' || *cur_ptr_ == '"' || *cur_ptr_ == '?' ||
          *cur_ptr_ == '\\' || *cur_ptr_ == 'a' || *cur_ptr_ == 'b' ||
          *cur_ptr_ == 'f' || *cur_ptr_ == 'n' || *cur_ptr_ == 'r' ||
          *cur_ptr_ == 't' || *cur_ptr_ == 'v') {
        // Simple escape sequence
        cur_ptr_++;
        continue;
      }
      if (*cur_ptr_ == 'x') {
        // Hexadecimal escape sequence
        cur_ptr_++;
        while (isHexChar(*cur_ptr_)) {
          cur_ptr_++;
        }
        continue;
      }
      if (*cur_ptr_ == 'u' || *cur_ptr_ == 'U') {
        // Universal character name
        cur_ptr_++;
        while (isHexChar(*cur_ptr_)) {
          cur_ptr_++;
        }
        continue;
      }
      // Octal escape sequence
      while (isOctalDigit(*cur_ptr_)) {
        cur_ptr_++;
      }
      continue;
    }
  }
  return Token(TokenType::CHARACTER_CONSTANT,
               absl::string_view(start, cur_ptr_ - start));
}

Token Lexer::LexIdentifier() {
  // Already parsed first character of identifier, need to parse the rest
  const char* start = cur_ptr_ - 1;
  bool go = true;
  while (go) {
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
        go = false;
        break;
    }
  }
  absl::string_view src(start, cur_ptr_ - start);
  IdentifierInfo* info = idents_.MakeIdentifier(src);
  if (info->is_keyword()) {
    return Token(info->type(), src);
  }
  return Token::NewIdentifier(src, info);
}

Token Lexer::LexNumericConstant() {
  // Already parsed first character of numeric constant which is in [0-9.],
  // need to parse the rest
  const char* start = cur_ptr_ - 1;
  const char start_char = *start;
  if (start_char == '0' && (*cur_ptr_ == 'x' || *cur_ptr_ == 'X')) {
    // Hex
    cur_ptr_++;
    while (isHexChar(*cur_ptr_)) {
      cur_ptr_++;
    }
    if (*cur_ptr_ != 'p' && *cur_ptr_ != 'P' && *cur_ptr_ != '.') {
      // Need parse integer-suffix (ignore whether it's well-formed for now)
      while (*cur_ptr_ == 'u' || *cur_ptr_ == 'U' || *cur_ptr_ == 'l' ||
             *cur_ptr_ == 'L') {
        cur_ptr_++;
      }
      return Token(TokenType::NUMERIC_CONSTANT,
                   absl::string_view(start, cur_ptr_ - start));
    }
    if (*cur_ptr_ == '.') {
      cur_ptr_++;
    }
    while (isHexChar(*cur_ptr_)) {
      cur_ptr_++;
    }
    if (*cur_ptr_ == 'p' || *cur_ptr_ == 'P') {
      cur_ptr_++;
    }
    if (*cur_ptr_ == '+' || *cur_ptr_ == '-') {
      cur_ptr_++;
    }
    while (*cur_ptr_ >= '0' && *cur_ptr_ <= '9') {
      cur_ptr_++;
    }
    if (*cur_ptr_ == 'f' || *cur_ptr_ == 'F' || *cur_ptr_ == 'l' ||
        *cur_ptr_ == 'L') {
      cur_ptr_++;
    }
    return Token(TokenType::NUMERIC_CONSTANT,
                 absl::string_view(start, cur_ptr_ - start));
  } else {
    // Non-Hex
    cur_ptr_++;
    while (*cur_ptr_ >= '0' && *cur_ptr_ <= '9') {
      cur_ptr_++;
    }
    if (*cur_ptr_ != 'e' && *cur_ptr_ != 'E' && *cur_ptr_ != '.') {
      // 6.4.4.1 Integer constants: decimal constant
      while (*cur_ptr_ == 'u' || *cur_ptr_ == 'U' || *cur_ptr_ == 'l' ||
             *cur_ptr_ == 'L') {
        cur_ptr_++;
      }
      return Token(TokenType::NUMERIC_CONSTANT,
                   absl::string_view(start, cur_ptr_ - start));
    }
    if (*cur_ptr_ == '.') {
      cur_ptr_++;
      while (*cur_ptr_ >= '0' && *cur_ptr_ <= '9') {
        cur_ptr_++;
      }
      if (*cur_ptr_ == 'e' || *cur_ptr_ == 'E') {
        cur_ptr_++;
        if (*cur_ptr_ == '+' || *cur_ptr_ == '-') {
          cur_ptr_++;
        }
        // TODO: Technically these digits have to be there
        while (*cur_ptr_ >= '0' && *cur_ptr_ <= '9') {
          cur_ptr_++;
        }
      }
      if (*cur_ptr_ == 'f' || *cur_ptr_ == 'F' || *cur_ptr_ == 'l' ||
          *cur_ptr_ == 'L') {
        cur_ptr_++;
      }
      return Token(TokenType::NUMERIC_CONSTANT,
                   absl::string_view(start, cur_ptr_ - start));
    } else {
      cur_ptr_++;
      // *cur_ptr_ == 'e' or 'E'
      // 6.4.4.2 Floating constants: decimal floating constant
      // Parse exponent part
      if (*cur_ptr_ == '+' || *cur_ptr_ == '-') {
        cur_ptr_++;
      }
      // TODO: Technically these digits have to be there
      while (*cur_ptr_ >= '0' && *cur_ptr_ <= '9') {
        cur_ptr_++;
      }
      if (*cur_ptr_ == 'f' || *cur_ptr_ == 'F' || *cur_ptr_ == 'l' ||
          *cur_ptr_ == 'L') {
        cur_ptr_++;
      }
      return Token(TokenType::NUMERIC_CONSTANT,
                   absl::string_view(start, cur_ptr_ - start));
    }
  }
}  // namespace ptcc

}  // namespace ptcc
