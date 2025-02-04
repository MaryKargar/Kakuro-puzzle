cmake_minimum_required(VERSION 3.15)
project(Kakuro3)

set(CMAKE_CXX_STANDARD 17)

#***********************************************************************************************************************
# SET PATH
# GaLib - Header
set(GALIB_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../galib")
# GaLib Library
set(GALIB_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../galib/cmake-build-release/libgalib.a")
#***********************************************************************************************************************

# Finde OpenMP
find_package(OpenMP REQUIRED)


# GaLib - Header
if(EXISTS "${GALIB_HEADER_PATH}")
    get_filename_component(ABSOLUTE_HEADER_PATH "${GALIB_HEADER_PATH}" ABSOLUTE)
    message(STATUS "Abs-Path of GaLib-Library Header-Files: ${ABSOLUTE_HEADER_PATH}")
    include_directories("${ABSOLUTE_HEADER_PATH}")
else()
    message(FATAL_ERROR "GaLib header path does not exist: ${GALIB_HEADER_PATH}")
endif()

add_executable(Kakuro3 main.cpp kakurosolver/kakurosolver.cpp)

# GaLib Library
if(EXISTS "${GALIB_LIBRARY_PATH}")
    get_filename_component(ABSOLUTE_LIBRARY_PATH "${GALIB_LIBRARY_PATH}" ABSOLUTE)
    message(STATUS "Abs-Path of GaLib-Library: ${ABSOLUTE_LIBRARY_PATH}")
    target_link_libraries(Kakuro3 "${ABSOLUTE_LIBRARY_PATH}")
else()
    message(FATAL_ERROR "GaLib library file does not exist: ${GALIB_LIBRARY_PATH}")
endif()

if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found, linking...")
    target_link_libraries(Kakuro3 OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "OpenMP not found.")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fopenmp)
    link_libraries(gomp)
endif()README.md