cmake_minimum_required(VERSION 3.22)
project(hello_shared_library)

# got shared library from src/Hello.cpp
add_library(hello_shared_library SHARED src/Hello.cpp)

# rename shared library from hello_shared_library to hello:library 
add_library(hello::library ALIAS hello_shared_library)


# append include's header file to shared library 
target_include_directories(hello_shared_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

# create an executable binary file from main.cpp which contains the main entry func 
add_executable(hello_shared_binary src/main.cpp)

# link shared library to the executable binary 
target_link_libraries(hello_shared_binary PRIVATE hello::library)