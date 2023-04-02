add_library(oss)

target_sources(oss
    PRIVATE
      ossSocket.cpp
      ossPrimitiveFileOp.cpp 
    PUBLIC 
      ossSocket.hpp
      ossXLatch.hpp
      ossQueue.hpp
      ossPrimitiveFileOp.hpp
)

target_link_libraries(oss core logger)

target_include_directories(oss
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
