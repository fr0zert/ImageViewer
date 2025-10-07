cmake_minimum_required(VERSION 4.0.2)

project(ImageViewer VERSION 0.0.1 LANGUAGES CXX)

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

# Define where to download and extract FreeImage
set(FREEIMAGE_URL "https://netix.dl.sourceforge.net/project/freeimage/Binary%20Distribution/3.18.0/FreeImage3180Win32Win64.zip?viasf=1")
set(FREEIMAGE_ZIP "${CMAKE_BINARY_DIR}/FreeImage3180Win32Win64.zip")
set(FREEIMAGE_EXTRACT_DIR "${CMAKE_BINARY_DIR}/_deps")

# Download FreeImage zip if not already present
if(NOT EXISTS "${FREEIMAGE_ZIP}")
    message(STATUS "Downloading FreeImage...")
    file(DOWNLOAD "${FREEIMAGE_URL}" "${FREEIMAGE_ZIP}" SHOW_PROGRESS)
endif()

# Extract FreeImage with 7z if not already extracted
if(NOT EXISTS "${FREEIMAGE_EXTRACT_DIR}/FreeImage/Dist/x64/FreeImage.lib")
    message(STATUS "Extracting FreeImage using 7z...")
    file(MAKE_DIRECTORY "${FREEIMAGE_EXTRACT_DIR}")
    execute_process(
        COMMAND 7z x "${FREEIMAGE_ZIP}" "-o${FREEIMAGE_EXTRACT_DIR}" -y
        RESULT_VARIABLE result
    )
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "7z extraction failed! Is 7z installed and in your PATH?")
    endif()
endif()


# Include directories and libraries for FreeImage
include_directories("${FREEIMAGE_EXTRACT_DIR}/FreeImage/Dist/x64")
link_directories("${FREEIMAGE_EXTRACT_DIR}/FreeImage/Dist/x64")

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
# src files
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.h")

# Enable debug console on Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
else()
  add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib FreeImage)

# Copy FreeImage.dll after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FREEIMAGE_EXTRACT_DIR}/FreeImage/Dist/x64/FreeImage.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/FreeImage.dll"
)