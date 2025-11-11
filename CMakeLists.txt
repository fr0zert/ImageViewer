# main cmake file
cmake_minimum_required(VERSION 3.31.6)

if(DEFINED ENV{PROGRAM_VERSION} AND NOT "$ENV{PROGRAM_VERSION}" STREQUAL "")
  set(VERSION $ENV{PROGRAM_VERSION})
else()
  set(VERSION 0.0.0)
  message(STATUS "Debug or Local build (version not specified)")
endif()

# project setup
project(ImageViewer VERSION ${VERSION} LANGUAGES C CXX)

set(COMPANY_NAME "fr0zert")
set(PRODUCT_NAME "${PROJECT_NAME}")
set(FILE_DESCRIPTION "A simple ${PROJECT_NAME} application.")

configure_file(
  "src/metadata/version.rc.in"
  "src/metadata/version.rc"
  @ONLY
)

# out-dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")

# dependencies
find_package(SailC++ CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
# add_subdirectory("dependencies")

# main code
add_subdirectory("src")
