cmake_minimum_required(VERSION 3.7)
project(argos)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(argos ${SOURCE_FILES})
target_link_libraries(argos sfml-graphics sfml-system sfml-window Box2D)