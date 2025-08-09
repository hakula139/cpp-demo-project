# Python Examples

This directory contains comprehensive examples demonstrating the Python bindings for the C++ features library.

## Examples Overview

### `basic_usage.py`

Demonstrates fundamental usage of each module:

- **Shapes**: Creating circles, rectangles, and squares with analysis
- **Containers**: Type-safe containers with filtering and transformation
- **Algorithms**: Sorting, counting, transforming, and functional chains
- **Memory**: RAII-style resource management with context managers
- **Exceptions**: Result types for safe error handling
- **Timing**: High-resolution timing and benchmarking
- **Random**: Type-safe random number generation with distributions
- **Integration**: Cross-module usage patterns

### `advanced_usage.py`

Shows advanced patterns and real-world use cases:

- **ShapeAnalyzer**: Performance-monitored shape collection analysis
- **DataProcessor**: Complex data processing pipelines with error handling
- **PerformanceBenchmarkSuite**: Comprehensive performance testing
- **Monte Carlo π Estimation**: Statistical computation example
- **Functional Programming**: Advanced chain operations and pipelines
- **Real-World Simulation**: Virtual environment analysis

## Running the Examples

### Prerequisites

1. Build the C++ library with Python bindings enabled:

   ```bash
   cmake -DBUILD_PYTHON_BINDINGS=ON -B build
   cmake --build build
   ```

2. Ensure the built Python extension is available (typically in `build/binding/`)

3. Install pytest for running tests:

   ```bash
   pip install pytest
   ```

### Running Examples

```bash
# Run basic usage examples
python python_examples/basic_usage.py

# Run advanced usage examples
python python_examples/advanced_usage.py
```

## Key Features Demonstrated

### Python 3.13 Features

- **Pattern Matching**: Used in factory functions and type dispatch
- **Enhanced Type Hints**: Modern typing with generics and unions
- **Dataclasses**: Immutable data structures for metrics
- **Context Managers**: RAII-like resource management
- **Enum Classes**: Type-safe enumerations

### Modern Python Patterns

- **Functional Programming**: Method chaining and pipeline composition
- **Error Handling**: Result types instead of exceptions
- **Performance Monitoring**: Integrated timing and benchmarking
- **Type Safety**: Generic containers and proper type annotations
- **Resource Management**: Automatic cleanup with context managers

### C++ Integration

- **Zero-Copy Operations**: Direct C++ container access where possible
- **Exception Translation**: C++ exceptions to Python Result types
- **Memory Safety**: RAII patterns adapted to Python
- **Performance**: Leveraging C++ performance for computationally intensive operations

## Code Style

The examples follow modern Python conventions:

- **Black formatting**: Consistent code style
- **Single quotes**: For string literals
- **Numpy-style docstrings**: Comprehensive documentation
- **Type hints**: Full type annotation coverage
- **Error handling**: Graceful error management with Result types

## Example Output

When run successfully, the examples demonstrate:

- Shape creation and analysis with geometric calculations
- Container operations with filtering and transformation
- Algorithm performance on different data sets
- Memory management with automatic cleanup
- Error handling with safe operations
- High-resolution timing measurements
- Random number generation with statistical properties
- Cross-module integration patterns

The advanced examples additionally show:

- Performance benchmarking across different scenarios
- Real-world simulation with complex data processing
- Statistical computation (Monte Carlo methods)
- Advanced functional programming patterns
- Comprehensive error handling strategies
