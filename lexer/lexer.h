#pragma once

#include "absl/strings/cord.h"
#include "lexer/token.h"

namespace ptcc {

class Lexer {
 public:
  explicit Lexer(absl::Cord text);

  Token NextToken();

 private:
  absl::Cord text_;
};

}  // namespace ptcc
