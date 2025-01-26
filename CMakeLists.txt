cmake_minimum_required(VERSION 3.15)
project(Kakuro_problem)

set(CMAKE_CXX_STANDARD 17)

# Include the GA library path
get_filename_component(GALIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../galib" ABSOLUTE)
message(STATUS "GaLib Path: ${GALIB_PATH}")
include_directories(${GALIB_PATH})

# Add the executable
add_executable(Kakuro_problem main.cpp)

# Link the GA library
get_filename_component(GALIB_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../galib/cmake-build-debug/libgalib.a" ABSOLUTE)
message(STATUS "GaLib Library Path: ${GALIB_LIBRARY_PATH}")
target_link_libraries(Kakuro_problem "${GALIB_LIBRARY_PATH}")
