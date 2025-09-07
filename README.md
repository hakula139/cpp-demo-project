# Modern C++ Features Demo Project

[![Build & Test](https://github.com/hakula139/cpp-demo-project/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/hakula139/cpp-demo-project/actions/workflows/build-and-test.yml)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org)
[![CMake](https://img.shields.io/badge/CMake-3.23+-blue.svg)](https://cmake.org)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A comprehensive demonstration project showcasing modern C++23 features, best practices, and advanced programming techniques.
This project serves as both a learning resource and a reference implementation for modern C++ development.

## 🚀 Features

### Core C++23 Features

- **std::print**: Modern formatted output with type safety
- **std::format**: Powerful string formatting with custom formatters
- **std::expected**: Railway-oriented error handling (via `Result` type)
- **std::ranges**: Algorithms and views for functional programming
- **std::numbers**: Mathematical constants (`π`, `e`) with precision
- **std::source_location**: Enhanced exception context and debugging
- **Concepts**: Compile-time type constraints and validation
- **Three-way comparison (`<=>`)**: Spaceship operator for robust comparisons

### Advanced Features

- **Modular Architecture**: Clean, scalable project structure with 8 separate components
- **Comprehensive Testing**: Full test coverage using Catch2 v3 with benchmarking
- **Advanced CMake**: Modern CMake build system with modular configuration and packaging
- **Memory Safety**: RAII patterns, smart pointers, and resource management utilities
- **Type Safety**: Concept-based constraints preventing common programming errors
- **Performance Tools**: Built-in timing utilities and benchmark framework
- **Error Handling**: Multiple error handling strategies (exceptions, `Result` type, `std::expected`)
- **Python Bindings**: Complete pybind11 integration with modern Python features

### Code Quality & Development

- **Automatic Formatting**: Pre-commit hooks with `clang-format` and `gersemi`
- **Static Analysis**: Integrated `clang-tidy` and `cppcheck` for code quality
- **Documentation**: Comprehensive Doxygen-style documentation
- **CI/CD Automation**: Automatic build and test workflows with CMake presets

## 📋 Table of Contents

- [🚀 Features](#-features)
  - [Core C++23 Features](#core-c23-features)
  - [Advanced Features](#advanced-features)
  - [Code Quality \& Development](#code-quality--development)
- [📋 Table of Contents](#-table-of-contents)
- [🎓 What You'll Learn](#-what-youll-learn)
- [🛠️ Installation](#️-installation)
  - [Prerequisites](#prerequisites)
  - [Quick Start](#quick-start)
    - [Clone the repository](#clone-the-repository)
    - [Build the project](#build-the-project)
    - [Run examples](#run-examples)
    - [Run tests](#run-tests)
  - [CMake Presets](#cmake-presets)
    - [Configure Presets](#configure-presets)
    - [Build \& Test Presets](#build--test-presets)
    - [Workflow Presets](#workflow-presets)
    - [Usage Examples](#usage-examples)
  - [Build Options](#build-options)
  - [Pre-commit Setup (Recommended)](#pre-commit-setup-recommended)
    - [Install pre-commit](#install-pre-commit)
    - [Install the git hooks](#install-the-git-hooks)
    - [Run on all files (optional)](#run-on-all-files-optional)
    - [What the hooks do](#what-the-hooks-do)
- [🎯 Usage](#-usage)
- [📁 Project Structure](#-project-structure)
- [🔧 Components Overview](#-components-overview)
- [💻 Development Notes](#-development-notes)
  - [Code Quality](#code-quality)
  - [Pre-commit Configuration](#pre-commit-configuration)
- [📄 License](#-license)

## 🎓 What You'll Learn

This project demonstrates practical applications of:

- **Modern C++ Concepts**: Type-safe template constraints and compile-time validation
- **Ranges & Views**: Functional programming patterns with lazy evaluation
- **Error Handling**: Multiple strategies from exceptions to functional error types
- **Memory Management**: RAII, smart pointers, and resource lifecycle management
- **Performance**: Timing utilities, benchmarking, and optimization techniques
- **Testing**: Comprehensive test suites with Catch2 v3 and template testing
- **Build Systems**: Modern CMake with modular design and package management
- **Code Quality**: Static analysis, formatting, and development workflows

## 🛠️ Installation

### Prerequisites

- **C++23 compatible compiler** ([Clang] 20+ / [GCC] 14+)
- **[CMake] 3.23+**
- **[Ninja] build system** (required for CMake - faster builds than Make)
- **Python 3.12+** (optional, for Python bindings)

It's recommended to use a development container for the best development experience.
See [`.devcontainer/README.md`](.devcontainer/README.md) for more details.

[GCC]: https://gcc.gnu.org
[Clang]: https://clang.llvm.org
[CMake]: https://cmake.org
[Ninja]: https://ninja-build.org

### Quick Start

#### Clone the repository

```bash
git clone https://github.com/hakula139/cpp-demo-project
cd cpp-demo-project
```

#### Build the project

```bash
cmake --preset release
cmake --build --preset release
```

#### Run examples

```bash
./build/examples/algorithms_example
./build/examples/containers_example
./build/examples/exceptions_example
./build/examples/memory_example
./build/examples/random_example
./build/examples/shapes_example
./build/examples/timing_example
```

#### Run tests

```bash
ctest --preset release
```

### CMake Presets

This project uses CMake presets for streamlined build configuration.

#### Configure Presets

| Preset                    | Description                                                                        |
| ------------------------- | ---------------------------------------------------------------------------------- |
| `debug`                   | Debug build with symbols and no optimization                                       |
| `release`                 | Release build with full optimization                                               |
| `debug-no-tests`          | Debug build without tests and examples (faster config)                             |
| `release-no-tests`        | Release build without tests and examples (faster config)                           |
| `debug-strict`            | Debug build with static analysis and warnings treated as errors                    |
| `release-strict`          | Release build with static analysis and warnings treated as errors                  |
| `debug-python`            | Debug build with Python bindings                                                   |
| `release-python`          | Release build with Python bindings                                                 |
| `debug-python-no-tests`   | Debug build with Python bindings, without tests and examples                       |
| `release-python-no-tests` | Release build with Python bindings, without tests and examples                     |
| `debug-python-strict`     | Debug build with Python bindings, static analysis and warnings treated as errors   |
| `release-python-strict`   | Release build with Python bindings, static analysis and warnings treated as errors |

#### Build & Test Presets

Each configure preset has corresponding build and test presets with the same names.

#### Workflow Presets

Each workflow preset is a combination of the configure, build, and test presets.

| Preset                           | Description                                          |
| -------------------------------- | ---------------------------------------------------- |
| `debug-workflow`                 | Complete workflow for `debug` preset                 |
| `release-workflow`               | Complete workflow for `release` preset               |
| `debug-strict-workflow`          | Complete workflow for `debug-strict` preset          |
| `release-strict-workflow`        | Complete workflow for `release-strict` preset        |
| `debug-python-workflow`          | Complete workflow for `debug-python` preset          |
| `release-python-workflow`        | Complete workflow for `release-python` preset        |
| `debug-python-strict-workflow`   | Complete workflow for `debug-python-strict` preset   |
| `release-python-strict-workflow` | Complete workflow for `release-python-strict` preset |

#### Usage Examples

```bash
# List available presets
cmake --list-presets=configure
cmake --list-presets=build
cmake --list-presets=test

# Quick development cycle
cmake --preset debug                        # Configure debug build
cmake --build --preset debug                # Build debug targets
ctest --preset debug                        # Run debug tests

# Fast iteration (no tests and examples)
cmake --preset debug-no-tests               # Configure without tests
cmake --build --preset debug-no-tests       # Build main targets only

# Production build
cmake --preset release                      # Configure release build
cmake --build --preset release              # Build release targets
ctest --preset release                      # Run release tests

# Automated workflows (configure + build + test)
cmake --workflow --preset debug-workflow    # Complete debug cycle
cmake --workflow --preset release-workflow  # Complete release cycle
```

### Build Options

See [`cmake/README.md`](cmake/README.md#options) for additional build options.

### Pre-commit Setup (Recommended)

This project uses [pre-commit](https://pre-commit.com) to ensure code quality and consistent formatting.
Set up pre-commit hooks to automatically format your code before each commit:

#### Install pre-commit

```bash
pip3 install pre-commit
```

#### Install the git hooks

```bash
pre-commit install
```

#### Run on all files (optional)

```bash
pre-commit run --all-files
```

#### What the hooks do

- **Standard checks**:
  - Removes trailing whitespace
  - Ensures proper file endings
  - Validates YAML syntax
  - Checks for added large files
- **clang-format**: Formats C++ code according to the project style
- **gersemi**: Formats CMake files with consistent indentation
- **ruff**: Lint and format Python code with consistent style
- **mypy**: Lints Python code with type hints
- **bandit**: Lints Python code with security best practices
- **markdownlint-cli2**: Lints Markdown files with consistent formatting

The hooks will run automatically on `git commit` and prevent commits with formatting issues.

## 🎯 Usage

```cpp
#include <print>

#include "algorithms/stl.hpp"
#include "containers/container.hpp"
#include "shapes/circle.hpp"

using cpp_features::algorithms::SortContainer;
using cpp_features::containers::Container;
using cpp_features::shapes::CreateCircle;

auto main() -> int {
  // Modern container with concept constraints
  Container<int> numbers{42, 17, 89, 3, 56};
  std::println("Original: {}", numbers);

  // Sort the container in place
  SortContainer(numbers);
  std::println("Sorted: {}", numbers);

  // Type-safe factory functions with validation
  auto circle = CreateCircle(5.0);
  auto area = circle->GetArea();
  auto perimeter = circle->GetPerimeter();
  std::println("Area: {:.2f}, Perimeter: {:.2f}", area, perimeter);

  return 0;
}
```

More examples can be found in the [`examples`](examples) directory.

## 📁 Project Structure

```text
cpp-demo-project/
├── .github/                     # GitHub Actions configuration
│   └── workflows/               # GitHub Actions workflows
├── .vscode/                     # VS Code configuration
│   ├── launch.json              # VS Code launch configuration
│   ├── settings.json            # VS Code settings
│   └── tasks.json               # VS Code tasks
├── build/                       # Build output (generated by CMake)
├── cmake/                       # CMake modules and utilities
│   ├── CompilerWarnings.cmake   # Compiler warning configuration
│   ├── Dependencies.cmake       # External dependencies configuration
│   ├── ModuleHelpers.cmake      # Module helper functions
│   ├── StaticAnalysis.cmake     # Static analysis tools
│   ├── config.cmake.in          # Package configuration
│   └── README.md                # CMake modules documentation
├── include/                     # Public C++ header files
│   ├── algorithms/              # STL algorithm wrappers with concepts
│   ├── concepts/                # Custom concepts and type traits
│   ├── containers/              # Modern container wrapper with ranges support
│   ├── exceptions/              # Custom exception hierarchy and Result type
│   ├── memory/                  # Resource management and RAII utilities
│   ├── random/                  # Type-safe random number generation
│   ├── shapes/                  # Polymorphic shapes with factory functions
│   └── timing/                  # Performance measurement and benchmarking
├── src/                         # C++ source implementation files
│   ├── CMakeLists.txt           # Components configuration
│   └── [module]/                # C++ source implementation for the component
├── examples/                    # C++ usage examples and demonstrations
│   └── [module]_example.cpp     # C++ usage examples for the component
├── tests/                       # C++ test suite using Catch2 v3
│   └── test_[module].cpp        # C++ unit tests for the component
├── binding/                     # pybind11 C++ binding files
│   ├── CMakeLists.txt           # Python bindings configuration
│   ├── cpp_features.cpp         # Main pybind11 module
│   └── [module]_binding.cpp     # Individual module bindings
├── python/                      # Python wrapper modules (see python/README.md for more details)
├── .clang-format                # clang-format configuration (for C++ code formatting)
├── .clang-tidy                  # clang-tidy configuration (for static analysis)
├── .clangd                      # clangd configuration (for code completion)
├── .gersemirc                   # gersemi configuration (for CMake code formatting)
├── .markdownlint.yaml           # markdownlint configuration (for Markdown formatting)
├── .pre-commit-config.yaml      # pre-commit hooks configuration
├── CMakeLists.txt               # Main project configuration
├── CMakePresets.json            # CMake presets configuration
├── LICENSE                      # MIT License
└── README.md                    # This file
```

## 🔧 Components Overview

| Component      | Purpose                | Key Features                                     |
| -------------- | ---------------------- | ------------------------------------------------ |
| **Algorithms** | STL algorithm wrappers | Concepts, ranges, type safety                    |
| **Concepts**   | Type constraints       | Arithmetic, container, utility concepts          |
| **Containers** | Enhanced containers    | Modern wrapper, ranges, `std::expected`          |
| **Exceptions** | Error handling         | Custom hierarchy, `Result` type, source location |
| **Memory**     | Resource management    | RAII, smart pointers, automatic cleanup          |
| **Random**     | Random generation      | Type-safe, multiple distributions, ranges        |
| **Shapes**     | OOP demonstration      | Polymorphism, factory functions, comparisons     |
| **Timing**     | Performance tools      | Benchmarking, scoped timing, statistics          |

## 💻 Development Notes

### Code Quality

- **Consistent formatting**
  - Uses `clang-format` for C++ code
  - Uses `gersemi` for CMake files
  - Uses `ruff` for Python code
  - Uses `markdownlint-cli2` for Markdown files
- **Static analysis**
  - Uses `clang-tidy` and `cppcheck` for C++ code
  - Uses `ruff`, `mypy` and `bandit` for Python code
- **Modern practices**
  - Follows Core Guidelines and modern C++23 best practices
  - Follows PEP 8 and modern Python conventions
- **Comprehensive documentation**
  - Doxygen-style documentation for C++ code
  - Numpy-style docstrings for Python code

### Pre-commit Configuration

The project includes a comprehensive pre-commit setup [`.pre-commit-config.yaml`](.pre-commit-config.yaml).

**Benefits:**

- Consistent code formatting across the entire project
- Automatic detection of common issues before commit
- Enforced coding standards for all contributors
- Integration with modern formatting tools

## 📄 License

This project is licensed under the **MIT License** - see the [`LICENSE`](LICENSE) file for details.
