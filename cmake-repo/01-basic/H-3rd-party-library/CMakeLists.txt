cmake_minimum_required(VERSION 3.25)

# set project name
project(third_party_include_bin)

# find a boost install with the libraries filesystem and system
find_package(Boost 1.75.0 REQUIRED COMPONENTS filesystem system)

# here check cmake inner declared variables often represent whether a lib is install correctly
if (Boost_FOUND)
    message("boost package found")
else ()
    message(FATAL_ERROR "boost package not find")
endif ()

# add main entry point file main.cpp to executable file
add_executable(third_party_bin main.cpp)

# link boost inner components to executable file
target_link_libraries(third_party_bin PRIVATE Boost::filesystem)
target_link_libraries(third_party_bin PRIVATE Boost::system)
