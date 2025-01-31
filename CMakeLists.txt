cmake_minimum_required(VERSION 3.22.1)
project(raytracer)

# adjust any compiler settings
add_compile_options(-g -Wall -Wextra -pedantic -std=c++20)

# add library for vector class and operations
add_library(vector3d vector3d.h vector3d.cpp)

# test program for vector3d
add_executable(test-vector test-vector3d.cpp)
target_link_libraries(test-vector PRIVATE vector3d)

# png support
add_library(png lodepng.h lodepng.cpp)
add_library(pixels pixels.h pixels.cpp color.h)
target_link_libraries(pixels PRIVATE png PUBLIC vector3d)

# testing pixels class
add_executable(test-pixels test-pixels.cpp)
target_link_libraries(test-pixels PRIVATE pixels)