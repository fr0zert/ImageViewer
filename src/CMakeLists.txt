qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION
    main.cpp
    ImageViewer.hpp
    ImageViewer.cpp
    ImageHandler.hpp
    ImageHandler.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
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
