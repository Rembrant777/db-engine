include(FetchContent)
FetchContent_Declare(gtest
  QUIET 
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz)

#configure build of googletest 
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

add_executable(
    unit_test_bson
    bson_test.cpp
)

add_executalbe(
    unit_test_oss
    oss_test.cpp
)

add_executable(
    core_test_core
    core_test.cpp
)


# link your unit test library to gtest_main library which is the lib build from google test codes  
target_link_libraries(bson
    PRIVATE 
    unit_tests 
    gtest_main)


# automatic discovery of unit tests 
include(GoogleTest)
gtest_discover_tests(unit_test_bson
  PROPERTIES 
  LABELS "ut"
  DISCOVERY_TIMEOUT # how long to wait(seconds) before program crashing
  200
)
