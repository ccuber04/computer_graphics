cmake_minimum_required(VERSION 3.22.1)
project(raytracer)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# adjust any compiler settings
add_compile_options(-ggdb -Wall -Wextra -pedantic -std=c++20 -flto -Ofast -march=native)
#add_compile_options(-ggdb -Wall -Wextra -pedantic -std=c++20)

add_subdirectory(src)
add_subdirectory(test)
