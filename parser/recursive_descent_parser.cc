#include "parser/recursive_descent_parser.h"

#include "parser/parse_tree.h"

#include "absl/strings/cord.h"

namespace ptcc::parser {

absl::StatusOr<ParseTree> RecursiveDescentParser::Parse(absl::Cord buffer) {
  return ParseTree{};
}

}  // namespace ptcc::parser
