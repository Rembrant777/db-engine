cmake_minimum_required(VERSION 3.25)
project(emeralddb VERSION 1.0.0 LANGUAGES C CXX)
enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_FLAGS ${CMAKE_FLAGS} "-std=c++11")

# options 
option(BSON_BUILD_TESTS "Build emeralddb lib bson unit tests" OFF)
option(EMERALDDB_BUILD_TESTS "Build emeralddb's unit tests" OFF)
option(EMERALDDB_BUILD_BENCHMARKS "Build emeralddb's benchmarks" OFF)
option(EMERALDDB_INSTALL "Install emeralddb's header and library" OFF)

# set boost include header dir && lib dir 
set(BOOST_INCLUDE_DIR "/usr/local/include")
set(BOOST_LIB_DIR "/usr/local/lib")

# set basic dirs && set edb include header dir 
set(EMERALDDB_PUBLIC_INCLUDE_DIR "include/emeralddb")
include_directories("${EMERALDDB_PUBLIC_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIR})

message("begin include FetchContent")
include(FetchContent)
FetchContent_Declare(
    googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 
    OVERRIDE_FIND_PACKAGE
)
message("FetchContent make available googletest")
FetchContent_MakeAvailable(googletest)

if (TARGET GTest::gtest_main)
    message("Target was found")
endif()
message("begin find package of gtest")
find_package(GTest 1.12.1 REQUIRED)

if (GTest_FOUND) 
    message("google test found")
endif(GTest_FOUND)



# find boost and find boost's program_options & filesystem modules  
find_package(Boost REQUIRED COMPONENTS program_options filesystem)

# create emeralddb as a library file in this lib contains all emeralddb source, header file and bsonsource and header files 
add_library(emeralddb "")
target_sources(emeralddb
  PRIVATE
  "include/emeralddb/core.hpp" 
  "oss/ossSocket.cpp"
  "include/emeralddb/ossSocket.hpp"
  "bson/base64.cpp"
  "bson/base64.h"
  "bson/bsonobj.cpp"
  "bson/bsonobj.h"
  "bson/bsonobjbuilder.h"
  "bson/bsonobjiterator.h"
  "bson/json.cpp"
  "bson/md5.cpp"
  "bson/nonce.cpp"
  "bson/nonce.h"
  "bson/bsontypes.h"
  "bson/atomic_int.h"
  "bson/bson-inl.h"
  "bson/bson.h"
  "bson/bson_db.h"
  "bson/bsonassert.h"
  "bson/bsonelement.h"
  "bson/bsonmisc.h"
  "bson/builder.h"
  "bson/embedded_builder.h"
  "bson/hex.h"
  "bson/inline_decls.h"
  "bson/json.h"
  "bson/md5.h"
  "bson/md5.hpp"
  "bson/misc.h"
  "bson/oid.cpp"
  "bson/oid.h"
  "bson/optime.h"
  "bson/ordering.h"
  "bson/stringdata.h"
  "bson/time_support.h")

add_executable(edb main.cpp)

if(ENABLE_UNIT_TESTS)
    message("enable bson build tests") 
    enable_testing()
    set(gtest_force_shared_ctr ON CACHE BOOL "" FORCE)
    set(install_gtest OFF)
    set(install_gmock OFF)
    set(build_mock ON)

    # test by google test framework 
    add_subdirectory("third_party/googletest")

    # GoogleTest triggers a missing field initalizers warning. 
    if (EMERALDDB_HAVE_NO_MISSING_FIELD_INITIALIZERS) 
        set_property(TARGET gtest APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
        set_property(TARGET gmock APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
    endif(EMERALDDB_HAVE_NO_MISSING_FIELD_INITIALIZERS)


    # here we add bson unit tests 
    add_executable(bson_tests "")
    target_sources(bson_tests
        PRIVATE 
        "bson/bson_tests.cc"
    )
    target_link_libraries(bson_tests bson gmock  gtest_main)
endif(ENABLE_UNIT_TESTS)

