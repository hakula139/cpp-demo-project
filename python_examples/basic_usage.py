#!/usr/bin/env python3
"""Basic usage examples for the C++ Python bindings.

This example demonstrates the fundamental features of each module.
"""

import sys
from pathlib import Path

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'python'))

from python import (algorithms, containers, exceptions, memory, random, shapes,
                    timing)


def shapes_example():
    """Demonstrate shapes module functionality."""
    print('=== Shapes Module ===')

    # Create different shapes using factory functions
    circle = shapes.create_shape('circle', 5.0)
    rectangle = shapes.create_shape('rectangle', 4.0, 3.0)
    square = shapes.create_shape('square', 6.0)

    print(f'Circle: area={circle.get_area():.2f}, perimeter={circle.get_perimeter():.2f}')
    print(f'Rectangle: area={rectangle.get_area():.2f}, perimeter={rectangle.get_perimeter():.2f}')
    print(f'Square: area={square.get_area():.2f}, perimeter={square.get_perimeter():.2f}')

    # Analyze shapes
    circle_metrics = shapes.analyze_shape(circle)
    print(f'Circle aspect ratio: {circle_metrics.aspect_ratio:.4f}')

    # Compare multiple shapes
    comparison = shapes.compare_shapes(circle, rectangle, square)
    print(f'Total area of all shapes: {comparison["total_area"]:.2f}')
    print(f'Largest shape by area: {comparison["largest_by_area"].name}')

    print()


def containers_example():
    """Demonstrate containers module functionality."""
    print('=== Containers Module ===')

    # Create containers with different types
    int_container = containers.create_container([1, 3, 2, 5, 4])
    str_container = containers.Container(str, ['apple', 'banana', 'cherry'])

    print(f'Integer container: {list(int_container)}')
    print(f'String container: {list(str_container)}')

    # Add and remove items
    int_container.add(6)
    removed = int_container.remove(3)
    print(f'After adding 6 and removing 3 (removed {removed}): {list(int_container)}')

    # Filter and transform
    even_numbers = int_container.filter(lambda x: x % 2 == 0)
    squared = int_container.transform(lambda x: x * x)

    print(f'Even numbers: {even_numbers}')
    print(f'Squared: {squared}')

    # String operations
    long_words = str_container.filter(lambda s: len(s) > 5)
    lengths = str_container.transform(len)

    print(f'Long words: {long_words}')
    print(f'Word lengths: {lengths}')

    print()


def algorithms_example():
    """Demonstrate algorithms module functionality."""
    print('=== Algorithms Module ===')

    # Sorting
    data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
    print(f'Original data: {data}')

    algorithms.sort_inplace(data)
    print(f'Sorted data: {data}')

    # Counting and transforming
    count_even = algorithms.count_if(data, lambda x: x % 2 == 0)
    doubled = algorithms.transform(data, lambda x: x * 2)
    min_val, max_val = algorithms.find_min_max(data)

    print(f'Even numbers count: {count_even}')
    print(f'Doubled values: {doubled}')
    print(f'Min: {min_val}, Max: {max_val}')

    # Functional chain
    result = (algorithms.functional_chain([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
              .filter(lambda x: x % 2 == 0)
              .map(lambda x: x * x)
              .take(3)
              .collect())

    print(f'Functional chain result: {result}')

    # Pipeline
    process = algorithms.pipeline(
        lambda lst: [x for x in lst if x > 3],
        lambda lst: [x * 2 for x in lst],
        sum
    )

    pipeline_result = process([1, 2, 3, 4, 5, 6])
    print(f'Pipeline result: {pipeline_result}')

    print()


def memory_example():
    """Demonstrate memory module functionality."""
    print('=== Memory Module ===')

    cleanup_log = []

    def cleanup_function(name: str):
        def cleanup():
            cleanup_log.append(f'Cleaned up {name}')
        return cleanup

    # Using resource manager as context manager
    with memory.managed_resources() as manager:
        # Create shapes through resource manager
        circle = manager.create_circle(3.0)
        rectangle = manager.create_rectangle(4.0, 5.0)

        print(f'Created circle: area={circle.get_area():.2f}')
        print(f'Created rectangle: area={rectangle.get_area():.2f}')

        # Register cleanup functions
        manager.register_cleanup(cleanup_function('circle'))
        manager.register_cleanup(cleanup_function('rectangle'))

    print('Cleanup log:', cleanup_log)

    # Using scoped resources
    cleanup_log.clear()

    with memory.scoped_resource('important_data', cleanup_function('important_data')) as data:
        print(f'Using scoped resource: {data}')

    print('Scoped cleanup log:', cleanup_log)

    print()


def exceptions_example():
    """Demonstrate exceptions module functionality."""
    print('=== Exceptions Module ===')

    # Safe operations that return Results
    safe_result = exceptions.safe_divide(10.0, 2.0)
    error_result = exceptions.safe_divide(10.0, 0.0)

    print(f'Safe division result: {safe_result.unwrap() if safe_result.is_ok else "Error"}')
    print(f'Division by zero: {"Success" if error_result.is_ok else "Error (expected)"}')

    # Result chaining
    sqrt_result = exceptions.safe_sqrt(16.0)
    chained_result = sqrt_result.map(lambda x: x * 2)  # Square root then double

    print(f'Sqrt(16) * 2 = {chained_result.unwrap()}')

    # Error handling with unwrap_or
    safe_value = error_result.unwrap_or(-1.0)
    print(f'Error result with default: {safe_value}')

    # Chaining operations
    def add_one(x: float) -> exceptions.Result[float]:
        return exceptions.Result.ok(x + 1.0)

    def multiply_by_three(x: float) -> exceptions.Result[float]:
        return exceptions.Result.ok(x * 3.0)

    chain = exceptions.chain_operations(add_one, multiply_by_three)
    chain_result = chain(5.0)  # (5 + 1) * 3 = 18

    print(f'Chained operations result: {chain_result.unwrap()}')

    print()


def timing_example():
    """Demonstrate timing module functionality."""
    print('=== Timing Module ===')

    # Basic timer usage
    with timing.Timer() as timer:
        # Simulate some work
        sum(i * i for i in range(1000))

    print(f'Operation took: {timer.elapsed_string}')

    # Measure time with context manager
    with timing.measure_time('computation'):
        result = sum(i for i in range(10000))

    print(f'Computation result: {result}')

    # Benchmark function
    def test_function():
        return sorted([i for i in range(100, 0, -1)])

    stats = timing.benchmark_function(test_function, iterations=5)
    print(f'Benchmark mean time: {stats["human_readable"]["mean"]}')

    # Time a single function call
    elapsed = timing.time_function(lambda: [x**2 for x in range(50)])
    print(f'Function execution time: {elapsed}')

    print()


def random_example():
    """Demonstrate random module functionality."""
    print('=== Random Module ===')

    # Create random generator
    gen = random.RandomGenerator(seed=42)  # Seeded for reproducible results

    # Generate different types of random values
    print(f'Random integer (1-100): {gen.randint(1, 100)}')
    print(f'Random float (0-1): {gen.random():.4f}')
    print(f'Random uniform (5-10): {gen.uniform(5.0, 10.0):.4f}')
    print(f'Random boolean: {gen.choice()}')
    print(f'Random normal: {gen.normal(mean=0.0, stddev=1.0):.4f}')

    # Generate vectors
    int_vector = gen.integers(1, 10, 5)
    float_vector = gen.floats(0.0, 1.0, 5)

    print(f'Random integers: {int_vector}')
    print(f'Random floats: {float_vector}')

    # Shuffle and sample
    data = list(range(1, 11))
    print(f'Original data: {data}')

    random.shuffle(data)
    print(f'Shuffled data: {data}')

    sample = random.sample(data, 3)
    print(f'Sample of 3: {sample}')

    # Distributions
    uniform_dist = random.UniformInt(gen, 10, 20)
    normal_dist = random.Normal(gen, mean=100.0, stddev=15.0)

    print(f'Uniform distribution sample: {uniform_dist.sample()}')
    print(f'Normal distribution samples: {normal_dist.samples(3)}')

    # Convenience functions
    print(f'Convenience randint: {random.randint(1, 6)}')
    print(f'Convenience random: {random.random():.4f}')

    print()


def integration_example():
    """Demonstrate integration between different modules."""
    print('=== Integration Example ===')

    # Create random data and process it through multiple modules
    gen = random.RandomGenerator(seed=123)

    with memory.managed_resources() as manager:
        # Generate random shape dimensions
        radii = gen.floats(1.0, 10.0, 3)
        widths = gen.floats(2.0, 8.0, 2)
        heights = gen.floats(3.0, 9.0, 2)

        # Create shapes using memory manager
        circles = [manager.create_circle(r) for r in radii]
        rectangles = [manager.create_rectangle(w, h) for w, h in zip(widths, heights)]

        all_shapes = circles + rectangles

        # Calculate areas using containers and algorithms
        area_container = containers.create_container([shape.get_area() for shape in all_shapes])

        # Sort areas
        areas = list(area_container)
        algorithms.sort_inplace(areas)

        # Find statistics
        min_area, max_area = algorithms.find_min_max(areas)
        large_areas = algorithms.count_if(areas, lambda x: x > 20.0)

        print(f'Created {len(all_shapes)} shapes')
        print(f'Areas (sorted): {[f"{a:.2f}" for a in areas]}')
        print(f'Min area: {min_area:.2f}, Max area: {max_area:.2f}')
        print(f'Large areas (>20): {large_areas}')

        # Time a complex operation
        with timing.Timer() as timer:
            # Complex calculation involving all shapes
            total_complexity = sum(
                shape.get_area() * shape.get_perimeter()
                for shape in all_shapes
            )

        print(f'Complex calculation result: {total_complexity:.2f}')
        print(f'Calculation time: {timer.elapsed_string}')

        # Use Result for safe operations
        safe_avg = exceptions.safe_divide(sum(areas), len(areas))
        if safe_avg.is_ok:
            print(f'Average area: {safe_avg.unwrap():.2f}')

    print('All resources cleaned up automatically')
    print()


def main():
    """Run all examples."""
    print('C++ Python Bindings Examples')
    print('============================')
    print()

    try:
        shapes_example()
        containers_example()
        algorithms_example()
        memory_example()
        exceptions_example()
        timing_example()
        random_example()
        integration_example()

        print('All examples completed successfully!')

    except Exception as e:
        print(f'Error running examples: {e}')
        import traceback
        traceback.print_exc()
        return 1

    return 0


if __name__ == '__main__':
    sys.exit(main())
