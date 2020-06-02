#include "lexer/lexer.h"
#include "util/file-utils.h"

#include "absl/flags/parse.h"

using ptcc::Lexer;
using ptcc::Token;
using ptcc::file::ReadFileToStringWithSentinel;

int main(int argc, char** argv) {
  auto pos_args = absl::ParseCommandLine(argc, argv);
  if (pos_args.size() < 2) return 1;

  const std::string buf = ReadFileToStringWithSentinel(pos_args[1], '\0');
  if (buf.empty()) {
    std::cerr << "Failed to open file: " << pos_args[1] << std::endl;
    return 1;
  }
  Lexer lexer(buf);
  Token tok = Token::NewEOF();
  while (!(tok = lexer.NextToken()).isEOF()) {
    std::cout << tok << std::endl;
  }
  std::cout << tok << std::endl;
}
