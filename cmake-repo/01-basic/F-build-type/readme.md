# Build Type 

## Introduction 
CMake has a number of built in build configurations which can be used to compile 
your project. These specify the optimization levels and if debug information is to be 
included in the binary. 

The levels provided are:
* Release - Adds the `-O3 -DNDEBUG` flags to the compiler 
* Debug - Add the `-g` flag 
* MinSizeRel - Adds the `-g` flag 
* RelWithDebInfo -- Adds `-O2 -g -DNDEBUG` flags

note: different levels compile supports different optimization strategy

## Concepts 
### Set Build Type 
The build type can be set using the following methods. 
- Using a gui tool such as ccmake / cmake-gui

[source,cmake]
----
cmake .. -DCMAKE_BUILD_TYPE=Release
----

### Set Default Build Type 
The default build type provided by CMake is to include no compiler flags for optimization.
For some project you may want to set a default build type so that you don't have to remember to set it. 

And to do this you can add the following to your top level CMakeLists.txt 

```cmake
# if neither cmake inner config variables cmake_build_type nor cmake_configuration_type 
# is set print message and set default configuration options for cmake 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    # here set the RelWithDebInfo as the default configuration option
    message("Setting build type to `RelWithDebInfo` as none was speicified")
    # again introduce the params passing to set this cmake inner defined function 
    #p1: cmake inner defined configuration name/key
    #p2: value passing to the cmake inner defined configuration option 
    #p3: CACHE STRING value's type, add CACHE means not only set value but cache this value 
    #p4: description messages for this set operation 
    #p5: FORCE force to overwrite cache key value pair which key is the CMAKE_BUILD_TYPE 
    # and the value is the RelWithDebInfo 
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
endif()
```



