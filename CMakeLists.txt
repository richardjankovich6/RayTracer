# cmake_minimum_required(VERSION 3.27)
cmake_minimum_required(VERSION 3.20)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add_executable(RayTracer src/main.cpp)

# target_sources(RayTracer PRIVATE
#     src/raytracer.cpp
#     src/primitives/primitive.cpp
#     src/primitives/sphere.cpp
#     src/primitives/triangle.cpp
# )

# set(HEADERS
#     "src/vector3.h"
#     "src/color.h"
#     "src/point.h"
#     "src/ray.h"
#     "src/light.h"
#     "src/camera.h"
#     "src/raytracer.h"
    
#     "src/primitives/primitive.h"
#     "src/primitives/triangle.h"
#     "src/primitives/sphere.h"
# )


file(GLOB_RECURSE SRCS
    "src/*.cpp"
    # "src/primitives/*.cpp"
)

file(GLOB_RECURSE HDRS
    "src/*.h"
    "src/*.hpp"
)


add_executable(RayTracer ${SRCS} ${HDRS})
target_include_directories(RayTracer PRIVATE "${CMAKE_SOURCE_DIR}/src")
