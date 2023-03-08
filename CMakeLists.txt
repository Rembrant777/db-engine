cmake_minimum_required(VERSION 3.25)
project(emeralddb VERSION 1.0.0 LANGUAGES C CXX)

# set the C++ standard 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dev")

# set boost options 
# set boost include header dir && lib dir
set(BOOST_INCLUDE_DIR "/usr/local/include")
set(BOOST_LIB_DIR "/usr/local/lib")

# set basic dirs && set edb include header dir
set(EMERALDDB_PUBLIC_INCLUDE_DIR "include/emeralddb")
include_directories("${EMERALDDB_PUBLIC_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIR})

# find boost and find boost's program_options & filesystem modules
find_package(Boost REQUIRED COMPONENTS program_options filesystem)


message("cxx compiler flags  "${CMAKE_CXX_FLAGS})
# place binaries and libraries according to GNU standards 
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# gcov is required if we wanna generate project's coverage test report 
find_program(GCOV_PATH gcov)

# create emeralddb as a library file in this lib contains all emeralddb source, header file and bsonsource and header files 
add_executable(edb_client client_main.cpp)
add_executable(edb_server server_main.cpp)

if(NOT GCOV_PATH)
    message(FATAL_ERROR "Code coverage analysis requires gcov!")
else(NOT GCOV_PATH) 
    message("Gcov path: "${GCOV_PATH})
endif()

add_subdirectory(bson)
add_subdirectory(core)
add_subdirectory(oss)
add_subdirectory(client)
add_subdirectory(test)
enable_testing()

target_link_libraries(edb_client bson core oss client)
target_link_libraries(edb_server bson core oss client)
