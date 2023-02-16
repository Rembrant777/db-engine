# C++ Standard Common Method 

## Introduction 
This example shows a common method to set the C++ Standard. And this can be used with 
most versions of CMake. However, if you are targeting a recent version of CMake there are more
convenient methods available. 

## Concepts 
### Checking Compile flags 
CMake has support for attempting to compile a program with any flags you pass into 
the function `CMAKE_CXX_COMPILER_FLAG`. And the result is then stored in a variable that you pass in. 
like 
```cmake
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
```

And this example will attempt to compile a program with the flag `-std=c++`
and the store the result in the variable `COMPILER_SUPPORTS_CXX11`.

And the line `include(CheckCXXCompilerFlag)` tells CMake to include 
this function to make it available fo ruse. 

### Adding the flag 
Once you have determined if the compiler supports a flag, you can then use the standard
cmake methods to add this flag to a target. And in this example we use the 
`CMAKE_CXX_FLAGS` to propegate the flag to all targets. 

like this 
```cmake
# here we check cmake inner defined variables: COMPILER_SUPPORTS_CXX11
# to detect whether compiler supports c++11 standard 
if(COMPILER_SUPPORTS_CXX11)#
    # here we know that the compiler supports c++ standard
    # then we append only c++11 supported flags -std=c++11 
    # append the flag to all c++ flags 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X) 
    # in this branch we check whether the compiler supports 
    # the c++0x standard if supports then append the c++0x
    # specified flags to the global scoped compiler flag list 
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    # if cmake detect that neither c++11 nor c++0x supported by the compiler 
    # then print a message with level = STATUS  
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
```
The above example only checks for the gcc version fo the compile flags
and supports. fallback from c++ 11 to the pre-standardisation c++0x flag. 
In real product env you may want to check for C++14, or add support for different methods 
of setting the compiler like `-std=gnu++11`

