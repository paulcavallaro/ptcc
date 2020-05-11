#pragma once

#include "absl/strings/cord.h"
#include "lexer/token.h"

namespace ptcc {

class Lexer {
 public:
  explicit Lexer(absl::string_view text);

  Token NextToken();

 private:
  absl::string_view text_;
  int cur_pos_;
  const char* cur_ptr_;
};

}  // namespace ptcc
