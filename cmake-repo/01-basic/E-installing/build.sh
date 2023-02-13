#!/bin/sh

if [ -d ./build/ ]; then 
    rm -rf ./build/ 
fi


mkdir ./build && cd ./build 
cmake .. && make && make install 
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib cmake_examples_inst_bin
make uninstall 

# all install path info will be recorded inside the build/install_manifest.txt 