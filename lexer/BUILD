cc_library(
  name='lexer',
  srcs=[
    'lexer.cc',
  ],
  hdrs=[
    'lexer.h',
  ],
  deps=[
    '@com_github_google_glog//:glog',
  ],
)


cc_library(
  name='token',
  srcs=[
    'token.cc',
  ],
  hdrs=[
    'token.h',
  ],
  deps=[
    '@com_github_google_glog//:glog',
  ],
)


cc_library(
  name='reader',
  srcs=[
    'reader.cc',
  ],
  hdrs=[
    'reader.h',
  ],
  deps=[
    '@com_google_absl//absl/status:status',
    '@com_google_absl//absl/strings:cord',
    '@com_google_absl//absl/strings:strings',
  ],
)
