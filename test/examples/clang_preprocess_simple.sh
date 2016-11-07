#!/bin/bash

clang -std=c11 -fno-blocks -E ./simple.c | grep -v "^#.*" > ./simple_preprocessed_nopragmas.c
