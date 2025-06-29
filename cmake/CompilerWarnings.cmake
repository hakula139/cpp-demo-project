#[=======================================================================[.rst:
CompilerWarnings
----------------

Comprehensive compiler warnings configuration for C++ projects.

This module provides functions to set up extensive compiler warnings
for both Clang and GCC compilers, with optional treatment of warnings as errors.

Based on: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md

Functions provided:
  set_project_warnings - Configure compiler warnings for a target

Options:
  WARNINGS_AS_ERRORS - Treat compiler warnings as errors (default: OFF)

Example:
  include(CompilerWarnings)
  set_project_warnings(my_target)
#]=======================================================================]

#[=======================================================================[.rst:
set_project_warnings
--------------------

Configure comprehensive compiler warnings for a target.

  set_project_warnings(<target_name>)

Arguments:
  <target_name> - Name of the target to apply warnings to

This function:
  - Detects the compiler (Clang or GCC)
  - Applies appropriate warning flags for the detected compiler
  - Supports both C and C++ languages
  - Optionally treats warnings as errors based on WARNINGS_AS_ERRORS option

Supported Compilers:
  - Clang / LLVM - Comprehensive warning set including thread safety analysis
  - GCC - Standard warning set with GCC-specific checks

Warning Categories Covered:
  - Type conversion and casting warnings
  - Uninitialized variable detection
  - Unreachable code detection
  - Thread safety analysis (Clang only)
  - Format string security
  - Shadow variable detection
  - And many more...

Example:
  include(CompilerWarnings)
  add_library(my_lib src.cpp)
  set_project_warnings(my_lib)
#]=======================================================================]
function(set_project_warnings PROJECT_NAME)
  option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

  set(CLANG_WARNINGS
    -Wall
    -Wextra # Reasonable and standard
    -Wcast-align # Warn for potential performance problem casts
    -Wcomma # Warn about suspicious uses of the comma operator
    -Wconditional-uninitialized # Warn about uninitialized variables in conditionals
    -Wconversion # Warn on type conversions that may lose data
    -Wctad-maybe-unsupported # Warn about potentially unsupported CTAD
    -Wdeprecated # Warn about deprecated features
    -Wdeprecated-copy # Warn about deprecated copy operations
    -Wdeprecated-copy-dtor # Warn about deprecated copy with destructor
    -Wdouble-promotion # Warn if float is implicit promoted to double
    -Wextra-semi # Warn about extra semicolons
    -Wextra-semi-stmt # Warn about extra semicolons in statements
    -Wformat=2 # Warn on security issues around functions that format output (i.e. printf)
    -Wimplicit-fallthrough # Warn on statements that fallthrough without an explicit annotation
    -Wloop-analysis # Warn about suspicious loop constructs
    -Wmisleading-indentation # Warn if indentation implies blocks where blocks do not exist
    -Wmost # Enable most warnings (Clang-specific, more comprehensive than -Wall)
    -Wmove # Warn about suspicious std::move usage
    -Wnon-virtual-dtor # Warn the user if a class with virtual functions has a non-virtual destructor
    -Wnull-dereference # Warn if a null dereference is detected
    -Wold-style-cast # Warn for C-style casts
    -Woverloaded-virtual # Warn if you overload (not override) a virtual function
    -Wpedantic # Warn if non-standard C++ is used
    -Wrange-loop-analysis # Warn about range-based for loop analysis
    -Wredundant-move # Warn about redundant std::move calls
    -Wself-assign # Warn about self-assignment
    -Wself-move # Warn about self-move
    -Wshadow # Warn the user if a variable declaration shadows one from a parent context
    -Wshift-sign-overflow # Warn about shift operations that overflow
    -Wsign-conversion # Warn on sign conversions
    -Wsizeof-array-argument # Warn about sizeof on array function parameters
    -Wstring-conversion # Warn about string literal to bool conversion
    -Wstring-plus-int # Warn about string + int operations
    -Wtautological-compare # Warn about tautological comparisons
    -Wthread-safety # Enable thread safety analysis
    -Wthread-safety-beta # Enable beta thread safety analysis
    -Wundefined-reinterpret-cast # Warn about undefined reinterpret_cast
    -Wuninitialized # Warn about uninitialized variables
    -Wunreachable-code # Warn about unreachable code
    -Wunreachable-code-break # Warn about unreachable break statements
    -Wunreachable-code-return # Warn about unreachable return statements
    -Wunused # Warn on anything being unused
    -Wunused-lambda-capture # Warn about unused lambda captures
    -Wuser-defined-literals # Warn about user-defined literals
    -Wvla # Warn about variable length arrays
  )

  if(WARNINGS_AS_ERRORS)
    list(APPEND CLANG_WARNINGS -Werror)
  endif()

  set(GCC_WARNINGS
    -Wall
    -Wextra # Reasonable and standard
    -Wcast-align # Warn for potential performance problem casts
    -Wconversion # Warn on type conversions that may lose data
    -Wdouble-promotion # Warn if float is implicit promoted to double
    -Wduplicated-branches # Warn if if / else branches have duplicated code
    -Wduplicated-cond # Warn if if / else chain has duplicated conditions
    -Wformat=2 # Warn on security issues around functions that format output (i.e. printf)
    -Wimplicit-fallthrough # Warn on statements that fallthrough without an explicit annotation
    -Wlogical-op # Warn about logical operations being used where bitwise were probably wanted
    -Wmisleading-indentation # Warn if indentation implies blocks where blocks do not exist
    -Wnon-virtual-dtor # Warn the user if a class with virtual functions has a non-virtual destructor
    -Wnull-dereference # Warn if a null dereference is detected
    -Wold-style-cast # Warn for C-style casts
    -Woverloaded-virtual # Warn if you overload (not override) a virtual function
    -Wpedantic # Warn if non-standard C++ is used
    -Wshadow # Warn the user if a variable declaration shadows one from a parent context
    -Wsign-conversion # Warn on sign conversions
    -Wunused # Warn on anything being unused
    -Wuseless-cast # Warn if you perform a cast to the same type
  )

  if(WARNINGS_AS_ERRORS)
    list(APPEND GCC_WARNINGS -Werror)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
  else()
    message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
  endif()

  # Use the same warning flags for C
  set(PROJECT_WARNINGS_C "${PROJECT_WARNINGS_CXX}")

  target_compile_options(${PROJECT_NAME} INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>
    $<$<COMPILE_LANGUAGE:C>:${PROJECT_WARNINGS_C}>
  )
endfunction()
