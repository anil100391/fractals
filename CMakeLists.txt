cmake_minimum_required(VERSION 3.15)
project(opengl CXX)
set (CMAKE_CXX_STANDARD 17)

add_executable (mandelbrot mandelbrot.cpp)
add_executable (tricorn tricorn.cpp)
add_executable (orbit mandelbrotOrbit.cxx)
add_executable (newton newton.cpp)

target_link_libraries (mandelbrot cairo pthread)
target_link_libraries (tricorn cairo pthread)
target_link_libraries (orbit cairo pthread)
target_link_libraries (newton cairo pthread)
