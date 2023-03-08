# set boost options 
# set boost include header dir && lib dir
set(BOOST_INCLUDE_DIR "/usr/local/include")
set(BOOST_LIB_DIR "/usr/local/lib")

# set basic dirs && set edb include header dir
set(EMERALDDB_PUBLIC_INCLUDE_DIR "include/emeralddb")
include_directories("${EMERALDDB_PUBLIC_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIR})

add_library(client)

target_sources(client
    PRIVATE
      command.cpp
      commandFactory.cpp
      edb.cpp
    PUBLIC 
      command.hpp
      commandFactory.hpp
      edb.hpp
)


target_link_libraries(client core oss bson)
