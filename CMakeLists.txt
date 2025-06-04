cmake_minimum_required(VERSION 4.0.2)

project(ImageViewer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ready")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ready")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
else()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE")
endif()

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW TRUE
)

set(BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${PROJECT_NAME} WIN32 ${SOURCE_DIR}/main.cpp)
else()
  add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)