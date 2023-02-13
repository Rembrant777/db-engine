#!/bin/sh

## here are the general steps for cmake compiling

if [ -d ./build ]; then 
	rm -rf ./build/
fi 

# first we execute cmake command to build current directories's code compile 
cmake . 

# then we create a folder for cache intermidate generated building files 
mkdir build 

cd ./build && cmake ../

# then get to outer folder and execute make command 
cd ../ && make 

# finally we got an executable binary file with the name of hello_make which we declared in CMakeList.txt's add_executable(xxx source.cpp)
./hello_cmake 
