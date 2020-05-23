#!/bin/bash

find . -name "*.h" | xargs -I{} -P4 clang-format -style=file -i {}
find . -name "*.cc" | xargs -I{} -P4 clang-format -style=file -i {}

