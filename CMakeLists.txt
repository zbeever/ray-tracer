cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(raytracer C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

file(GLOB raytracer_SRC
    "src/*.h"
    "src/*.cpp"
    "src/geometry/*.h"
    "src/geometry/*.cpp"
    "src/engine/*.h"
    "src/engine/*.cpp"
)

add_executable(raytracer ${raytracer_SRC})
