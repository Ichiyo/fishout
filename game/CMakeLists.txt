cmake_minimum_required(VERSION 2.8.9)
project (game C)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

if(MACOSX)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
set(PROJECT_LINK_LIBS math data utils m pthread SDL2 SDL2_image ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
include_directories(/usr/local/include)

file(GLOB_RECURSE SOURCES "*.c" "*.m")

else() #linux

set(PROJECT_LINK_LIBS math data utils m pthread GL GLU glut SDL2 SDL2_image)

file(GLOB_RECURSE SOURCES "*.c")

endif()

include_directories(../math)
include_directories(../data)
include_directories(../utils)
include_directories(../platform)

add_library(game STATIC ${SOURCES})
target_link_libraries(game ${PROJECT_LINK_LIBS})
