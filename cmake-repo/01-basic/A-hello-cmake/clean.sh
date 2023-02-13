#!/bin/sh

# clean shell for cleaning compiling cache files 
if [ -d ./build/ ]; then 
	rm -rf ./build/
fi

if [ -f ./CMakeCache.txt ]; then 
	rm ./CMakeCache.txt 
fi 

if [ -f ./cmake_install.cmake ]; then 
	rm ./hello_cmake && rm ./cmake_install.cmake 
fi

if [ -f ./Makefile ]; then 
	rm ./Makefile 
fi 

if [ -d ./CMakeFiles ]; then 
	rm -rf ./CMakeFiles 
fi 
