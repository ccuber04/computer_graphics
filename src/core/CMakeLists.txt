add_library(core
  aabb.h aabb.cpp
  color.h
  constants.h
  hit.h hit.cpp
  material.h material.cpp
  object.h object.cpp
  texture.h texture.cpp
  point3d.h
  random.h random.cpp
  ray.h ray.cpp
  vector3d.h vector3d.cpp
  xorshiftrplus.h xorshiftrplus.cpp
  noise.h noise.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
