#!/bin/bash

clang -std=c11 -fno-blocks -E ./simple.c > ./simple_preprocessed_clang.c
