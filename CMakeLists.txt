cmake_minimum_required(VERSION 3.31.6)

set(VCPKG_TARGET_TRIPLET "x64-windows-static-md" CACHE STRING "Vcpkg target triplet" FORCE)
set(VCPKG_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Vcpkg build type" FORCE)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file" FORCE)
    message(STATUS "Using vcpkg toolchain from: $ENV{VCPKG_ROOT}")
  else()
    message(WARNING "VCPKG_ROOT not set — vcpkg will not be used.")
  endif()
endif()

if(DEFINED ENV{PROGRAM_VERSION} AND NOT "$ENV{PROGRAM_VERSION}" STREQUAL "")
    set(VERSION $ENV{PROGRAM_VERSION})
else()
    set(VERSION 0.0.0)
    message(STATUS "Local build (version not specified)")
endif()

project(ImageViewer VERSION ${VERSION} LANGUAGES CXX)

set(COMPANY_NAME "fr0zert")
set(PRODUCT_NAME "${PROJECT_NAME}")
set(FILE_DESCRIPTION "A simple ${PROJECT_NAME} appcation.")


# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(raylib CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(freeimage CONFIG REQUIRED)

# Out folder for ready executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")

# static linking windows libs so i can distribute it without any dependencies for testing
# cl alternatives to /MT and /MTd
# https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fms-runtime-lib
# if (CMAKE_BUILD_TYPE STREQUAL "Release")
#     add_compile_options(-fms-runtime-lib="static")
#     add_link_options(-fms-runtime-lib="static")
# elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_compile_options(-fms-runtime-lib="static_dbg")
#     add_link_options(-fms-runtime-lib="static_dbg")
# endif()

# FIX for compiling, compiler trying to use WinMain while im need main because it is easier to access argc and argv
# hope to fix it later actually
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")



# src directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

configure_file(
    "${SOURCE_DIR}/metadata/version.rc.in"
    "${SOURCE_DIR}/metadata/version.rc"
    @ONLY
)
# src files
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.h" "${SOURCE_DIR}/metadata/version.rc")

# Enable debug console on Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
else()
  add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib freeimage::FreeImage freeimage::FreeImagePlus glfw winmm )