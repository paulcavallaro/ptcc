#ifndef __PTCC_LEXER_READER_H__
#define __PTCC_LEXER_READER_H__

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

#endif
