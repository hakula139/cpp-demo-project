#!/usr/bin/env python3
"""Advanced usage examples for the C++ Python bindings.

This example demonstrates advanced patterns and real-world use cases.
"""

import sys
from pathlib import Path
from typing import List, Tuple

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'python'))

from python import (algorithms, containers, exceptions, memory, random, shapes,
                    timing)


class ShapeAnalyzer:
    """Advanced shape analysis with performance monitoring."""

    def __init__(self):
        self.benchmark = timing.Benchmark('ShapeAnalyzer')
        self.operation_count = 0

    def analyze_shape_collection(self, shape_specs: List[Tuple[str, List[float]]]) -> dict:
        """Analyze a collection of shapes with comprehensive metrics."""

        def create_shapes():
            """Create shapes from specifications."""
            created_shapes = []
            with memory.managed_resources() as manager:
                for shape_type, params in shape_specs:
                    try:
                        if shape_type == 'circle':
                            shape = manager.create_circle(params[0])
                        elif shape_type == 'rectangle':
                            shape = manager.create_rectangle(params[0], params[1])
                        elif shape_type == 'square':
                            shape = manager.create_rectangle(params[0], params[0])
                        else:
                            continue
                        created_shapes.append(shape)
                    except Exception:
                        continue  # Skip invalid shapes
            return created_shapes

        # Time the shape creation
        creation_stats = self.benchmark.run(create_shapes, iterations=1)
        all_shapes = create_shapes()

        if not all_shapes:
            return {'error': 'No valid shapes created'}

        # Analyze areas and perimeters
        areas = [shape.get_area() for shape in all_shapes]
        perimeters = [shape.get_perimeter() for shape in all_shapes]

        # Use algorithms for analysis
        algorithms.sort_inplace(areas)
        algorithms.sort_inplace(perimeters)

        area_min, area_max = algorithms.find_min_max(areas)
        perim_min, perim_max = algorithms.find_min_max(perimeters)

        # Statistical analysis
        large_shapes = algorithms.count_if(areas, lambda x: x > 50.0)
        complex_shapes = algorithms.count_if(perimeters, lambda x: x > 20.0)

        # Transform for efficiency ratios (area/perimeter)
        efficiency_ratios = algorithms.transform(
            list(zip(areas, perimeters)),
            lambda ap: ap[0] / ap[1] if ap[1] > 0 else 0
        )

        self.operation_count += 1

        return {
            'shape_count': len(all_shapes),
            'areas': {'min': area_min, 'max': area_max, 'sorted': areas},
            'perimeters': {'min': perim_min, 'max': perim_max, 'sorted': perimeters},
            'statistics': {
                'large_shapes': large_shapes,
                'complex_shapes': complex_shapes,
                'avg_efficiency': sum(efficiency_ratios) / len(efficiency_ratios)
            },
            'creation_time': creation_stats['human_readable']['mean'],
            'operation_count': self.operation_count
        }


class DataProcessor:
    """Advanced data processing pipeline with error handling."""

    def __init__(self, seed: int = None):
        self.generator = random.RandomGenerator(seed) if seed else random.RandomGenerator()
        self.processor_id = self.generator.randint(1000, 9999)

    def process_numeric_pipeline(self, size: int = 1000) -> exceptions.Result[dict]:
        """Process numeric data through a complex pipeline."""

        try:
            # Generate random data
            data = self.generator.integers(-100, 100, size)

            # Create container for processing
            container = containers.create_container(data)

            # Pipeline operations with error handling
            def safe_filter_positive():
                return container.filter(lambda x: x > 0)

            def safe_transform_squares(positive_nums):
                return algorithms.transform(positive_nums, lambda x: x * x)

            def safe_statistical_analysis(squares):
                if not squares:
                    raise ValueError('No positive numbers to analyze')

                algorithms.sort_inplace(squares)
                min_val, max_val = algorithms.find_min_max(squares)

                return {
                    'count': len(squares),
                    'min': min_val,
                    'max': max_val,
                    'median': squares[len(squares) // 2],
                    'sum': sum(squares),
                    'large_values': algorithms.count_if(squares, lambda x: x > 1000)
                }

            # Execute pipeline
            positive_numbers = safe_filter_positive()
            squared_numbers = safe_transform_squares(positive_numbers)
            stats = safe_statistical_analysis(squared_numbers)

            # Add metadata
            stats['processor_id'] = self.processor_id
            stats['original_size'] = size
            stats['positive_ratio'] = len(positive_numbers) / size

            return exceptions.Result.ok(stats)

        except Exception as e:
            return exceptions.Result.error(f'Pipeline error: {str(e)}')

    def generate_random_shapes(self, count: int) -> List[Tuple[str, List[float]]]:
        """Generate random shape specifications."""
        shape_specs = []

        for _ in range(count):
            shape_choice = self.generator.choice(0.6)  # 60% chance for circles

            if shape_choice:  # Circle
                radius = self.generator.uniform(1.0, 10.0)
                shape_specs.append(('circle', [radius]))
            else:  # Rectangle
                width = self.generator.uniform(2.0, 8.0)
                height = self.generator.uniform(2.0, 8.0)
                shape_specs.append(('rectangle', [width, height]))

        return shape_specs


class PerformanceBenchmarkSuite:
    """Comprehensive performance benchmarking suite."""

    def __init__(self):
        self.results = {}

    def benchmark_sorting_algorithms(self) -> dict:
        """Benchmark different sorting scenarios."""
        scenarios = {
            'small_random': lambda: random.RandomGenerator(42).integers(1, 100, 50),
            'large_random': lambda: random.RandomGenerator(42).integers(1, 1000, 1000),
            'already_sorted': lambda: list(range(1, 501)),
            'reverse_sorted': lambda: list(range(500, 0, -1)),
            'mostly_sorted': lambda: list(range(1, 500)) + [499, 498, 497]
        }

        results = {}

        for scenario_name, data_gen in scenarios.items():
            def sort_test():
                data = data_gen()
                algorithms.sort_inplace(data)
                return data

            stats = timing.benchmark_function(sort_test, iterations=10, name=scenario_name)
            results[scenario_name] = {
                'mean_time': stats['human_readable']['mean'],
                'iterations': stats['iterations'],
                'data_size': len(data_gen())
            }

        return results

    def benchmark_container_operations(self) -> dict:
        """Benchmark container operations."""
        gen = random.RandomGenerator(123)

        def setup_large_container():
            return containers.create_container(gen.integers(1, 1000, 5000))

        def filter_operation():
            container = setup_large_container()
            return container.filter(lambda x: x % 2 == 0)

        def transform_operation():
            container = setup_large_container()
            return container.transform(lambda x: x * x)

        def mixed_operations():
            container = setup_large_container()
            filtered = container.filter(lambda x: x > 500)
            return container.transform(lambda x: x + 1)

        operations = {
            'filter': filter_operation,
            'transform': transform_operation,
            'mixed': mixed_operations
        }

        results = {}
        for op_name, op_func in operations.items():
            stats = timing.benchmark_function(op_func, iterations=5, name=op_name)
            results[op_name] = stats['human_readable']['mean']

        return results

    def benchmark_memory_management(self) -> dict:
        """Benchmark memory management operations."""

        def create_many_shapes():
            with memory.managed_resources() as manager:
                shapes_created = []
                for i in range(100):
                    if i % 2 == 0:
                        shapes_created.append(manager.create_circle(float(i + 1)))
                    else:
                        shapes_created.append(manager.create_rectangle(float(i + 1), float(i + 2)))
                return len(shapes_created)

        def cleanup_heavy_operations():
            cleanup_count = 0
            with memory.managed_resources() as manager:
                for i in range(50):
                    manager.register_cleanup(lambda: None)  # Dummy cleanup
                    cleanup_count += 1
            return cleanup_count

        operations = {
            'shape_creation': create_many_shapes,
            'cleanup_operations': cleanup_heavy_operations
        }

        results = {}
        for op_name, op_func in operations.items():
            stats = timing.benchmark_function(op_func, iterations=3, name=op_name)
            results[op_name] = stats['human_readable']['mean']

        return results

    def run_full_benchmark(self) -> dict:
        """Run comprehensive benchmark suite."""
        print('Running performance benchmark suite...')

        with timing.measure_time('Full benchmark suite'):
            self.results = {
                'sorting': self.benchmark_sorting_algorithms(),
                'containers': self.benchmark_container_operations(),
                'memory': self.benchmark_memory_management()
            }

        return self.results


def monte_carlo_pi_estimation(samples: int = 1000000) -> exceptions.Result[float]:
    """Estimate π using Monte Carlo method with error handling."""

    try:
        if samples <= 0:
            return exceptions.Result.error('Sample count must be positive')

        gen = random.RandomGenerator()

        # Generate random points
        x_coords = gen.floats(-1.0, 1.0, samples)
        y_coords = gen.floats(-1.0, 1.0, samples)

        # Count points inside unit circle
        inside_circle = algorithms.count_if(
            list(zip(x_coords, y_coords)),
            lambda p: p[0]**2 + p[1]**2 <= 1.0
        )

        # Estimate π
        pi_estimate = 4.0 * inside_circle / samples

        return exceptions.Result.ok(pi_estimate)

    except Exception as e:
        return exceptions.Result.error(f'Monte Carlo estimation failed: {str(e)}')


def advanced_functional_programming_example():
    """Demonstrate advanced functional programming patterns."""
    print('=== Advanced Functional Programming ===')

    # Generate sample data
    gen = random.RandomGenerator(456)
    data = gen.integers(1, 100, 50)

    # Complex functional chain with multiple transformations
    result = (algorithms.functional_chain(data)
              .filter(lambda x: x % 3 == 0)  # Divisible by 3
              .map(lambda x: x * x)          # Square them
              .filter(lambda x: x < 1000)    # Keep reasonable size
              .sort(reverse=True)             # Sort descending
              .take(10)                       # Take top 10
              .map(lambda x: x / 9)          # Divide by 9
              .collect())

    print(f'Functional chain result: {result}')

    # Pipeline composition
    numerical_pipeline = algorithms.pipeline(
        lambda nums: [x for x in nums if x > 20],     # Filter
        lambda nums: [x * 2 for x in nums],           # Double
        lambda nums: algorithms.transform(nums, lambda x: x + 1),  # Add 1
        lambda nums: sorted(nums),                     # Sort
        lambda nums: nums[:5]                          # Take first 5
    )

    pipeline_result = numerical_pipeline(gen.integers(1, 50, 20))
    print(f'Pipeline result: {pipeline_result}')

    # Error handling in functional chains
    def safe_square_root(x: float) -> exceptions.Result[float]:
        return exceptions.safe_sqrt(x)

    def safe_logarithm(x: float) -> exceptions.Result[float]:
        if x <= 0:
            return exceptions.Result.error('Logarithm of non-positive number')
        import math
        return exceptions.Result.ok(math.log(x))

    # Chain safe operations
    safe_chain = exceptions.chain_operations(safe_square_root, safe_logarithm)

    test_values = [16.0, 25.0, -4.0, 0.0, 100.0]
    for val in test_values:
        result = safe_chain(val)
        if result.is_ok:
            print(f'√{val} → log = {result.unwrap():.4f}')
        else:
            print(f'√{val} → log failed (expected for {val})')

    print()


def real_world_simulation():
    """Real-world simulation: analyzing geometric shapes in a virtual environment."""
    print('=== Real-World Simulation: Virtual Environment ===')

    # Setup
    processor = DataProcessor(seed=789)
    analyzer = ShapeAnalyzer()

    # Generate a virtual environment with random shapes
    num_objects = 25
    shape_specs = processor.generate_random_shapes(num_objects)

    print(f'Generated {len(shape_specs)} objects in virtual environment')

    # Analyze the environment
    with timing.measure_time('Environment analysis'):
        analysis = analyzer.analyze_shape_collection(shape_specs)

    if 'error' not in analysis:
        print(f"Environment Analysis Results:")
        print(f"  - Total shapes: {analysis['shape_count']}")
        print(f"  - Area range: {analysis['areas']['min']:.2f} - {analysis['areas']['max']:.2f}")
        print(f"  - Perimeter range: {analysis['perimeters']['min']:.2f} - {analysis['perimeters']['max']:.2f}")
        print(f"  - Large shapes (area > 50): {analysis['statistics']['large_shapes']}")
        print(f"  - Complex shapes (perimeter > 20): {analysis['statistics']['complex_shapes']}")
        print(f"  - Average efficiency ratio: {analysis['statistics']['avg_efficiency']:.4f}")
        print(f"  - Shape creation time: {analysis['creation_time']}")

    # Process numeric data from the environment
    numeric_result = processor.process_numeric_pipeline(size=500)

    if numeric_result.is_ok:
        stats = numeric_result.unwrap()
        print(f"\nNumeric Data Processing:")
        print(f"  - Processor ID: {stats['processor_id']}")
        print(f"  - Positive numbers: {stats['count']} out of {stats['original_size']}")
        print(f"  - Positive ratio: {stats['positive_ratio']:.2%}")
        print(f"  - Square range: {stats['min']} - {stats['max']}")
        print(f"  - Large values (> 1000): {stats['large_values']}")
    else:
        print(f"\nNumeric processing failed: {numeric_result}")

    # Monte Carlo π estimation
    pi_result = monte_carlo_pi_estimation(100000)
    if pi_result.is_ok:
        estimated_pi = pi_result.unwrap()
        error = abs(estimated_pi - 3.14159265359)
        print(f"\nMonte Carlo π estimation: {estimated_pi:.6f} (error: {error:.6f})")

    print()


def main():
    """Run all advanced examples."""
    print('C++ Python Bindings - Advanced Examples')
    print('=======================================')
    print()

    try:
        # Advanced functional programming
        advanced_functional_programming_example()

        # Real-world simulation
        real_world_simulation()

        # Performance benchmarking
        benchmark_suite = PerformanceBenchmarkSuite()
        results = benchmark_suite.run_full_benchmark()

        print('=== Performance Benchmark Results ===')
        print('Sorting benchmarks:')
        for scenario, stats in results['sorting'].items():
            print(f'  {scenario}: {stats["mean_time"]} (size: {stats["data_size"]})')

        print('\nContainer operation benchmarks:')
        for operation, time_str in results['containers'].items():
            print(f'  {operation}: {time_str}')

        print('\nMemory management benchmarks:')
        for operation, time_str in results['memory'].items():
            print(f'  {operation}: {time_str}')

        print('\nAll advanced examples completed successfully!')

    except Exception as e:
        print(f'Error running advanced examples: {e}')
        import traceback
        traceback.print_exc()
        return 1

    return 0


if __name__ == '__main__':
    sys.exit(main())
