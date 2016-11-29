cmake_minimum_required(VERSION 2.8.9)
project (utils C)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

if(MACOSX)

file(GLOB_RECURSE SOURCES "*.c" "*.m")

else()

file(GLOB_RECURSE SOURCES "*.c")

endif()

set(PROJECT_LINK_LIBS data)
include_directories(../platform ../data)

add_library(utils STATIC ${SOURCES})
target_link_libraries(utils ${PROJECT_LINK_LIBS})
