cmake_minimum_required(VERSION 3.7)
project(argos)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

set(SOURCE_FILES main.cpp src/GameObject.cpp src/GameObject.h src/enemies/Snake.cpp src/enemies/Snake.h src/players/Player.cpp src/players/Player.h src/level/Level.cpp src/level/Level.h src/level/Tile.cpp src/level/Tile.h src/gamedata/GameData.cpp src/gamedata/GameData.h src/gamedata/TileData.cpp src/gamedata/TileData.h src/objects/Stone.cpp src/objects/Stone.h src/Logger.cpp src/Logger.h)
add_executable(argos ${SOURCE_FILES})
target_link_libraries(argos sfml-graphics sfml-system sfml-window Box2D)