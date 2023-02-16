project(sublib2)

add_library(${PROJECT_NAME} src/sublib2.cpp)
add_library(sub::lib2 ALIAS ${PROJECT_NAME})


## here https://cmake.org/cmake/help/v3.4/command/add_library.html
# here is the alias libraries
# add_library(<name> ALIAS <target>)
# this is used to create an alias target, such that <name>
# can be used to refer to <target> in subsequent commands.
# and the <name> does not appear in generated build system as a make
# target. And te <target> may not be an imported target or an alias.

# here do not forget add the current sub project's include header file in which we declare different methods apis.
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)