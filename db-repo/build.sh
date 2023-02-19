#!/bin/sh 

if [ -d ./build/ ]; then 
  rm -rf ./build/ 
fi 

mkdir ./build/ && cd ./build/ && cmake .. -DBSON_BUILD_TESTS=ON  -DGTEST_HAS_FILE_SYSTEM=ON -DGTEST_HAS_FILE_SYSTEM=ON && make  
