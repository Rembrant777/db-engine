add_library(core)

target_sources(core
    PRIVATE
        core.cpp
    PUBLIC 
        core.hpp
    )

target_include_directories(core
    PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}
)
