cmake_minimum_required(VERSION 3.10.0)
project(threadlet VERSION 0.1.0 LANGUAGES C)

add_executable(threadlet
    "${CMAKE_SOURCE_DIR}/src/main.c"
    "${CMAKE_SOURCE_DIR}//src/threadlet.c"
    "${CMAKE_SOURCE_DIR}/src/get_cpu_count.c"
)
