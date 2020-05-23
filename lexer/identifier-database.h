#pragma once

#include "absl/container/node_hash_map.h"

namespace ptcc {

class IdentifierInfo {};

class IdentifierDatabase {
 public:
  IdentifierDatabase();

  IdentifierInfo* MakeIdentifier(absl::string_view ident);

 private:
  absl::node_hash_map<std::string, IdentifierInfo> idents_;
};

}  // namespace ptcc
