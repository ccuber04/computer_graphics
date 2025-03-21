cmake_minimum_required(VERSION 3.22.1)
project(raytracer)

# adjust any compiler settings
add_compile_options(-g -Wall -Wextra -pedantic -std=c++20 -flto -O3)

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

# shapes
add_library(shapes
        ray.h ray.cpp
        sphere.h sphere.cpp
        hit.h hit.cpp)
target_link_libraries(shapes PUBLIC vector3d)

# testing shapes
add_executable(test-intersect test-intersect.cpp)
target_link_libraries(test-intersect PRIVATE shapes pixels)

# camera
add_library(camera camera.h camera.cpp)
target_link_libraries(camera PUBLIC vector3d)

# testing camera
add_executable(test-camera test-camera.cpp)
target_link_libraries(test-camera PRIVATE vector3d shapes pixels camera)

# testing world
add_executable(test-world test-world.cpp world.cpp random.h random.cpp)
target_link_libraries(test-world PRIVATE camera pixels shapes png)

# testing random
add_executable(test-random test-random.cpp random.h random.cpp)
target_link_libraries(test-random PRIVATE  vector3d)

# test materials
add_executable(test-materials test-materials.cpp world.h world.cpp camera.h camera.cpp random.h random.cpp
        material.h material.cpp diffuse.h diffuse.cpp
        lambertian.h lambertian.cpp
        specular.h specular.cpp
        metallic.h metallic.cpp
        glass.h glass.cpp)
target_link_libraries(test-materials PRIVATE pixels shapes)

# test parser
add_executable(test-parser test-parser.cpp world.h world.cpp camera.h camera.cpp random.h random.cpp
        material.h material.cpp diffuse.h diffuse.cpp
        lambertian.h lambertian.cpp
        specular.h specular.cpp
        metallic.h metallic.cpp
        glass.h glass.cpp
        parser.h parser.cpp)
target_link_libraries(test-parser PRIVATE pixels shapes)
