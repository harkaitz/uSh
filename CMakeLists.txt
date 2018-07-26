#l:
#l:  Copyright (c) 2018 Harkaitz Agirre Ezama, harkaitzv@gmail.com
#l:
#l:  Permission is hereby granted, free of charge, to any person obtaining
#l:  a copy of this software and associated documentation files (the
#l:  "Software"), to deal in the Software without restriction, including
#l:  without limitation the rights to use, copy, modify, merge, publish,
#l:  distribute, sublicense, and/or sell copies of the Software, and to
#l:  permit persons to whom the Software is furnished to do so, subject to
#l:  the following conditions:
#l:
#l:  The above copyright notice and this permission notice shall be
#l:  included in all copies or substantial portions of the Software.
#l:
#l:  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#l:  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#l:  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#l:  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
#l:  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
#l:  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#l:  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#l:
cmake_minimum_required(VERSION 3.0)
project(uSh VERSION 0.1)

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${uSh_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${uSh_VERSION_MINOR}")
set (CPACK_GENERATOR             ZIP STGZ)
include (CPack)


# C/C++ flags.
if ( CMAKE_COMPILER_IS_GNUCC )
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -Werror -std=gnu99")
  if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -static")
  endif()
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
  set(CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} /W4")
endif ( MSVC )

# Source all .cmake files.
file(GLOB cmakefiles "${CMAKE_SOURCE_DIR}/cmake/*.cmake")
foreach(cmakefile ${cmakefiles})
  message("** Sourcing `${cmakefile}` ...")
  include(${cmakefile})
endforeach()

# Package config.
if(EXISTS cmake/${PROJECT_NAME}-config.cmake.in)
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    cmake/${PROJECT_NAME}-config.cmake.in
    ${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION lib/${PROJECT_NAME}-${PROJECT_VERSION})
  write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
  install(
    FILES
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION lib/${PROJECT_NAME}-${PROJECT_VERSION})
endif()

# 
if (EXISTS ${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src)
endif()

