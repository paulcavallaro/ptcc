#pragma once

#include "lexer/token-type.h"

#include "absl/container/node_hash_map.h"

namespace ptcc {

class IdentifierInfo {
 public:
  IdentifierInfo() {}
  explicit IdentifierInfo(TokenType type) : type_(type) {}

  bool is_keyword() { return type_ != TokenType::IDENTIFIER; }
  TokenType type() { return type_; }

 private:
  TokenType type_ = TokenType::IDENTIFIER;
};

class IdentifierDatabase {
 public:
  IdentifierDatabase();

  IdentifierInfo* MakeIdentifier(absl::string_view ident);

 private:
  absl::node_hash_map<std::string, IdentifierInfo> idents_;
};

}  // namespace ptcc
