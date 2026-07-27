cmake_minimum_required(VERSION 3.25)

include(${CMAKE_CURRENT_SOURCE_DIR}/bootstrap-vcpkg.cmake)

project(ImageViewer VERSION 0.0.1.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)
find_package(SailC++ CONFIG REQUIRED)

qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
    "src/main.cpp"

    "src/app/MainWindow.cpp"
    "src/app/MainWindow.hpp"

    "src/core/ImageController.cpp"
    "src/core/ImageController.hpp"
    "src/core/ImageLoader.cpp"
    "src/core/ImageLoader.hpp"

    "src/ui/ImageConvas.cpp"
    "src/ui/ImageConvas.hpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent
    SAIL::sail-c++
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)

# via vcpkg qt6 dont copy platforms specific dll to build folder
if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Qt6::QWindowsIntegrationPlugin>"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/"
    )
endif()
