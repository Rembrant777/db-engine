enable_testing()
#ifind_package(GTest)

if (NOT GTest_FOUND)
  set(GTest_LOCAL_ROOT_DIR /usr/local/)
  find_path(GTest_INCLUDE_DIR NAMES gtest PATHS ${GTest_LOCAL_ROOT_DIR}/include)
  message("gtest include dir "${GTest_INCLUDE_DIR})
  
  find_library(GTest_LIBRARIES libgtest.a HINTS ${GTest_LOCAL_ROOT_DIR}/lib)
  message("gtest libgtest.a dir: "${GTest_LIBRARIES})
  
  find_library(GTest_Main_LIBRARIES libgtest_main.a HINTS ${GTest_LOCAL_ROOT_DIR}/lib)
  message("gtest libtest_main.a dir: "${GTest_Main_LIBRARIES})

  include_directories(${GTest_INCLUDE_DIR})
  include_directories(${GTest_LIBRARIES})
  include_directories(${GTest_Main_LIBRARIES})
else (NOT GTest_FOUND) 
  message(FATAL "unit test module requires GTest!")
endif()

# if still not found exit print error and exit
if (NOT GTest_FOUND AND NOT GTest_INCLUDE_DIR)
  message(FATAL "unit test module requires GTest!")
endif()

add_executable(
    unit_test_oss
    oss_test.cpp
)

target_link_libraries(unit_test_oss oss ${GTest_LIBRARIES} ${GTest_Main_LIBRARIES})

#gtest_discover_tests(unit_test_oss)
