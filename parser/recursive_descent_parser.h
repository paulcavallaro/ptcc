#pragma once

#include "parser/parse_tree.h"

#include "absl/status/statusor.h"
#include "absl/strings/cord.h"

namespace ptcc::parser {

class RecursiveDescentParser {
 public:
  RecursiveDescentParser() = default;
  ~RecursiveDescentParser() = default;

  absl::StatusOr<ParseTree> Parse(absl::Cord buffer);

 private:
};

}  // namespace ptcc::parser
