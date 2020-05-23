#pragma once

#include "absl/strings/cord.h"
#include "lexer/token.h"

namespace ptcc {

class Lexer {
 public:
  explicit Lexer(absl::string_view text);

  Token NextToken();

  Token LexIdentifier();

 private:
  absl::string_view text_;  // Must be null-terminated
  const char* cur_ptr_;
};

}  // namespace ptcc
