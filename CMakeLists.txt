cmake_minimum_required(VERSION 4.0.2)

project(ImageViewer VERSION 0.1.0 LANGUAGES CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Out folder for ready executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output/${CMAKE_BUILD_TYPE}")

# static linking windows libs so i can distribute it without any dependencies for testing
# cl alternatives to /MT and /MTd
# https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fms-runtime-lib
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-fms-runtime-lib="static")
    add_link_options(-fms-runtime-lib="static")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fms-runtime-lib="static_dbg")
    add_link_options(-fms-runtime-lib="static_dbg")
endif()

# FIX for compiling, compiler trying to use WinMain while im need main because it is easier to access argc and argv
# hope to fix it later actually
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")

# Dependencies
include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW TRUE
)

# idk if works, someone said it dont, and i need use config.h | figure it out later 
set(BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# src directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Enable debug console on Debug build
# TODO dont use filenames, group somehow
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/ImageHandler.cpp) 
else()
  add_executable(${PROJECT_NAME} WIN32 ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/ImageHandler.cpp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)