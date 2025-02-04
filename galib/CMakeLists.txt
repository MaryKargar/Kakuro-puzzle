cmake_minimum_required(VERSION 3.16)

project(GAlib VERSION 2.4.7 LANGUAGES CXX)

include(GNUInstallDirs)

# Check if this is the top-level project (standalone), or a subproject
set(_galib_standalone FALSE)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(_galib_standalone TRUE)
endif()

#
# Options
#

# GALIB_BUILD_SHARED controls if GAlib libraries are built as shared or
# static
#
# It defaults to the value of BUILD_SHARED_LIBS if set, and in most cases
# that should be used instead. The purpose of GALIB_BUILD_SHARED is to allow
# overriding it when built as a subproject. For static libraries with position
# independent code, set CMAKE_POSITION_INDEPENDENT_CODE to True.
set(_galib_shared_default OFF)
if(DEFINED BUILD_SHARED_LIBS)
  set(_galib_shared_default ${BUILD_SHARED_LIBS})
endif()
option(GALIB_BUILD_SHARED "Build shared libraries" ${_galib_shared_default})
unset(_galib_shared_default)

if(GALIB_BUILD_SHARED)
  set(_galib_library_type SHARED)
else()
  set(_galib_library_type STATIC)
endif()

# GALIB_BUILD_INSTALL controls if GAlib adds install support
#
# When built standalone or as a shared library subproject, the default is ON,
# and for static library subproject the default is OFF.
if(_galib_standalone OR GALIB_BUILD_SHARED)
  option(GALIB_BUILD_INSTALL "Add install support" ON)
else()
  option(GALIB_BUILD_INSTALL "Add install support" OFF)
endif()

#
# galib
#
add_library(galib ${_galib_library_type}
  ga/GA1DArrayGenome.C
  ga/GA1DBinStrGenome.C
  ga/GA2DArrayGenome.C
  ga/GA2DBinStrGenome.C
  ga/GA3DArrayGenome.C
  ga/GA3DBinStrGenome.C
  ga/GABaseGA.C
  ga/GABin2DecGenome.C
  ga/gabincvt.C
  ga/GABinStr.C
  ga/GADCrowdingGA.C
  ga/GADemeGA.C
  ga/gaerror.C
  ga/GAGenome.C
  ga/GAIncGA.C
  ga/GAList.C
  ga/GAListBASE.C
  ga/GAListGenome.C
  ga/GAParameter.C
  ga/GAPopulation.C
  ga/garandom.C
  ga/GARealGenome.C
  ga/GAScaling.C
  ga/GASelector.C
  ga/GASimpleGA.C
  ga/GASStateGA.C
  ga/GAStatistics.C
  ga/GAStringGenome.C
  ga/GATree.C
  ga/GATreeBASE.C
  ga/GATreeGenome.C
  ga/GAAllele.C
)

target_include_directories(galib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(galib PROPERTIES
  EXPORT_NAME GAlib
  VERSION ${GAlib_VERSION}
  SOVERSION ${GAlib_VERSION_MAJOR}
)

if(MSVC AND GALIB_BUILD_SHARED)
  # The defines below for compiling and using as a DLL are described in
  # gaconfig.h, but do not seem to be implemented yet. So we use this
  # brute-force approach to generate an import library.
  set_target_properties(galib PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
  target_compile_definitions(galib
    PRIVATE COMPILE_GALIB_AS_DLL
    PUBLIC USE_GALIB_AS_DLL
  )
endif()

if(MSVC)
  target_compile_definitions(galib PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Create alias
#
# Makes targets available to projects using GAlib as a subproject using the
# same names as in the config file package.
add_library(GAlib::GAlib ALIAS galib)

#
# Install
#
if(GALIB_BUILD_INSTALL)
  install(TARGETS galib
    EXPORT galib_targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  install(DIRECTORY ga
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
      PATTERN *.h
  )
  install(FILES
    ga/GA1DArrayGenome.C
    ga/GA2DArrayGenome.C
    ga/GA3DArrayGenome.C
    ga/GAList.C
    ga/GAListGenome.C
    ga/GARealGenome.C
    ga/GAStringGenome.C
    ga/GATree.C
    ga/GATreeGenome.C
    ga/GAAllele.C
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ga
  )

  # Install config file package
  #
  # This allows CMake based projects to use the installed libraries with
  # find_package(GAlib).
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/GAlibConfigVersion.cmake
    VERSION ${GAlib_VERSION}
    COMPATIBILITY SameMajorVersion
  )
  # Since we have no dependencies, use export file directly as config file
  install(EXPORT galib_targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GAlib
    NAMESPACE GAlib::
    FILE GAlibConfig.cmake
  )
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/GAlibConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GAlib
  )
endif()
