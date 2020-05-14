cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(nutracer C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

file(GLOB nutracer_SRC
    "src/*.h"
    "src/*.cpp"
    "src/geometry/*.h"
    "src/geometry/*.cpp"
    "src/scene/*.h"
    "src/scene/*.cpp"
)

add_executable(nutracer ${nutracer_SRC})
