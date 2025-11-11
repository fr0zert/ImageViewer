set(SRC_FILES
  "main.cpp"
  "DebugPanel.cpp"
  "EventHandler.cpp"
  "ImageHandler.cpp"
)

if(WIN32)
  add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} "metadata/version.rc")
elseif(UNIX)
  # TODO LINUX
  message(FATAL_ERROR "TODO: Linux build not supported yet")
endif()

# Include the current source dir so headers in src/ resolve
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SAIL::sail-c++)

if(WIN32)
  # On windows machines i think i want static link all stuff
  target_compile_definitions(${PROJECT_NAME} PRIVATE SAIL_STATIC)
elseif(UNIX)
  # TODO LINUX
  message(FATAL_ERROR "TODO: Linux build not supported yet")
endif()
