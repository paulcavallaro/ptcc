#pragma once

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
  EXTERN,
  FLOAT,
  FOR,
  GOTO,
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

 private:
  TokenType type_;
};

}  // namespace ptcc
