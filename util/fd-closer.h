#ifndef __PTCC_UTIL_FD_CLOSER_H__
#define __PTCC_UTIL_FD_CLOSER_H__

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

#endif
