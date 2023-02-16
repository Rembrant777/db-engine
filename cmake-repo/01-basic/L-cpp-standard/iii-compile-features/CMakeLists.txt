cmake_minimum_required(VERSION 3.25)
project(c11_test)
add_executable(cxx_test_bin main.cpp)

# set the c++ standard to the appropriate standard for using auto
target_compile_features(cxx_test_bin PUBLIC cxx_auto_type)

# print the list of known compile flags for this version of cmake
message("List of compile features: ${CMAKE_CXX_COMPILE_FEATURES}")
message("List of compile flags: ${CMAKE_CXX_FLAGS}")