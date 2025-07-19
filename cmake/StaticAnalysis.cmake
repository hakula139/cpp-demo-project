#[=======================================================================[.rst:
StaticAnalysis
--------------

Static analysis tools configuration for C++ projects.

This module provides configuration for static analysis tools including
clang-tidy and cppcheck. Tools are only enabled if explicitly requested
and the corresponding executable is found on the system.

Options:
  ENABLE_CLANG_TIDY - Enable static analysis with clang-tidy (default: OFF)
  ENABLE_CPPCHECK   - Enable static analysis with cppcheck (default: OFF)

Variables set:
  CMAKE_CXX_CLANG_TIDY - Set if clang-tidy is enabled and found
  CMAKE_CXX_CPPCHECK   - Set if cppcheck is enabled and found

Example:
  set(ENABLE_CLANG_TIDY ON)
  include(StaticAnalysis)
#]=======================================================================]

# Static analysis tools configuration

option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY clang-tidy)
  if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
    message(STATUS "clang-tidy finished setting up")
  else()
    message(WARNING "clang-tidy requested but executable not found")
  endif()
endif()

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(
      CMAKE_CXX_CPPCHECK
      ${CPPCHECK}
      --enable=all
      --inconclusive
      --quiet
      --suppress=missingIncludeSystem
      --suppress=unmatchedSuppression
      --suppress=unusedFunction
    )
    message(STATUS "cppcheck finished setting up")
  else()
    message(WARNING "cppcheck requested but executable not found")
  endif()
endif()
