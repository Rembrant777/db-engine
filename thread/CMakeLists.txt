add_library(oss)

target_sources(threadpool 
    PRIVATE 
      threadpool.hpp
    PUBLIC 
      threadpool.cpp 
)

target_sources(nonblocking_threadpool
    PRIVATE
      nonblocking_threadpool.hpp
    PUBLIC 
      nonblocking_threadpool.cpp 
)


target_link_libraries(threadpool core)

target_link_libraries(nonblocking_threadpool core)

target_link_libraries(threadpool nonblocking_threadpool)

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
