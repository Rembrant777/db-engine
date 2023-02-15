cmake_minimum_required(VERSION 3.5)
project(cmake_example_install)

# create a library: generate the shared library from the library sources 
# yes, we take Hello.* as a shared library 
add_library(cmake_example_inst SHARED src/Hello.cpp ../G-compile-flags/main.cpp)
target_include_directories(cmake_example_inst PUBLIC ${PROJECT_SOURCE_DIR}/include)
# create an executable binary file 
# add an executable with the abouve sources 
add_executable(cmake_examples_inst_bin src/main.cpp)

# link the new hello_library target with the hello_binary target 
target_link_libraries(cmake_examples_inst_bin PRIVATE cmake_example_inst)

# install 

# binaries 
# we will finally got an executable file with the name of cmake_examples_inst_bin 
# and move the binary file to sysetm's bin folder 
# like /usr/local/bin 
install (TARGETS cmake_examples_inst_bin DESTINATION bin)

# library 
# and we will also get a shared library different from static library shared library 
# is unique in current system so we move it to system's lib folder 
# like /usr/local/lib 
install (TARGETS cmake_example_inst LIBRARY DESTINATION lib)

# header files 
# header files contains different lib's declared API name and parameters
# there also a system directory to store all the header files -- 'include'
# like /usr/local/include
install (DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

# configs 
# same as header file if we need to install a package into our system 
# we need to create config files and during the install process we set the config files 
# of the system as etc like /usr/local/etc
install (FILES cmake-examples.conf DESTINATION etc)