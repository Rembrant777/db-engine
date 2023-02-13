#!/bin/sh

# clean shell for cleaning compiling cache files 
if [ -d ./build/ ]; then 
	rm -rf ./build/
fi

if [ -f ./libhello_library.a ]; then 
    rm ./libhello_library.a
fi 

mkdir build && cd ./build  && cmake ../ 
make  && ./hello_binary 
cd ../