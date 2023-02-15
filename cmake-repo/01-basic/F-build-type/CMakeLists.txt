cmake_minimum_required(VERSION 3.25)

# here we set the default cmake_build_type value

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("cmake_build_type content " ${CMAKE_BUILD_TYPE})
    message("cmake_configuration_types content " ${CMAKE_CONFIGURATION_TYPES})

    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "set cmake_build_type force" FORCE)
    set(CMAKE_CONFIGURATION_TYPES RelWithDebInfo CACHE STRING "set cmake_build_type force" FORCE)

    message("cmake_build_type content after setting to cache force " ${CMAKE_BUILD_TYPE})
    message("cmake_configuration_types after setting to cache force" ${CMAKE_CONFIGURATION_TYPES})
else ()
    message("cmake_build_type already set with value " ${CMAKE_BUILD_TYPE})
    message("cmake_configuration_types already set with value " ${CMAKE_CONFIGURATION_TYPES})
endif ()


project(cmake_build_type_bin)
add_executable(cmake_build_type main.cpp)
