add_library(logger)

target_sources(logger
    PRIVATE
      pd.cpp
    PUBLIC
      pd.hpp 
)

target_link_libraries(logger core oss)

target_include_directories(logger
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
  target_compile_options(oss
      PUBLIC
        "--coverage"
  )
  target_link_options(oss
      INTERFACE
        "--coverage"
  )
endif()
