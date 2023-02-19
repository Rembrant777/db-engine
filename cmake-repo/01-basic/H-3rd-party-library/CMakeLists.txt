cmake_minimum_required(VERSION 3.25)

# set project name
project(test_boost_bin)


set(BOOST_INCLUDE_DIR "/usr/local/include")
set(BOOST_LIB_DIR "/usr/local/lib")



# Boost::Algorithm
add_library(Boost::Algorithm INTERFACE IMPORTED GLOBAL)
set_property(TARGET Boost::Algorithm PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
add_dependencies(Boost::Algorithm boost)

# Boost::ASIO
add_library(Boost::ASIO INTERFACE IMPORTED GLOBAL)
set_property(TARGET Boost::ASIO PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
set_property(TARGET Boost::ASIO PROPERTY INTERFACE_LINK_LIBRARIES Boost::DateTime Boost::Regex Boost::Serialization
                                                                  Boost::System Boost::filesystem)
add_dependencies(Boost::ASIO boost)

# Boost::DateTime
add_library(Boost::DateTime STATIC IMPORTED GLOBAL)
set_property(TARGET Boost::DateTime PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
set_property(TARGET Boost::DateTime PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_date_time${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET Boost::DateTime PROPERTY INTERFACE_LINK_LIBRARIES Boost::Serialization)
add_dependencies(Boost::DateTime boost)

# Boost::Regex
add_library(Boost::Regex STATIC IMPORTED GLOBAL)
set_property(TARGET Boost::Regex PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
set_property(TARGET Boost::Regex PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_regex${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(Boost::Regex boost)

# Boost::Serialization
add_library(Boost::Serialization STATIC IMPORTED GLOBAL)
set_property(TARGET Boost::Serialization PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
set_property(TARGET Boost::Serialization PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_serialization${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(Boost::Serialization boost)

# Boost::System
add_library(Boost::System STATIC IMPORTED GLOBAL)
set_property(TARGET Boost::System PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
set_property(TARGET Boost::System PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_system${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(Boost::System boost)

# Boost::filesystem
add_library(Boost::filesystem STATIC IMPORTED GLOBAL)
set_property(TARGET Boost::filesystem PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR})
set_property(TARGET Boost::filesystem PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}boost_filesystem${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(Boost::filesystem boost)


# add main entry point file main.cpp to executable file
add_executable(test_boost_bin main.cpp)

# link boost inner components to executable file
target_link_libraries(test_boost_bin PRIVATE Boost::filesystem)
target_link_libraries(test_boost_bin PRIVATE Boost::System)
