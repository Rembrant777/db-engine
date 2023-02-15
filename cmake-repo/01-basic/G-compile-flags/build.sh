#!/bin/sh

if [ -d ./build/ ]; then
  rm -rf ./build
fi

mkdir build && cd build
cmake .. && make VERBOSE=1
./cmake_compile_flags_bin
# what we gonna got is
#CMake support compiler flag EX2 and EX3!
#Hello Compile Flag EX2!
#Hello Compiler Flag EX3!
