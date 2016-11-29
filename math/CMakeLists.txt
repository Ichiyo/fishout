cmake_minimum_required(VERSION 2.8.9)
project (math C)

set(PROJECT_LINK_LIBS m)

file(GLOB_RECURSE SOURCES "*.c")

add_library(math STATIC ${SOURCES})
target_link_libraries(math ${PROJECT_LINK_LIBS})
