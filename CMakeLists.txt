cmake_minimum_required(VERSION 3.23)

# Project configuration
project(demo VERSION 1.0.0 DESCRIPTION "Demo project for modern C++ features" LANGUAGES CXX)

# Only build tests if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_MAIN_PROJECT TRUE)
else()
  set(IS_MAIN_PROJECT FALSE)
endif()

# Global project settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include CMake utilities
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Options
option(BUILD_TESTS "Build tests" ${IS_MAIN_PROJECT})
option(BUILD_EXAMPLES "Build examples" ${IS_MAIN_PROJECT})
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

# macOS-specific fixes for Mach-O linker errors
if(APPLE)
  # Ensure we use the system's default archive tools to prevent corrupted static libraries
  # This fixes "archive member '/' not a mach-o file" errors
  set(CMAKE_AR "/usr/bin/ar" CACHE FILEPATH "Path to ar archiver" FORCE)
  set(CMAKE_RANLIB "/usr/bin/ranlib" CACHE FILEPATH "Path to ranlib" FORCE)

  # Additional macOS linker settings to prevent archive corruption
  set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")
  set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")

  # Ensure proper handling of static libraries
  set(
    CMAKE_FIND_LIBRARY_SUFFIXES
    ".a"
    ".dylib"
    ".so"
  )

  # Set proper flags for static library creation (ranlib handled by ARCHIVE_FINISH)
  set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> <LINK_FLAGS> cr <TARGET> <OBJECTS>")
  set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> <LINK_FLAGS> cr <TARGET> <OBJECTS>")
endif()

# Add custom CMake modules
include(CompilerWarnings)
include(StaticAnalysis)
include(Dependencies)
include(ModuleHelpers)

# Create interface target for project options
add_library(${PROJECT_NAME}_project_options INTERFACE)
add_library(${PROJECT_NAME}::project_options ALIAS ${PROJECT_NAME}_project_options)
target_compile_features(${PROJECT_NAME}_project_options INTERFACE cxx_std_23)

# Create interface target for warnings
add_library(${PROJECT_NAME}_project_warnings INTERFACE)
add_library(${PROJECT_NAME}::project_warnings ALIAS ${PROJECT_NAME}_project_warnings)

if(ENABLE_WARNINGS)
  set_project_warnings(${PROJECT_NAME}_project_warnings)
endif()

# Build type specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(
    ${PROJECT_NAME}_project_options
    INTERFACE
      -g
      -O0
  )
else()
  target_compile_options(
    ${PROJECT_NAME}_project_options
    INTERFACE
      -O3
      -DNDEBUG
  )
endif()

# Setup project dependencies (after project options are fully configured)
setup_project_dependencies()

# Add subdirectories
add_subdirectory(src)

# Build tests if enabled
if(BUILD_TESTS)
  enable_testing()
  include(CTest)

  # Temporarily disable cppcheck for tests
  if(DEFINED CMAKE_CXX_CPPCHECK)
    set(CMAKE_CXX_CPPCHECK_BACKUP ${CMAKE_CXX_CPPCHECK})
    unset(CMAKE_CXX_CPPCHECK)
  endif()

  add_subdirectory(tests)

  # Restore cppcheck settings
  if(DEFINED CMAKE_CXX_CPPCHECK_BACKUP)
    set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK_BACKUP})
    unset(CMAKE_CXX_CPPCHECK_BACKUP)
  endif()
endif()

# Build examples if enabled
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
  TARGETS
    ${PROJECT_NAME}_project_options
    ${PROJECT_NAME}_project_warnings
  EXPORT ${PROJECT_NAME}-targets
)

install(
  EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Create package config files
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
