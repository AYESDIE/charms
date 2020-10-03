cmake_minimum_required(VERSION 3.17)
project(charms)

set(CMAKE_CXX_STANDARD 20)

add_executable(charms main.cpp src/snake.hpp src/engine.hpp)

find_package(SFML 2 REQUIRED graphics)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(charms sfml-graphics)
endif()
