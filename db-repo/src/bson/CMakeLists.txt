cmake_minimum_required(VERSION 3.25)
project(bson_lib)
set(CMAKE_CXX_STANDARD 14)
include_directories("/usr/local/include")

add_library(${PROJECT_NAME} src/base64.cpp src/nonce.cpp src/oid.cpp src/md5.cpp src/json.cpp src/bsonobj.cpp src/oid.cpp)

add_library(bson ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

