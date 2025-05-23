# test vector3d operations
add_executable(test-vector test-vector3d.cpp)
target_link_libraries(test-vector PRIVATE core)


# testing pixels class by creating some images
add_executable(test-pixels test-pixels.cpp)
target_link_libraries(test-pixels PRIVATE image)

# testing ray sphere intersections, make an image of intersections
add_executable(test-intersect test-intersect.cpp)
target_link_libraries(test-intersect PRIVATE objects image)

# testing the camera by displaying two spheres
add_executable(test-camera test-camera.cpp)
target_link_libraries(test-camera PRIVATE raytracer)

# testing random number generation
add_executable(test-random test-random.cpp)
target_link_libraries(test-random PRIVATE core)

# test the world by intersection some number of spheres
add_executable(test-world test-world.cpp)
target_link_libraries(test-world PRIVATE raytracer)

# test materials by path tracing a scene from a given input file
add_executable(test-materials test-materials.cpp)
target_link_libraries(test-materials PRIVATE raytracer)

add_executable(test-textures test-textures.cpp)
target_link_libraries(test-textures PRIVATE raytracer)

# test triangle
add_executable(test-triangle test-triangle.cpp)
target_link_libraries(test-triangle PRIVATE raytracer)

# test fog
add_executable(test-fog test-fog.cpp)
target_link_libraries(test-fog PRIVATE raytracer)

# test bvh
add_executable(test-bvh test-bvh.cpp)
target_link_libraries(test-bvh PRIVATE raytracer)