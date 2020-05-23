#include "lexer/identifier-database.h"

namespace ptcc {

IdentifierDatabase::IdentifierDatabase(){};

IdentifierInfo* IdentifierDatabase::MakeIdentifier(absl::string_view ident) {
  auto it = idents_.find(ident);
  if (it != idents_.end()) {
    return &it->second;
  }
  it = idents_.emplace_hint(it, ident, IdentifierInfo{});
  return &it->second;
}

}  // namespace ptcc
