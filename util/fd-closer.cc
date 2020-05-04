#include "util/fd-closer.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "glog/logging.h"

namespace ptcc {

FDCloser::FDCloser(FILE* fd) : fd_(fd) {}

FDCloser::~FDCloser() {
  const int ret = fclose(fd_);
  if (ret) {
    VLOG(1) << "Failed to close fd with error: " << strerror(errno);
  }
}

}  // namespace ptcc
