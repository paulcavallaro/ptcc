#pragma once

#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace ptcc {

class Reader {
 public:
  Reader(absl::string_view fname);

  absl::Cord Read();

 private:
  std::string fname_;
};

}  // namespace ptcc
