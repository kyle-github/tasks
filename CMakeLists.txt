cmake_minimum_required(VERSION 3.10.0)
project(threadlet VERSION 0.1.0 LANGUAGES C)

# Set the build type to Debug (this is crucial)
if(NOT CMAKE_BUILD_TYPE)
    message("Setting CMAKE_BUILD_TYPE to Debug")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
else()
    message("CMAKE_BUILD_TYPE already set to ${CMAKE_BUILD_TYPE}")
endif()

# Add debug flags for the compiler
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -gdwarf-4")

add_executable(threadlet
    "${CMAKE_SOURCE_DIR}/src/main.c"
    "${CMAKE_SOURCE_DIR}/src/threadlet.c"
    "${CMAKE_SOURCE_DIR}/src/get_cpu_count.c"
)
