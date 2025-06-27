#[[
Dependencies.cmake - Handle all project dependencies

This module provides functions to find and fetch project dependencies.
It handles both system-installed packages and fallback to FetchContent
when packages are not found.
]]

include(FetchContent)

#[[
Function: setup_catch2_dependency
Brief: Find or fetch Catch2 testing framework
Parameters:
  - None (uses DEMO_BUILD_TESTS option from parent scope)
]]
function(setup_catch2_dependency)
    if(NOT DEMO_BUILD_TESTS)
        return()
    endif()

        # Try to find Catch2 first
    find_package(Catch2 3 QUIET)

    if(NOT Catch2_FOUND)
        message(STATUS "Catch2 not found, fetching from GitHub...")

        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.8.1
            GIT_SHALLOW    TRUE
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

#[[
Function: setup_project_dependencies
Brief: Setup all project dependencies
Parameters:
  - None
]]
function(setup_project_dependencies)
    setup_catch2_dependency()
endfunction()
