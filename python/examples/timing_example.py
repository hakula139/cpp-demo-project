#!/usr/bin/env python3
"""Timing module example demonstrating high-resolution timing and benchmarking.

This example shows how to use the timing module for performance measurement,
benchmarking, and timing analysis with nanosecond precision.
"""

import random
import sys
import time
from pathlib import Path
from typing import Any, Callable, Dict, List

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from timing import Benchmark, Timer, benchmark_function, measure_time, time_function


def basic_timer_demo() -> None:
    """Demonstrate basic timer usage.

    Shows how to use the Timer class for measuring execution time
    of code blocks and operations.
    """
    print('=== Basic Timer Demo ===')

    # Basic timer usage
    print('Basic timer with context manager:')

    with Timer() as timer:
        # Simulate some work
        total = sum(i * i for i in range(1000))
        time.sleep(0.01)  # Add small delay

    print(f'Operation completed in: {timer.elapsed_string}')
    print(f'Raw elapsed time: {timer.elapsed_ns} nanoseconds')
    print(f'Result: {total}')

    # Manual timer control
    print(f'\nManual timer control:')

    timer = Timer()
    timer.start()

    # Simulate multiple operations
    results = []
    for i in range(5):
        operation_result = sum(j for j in range(100 * (i + 1)))
        results.append(operation_result)
        print(
            f'  Operation {i+1}: result={operation_result}, time={timer.elapsed_string}'
        )

    timer.stop()
    print(f'Total time for all operations: {timer.elapsed_string}')

    # Timer with reset functionality
    print(f'\nTimer with reset:')

    timer.reset()
    timer.start()

    quick_calc = 2**10
    timer.stop()
    print(f'Quick calculation (2^10 = {quick_calc}): {timer.elapsed_string}')

    print()


def measure_time_demo() -> None:
    """Demonstrate the measure_time context manager.

    Shows how to use measure_time for convenient timing
    with automatic logging and labeling.
    """
    print('=== Measure Time Demo ===')

    # Basic measure_time usage
    print('Measuring time for different operations:')

    with measure_time('List comprehension'):
        squares = [x**2 for x in range(1000)]

    with measure_time('Generator expression'):
        squares_gen = list(x**2 for x in range(1000))

    with measure_time('Traditional loop'):
        squares_loop = []
        for x in range(1000):
            squares_loop.append(x**2)

    # Verify results are the same
    print(f'Results match: {squares == squares_gen == squares_loop}')

    # Nested timing
    print(f'\nNested timing measurements:')

    with measure_time('Data processing pipeline'):
        data = list(range(500))

        with measure_time('  Data filtering'):
            filtered = [x for x in data if x % 2 == 0]

        with measure_time('  Data transformation'):
            transformed = [x * 3 + 1 for x in filtered]

        with measure_time('  Data sorting'):
            sorted_data = sorted(transformed, reverse=True)

    print(f'Final result size: {len(sorted_data)}')

    # Measuring different algorithms
    print(f'\nComparing sorting algorithms:')

    # Generate test data
    test_data = [random.randint(1, 1000) for _ in range(500)]

    with measure_time('Built-in sorted()'):
        result1 = sorted(test_data.copy())

    with measure_time('List.sort() method'):
        data_copy = test_data.copy()
        data_copy.sort()
        result2 = data_copy

    print(f'Results identical: {result1 == result2}')

    print()


def benchmark_function_demo() -> None:
    """Demonstrate function benchmarking capabilities.

    Shows how to benchmark functions with multiple iterations
    and statistical analysis of performance.
    """
    print('=== Benchmark Function Demo ===')

    # Define test functions
    def bubble_sort(arr: List[int]) -> List[int]:
        """Simple bubble sort implementation."""
        arr = arr.copy()
        n = len(arr)
        for i in range(n):
            for j in range(0, n - i - 1):
                if arr[j] > arr[j + 1]:
                    arr[j], arr[j + 1] = arr[j + 1], arr[j]
        return arr

    def python_sort(arr: List[int]) -> List[int]:
        """Python's built-in sort."""
        return sorted(arr)

    def manual_sort(arr: List[int]) -> List[int]:
        """Manual implementation using list.sort()."""
        arr = arr.copy()
        arr.sort()
        return arr

    # Test data
    small_data = [random.randint(1, 100) for _ in range(50)]
    medium_data = [random.randint(1, 1000) for _ in range(200)]

    print('Benchmarking different sorting algorithms:')
    print(f'Small dataset ({len(small_data)} elements):')

    # Benchmark each function
    algorithms = [
        ('Bubble Sort', bubble_sort),
        ('Python sorted()', python_sort),
        ('List.sort()', manual_sort),
    ]

    small_results = {}
    for name, func in algorithms:
        stats = benchmark_function(
            lambda: func(small_data), iterations=10, name=f'{name} (small)'
        )
        small_results[name] = stats
        print(f'  {name}:')
        print(f'    Mean: {stats["human_readable"]["mean"]}')
        print(f'    Min:  {stats["human_readable"]["min"]}')
        print(f'    Max:  {stats["human_readable"]["max"]}')

    # Benchmark with larger dataset (skip bubble sort - too slow)
    print(f'\nMedium dataset ({len(medium_data)} elements):')

    fast_algorithms = [('Python sorted()', python_sort), ('List.sort()', manual_sort)]
    medium_results = {}

    for name, func in fast_algorithms:
        stats = benchmark_function(
            lambda f=func: f(medium_data), iterations=50, name=f'{name} (medium)'
        )
        medium_results[name] = stats
        print(f'  {name}:')
        print(f'    Mean: {stats["human_readable"]["mean"]}')
        print(f'    Std:  {stats["human_readable"]["std"]}')

    print()


def time_function_demo() -> None:
    """Demonstrate single function timing.

    Shows how to time individual function calls for
    quick performance measurements.
    """
    print('=== Time Function Demo ===')

    # Time different operations
    operations = [
        ('List creation', lambda: list(range(1000))),
        ('List comprehension', lambda: [x**2 for x in range(1000)]),
        ('String operations', lambda: ''.join(str(i) for i in range(100))),
        ('Dictionary creation', lambda: {i: i**2 for i in range(500)}),
        ('Set operations', lambda: set(range(1000)) & set(range(500, 1500))),
    ]

    print('Single function timing:')

    for name, operation in operations:
        elapsed = time_function(operation)
        print(f'  {name}: {elapsed}')

    # Time mathematical operations
    print(f'\nMathematical operations:')

    math_ops = [
        ('Sum 1-10000', lambda: sum(range(10000))),
        (
            'Factorial 20',
            lambda: eval('1*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20'),
        ),
        ('Power operations', lambda: [2**i for i in range(20)]),
        ('Square roots', lambda: [i**0.5 for i in range(1000)]),
    ]

    for name, operation in math_ops:
        elapsed = time_function(operation)
        print(f'  {name}: {elapsed}')

    print()


def benchmark_class_demo() -> None:
    """Demonstrate the Benchmark class for complex benchmarking.

    Shows how to use the Benchmark class for organizing
    and managing multiple related performance tests.
    """
    print('=== Benchmark Class Demo ===')

    # Create benchmark suite
    bench = Benchmark('Data Structure Operations')

    # Test different data structures
    test_size = 1000
    test_data = list(range(test_size))

    print(f'Benchmarking data structure operations ({test_size} elements):')

    # List operations
    def list_operations():
        """Benchmark list operations."""
        data = test_data.copy()
        data.append(test_size)
        data.insert(0, -1)
        data.remove(-1)
        data.pop()
        return len(data)

    list_stats = bench.run(list_operations, iterations=100, name='List operations')
    print(f'  List operations: {list_stats["human_readable"]["mean"]}')

    # Dictionary operations
    def dict_operations():
        """Benchmark dictionary operations."""
        data = {i: i**2 for i in test_data}
        data[test_size] = test_size**2
        del data[0]
        return len(data)

    dict_stats = bench.run(dict_operations, iterations=100, name='Dict operations')
    print(f'  Dict operations: {dict_stats["human_readable"]["mean"]}')

    # Set operations
    def set_operations():
        """Benchmark set operations."""
        data = set(test_data)
        data.add(test_size)
        data.discard(0)
        return len(data)

    set_stats = bench.run(set_operations, iterations=100, name='Set operations')
    print(f'  Set operations: {set_stats["human_readable"]["mean"]}')

    # Compare results
    print(f'\nPerformance comparison:')
    results = [
        ('List', list_stats['time_ns']),
        ('Dict', dict_stats['time_ns']),
        ('Set', set_stats['time_ns']),
    ]

    # Sort by performance
    results.sort(key=lambda x: x[1])
    fastest = results[0][1]

    for name, time_ns in results:
        ratio = time_ns / fastest
        print(
            f'  {name}: {ratio:.2f}x slower than fastest'
            if ratio > 1
            else f'  {name}: fastest'
        )

    print()


def real_world_benchmarking_demo() -> None:
    """Demonstrate real-world benchmarking scenarios.

    Shows practical examples of benchmarking actual algorithms
    and data processing tasks that might be found in applications.
    """
    print('=== Real-World Benchmarking Demo ===')

    # File processing simulation
    print('File processing simulation:')

    # Simulate file content
    file_content = []
    for i in range(1000):
        line = f'data_{i}:value_{i**2}:timestamp_{i*1000}'
        file_content.append(line)

    def csv_like_parsing():
        """Parse CSV-like data."""
        parsed = []
        for line in file_content:
            parts = line.split(':')
            if len(parts) == 3:
                parsed.append(
                    {
                        'data': parts[0],
                        'value': int(parts[1].split('_')[1]),
                        'timestamp': int(parts[2].split('_')[1]),
                    }
                )
        return len(parsed)

    def regex_parsing():
        """Parse using string operations."""
        import re

        pattern = r'data_(\d+):value_(\d+):timestamp_(\d+)'
        parsed = []
        for line in file_content:
            match = re.match(pattern, line)
            if match:
                parsed.append(
                    {
                        'data': int(match.group(1)),
                        'value': int(match.group(2)),
                        'timestamp': int(match.group(3)),
                    }
                )
        return len(parsed)

    # Benchmark parsing methods
    csv_time = time_function(csv_like_parsing)
    regex_time = time_function(regex_parsing)

    print(f'  CSV-like parsing: {csv_time}')
    print(f'  Regex parsing: {regex_time}')

    # Data aggregation simulation
    print(f'\nData aggregation methods:')

    numbers = [random.randint(1, 100) for _ in range(5000)]

    def groupby_aggregation():
        """Group and aggregate using itertools."""
        from itertools import groupby

        sorted_nums = sorted(numbers)
        groups = {}
        for key, group in groupby(sorted_nums):
            groups[key] = len(list(group))
        return len(groups)

    def dict_aggregation():
        """Aggregate using dictionary."""
        groups = {}
        for num in numbers:
            groups[num] = groups.get(num, 0) + 1
        return len(groups)

    def counter_aggregation():
        """Aggregate using Counter."""
        from collections import Counter

        groups = Counter(numbers)
        return len(groups)

    # Time each approach
    groupby_time = time_function(groupby_aggregation)
    dict_time = time_function(dict_aggregation)
    counter_time = time_function(counter_aggregation)

    print(f'  Groupby method: {groupby_time}')
    print(f'  Dictionary method: {dict_time}')
    print(f'  Counter method: {counter_time}')

    # Algorithm comparison
    print(f'\nAlgorithm optimization:')

    search_data = list(range(10000))
    target = 7500

    def linear_search():
        """Linear search implementation."""
        for i, value in enumerate(search_data):
            if value == target:
                return i
        return -1

    def binary_search():
        """Binary search implementation."""
        left, right = 0, len(search_data) - 1
        while left <= right:
            mid = (left + right) // 2
            if search_data[mid] == target:
                return mid
            elif search_data[mid] < target:
                left = mid + 1
            else:
                right = mid - 1
        return -1

    def builtin_search():
        """Using built-in index method."""
        try:
            return search_data.index(target)
        except ValueError:
            return -1

    # Benchmark search algorithms
    search_algorithms = [
        ('Linear search', linear_search),
        ('Binary search', binary_search),
        ('Built-in index', builtin_search),
    ]

    print(f'Searching for {target} in {len(search_data)} elements:')

    search_results = {}
    for name, func in search_algorithms:
        stats = benchmark_function(func, iterations=1000, name=name)
        search_results[name] = stats
        print(f'  {name}: {stats["human_readable"]["mean"]}')

    print()


def performance_analysis_demo() -> None:
    """Demonstrate performance analysis and profiling.

    Shows how to analyze timing results and identify
    performance bottlenecks in code.
    """
    print('=== Performance Analysis Demo ===')

    # Analyze performance patterns
    print('Performance pattern analysis:')

    def create_test_function(size_factor: int):
        """Create a test function with different computational complexity."""

        def test_func():
            n = 100 * size_factor
            return sum(i * j for i in range(n) for j in range(10))

        return test_func

    # Test with different sizes
    size_factors = [1, 2, 3, 4, 5]
    timing_results = []

    for factor in size_factors:
        test_func = create_test_function(factor)
        stats = benchmark_function(test_func, iterations=10, name=f'Size {factor}')
        mean_time = stats['time_ns'] / 1_000_000  # Convert to milliseconds
        timing_results.append((factor, mean_time))
        print(f'  Size factor {factor}: {stats["human_readable"]["mean"]}')

    # Analyze scaling
    print(f'\nScaling analysis:')
    for i in range(1, len(timing_results)):
        prev_factor, prev_time = timing_results[i - 1]
        curr_factor, curr_time = timing_results[i]

        time_ratio = curr_time / prev_time
        size_ratio = curr_factor / prev_factor

        print(
            f'  Factor {prev_factor} -> {curr_factor}: {time_ratio:.2f}x slower ({size_ratio:.1f}x larger)'
        )

    # Memory vs. computation tradeoff
    print(f'\nMemory vs. computation tradeoff:')

    def compute_heavy():
        """Computation-heavy approach."""
        total = 0
        for i in range(1000):
            total += sum(j**2 for j in range(50))
        return total

    def memory_heavy():
        """Memory-heavy approach."""
        precomputed = [j**2 for j in range(50)]
        total = 0
        for i in range(1000):
            total += sum(precomputed)
        return total

    compute_time = time_function(compute_heavy)
    memory_time = time_function(memory_heavy)

    print(f'  Compute-heavy approach: {compute_time}')
    print(f'  Memory-heavy approach: {memory_time}')

    if float(memory_time.split()[0]) < float(compute_time.split()[0]):
        print('  Memory optimization wins!')
    else:
        print('  Computation optimization wins!')

    print()


def main() -> int:
    """Run all timing examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Timing Module Example')
    print('====================')
    print()

    try:
        basic_timer_demo()
        measure_time_demo()
        benchmark_function_demo()
        time_function_demo()
        benchmark_class_demo()
        real_world_benchmarking_demo()
        performance_analysis_demo()

        print('All timing examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running timing examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
