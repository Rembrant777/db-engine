add_library(bson)


target_sources(bson
    PRIVATE
      base64.cpp
      bsonobj.cpp
      json.cpp
      md5.cpp
      nonce.cpp
      oid.cpp
    PUBLIC
      base64.h
      bsonobj.h
      bsonobjbuilder.h
      bsonobjiterator.h
      nonce.h
      bsontypes.h
      atomic_int.h
      bson-inl.h
      bson.h
      bson_db.h
      bsonassert.h
      bsonelement.h
      bsonmisc.h
      builder.h
      hex.h
      inline_decls.h
      json.h
      misc.h
      oid.h
      optime.h
      ordering.h
      stringdata.h
      time_support.h 
)

target_include_directories(bson
    PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}
)

# use this to get code coverage 
# flags are only valid with the GNU compiler and on Linux
if (CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(bson
        PUBLIC 
          "--coverage"
    )
   target_link_options(bson
        INTERFACE
          "--coverage"
   )
endif()
