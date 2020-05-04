#include "lexer/reader.h"

#include <stdio.h>

#include "absl/status/status.h"
#include "absl/strings/cord.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "util/fd-closer.h"

namespace ptcc {

Reader::Reader(absl::string_view fname) : fname_(fname) {}

// TODO(paulcavallaro): Error handling w/ StatusOr
absl::Cord Reader::Read() {
  FILE* file = fopen(fname_.c_str(), "r");
  FDCloser closer(file);
  if (file == nullptr) {
    perror(absl::StrCat("Failed to open file: ", fname_).c_str());
    // TODO(paulcavallaro): Error handling w/ StatusOr
    return absl::Cord{};
  }
  absl::Cord data;
  char buf[4096];
  while (true) {
    size_t read = fread(buf, 1, 4096, file);
    data.Append(absl::string_view{buf, read});
    if (read == 4096) continue;
    if (feof(file)) break;
    perror(absl::StrCat("Failed to read file: ", fname_).c_str());
    // TODO(paulcavallaro): Error handling w/ StatusOr
    return absl::Cord{};
  }
  return data;
}

}  // namespace ptcc
