# cmake_minimum_required(VERSION 3.27)
cmake_minimum_required(VERSION 3.20)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRCS
    "src/*.cpp"
)

file(GLOB_RECURSE HDRS
    "src/*.h"
    "src/*.hpp"
)

add_executable(RayTracer ${SRCS} ${HDRS})
target_include_directories(RayTracer PRIVATE "${CMAKE_SOURCE_DIR}/src")
