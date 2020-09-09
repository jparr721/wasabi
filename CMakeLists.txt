cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(wasabi LANGUAGES CXX CUDA)

include(CheckLanguage)
check_language(CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(INSTALL_LIB_DIR lib CACHE PATH "Install directory for library code")
set(INSTALL_BIN_DIR CACHE PATH "Install directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Install directory for header files")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/wasabi)
endif()

if(NOT DEFINED CMAKE_CUDA_STANDARD)
  set(CMAKE_CUDA_STANDARD 14)
  set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

find_package(Boost 1.69 COMPONENTS unit_test_framework REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}"
  ${Boost_INCLUDE_DIRS}
)

add_subdirectory(vocabulary)
add_subdirectory(corpus)
add_subdirectory(tokenizer)
add_subdirectory(utils)
add_subdirectory(bayes)
