project(karolherbst-testubg)
cmake_minimum_required(VERSION 2.8)

find_path(HYPODERMIC_INCLUDE_DIRS Hypodermic/ContainerBuilder.h)
find_library(HYPODERMIC_LIBRARIES NAMES libhypodermic hypodermic)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HYPODERMIC DEFAULT_MSG HYPODERMIC_LIBRARIES HYPODERMIC_INCLUDE_DIRS)
mark_as_advanced(HYPODERMIC_INCLUDE_DIRS HYPODERMIC_LIBRARIES)

set(CMAKE_EXTERNAL_BINARY_DIR "${CMAKE_BINARY_DIR}/external")

if(NOT HYPODERMIC_FOUND)
  include(ExternalProject)
  ExternalProject_Add(
    hypodermic
    URL "https://github.com/karolherbst/hypodermic/archive/master.zip"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_EXTERNAL_BINARY_DIR}/hypodermic
    BUILD_IN_SOURCE 0
    SOURCE_DIR <prefix>/src/<name>/hypodermic-master/
  )
endif()
