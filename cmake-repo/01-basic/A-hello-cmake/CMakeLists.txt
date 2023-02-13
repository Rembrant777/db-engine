# Set the minimum version of CMake that can be sued 
# To find the cmake version run 
# $ cmake --version: cmake version 3.22.1
cmake_minimum_required(VERSION 3.20)

# set the project name here 
project (hello_cmake)

# add an executable 
add_executable(hello_cmake main.cpp)