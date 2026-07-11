# Must be included from the top of the top-level CMakeLists.txt, BEFORE project().
# Picks a vcpkg install, in priority order:
#   1. CMAKE_TOOLCHAIN_FILE already set explicitly (e.g. passed via -D or a preset) -> respected as-is
#   2. VCPKG_ROOT environment variable (the standard way people set up a global vcpkg)
#   3. `vcpkg` executable found on PATH
#   4. Common global install locations
#   5. Fallback: local clone at <project-root>/vcpkg

if(DEFINED CMAKE_TOOLCHAIN_FILE)
  return()
endif()

if(DEFINED ENV{VCPKG_ROOT})
  set(_vcpkg_root "$ENV{VCPKG_ROOT}")
endif()

if(NOT _vcpkg_root)
  find_program(_vcpkg_exe NAMES vcpkg)
  if(_vcpkg_exe)
    get_filename_component(_vcpkg_root "${_vcpkg_exe}" DIRECTORY)
  endif()
endif()

if(NOT _vcpkg_root)
  foreach(_candidate
        "$ENV{HOME}/vcpkg" "$ENV{USERPROFILE}/vcpkg"
        "C:/vcpkg"
        "/usr/local/share/vcpkg" "/opt/vcpkg")
    if(EXISTS "${_candidate}/scripts/buildsystems/vcpkg.cmake")
      set(_vcpkg_root "${_candidate}")
      break()
    endif()
  endforeach()
endif()

if(NOT _vcpkg_root AND EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
  set(_vcpkg_root "${CMAKE_SOURCE_DIR}/vcpkg")
endif()

if(_vcpkg_root AND EXISTS "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
  message(STATUS "Using vcpkg at: ${_vcpkg_root}")
  set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")
else()
  message(FATAL_ERROR
        "vcpkg not found. Either:\n"
        "  - set the VCPKG_ROOT environment variable to an existing vcpkg install, or\n"
        "  - clone one locally: git clone https://github.com/microsoft/vcpkg && ./vcpkg/bootstrap-vcpkg.sh"
    )
endif()
