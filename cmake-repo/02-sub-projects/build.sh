#!/bin/sh

if [ -d ./build/ ]; then
  rm -rf ./build/
fi

mkdir build && cd build
cmake .. && make

cd main_proj && ./main_proj

cd ..
# clean 
if [ -d ./build/ ]; then 
  rm -rf ./build/
fi 
