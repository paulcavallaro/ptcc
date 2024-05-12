package(default_visibility = ['//visibility:public'])

cc_library(
  name='fd-closer',
  srcs=[
    'fd-closer.cc',
  ],
  hdrs=[
    'fd-closer.h',
  ],
  deps=[
    '@glog//:glog',
  ],
)

cc_library(
  name='file-utils',
  srcs=[
    'file-utils.cc',
  ],
  hdrs=[
    'file-utils.h'
  ],
  deps=[
    ':fd-closer',
    '@abseil-cpp//absl/strings:strings',
  ],
)
