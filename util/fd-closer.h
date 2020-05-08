#pragma once

#include <stdio.h>

namespace ptcc {

class FDCloser {
 public:
  explicit FDCloser(FILE* fd);

  ~FDCloser();

 private:
  FILE* fd_;
};

}  // namespace ptcc
