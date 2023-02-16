## C++ Standard 
Since the release of C++11 and C+14 a common use case is to invoke the compiler to use these standards. 
As CMake has evolved, it has added features to make this easier and new versions of CMake have changed how this is achieved. 

The following examples show different methods to setting the C++ standard and what versions of CMake they 
are available from.

The examples include: 
i-common-method in this subdirectory gonna create a simple version that should work with most versions of CMake. 
ii-cxx-standard: in this subdirectory gonna use `CMAKE_CXX_STANDARD` this variable to introduce in CMake v3.1; 
iii-compile-features: in this subdirectory we gonna use the `target_compile_features` function introduced in CMake v3.1