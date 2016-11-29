cmake_minimum_required(VERSION 2.8.9)
project (data C)

set(PROJECT_LINK_LIBS )

file(GLOB_RECURSE SOURCES "*.c")

add_library(data STATIC ${SOURCES})
target_link_libraries(data ${PROJECT_LINK_LIBS})
