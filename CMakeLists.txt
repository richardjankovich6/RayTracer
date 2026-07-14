# cmake_minimum_required(VERSION 4.0)

cmake_minimum_required(VERSION 3.27)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    "src/main.cpp"
)

set(HEADERS
    "src/vector3.h"
    "src/color.h"
    "src/globals.h"
    "src/light.h"
    "src/point.h"
    "src/ray.h"
    "src/triangle.h"
    "src/primitive.h"
)


# Add your source files to executable target
# file(GLOB MAIN
    
#     # "src/vector3.h"
#     # "src/color.h"
#     # "src/globals.h"
#     # "src/light.h"
#     # "src/point.h"
#     # "src/ray.h"
#     # "src/triangle.h"
#     # "src/primitive.h"
#     "src/main.cpp"
#     # "src/triangle.h"
#     # "src/utils.h"
#     )


add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE "${CMAKE_SOURCE_DIR}/src")

# add_executable(RayTracer ${MAIN})