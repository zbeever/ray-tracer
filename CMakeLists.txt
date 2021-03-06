cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(raytracer C CXX)

SET(CMAKE_CXX_STANDARD 17)

file(GLOB raytracer_SRC
    "inc/*.h"
    "src/*.h"
    "src/*.cc"
    "src/primitives/*.h"
    "src/primitives/*.cc"
    "src/materials/*.h"
    "src/materials/*.cc"
    "src/engine/*.h"
    "src/engine/*.cc"
    "src/textures/*.h"
    "src/textures/*.cc"
    "src/transformations/*.h"
    "src/transformations/*.cc"
    "src/distributions/*.h"
    "src/distributions/*.cc"
)


find_package(OpenMP REQUIRED)
add_executable(raytracer ${raytracer_SRC})
target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
