cmake_minimum_required(VERSION 2.6)
project(VEHICLE_COUNTING)

add_compile_options(-std=c++11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIR})

link_directories(${OpenCV_LIBRARY_DIR})

include_directories(${VEHICLE_COUNTING_SOURCE_DIR}/src)
add_subdirectory(src)
