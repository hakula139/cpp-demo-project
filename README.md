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
- **Python Bindings**: Complete pybind11 integration with modern Python 3.13 features

### Code Quality & Development

- **Static Analysis**: Integrated `clang-tidy` and `cppcheck` for code quality
- **Automatic Formatting**: Pre-commit hooks with `clang-format` and `gersemi`
- **Documentation**: Comprehensive Doxygen-style documentation
- **CI/CD Ready**: Modern CMake configuration for easy integration

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
    - [Run individual examples](#run-individual-examples)
    - [Run tests](#run-tests)
    - [Build Python bindings (optional)](#build-python-bindings-optional)
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
  - [Python Usage](#python-usage)
- [📁 Project Structure](#-project-structure)
- [🔧 Components Overview](#-components-overview)
- [💻 Development Notes](#-development-notes)
  - [Code Style](#code-style)
  - [Pre-commit Configuration](#pre-commit-configuration)
  - [Python Development](#python-development)
    - [Testing](#testing)
    - [Examples and Documentation](#examples-and-documentation)
    - [Code Quality](#code-quality)
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
- **Python 3.13+** (optional, for Python bindings)

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

#### Run individual examples

```bash
./build/release/examples/algorithms_example
./build/release/examples/containers_example
./build/release/examples/exceptions_example
./build/release/examples/memory_example
./build/release/examples/random_example
./build/release/examples/shapes_example
```

#### Run tests

```bash
ctest --preset release
```

#### Build Python bindings (optional)

```bash
cmake --preset release -DBUILD_PYTHON_BINDINGS=ON
cmake --build --preset release

# Test Python bindings (optional)
cd python_tests
python -m pytest --verbose

# Run Python examples (optional)
python python_examples/basic_usage.py
python python_examples/advanced_usage.py
```

### CMake Presets

This project uses CMake presets for streamlined build configuration.

#### Configure Presets

| Preset             | Description                                                       |
| ------------------ | ----------------------------------------------------------------- |
| `debug`            | Debug build with symbols and no optimization                      |
| `release`          | Release build with full optimization                              |
| `debug-no-tests`   | Debug build without tests and examples (faster config)            |
| `release-no-tests` | Release build without tests and examples (faster config)          |
| `debug-strict`     | Debug build with static analysis and warnings treated as errors   |
| `release-strict`   | Release build with static analysis and warnings treated as errors |

#### Build & Test Presets

Each configure preset has corresponding build and test presets with the same names.

#### Workflow Presets

| Preset             | Description                                         |
| ------------------ | --------------------------------------------------- |
| `debug-workflow`   | Complete debug workflow: configure + build + test   |
| `release-workflow` | Complete release workflow: configure + build + test |

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

| Option                  | Description                        | Default               |
|-------------------------|------------------------------------|-----------------------|
| `BUILD_TESTS`           | Build test suite                   | `ON` (main project)   |
| `BUILD_EXAMPLES`        | Build example applications         | `ON` (main project)   |
| `BUILD_PYTHON_BINDINGS`| Build Python bindings with pybind11| `OFF`                |
| `ENABLE_WARNINGS`       | Enable compiler warnings           | `ON`                  |

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

  SortContainer(numbers);
  std::println("Sorted: {}", numbers);

  // Type-safe factory functions with validation
  auto circle = CreateCircle(5.0);
  std::println("Area: {:.2f}, Perimeter: {:.2f}", circle->GetArea(), circle->GetPerimeter());

  return 0;
}
```

### Python Usage

```python
from cpp_features import shapes, containers, algorithms

# Create and use shapes
circle = shapes.create_shape("circle", 5.0)
rectangle = shapes.create_shape("rectangle", 4.0, 3.0)

print(f"Circle area: {circle.get_area():.2f}")
print(f"Rectangle area: {rectangle.get_area():.2f}")

# Use enhanced containers
container = containers.create_container([1, 3, 2, 5, 4])
algorithms.sort_inplace(container)
print(f"Sorted: {list(container)}")

# Functional programming with modern Python features
from cpp_features.algorithms import functional_chain

result = (functional_chain([1, 2, 3, 4, 5, 6])
          .filter(lambda x: x % 2 == 0)
          .map(lambda x: x * x)
          .collect())
print(f"Even squares: {result}")  # [4, 16, 36]
```

## 📁 Project Structure

```text
cpp-demo-project/
├── .github/                    # GitHub Actions configuration
│   └── workflows/              # GitHub Actions workflows
├── .vscode/                    # VS Code configuration
│   ├── launch.json             # VS Code launch configuration
│   ├── settings.json           # VS Code settings
│   └── tasks.json              # VS Code tasks
├── binding/                    # Python bindings using pybind11
│   ├── CMakeLists.txt          # Python bindings build configuration
│   ├── cpp_features.cpp        # Main pybind11 module
│   ├── shapes_binding.cpp      # Shapes module bindings
│   ├── containers_binding.cpp  # Containers module bindings
│   ├── algorithms_binding.cpp  # Algorithms module bindings
│   ├── exceptions_binding.cpp  # Exceptions module bindings
│   ├── memory_binding.cpp      # Memory module bindings
│   ├── timing_binding.cpp      # Timing module bindings
│   └── random_binding.cpp      # Random module bindings
├── build/                      # Build output (generated by CMake)
│   ├── debug/                  # Debug build output
│   ├── release/                # Release build output
│   └── [other presets]
├── cmake/                      # CMake modules and utilities
│   ├── CompilerWarnings.cmake  # Compiler warning configuration
│   ├── Dependencies.cmake      # External dependencies configuration
│   ├── ModuleHelpers.cmake     # Module helper functions
│   ├── StaticAnalysis.cmake    # Static analysis tools
│   ├── config.cmake.in         # Package configuration
│   └── README.md               # CMake modules documentation
├── include/                    # Public header files
│   ├── algorithms/             # STL algorithm wrappers with concepts
│   ├── concepts/               # Custom concepts and type traits
│   ├── containers/             # Modern container wrapper with ranges support
│   ├── exceptions/             # Custom exception hierarchy and Result type
│   ├── memory/                 # Resource management and RAII utilities
│   ├── random/                 # Type-safe random number generation
│   ├── shapes/                 # Polymorphic shapes with factory functions
│   └── timing/                 # Performance measurement and benchmarking
├── python/                     # Python wrapper modules
│   ├── __init__.py             # Package initialization
│   ├── shapes.py               # Enhanced shapes module with Python 3.13 features
│   ├── containers.py           # Enhanced containers module
│   ├── algorithms.py           # Functional programming utilities
│   ├── exceptions.py           # Result types and error handling
│   ├── memory.py               # RAII and resource management
│   ├── timing.py               # Benchmarking and timing utilities
│   └── random.py               # Enhanced random number generation
├── src/                        # Source implementation files
│   ├── CMakeLists.txt          # Components configuration
│   └── [mirrors include structure]
├── binding/                    # pybind11 C++ binding files
│   ├── CMakeLists.txt          # Python bindings configuration
│   ├── cpp_features.cpp        # Main pybind11 module
│   └── [module]_binding.cpp    # Individual module bindings
├── python/                     # Python wrapper modules
│   ├── __init__.py             # Package initialization
│   ├── shapes.py               # Enhanced shape functionality
│   ├── containers.py           # Pythonic container wrappers
│   ├── algorithms.py           # Functional programming utilities
│   ├── exceptions.py           # Result types and error handling
│   ├── memory.py               # RAII and resource management
│   ├── timing.py               # High-resolution timing utilities
│   └── random.py               # Enhanced random number generation
├── python_tests/               # Python test suite using pytest
│   ├── __init__.py             # Test package initialization
│   ├── conftest.py             # pytest configuration and fixtures
│   └── test_[module].py        # Comprehensive module tests
├── python_examples/            # Python usage examples
│   ├── README.md               # Examples documentation
│   ├── basic_usage.py          # Fundamental usage patterns
│   └── advanced_usage.py       # Advanced and real-world examples
├── examples/                   # C++ usage examples and demonstrations
├── tests/                      # C++ test suite using Catch2 v3
├── .clang-format               # clang-format configuration (for C++ code formatting)
├── .clang-tidy                 # clang-tidy configuration (for static analysis)
├── .clangd                     # clangd configuration (for code completion)
├── .gersemirc                  # gersemi configuration (for CMake code formatting)
├── .markdownlint.yaml          # markdownlint configuration (for Markdown formatting)
├── .pre-commit-config.yaml     # pre-commit hooks configuration
├── CMakeLists.txt              # Main project configuration
├── CMakePresets.json           # CMake presets configuration
├── LICENSE                     # MIT License
└── README.md                   # This file
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

### Code Style

This project follows the **Google C++ Style Guide** with some modifications:

- **Automatic formatting**: Uses `.clang-format` for C++ code and `gersemi` for CMake files
- **Static analysis**: Enabled with `.clang-tidy` for code quality checks
- **Modern C++ practices**: Follows Core Guidelines and C++23 best practices
- **Documentation**: Comprehensive Doxygen-style documentation

### Pre-commit Configuration

The project includes a comprehensive pre-commit setup (`.pre-commit-config.yaml`):

```yaml
repos:
  # Standard pre-commit hooks
  - repo: https://github.com/pre-commit/pre-commit-hooks
    rev: v5.0.0
    hooks:
      - id: check-added-large-files
      - id: check-yaml
      - id: end-of-file-fixer
      - id: trailing-whitespace

  # C++ formatting with clang-format
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v20.1.7
    hooks:
      - id: clang-format
        files: \.(cpp|hpp|h)$

  # CMake formatting with gersemi
  - repo: https://github.com/BlankSpruce/gersemi
    rev: 0.19.3
    hooks:
      - id: gersemi
        files: (\.cmake|CMakeLists\.txt)$

  # Markdown linting and formatting
  - repo: https://github.com/DavidAnson/markdownlint-cli2
    rev: v0.18.1
    hooks:
      - id: markdownlint-cli2
        args: ['--config', '.markdownlint.yaml']
```

**Benefits:**

- Consistent code formatting across the entire project
- Automatic detection of common issues before commit
- Enforced coding standards for all contributors
- Integration with modern formatting tools

### Python Development

The project includes comprehensive Python bindings and tooling:

#### Testing

- **pytest-based test suite**: Located in `python_tests/` with comprehensive coverage
- **C++ test pattern adoption**: Python tests mirror C++ test structure and patterns
- **Fixtures and configuration**: Shared test utilities in `conftest.py`
- **Type safety testing**: Verifies type annotations and generic behavior

#### Examples and Documentation

- **Basic examples**: `python_examples/basic_usage.py` demonstrates fundamental usage
- **Advanced examples**: `python_examples/advanced_usage.py` shows real-world patterns
- **Comprehensive documentation**: Numpy-style docstrings throughout
- **Modern Python features**: Extensive use of Python 3.13 features like pattern matching

#### Code Quality

- **Black formatting**: Consistent Python code style
- **Type annotations**: Full type hint coverage with modern typing
- **Error handling**: Result types for functional error management
- **Performance integration**: Built-in timing and benchmarking utilities

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
