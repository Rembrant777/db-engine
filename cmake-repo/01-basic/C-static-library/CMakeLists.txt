cmake_minimum_required(VERSION 3.22)

# claim that the finally executable binary file name is hello_library 
project(hello_library)

# here we create a library 
# generate the static library from the library source 
# static library just like mvn compiled jar file can be re-loaded multiple times and binded into other projects 
add_library(hello_library STATIC src/Hello.cpp)

target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

# add an executable binary file with the above source 
add_executable(hello_binary src/main.cpp)

# link the new hello_library target with the hello_binary target during link time 
target_link_libraries(hello_binary PRIVATE hello_library)


# here the PRIVATE, PUBLIC is the means of the scope of the added file 
# another scope is INTERFACE

