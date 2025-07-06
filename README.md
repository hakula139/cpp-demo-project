# Modern C++ Features Demo Project

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com)
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
  - [Pre-commit Setup (Recommended)](#pre-commit-setup-recommended)
    - [Install pre-commit](#install-pre-commit)
    - [Install the git hooks](#install-the-git-hooks)
    - [Run on all files (optional)](#run-on-all-files-optional)
    - [What the hooks do](#what-the-hooks-do)
  - [Build Options](#build-options)
- [🎯 Usage](#-usage)
- [📁 Project Structure](#-project-structure)
- [🔧 Components Overview](#-components-overview)
- [💻 Development Notes](#-development-notes)
  - [Code Style](#code-style)
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

- **C++23 compatible compiler** (GCC 13+ / Clang 16+)
- **CMake 3.23+**

### Quick Start

#### Clone the repository

```bash
git clone https://github.com/hakula139/cpp-demo-project
cd cpp-demo-project
```

#### Build the project

```bash
cmake -B build
cmake --build build --parallel $(nproc)
```

#### Run individual examples

```bash
./build/examples/algorithms_example
./build/examples/containers_example
./build/examples/exceptions_example
./build/examples/memory_example
./build/examples/random_example
./build/examples/shapes_example
```

#### Run tests

```bash
ctest --test-dir build --verbose
```

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

The hooks will run automatically on `git commit` and prevent commits with formatting issues.

### Build Options

See [`cmake/README.md`](cmake/README.md#options) for available build options.

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

## 📁 Project Structure

```text
cpp-demo-project/
├── CMakeLists.txt              # Main project configuration
├── README.md                   # This file
├── LICENSE                     # MIT License
├── .clang-format               # clang-format configuration (for C++ code formatting)
├── .clang-tidy                 # clang-tidy configuration (for static analysis)
├── .gersemirc                  # gersemi configuration (for CMake code formatting)
├── .markdownlint.yaml          # markdownlint configuration (for Markdown formatting)
├── .pre-commit-config.yaml     # pre-commit hooks configuration
├── cmake/                      # CMake modules and utilities
│   ├── ModuleHelpers.cmake     # Module helper functions
│   ├── Dependencies.cmake      # External dependencies configuration
│   ├── CompilerWarnings.cmake  # Compiler warning configuration
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
├── src/                        # Source implementation files
│   ├── CMakeLists.txt          # Components configuration
│   └── [mirrors include structure]
├── examples/                   # Usage examples and demonstrations
└── tests/                      # Test suite using Catch2 v3
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

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
