project(main_proj)

add_executable(${PROJECT_NAME} main.cpp)

# here link the static library from sublib1 to current project
# then link the static library from sublib2 to current project
# actually it is ok to use the sublib1, sublib2 those sublibraries'
# project name as current project's sub library name
# but alias can let other devs easily get the hierarchy of the sub-libraries
target_link_libraries(${PROJECT_NAME} sub::lib1 sub::lib2)

# and finally under the build/ directory
# there are multiple folders with the name of the main project and sub library1, sub library2
# the generated executable binary file is located under the main project `main_proj`/`main_proj`