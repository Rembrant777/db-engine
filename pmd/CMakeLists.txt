add_library(pmd)


target_sources(pmd
    PRIVATE
      pmdOptions.cpp
    PUBLIC
      pmdOptions.hpp
)

target_include_directories(pmd
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)


target_link_libraries(pmd core oss logger)


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
