cmake_minimum_required(VERSION 3.13)  # version check
project(comp_geometry)

# specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add directories
include_directories(./src/graphics)

# add executable for source file
add_executable(comp_geometry
  ./main.cpp
  ./src/glad.c
  ./src/graphics/graphics.cpp
  ./src/2D/shapes.cpp
)

# Libraries
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(comp_geometry glfw)
