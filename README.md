# C++23 Features Demo Project

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.23+-blue.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A comprehensive demonstration project showcasing modern C++23 features, best practices, and advanced programming techniques. This project serves as both a learning resource and a reference implementation for modern C++ development.

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
- [🏃‍♂️ Quick Start](#️-quick-start)
- [🛠️ Installation](#️-installation)
  - [Prerequisites](#prerequisites)
  - [Build Instructions](#build-instructions)
  - [Build Options](#build-options)
- [🎯 Usage](#-usage)
  - [Running the Demo](#running-the-demo)
  - [Using Individual Components](#using-individual-components)
- [📁 Project Structure](#-project-structure)
- [🧩 Components](#-components)
  - [🔷 Shapes Module](#-shapes-module)
  - [🔷 Containers Module](#-containers-module)
  - [🔷 Concepts Module](#-concepts-module)
  - [🔷 Memory Module](#-memory-module)
  - [🔷 Algorithms Module](#-algorithms-module)
- [🧪 Testing](#-testing)
- [💻 Development Notes](#-development-notes)
  - [Code Style](#code-style)
- [📚 Learning Resources](#-learning-resources)
- [📄 License](#-license)
- [🙏 Acknowledgments](#-acknowledgments)

## 🏃‍♂️ Quick Start

```bash
# Clone the repository
git clone <repository-url>
cd cpp-test-env

# Build the project
mkdir build && cd build
cmake ..
make -j4

# Run the demo
./src/demo

# Run tests
ctest --verbose
```

## 🛠️ Installation

### Prerequisites

- **C++23 compatible compiler** (GCC 13+, Clang 16+, or MSVC 2022+)
- **CMake 3.23** or higher
- **Git** for cloning the repository

### Build Instructions

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd cpp-test-env
   ```

2. **Create build directory**:
   ```bash
   mkdir build && cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   make -j$(nproc)
   ```

### Build Options

The project supports several build configuration options:

```bash
# Build with tests (default)
cmake .. -DDEMO_BUILD_TESTS=ON

# Build with examples
cmake .. -DDEMO_BUILD_EXAMPLES=ON

# Enable static analysis
cmake .. -DENABLE_CLANG_TIDY=ON -DENABLE_CPPCHECK=ON

# Enable all warnings as errors
cmake .. -DWARNINGS_AS_ERRORS=ON

# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release
```

## 🎯 Usage

### Running the Demo

The main executable demonstrates various C++23 features:

```bash
./src/demo
```

**Sample Output:**
```
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
#include "shapes/circle.hpp"
#include "containers/container.hpp"

using namespace cpp_features;

int main() {
    // Create and use shapes
    auto circle = shapes::CreateCircle(5.0);
    if (auto area = circle->GetArea(); area) {
        std::cout << "Area: " << area.value() << std::endl;
    }

    // Use modern containers
    containers::Container<int> numbers{1, 2, 3, 4, 5};
    numbers.Display();

    return 0;
}
```

## 📁 Project Structure

```
cpp-test-env/
├── CMakeLists.txt              # Main project configuration
├── README.md                   # This file
├── LICENSE                     # MIT License
├── PROJECT_STRUCTURE.md        # Detailed architecture documentation
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
│   ├── test_shapes.cpp
│   ├── test_containers.cpp
│   ├── test_concepts.cpp
│   └── test_main.cpp
└── examples/                   # Usage examples (optional)
```

## 🧩 Components

### 🔷 Shapes Module
Demonstrates object-oriented programming with modern C++ features:
- **Polymorphic base class** with virtual methods
- **Factory functions** using concepts and perfect forwarding
- **Error handling** with `std::expected<T, E>`
- **RAII principles** and smart pointer usage

### 🔷 Containers Module
Modern container implementations showcasing:
- **Template metaprogramming** with concepts
- **Range-based algorithms** and views
- **Perfect forwarding** and move semantics
- **Custom iterators** and STL compatibility

### 🔷 Concepts Module
C++20/23 concepts demonstrations:
- **Arithmetic concepts** for type constraints
- **Custom concepts** for domain-specific requirements
- **Template specialization** with concept requirements
- **Type trait concepts** for advanced metaprogramming

### 🔷 Memory Module
Smart pointer and memory management:
- **RAII principles** implementation
- **Custom deleters** and specialized smart pointers
- **Memory safety** best practices
- **Resource management** patterns

### 🔷 Algorithms Module
STL algorithms and ranges:
- **Modern algorithms** with execution policies
- **Range adaptors** and views
- **Custom algorithms** with concepts
- **Functional programming** patterns

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

This is a personal learning and demonstration project. Key development practices:

### Code Style

This project follows the **Google C++ Style Guide** with some modifications:
- Use `.clang-format` for automatic formatting
- Enable `.clang-tidy` for static analysis
- Follow modern C++ best practices (Core Guidelines)
- Use meaningful names and comprehensive documentation

## 📚 Learning Resources

This project demonstrates concepts from:
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)
- [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)
- [C++23 Standard Features](https://en.cppreference.com/w/cpp/23)

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **C++ Standards Committee** for continuous language evolution
- **Catch2 Framework** for excellent testing capabilities
- **CMake Community** for modern build system practices
- **Open Source Contributors** who inspire better software development

---

**This is a personal demonstration project for learning modern C++ features.**
