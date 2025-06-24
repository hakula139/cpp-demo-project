cmake_minimum_required(VERSION 3.23)

# Project configuration
project(demo
    VERSION 1.0.0
    DESCRIPTION "Demo project for modern C++ features"
    LANGUAGES CXX
)

# Only build tests if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_MAIN_PROJECT TRUE)
else()
    set(IS_MAIN_PROJECT FALSE)
endif()

# Standard C++ settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build settings
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include cmake utilities
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Options
option(DEMO_BUILD_TESTS "Build tests" ${IS_MAIN_PROJECT})
option(DEMO_BUILD_EXAMPLES "Build examples" ${IS_MAIN_PROJECT})
option(DEMO_ENABLE_WARNINGS "Enable compiler warnings" ON)

# Add custom CMake modules
include(CompilerWarnings)
include(StaticAnalysis)

# Create interface target for project options
add_library(demo_project_options INTERFACE)
add_library(demo::project_options ALIAS demo_project_options)

target_compile_features(demo_project_options INTERFACE cxx_std_23)

# Create interface target for warnings
add_library(demo_project_warnings INTERFACE)
add_library(demo::project_warnings ALIAS demo_project_warnings)

if(DEMO_ENABLE_WARNINGS)
    set_project_warnings(demo_project_warnings)
endif()

# Build type specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(demo_project_options INTERFACE -g -O0)
else()
    target_compile_options(demo_project_options INTERFACE -O3 -DNDEBUG)
endif()

# Find dependencies
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND AND DEMO_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

# Add subdirectories
add_subdirectory(src)

# Build tests if enabled
if(DEMO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Build examples if enabled
if(DEMO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS demo_project_options demo_project_warnings
    EXPORT demo-targets
)

install(EXPORT demo-targets
    FILE demo-targets.cmake
    NAMESPACE demo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/demo
)

# Create package config files
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/demo-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/demo-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/demo
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/demo-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/demo-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/demo-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/demo
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
