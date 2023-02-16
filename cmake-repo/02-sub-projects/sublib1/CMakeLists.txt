project(sublib1)

add_library(${PROJECT_NAME} src/sublib1.cpp)
#
add_library(sub::lib1 ALIAS ${PROJECT_NAME})

# do not forget add current sub project's include directory to project's directory
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)