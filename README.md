# Modern C++ Features Demo Project

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org)
[![CMake](https://img.shields.io/badge/CMake-3.23+-blue.svg)](https://cmake.org)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A comprehensive demonstration project showcasing modern C++23 features, best practices, and advanced programming techniques.
This project serves as both a learning resource and a reference implementation for modern C++ development.

## 🚀 Features

- **Modern C++23 Standards**: Demonstrates the latest C++ features including concepts, ranges, expected, and more
- **Modular Architecture**: Clean, scalable project structure with separate components
- **Comprehensive Testing**: Full test coverage using Catch2 v3 framework
- **Advanced CMake**: Modern CMake build system with modular configuration
- **Memory Safety**: Smart pointer demonstrations and RAII principles
- **Error Handling**: Modern error handling with `std::expected`
- **STL Algorithms**: Advanced usage of standard library algorithms and ranges
- **Design Patterns**: Implementation of common design patterns in modern C++

## 📋 Table of Contents

- [🚀 Features](#-features)
- [📋 Table of Contents](#-table-of-contents)
- [🛠️ Installation](#️-installation)
  - [Prerequisites](#prerequisites)
  - [Quick Start](#quick-start)
    - [Clone the repository](#clone-the-repository)
    - [Build the project](#build-the-project)
    - [Run the demo](#run-the-demo)
    - [Run tests](#run-tests)
  - [Build Options](#build-options)
- [🎯 Usage](#-usage)
  - [Running the Demo](#running-the-demo)
  - [Using Individual Components](#using-individual-components)
- [📁 Project Structure](#-project-structure)
- [🧩 Components](#-components)
  - [🔷 Algorithms Module](#-algorithms-module)
  - [🔷 Concepts Module](#-concepts-module)
  - [🔷 Containers Module](#-containers-module)
  - [🔷 Memory Module](#-memory-module)
- [🧪 Testing](#-testing)
- [💻 Development Notes](#-development-notes)
  - [Code Style](#code-style)
- [📄 License](#-license)

## 🛠️ Installation

### Prerequisites

- **C++23 compatible compiler** (GCC 13+ / Clang 16+)
- **CMake 3.23** or higher

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

#### Run the demo

```bash
./build/src/demo
```

#### Run tests

```bash
ctest --test-dir build --verbose
```

### Build Options

See [`cmake/README.md`](cmake/README.md#options) for available build options.

## 🎯 Usage

### Running the Demo

The main executable demonstrates various C++23 features:

```bash
./build/src/demo
```

Sample Output:

```text
=== C++23 Feature Demonstration Program ===
Testing various modern C++ features and libraries.

=== Shape Demonstrations ===
Drawing Circle
Circle area: 78.54

Drawing Rectangle
Rectangle area: 24.00

=== Container Demonstrations ===
Container contents: 1 2 3 4 5

=== Smart Pointer Demonstrations ===
[Smart pointer examples...]

Program completed successfully!
```

### Using Individual Components

Each component can be used independently:

```cpp
#include "containers/container.hpp"
#include "shapes/circle.hpp"

using cpp_features::containers;
using cpp_features::shapes;

int main() {
    // Use modern containers
    containers::Container<int> numbers{1, 2, 3, 4, 5};
    numbers.Display();

    // Create and use shapes
    auto circle = shapes::CreateCircle(5.0);
    if (auto area = circle->GetArea(); area) {
        std::cout << "Area: " << area.value() << std::endl;
    }

    return 0;
}
```

## 📁 Project Structure

```text
cpp-demo-project/
├── CMakeLists.txt              # Main project configuration
├── README.md                   # This file
├── LICENSE                     # MIT License
├── cmake/                      # CMake modules and utilities
│   ├── CompilerWarnings.cmake  # Compiler warning configuration
│   ├── StaticAnalysis.cmake    # Static analysis tools
│   └── demo-config.cmake.in    # Package configuration
├── include/                    # Public header files
│   ├── algorithms/             # STL algorithms and ranges
│   ├── concepts/               # C++20/23 concepts
│   ├── containers/             # Modern container implementations
│   ├── exceptions/             # Custom exception classes
│   ├── memory/                 # Smart pointer utilities
│   ├── random/                 # Random number generation
│   ├── shapes/                 # Geometric shapes with polymorphism
│   ├── strings/                # String manipulation utilities
│   └── timing/                 # Performance timing utilities
├── src/                        # Source implementation files
│   ├── [mirrors include structure]
│   └── main.cpp                # Main demonstration program
├── tests/                      # Test suite using Catch2
│   ├── test_concepts.cpp
│   ├── test_containers.cpp
│   ├── test_shapes.cpp
│   └── test_main.cpp
└── examples/                   # Usage examples (optional)
```

## 🧩 Components

### 🔷 Algorithms Module

STL algorithms and ranges:

- **Modern algorithms** with execution policies
- **Range adaptors** and views
- **Custom algorithms** with concepts
- **Functional programming** patterns

### 🔷 Concepts Module

C++20/23 concepts demonstrations:

- **Arithmetic concepts** for type constraints
- **Custom concepts** for domain-specific requirements
- **Template specialization** with concept requirements
- **Type trait concepts** for advanced metaprogramming

### 🔷 Containers Module

Modern container implementations showcasing:

- **Template metaprogramming** with concepts
- **Range-based algorithms** and views
- **Perfect forwarding** and move semantics
- **Custom iterators** and STL compatibility

### 🔷 Memory Module

Smart pointer and memory management:

- **RAII principles** implementation
- **Custom deleters** and specialized smart pointers
- **Memory safety** best practices
- **Resource management** patterns

## 🧪 Testing

The project includes comprehensive tests using **Catch2 v3**:

```bash
# Run all tests
ctest

# Run tests with verbose output
ctest --verbose

# Run specific test suite
ctest -R "Shape tests"

# Generate test coverage report (if configured)
make coverage
```

**Test Coverage:**

- ✅ Shape operations and polymorphism
- ✅ Container functionality and edge cases
- ✅ Concept validation and constraints
- ✅ Memory management and smart pointers
- ✅ Algorithm correctness and performance
- ✅ Error handling and exception safety

## 💻 Development Notes

### Code Style

This project follows the **Google C++ Style Guide** with some modifications:

- Use `.clang-format` for automatic formatting
- Enable `.clang-tidy` for static analysis
- Follow modern C++ best practices (Core Guidelines)
- Use meaningful names and comprehensive documentation

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
