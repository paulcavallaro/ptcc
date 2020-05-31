#pragma once

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "lexer/identifier-database.h"

#include <ostream>

namespace ptcc {

enum class TokenType {
  ALIGN_OF,
  AUTO,
  BREAK,
  CASE,
  CHAR,
  COMMA,
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
  L_PAREN,
  L_CURLY_BRACE,
  NUMERIC_CONSTANT,
  PLUS,
  R_CURLY_BRACE,
  R_PAREN,
  REGISTER,
  RESTRICT,
  RETURN,
  SEMICOLON,
  SHORT,
  SIGNED,
  SIZEOF,
  STAR,
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
  Token(TokenType type, absl::string_view src) : type_(type), src_(src) {}

  TokenType Type();

  void SetIdentifierInfo(IdentifierInfo* info);
  bool isEOF();

  static Token NewEOF();
  static Token NewIdentifier(absl::string_view src, IdentifierInfo* info);

  friend std::ostream& operator<<(std::ostream& os, const Token& tok);

 private:
  union DataPointer {
    IdentifierInfo* ident_;
  } data_ptr_;

  TokenType type_;
  absl::string_view src_;
};

std::ostream& operator<<(std::ostream& os, const Token& tok);

}  // namespace ptcc
