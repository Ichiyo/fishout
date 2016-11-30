cmake_minimum_required(VERSION 2.8.9)
project (fishout C)

add_definitions(-DUSE_SDL)

add_subdirectory(math)
add_subdirectory(data)
add_subdirectory(utils)
add_subdirectory(game)

include_directories(data game math platform utils)

add_executable(fishout main.c)
add_dependencies(fishout game math data utils)
target_link_libraries(fishout game math data utils)
