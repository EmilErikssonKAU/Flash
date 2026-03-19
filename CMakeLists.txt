cmake_minimum_required(VERSION 3.13)

project(codecrafters-shell)

file(GLOB_RECURSE SOURCE_FILES src/*.c)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard

add_executable(shell ${SOURCE_FILES})
target_include_directories(shell PRIVATE src include)
target_compile_options(shell PRIVATE -Wall)
target_link_libraries(shell PRIVATE readline)
