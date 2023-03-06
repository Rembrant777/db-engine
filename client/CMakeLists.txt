# set boost options 
# set boost include header dir && lib dir
set(BOOST_INCLUDE_DIR "/usr/local/include")
set(BOOST_LIB_DIR "/usr/local/lib")

# set basic dirs && set edb include header dir
set(EMERALDDB_PUBLIC_INCLUDE_DIR "include/emeralddb")
include_directories("${EMERALDDB_PUBLIC_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIR})

add_executable(client edb.cpp)

add_library(cmd)
add_library(cmdFactory)
#add_library(client)

target_sources(cmd
    PRIVATE
      command.cpp
    PUBLIC 
      command.hpp
)

target_sources(cmdFactory
    PRIVATE 
      commandFactory.cpp
    PUBLIC 
      commandFactory.hpp
)

#target_sources(client
#    PRIVATE 
#      edb.cpp 
#    PUBLIC
#      edb.hpp
#)

target_link_libraries(cmd core oss bson)

target_link_libraries(cmdFactory cmd)

target_link_libraries(client cmd core oss bson cmdFactory)
