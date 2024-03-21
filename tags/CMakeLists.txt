cmake_minimum_required(VERSION 3.27)
project(tags)

set(CMAKE_CXX_STANDARD 17)

find_package(TagLib 2.0.0 REQUIRED)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

add_executable(tags main.cpp)
target_link_libraries(tags PRIVATE TagLib::tag)
target_link_libraries(tags PRIVATE SDL2::SDL2main)
target_link_libraries(tags PRIVATE SDL2::SDL2)
target_link_libraries(tags PRIVATE Qt6::Multimedia)

