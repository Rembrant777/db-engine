enable_testing()
find_package(GTest)

if (NOT GTest_FOUND)
  message("download and install gtest by cmake fetch content declare method")
  include(FetchContent)
  FetchContent_Declare(gtest 
      QUIET
      URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(gtest)
else (NOT GTest_FOUND)
  message("Gtest Root Found"${GTEST_ROOT})
  include_directories(${GTEST_INCLUDE_DIRS}) 
  include_directories(${GTEST_MAIN_INCLUDE_DIRS}) 
endif()

# after install check GTest again 
find_package(GTest REQUIRED)

if (NOT GTest_FOUND) 
  message(FATAL "unit test module requires GTest!")
endif()

add_executable(
    unit_test_oss
    oss_test.cpp
)

target_link_libraries(unit_test_oss oss ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

gtest_discover_tests(unit_test_oss)
