# Set the minimum version of CMake that can be used 
# To find the cmake version run 
# $ cmake --verison 
cmake_minimum_required(VERSION 3.22)

# set the project name 
project (hello_headers)

# create a source variable with a link to all cpp files to compile 
set(SOURCES
   src/Hello.cpp
   src/main.cpp)

# add an executable with the above sources 
add_executable(hello_headers ${SOURCES})

# set the directories that should be included in the build command for this target 
# when running g++ these will be included as -I/directory/path/
target_include_directories(hello_headers
PRIVATE 
${PROJECT_SOURCE_DIR}/include
)