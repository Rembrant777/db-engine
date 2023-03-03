#!/bin/sh 

if [ -d ./build/ ]; then 
  rm -rf ./build/ 
fi 

mkdir ./build/ && cd ./build/ && cmake .. && make 

## -- test unit_test_pthread 
if [ -f ./bin/unit_test_oss ]; then 
    echo "unit test of oss module binary executable file is generated ok run it!"
    ./bin/unit_test_oss 
fi 

## -- test unit_test_pthread 
if [ -f ./bin/unit_test_pthread ]; then 
    echo "unit test of pthread binary executable file is generated ok run it!" 
    ./bin/unit_test_pthread 
fi  
