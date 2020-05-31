#pragma once

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "lexer/identifier-database.h"
#include "lexer/token-type.h"

#include <ostream>

namespace ptcc {

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
