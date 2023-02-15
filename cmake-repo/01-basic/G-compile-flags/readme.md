# Compile Flags 
## Introduction 
CMake supports setting compile flags in a number of different ways:
* using +target_compile_definitions()+ function 
* using the +CMAKE_C_FLAGS+ and +CMAKE_CXX_FLAGS+ variables. 

## Concepts 
### Set Per-Traget C++ Flags 
The recommended way to set C++ flags in modern CMake is to use per-target flags which can be populated to other targets
through the +target_compile_definitions()+ like 
```cmake
target_compile_definitions(cmake_examples_compile_flags PRIVATE EX3)
```
The above cmake will cause the compiler to add the defintion +-DEX3+ when compiling the target. 

And if the target was a library and the scope is +PUBLIC+ or +INTERFACE+ which has been chose
the defintion would also be included in any executables that link this target. 

* CMAKE_CXX_FLAGS and CMAKE_C_FLAGS
```cmake
# set with FORCE parameter will force refresh/overlap the cache's parameters value 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set C++ Compiler Flags" FORCE)
# param1: CMAKE_CXX_FLAGS this is the cmake's variable 
# param2: "${CMAKE_CXX_FLAGS} -DEX2" this is append CMAKE_CXX_FLAGS with -DEX2 
# param3: CACHE STRING means that cache the variable in cache and cache type is string 
# param4: "Set C++ Compiler Flags" is this set parameter's description content 
# param5: FORCE this mean set parameter's operation is force overwrite existing entries
```
* [cmake set doc](https://cmake.org/cmake/help/latest/command/set.html#set-cache-entry)
Once set the +CMAKE_C_FLAGS+ and +CMAKE_CXX_FLAGS+ wil set a compiler flag / defintions globally for all targets
in this directory or any included sub-directoreis CXX is referring to cpp and C is referring c files.


### Set CMake Flags 
cmake command appending with flags also supports cli like this :
```shell
cmake .. -DCMAKE_CXX_FLAGS="-DEX3" -DCMAKE_C_FLAGS="-DEX2"
```




