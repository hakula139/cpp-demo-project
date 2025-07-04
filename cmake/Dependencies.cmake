#[=======================================================================[.rst:
Dependencies
------------

Handle all project dependencies for the demo project.

This module provides functions to find and fetch project dependencies.
It handles both system-installed packages and fallback to FetchContent
when packages are not found.

Functions provided:
  setup_catch2_dependency     - Find or fetch Catch2 testing framework
  setup_project_dependencies  - Setup all project dependencies

Dependencies managed:
  - Catch2 v3.8.1 - Modern C++ testing framework (only if BUILD_TESTS is ON)

Example:
  include(Dependencies)
  setup_project_dependencies()
#]=======================================================================]

include(FetchContent)

#[=======================================================================[.rst:
setup_catch2_dependency
-----------------------

Find or fetch Catch2 testing framework.

  setup_catch2_dependency()

This function:
  - Checks if BUILD_TESTS option is enabled
  - Attempts to find Catch2 v3 using find_package()
  - Falls back to FetchContent if not found
  - Sets up Catch2 CMake modules for test discovery

Dependencies:
  - Git (for fetching from GitHub)
  - Catch2 v3.8.1 from https://github.com/catchorg/Catch2.git

Variables used:
  BUILD_TESTS - Must be ON to enable Catch2 setup

Variables set:
  CMAKE_MODULE_PATH - Extended with Catch2's CMake modules

Example:
  set(BUILD_TESTS ON)
  setup_catch2_dependency()
#]=======================================================================]
function(setup_catch2_dependency)
  if(NOT BUILD_TESTS)
    return()
  endif()

  find_package(Catch2 3 QUIET)

  if(NOT Catch2_FOUND)
    message(STATUS "Catch2 not found, fetching from GitHub...")

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.8.1
      GIT_SHALLOW TRUE
    )

    # Temporarily disable static analysis for dependencies
    if(DEFINED CMAKE_CXX_CLANG_TIDY)
      set(CMAKE_CXX_CLANG_TIDY_BACKUP ${CMAKE_CXX_CLANG_TIDY})
      unset(CMAKE_CXX_CLANG_TIDY)
    endif()
    if(DEFINED CMAKE_CXX_CPPCHECK)
      set(CMAKE_CXX_CPPCHECK_BACKUP ${CMAKE_CXX_CPPCHECK})
      unset(CMAKE_CXX_CPPCHECK)
    endif()

    FetchContent_MakeAvailable(Catch2)

    # Restore static analysis settings
    if(DEFINED CMAKE_CXX_CLANG_TIDY_BACKUP)
      set(CMAKE_CXX_CLANG_TIDY ${CMAKE_CXX_CLANG_TIDY_BACKUP} PARENT_SCOPE)
      unset(CMAKE_CXX_CLANG_TIDY_BACKUP)
    endif()
    if(DEFINED CMAKE_CXX_CPPCHECK_BACKUP)
      set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK_BACKUP} PARENT_SCOPE)
      unset(CMAKE_CXX_CPPCHECK_BACKUP)
    endif()

    # Add Catch2 CMake module path to the parent scope
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)

    include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
    message(STATUS "Catch2 fetched successfully")
  else()
    include(Catch)
    message(STATUS "Found Catch2: ${Catch2_VERSION}")
  endif()
endfunction()

#[=======================================================================[.rst:
setup_project_dependencies
---------------------------

Setup all project dependencies.

  setup_project_dependencies()

This function calls all individual dependency setup functions
to configure the complete dependency environment for the project.

Currently sets up:
  - Catch2 testing framework (if BUILD_TESTS is enabled)

This function can be extended to handle additional dependencies
as the project grows.

Example:
  include(Dependencies)
  setup_project_dependencies()
#]=======================================================================]
function(setup_project_dependencies)
  setup_catch2_dependency()
endfunction()
