#pragma once

#include "absl/strings/string_view.h"

namespace ptcc {
namespace file {

// ReadFileToStringWithSentinel reads the file named by `fname` into a
// std::string with at least one trailing `sentinel` character.
std::string ReadFileToStringWithSentinel(absl::string_view fname,
                                         const char sentinel);

}  // namespace file
}  // namespace ptcc
