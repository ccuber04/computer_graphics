add_library(textures
    solid.h solid.cpp
    gradient.h gradient.cpp
    image.h image.cpp
    checkerboard.h checkerboard.cpp
    normal.h normal.cpp
    interesting.h interesting.cpp
    marble.h marble.cpp
)
target_include_directories(textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(textures PUBLIC core image)