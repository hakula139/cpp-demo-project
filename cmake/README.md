# CMake Modules

This directory contains custom CMake modules and utilities for the project.

## Files

### `CompilerWarnings.cmake`

Provides the `set_project_warnings()` function to apply comprehensive compiler warnings across different compilers.
Warnings can optionally be treated as errors.

Usage:

```cmake
include(CompilerWarnings)
add_library(demo_project_warnings INTERFACE)
set_project_warnings(demo_project_warnings)
```

### `StaticAnalysis.cmake`

Configures static analysis tools:

- **clang-tidy**: Enable with `-DENABLE_CLANG_TIDY=ON`
- **cppcheck**: Enable with `-DENABLE_CPPCHECK=ON`

### `demo-config.cmake.in`

Template for the package configuration file that allows other projects to find and use this package with `find_package(demo)`.

## Options

The project provides several configuration options:

- `DEMO_BUILD_TESTS`: Build test suite (default: ON for main project)
- `DEMO_BUILD_EXAMPLES`: Build example programs (default: ON for main project)
- `DEMO_ENABLE_WARNINGS`: Enable compiler warnings (default: ON)
- `WARNINGS_AS_ERRORS`: Treat warnings as errors (default: OFF)
- `ENABLE_CLANG_TIDY`: Enable clang-tidy static analysis (default: ON)
- `ENABLE_CPPCHECK`: Enable cppcheck static analysis (default: ON)

## Modern CMake Practices

This build system follows modern CMake best practices:

1. **Target-based approach**: Uses targets instead of global variables
2. **Interface libraries**: For sharing common settings across targets
3. **Generator expressions**: For build-type specific configurations
4. **Proper installation**: With package config files for easy consumption
5. **Modular structure**: Each component is a separate library
6. **Namespace aliases**: All targets have `demo::` namespaced aliases
