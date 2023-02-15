#!/bin/sh

if [ -d ./build/ ]; then
  rm -rf ./build/
fi

mkdir build && cd build
# here we set the verbose value = 1 means that during make(compile) period
# more detail info will print on the console
cmake .. && make VERBOSE=1
