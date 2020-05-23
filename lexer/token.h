#pragma once

#include "absl/strings/string_view.h"
#include "lexer/identifier-database.h"

namespace ptcc {

enum class TokenType {
  ALIGN_OF,
  AUTO,
  BREAK,
  CASE,
  CHAR,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  DOUBLE,
  ELSE,
  ENUM,
  EOF_SENTINEL,
  EXTERN,
  FLOAT,
  FOR,
  GOTO,
  IDENTIFIER,
  IF,
  INLINE,
  INT,
  LONG,
  REGISTER,
  RESTRICT,
  RETURN,
  SHORT,
  SIGNED,
  SIZEOF,
  STATIC,
  STRUCT,
  SWITCH,
  TYPEDEF,
  UNION,
  UNSIGNED,
  VOID,
  VOLATILE,
  WHILE,
  _ALIGNAS,
  _ATOMIC,
  _BOOL,
  _COMPLEX,
  _IMAGINARY,
  _NORETURN,
  _STATIC_ASSERT,
  _THREAD_LOCAL,
};

class Token {
 public:
  Token(TokenType type) : type_(type) {}

  TokenType Type();

  void SetIdentifierInfo(IdentifierInfo* info);

  static Token NewEOF();
  static Token NewIdentifier(absl::string_view);

 private:
  union DataPointer {
    IdentifierInfo* ident_;
  } data_ptr_;

  TokenType type_;
};

}  // namespace ptcc
