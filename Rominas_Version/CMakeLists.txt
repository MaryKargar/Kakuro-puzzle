cmake_minimum_required(VERSION 3.30)
project(Kakuro2)

set(CMAKE_CXX_STANDARD 20)

include_directories("../../galib")
add_executable(Kakuro2 main.cpp)
get_filename_component(ABSOLUTE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../galib/cmake-build-debug/libGAlib.a" ABSOLUTE)
message(STATUS "Abs-Path of GaLib-Library: ${ABSOLUTE_LIBRARY_PATH}")
target_link_libraries(Kakuro2 "${ABSOLUTE_LIBRARY_PATH}")
