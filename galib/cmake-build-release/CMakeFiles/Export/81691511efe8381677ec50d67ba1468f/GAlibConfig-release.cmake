#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GAlib::GAlib" for configuration "Release"
set_property(TARGET GAlib::GAlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GAlib::GAlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libgalib.a"
  )

list(APPEND _cmake_import_check_targets GAlib::GAlib )
list(APPEND _cmake_import_check_files_for_GAlib::GAlib "${_IMPORT_PREFIX}/lib/libgalib.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
