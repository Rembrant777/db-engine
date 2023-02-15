cmake_minimum_required(VERSION 3.25)

# set a default C++ compile flag
message("we got cmake default cxx flags before set the cmake cxx flags "${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set cmake cxx flags" FORCE)
message("we got cmake default cxx flags after set the cmake cxx flags "${CMAKE_CXX_FLAGS})

message("we got cmake c flags before set the cmake cxx flags "${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX3" CACHE STRING "Set cmake cxx flags" FORCE)
message("we got cmake cxx flags after set the cmake cxx flags "${CMAKE_CXX_FLAGS})
# we got cmake cxx flags after set the cmake cxx flags  -DEX2 -DEX3
# cause we always use "${CMAKE_CXX_FLAGS} -DEX3" which means we append new -DEX3  after -DEX2
# in this way, we let cmake_cxx_flags append with -DEX2 -DEX3 and passing the flags to the g++ compiler

project(cmake_compile_flags_bin)
add_executable(cmake_compile_flags_bin main.cpp)

## here we use set to append flags to CMAKE_CXX_FLAGS the method defined inside cmake
## supports the same function as set + variable name = CMAKE_CXX_FLAGS
target_compile_definitions(cmake_compile_flags_bin PRIVATE EX3)
# target_compile_defintions
# param1: the name of the project's executable file
# param2: scope which means the macro declaration codes's visibility
# param3: EX3 the cmake_cxx_flags you wanna append to