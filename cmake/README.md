# CMake Modules

This directory contains custom CMake modules and utilities for the project.

## Options

The project provides several configuration options:

- `BUILD_TESTS`: Build test suite (default: ON for main project)
- `BUILD_EXAMPLES`: Build example programs (default: ON for main project)
- `ENABLE_WARNINGS`: Enable compiler warnings (default: ON)
- `WARNINGS_AS_ERRORS`: Treat warnings as errors (default: OFF)
- `ENABLE_CLANG_TIDY`: Enable clang-tidy static analysis (default: OFF)
- `ENABLE_CPPCHECK`: Enable cppcheck static analysis (default: OFF)

## Files

### `ModuleHelpers.cmake`

Provides the `add_module()` function to simplify creating project modules.

Usage:

```cmake
include(ModuleHelpers)
add_module(concepts)                           # Header-only module
add_module(memory SOURCES smart_pointers.cpp)  # Regular module with sources
add_module(network DEPENDENCIES OpenSSL::SSL)  # With dependencies
```

Features:

- Automatically creates proper target names and namespaced aliases
- Sets up include directories for both build and install interfaces
- Links to project options and warnings targets
- Configures installation rules

### `Dependencies.cmake`

Manages external project dependencies using a find-first, fetch-fallback approach.

Usage:

```cmake
include(Dependencies)
setup_project_dependencies()
```

Currently handles:

- **Catch2 v3.8.1**: Modern C++ testing framework (only when `BUILD_TESTS` is enabled)

Features:

- Uses `find_package()` first to find system-installed dependencies
- Falls back to `FetchContent` when dependencies are not found
- Automatically configures CMake modules for test discovery

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

### `config.cmake.in`

Template for the package configuration file that allows other projects to find and use this package with `find_package()`.

## Modern CMake Practices

This build system follows modern CMake best practices:

1. **Target-based approach**: Uses targets instead of global variables
2. **Interface libraries**: For sharing common settings across targets
3. **Generator expressions**: For build-type specific configurations
4. **Proper installation**: With package config files for easy consumption
5. **Modular structure**: Each component is a separate library
6. **Namespace aliases**: All targets have `${PROJECT_PREFIX}::` namespaced aliases
