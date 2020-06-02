#include "util/file-utils.h"

#include "absl/strings/string_view.h"
#include "util/fd-closer.h"

#include <stdio.h>
#include <sys/stat.h>

namespace ptcc {
namespace file {

std::string ReadFileToStringWithSentinel(absl::string_view fname,
                                         const char sentinel) {
  FILE* file = fopen(std::string(fname).c_str(), "r");
  if (file == nullptr) return "";
  FDCloser closer(file);
  struct stat info;
  const int err = fstat(fileno(file), &info);
  if (err != 0) return "";
  char* ret = static_cast<char*>(::operator new(info.st_size + 1));
  ret[info.st_size] = sentinel;
  char* buf = ret;
  size_t to_read = info.st_size;
  while (true) {
    const ssize_t read = fread(buf, 1, to_read, file);
    if (read == info.st_size) break;
    if (feof(file)) break;
    // Read less than size
    buf += read;
    to_read -= read;
  }
  return ret;
}

}  // namespace file
}  // namespace ptcc
