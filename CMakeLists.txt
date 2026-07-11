cmake_minimum_required(VERSION 4.0)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Add your source files to executable target
file(GLOB MAIN
    "src/main.cpp"
    "src/vector3.h"
    # "src/utils.h"
    )
add_executable(RayTracer ${MAIN})