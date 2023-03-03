enable_testing()

set(GTest_LOCAL_ROOT_DIR /usr/local/)
find_path(GTest_INCLUDE_DIR NAMES gtest PATHS ${GTest_LOCAL_ROOT_DIR}/include)
message("gtest include dir "${GTest_INCLUDE_DIR})
  
find_library(GTest_LIBRARIES libgtest.a HINTS ${GTest_LOCAL_ROOT_DIR}/lib)
message("gtest libgtest.a dir: "${GTest_LIBRARIES})
  
find_library(GTest_Main_LIBRARIES libgtest_main.a HINTS ${GTest_LOCAL_ROOT_DIR}/lib)
message("gtest_main libgtest_main.a dir: "${GTest_Main_LIBRARIES})

find_library(GMock_LIBRARIES libgmock.a HINTS ${GTest_LOCAL_ROOT_DIR}/lib)
message("gmock libgmock.a dir "${GMock_LIBRARIES}) 

find_library(GMock_Main_LIBRARIES libgmock.a HINTS ${GTest_LOCAL_ROOT_DIR}/lib)
message("gmock_main libgmock_main.a dir "${GMock_Main_LIBRARIES}) 

include_directories(${GTest_INCLUDE_DIR})
include_directories(${GTest_LIBRARIES})
include_directories(${GTest_Main_LIBRARIES})
include_directories(${GMock_LIBRARIES})
include_directories(${GMock_Main_LIBRARIES})

# if still not found exit print error and exit
if (NOT GTest_INCLUDE_DIR)
  message(FATAL "unit test module requires GTest!")
endif()

add_executable(
    unit_test_oss
    oss_test.cpp
)

add_executable(
    unit_test_pthread 
    pthread_test.cpp 
)

target_link_libraries(unit_test_oss oss core ${GTest_LIBRARIES} ${GTest_Main_LIBRARIES} ${GMock_LIBRARIES} ${GMOck_Main_LIBRARIES})

target_link_libraries(unit_test_pthread core ${GTest_LIBRARIES} ${GTest_Main_LIBRARIES} ${GMock_LIBRARIES} ${GMOck_Main_LIBRARIES})

