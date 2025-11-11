# This file resolves the VCPKG root path
# using environment variables and fallbacks.

set(VCPKG_ROOT_CANDIDATES "")

if (DEFINED ENV{VCPKG_ROOT})
  list(APPEND VCPKG_ROOT_CANDIDATES "$ENV{VCPKG_ROOT}")
endif()

if (DEFINED ENV{VCPKG_INSTALLATION_ROOT})
  list(APPEND VCPKG_ROOT_CANDIDATES "$ENV{VCPKG_INSTALLATION_ROOT}")
endif()

# Add project-local fallback
list(APPEND VCPKG_ROOT_CANDIDATES "${CMAKE_SOURCE_DIR}/vcpkg")

set(VCPKG_ROOT_PATH "" CACHE INTERNAL "Resolved Vcpkg root path")
foreach(ROOT_PATH IN ITEMS ${VCPKG_ROOT_CANDIDATES})
  set(TOOLCHAIN_FILE_PATH
    "${ROOT_PATH}/scripts/buildsystems/vcpkg.cmake")
  if (EXISTS "${TOOLCHAIN_FILE_PATH}")
    set(VCPKG_ROOT_PATH "${ROOT_PATH}"
      CACHE INTERNAL "Resolved Vcpkg root path" FORCE)
    set(VCPKG_TOOLCHAIN_FILE_FINAL "${TOOLCHAIN_FILE_PATH}"
      CACHE INTERNAL "Final Vcpkg toolchain path" FORCE)
    break()
  endif()
endforeach()

# 3. Chainload the actual Vcpkg toolchain file if found, otherwise fail.
if (EXISTS "${VCPKG_TOOLCHAIN_FILE_FINAL}")
  message(STATUS "Vcpkg toolchain found at: ${VCPKG_TOOLCHAIN_FILE_FINAL}")
  # Chainload the actual vcpkg toolchain file
  include("${VCPKG_TOOLCHAIN_FILE_FINAL}")
else()
  message(FATAL_ERROR "Could not find vcpkg toolchain file.
    Please ensure VCPKG_ROOT or VCPKG_INSTALLATION_ROOT is set,
    or vcpkg is cloned to ${CMAKE_SOURCE_DIR}/vcpkg.")
endif()
