# Install script for directory: C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/GAlib")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/mingw32/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/cmake-build-release/libgalib.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ga" TYPE FILE FILES
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GA1DArrayGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GA2DArrayGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GA3DArrayGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GAList.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GAListGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GARealGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GAStringGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GATree.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GATreeGenome.C"
    "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/ga/GAAllele.C"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib/GAlibConfig.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib/GAlibConfig.cmake"
         "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/cmake-build-release/CMakeFiles/Export/81691511efe8381677ec50d67ba1468f/GAlibConfig.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib/GAlibConfig-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib/GAlibConfig.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib" TYPE FILE FILES "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/cmake-build-release/CMakeFiles/Export/81691511efe8381677ec50d67ba1468f/GAlibConfig.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib" TYPE FILE FILES "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/cmake-build-release/CMakeFiles/Export/81691511efe8381677ec50d67ba1468f/GAlibConfig-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GAlib" TYPE FILE FILES "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/cmake-build-release/GAlibConfigVersion.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/nkamca/OneDrive - Wiener Stadtwerke/Desktop/MAI/Advanced Programming/evol/galib/cmake-build-release/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
