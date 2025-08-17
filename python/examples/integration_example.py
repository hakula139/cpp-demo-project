#!/usr/bin/env python3
"""Integration example demonstrating cross-module usage patterns.

This example shows how to combine multiple modules to create complex
applications and demonstrates real-world integration scenarios.
"""

import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from random import RandomGenerator

from algorithms import count_if, find_min_max, sort_inplace, transform
from containers import Container, create_container
from exceptions import Result, safe_divide, safe_sqrt
from memory import managed_resources
from shapes import analyze_shape, compare_shapes, create_shape
from timing import Timer, benchmark_function, measure_time


def geometric_data_analysis() -> None:
    """Demonstrate geometric data analysis with multiple modules.

    Shows how to generate random geometric data, analyze it using
    containers and algorithms, and manage resources safely.
    """
    print('=== Geometric Data Analysis ===')

    # Generate random geometric data
    gen = RandomGenerator(seed=123)

    with managed_resources() as manager:
        print('Generating random geometric shapes...')

        # Create random shapes
        shapes = []
        for i in range(20):
            shape_type = gen.choice(0.6)  # 60% circles, 40% rectangles

            if shape_type:  # Circle
                radius = gen.uniform(1.0, 10.0)
                shape = manager.create_circle(radius)
            else:  # Rectangle
                width = gen.uniform(2.0, 8.0)
                height = gen.uniform(2.0, 8.0)
                shape = manager.create_rectangle(width, height)

            shapes.append(shape)

        print(f'Created {len(shapes)} random shapes')

        # Analyze shapes using containers and algorithms
        with measure_time('Shape analysis'):
            # Extract metrics
            areas = [shape.get_area() for shape in shapes]
            perimeters = [shape.get_perimeter() for shape in shapes]

            # Create containers for analysis
            area_container = create_container(areas)
            perimeter_container = create_container(perimeters)

            # Sort for analysis
            sort_inplace(areas)
            sort_inplace(perimeters)

            # Find extrema
            min_area, max_area = find_min_max(areas)
            min_perim, max_perim = find_min_max(perimeters)

            # Statistical analysis
            large_shapes = count_if(areas, lambda x: x > 25.0)
            complex_shapes = count_if(perimeters, lambda x: x > 15.0)

            # Transform for efficiency analysis
            efficiency_ratios = []
            for area, perim in zip(areas, perimeters):
                if perim > 0:
                    efficiency_ratios.append(area / perim)
                else:
                    efficiency_ratios.append(0.0)

            sort_inplace(efficiency_ratios)

        # Display results
        print(f'\nAnalysis Results:')
        print(f'  Area statistics:')
        print(f'    Range: {min_area:.2f} - {max_area:.2f}')
        print(f'    Large shapes (area > 25): {large_shapes}')
        print(f'  Perimeter statistics:')
        print(f'    Range: {min_perim:.2f} - {max_perim:.2f}')
        print(f'    Complex shapes (perimeter > 15): {complex_shapes}')

    print()


def data_processing_pipeline() -> None:
    """Demonstrate a complete data processing pipeline.

    Shows error handling, data transformation, statistical analysis,
    and performance monitoring in an integrated workflow.
    """
    print('=== Data Processing Pipeline ===')

    # Generate test dataset
    gen = RandomGenerator(seed=456)
    raw_data = gen.integers(-50, 150, 100)

    print(f'Processing {len(raw_data)} data points...')

    with measure_time('Complete pipeline'):
        # Stage 1: Data validation and cleaning
        with measure_time('  Data validation'):
            valid_data = []
            errors = []

            for i, value in enumerate(raw_data):
                if 0 <= value <= 100:
                    valid_data.append(value)
                else:
                    errors.append((i, value, 'out_of_range'))

            print(f'  Valid data points: {len(valid_data)}/{len(raw_data)}')
            print(f'  Errors detected: {len(errors)}')

        # Stage 2: Data transformation with error handling
        with measure_time('  Data transformation'):
            transformed_results = []
            transformation_errors = []

            for value in valid_data:
                # Safe square root transformation
                sqrt_result = safe_sqrt(float(value))
                if sqrt_result.is_ok:
                    # Scale and add noise
                    scaled = sqrt_result.unwrap() * 2.5
                    noise = gen.normal(0.0, 0.1)
                    final_value = scaled + noise
                    transformed_results.append(final_value)
                else:
                    transformation_errors.append((value, sqrt_result.error_message))

            print(f'  Transformed values: {len(transformed_results)}')
            print(f'  Transformation errors: {len(transformation_errors)}')

        # Stage 3: Statistical analysis
        with measure_time('  Statistical analysis'):
            if transformed_results:
                # Use containers for analysis
                data_container = create_container(transformed_results)

                # Basic statistics
                sorted_data = sorted(transformed_results)
                mean = sum(sorted_data) / len(sorted_data)
                median = sorted_data[len(sorted_data) // 2]
                min_val, max_val = find_min_max(sorted_data)

                # Quartiles
                q1_idx = len(sorted_data) // 4
                q3_idx = 3 * len(sorted_data) // 4
                q1 = sorted_data[q1_idx]
                q3 = sorted_data[q3_idx]

                # Count values in different ranges
                low_values = count_if(transformed_results, lambda x: x < mean - 1.0)
                high_values = count_if(transformed_results, lambda x: x > mean + 1.0)

                stats = {
                    'count': len(sorted_data),
                    'mean': mean,
                    'median': median,
                    'min': min_val,
                    'max': max_val,
                    'q1': q1,
                    'q3': q3,
                    'range': max_val - min_val,
                    'iqr': q3 - q1,
                    'low_outliers': low_values,
                    'high_outliers': high_values,
                }

                print(f'  Statistical summary:')
                print(f'    Count: {stats["count"]}')
                print(f'    Mean: {stats["mean"]:.3f}')
                print(f'    Median: {stats["median"]:.3f}')
                print(f'    Range: {stats["min"]:.3f} - {stats["max"]:.3f}')
                print(f'    IQR: {stats["iqr"]:.3f}')
                print(
                    f'    Outliers: {stats["low_outliers"]} low, {stats["high_outliers"]} high'
                )

        # Stage 4: Results validation
        with measure_time('  Results validation'):
            validation_results = []

            for value in transformed_results:
                # Validate transformed values are reasonable
                if 0 <= value <= 50:  # Expected range after transformation
                    validation_results.append(('valid', value))
                else:
                    validation_results.append(('suspicious', value))

            valid_count = count_if(validation_results, lambda x: x[0] == 'valid')
            suspicious_count = len(validation_results) - valid_count

            print(f'  Validation results:')
            print(f'    Valid: {valid_count}')
            print(f'    Suspicious: {suspicious_count}')

    print()


def scientific_simulation() -> None:
    """Demonstrate a scientific simulation with error handling.

    Shows Monte Carlo simulation with statistical analysis,
    memory management, and comprehensive error handling.
    """
    print('=== Scientific Simulation ===')

    # Monte Carlo simulation parameters
    num_experiments = 5
    samples_per_experiment = 10000

    print(f'Running {num_experiments} Monte Carlo experiments...')
    print(f'Samples per experiment: {samples_per_experiment}')

    with managed_resources() as manager:
        experiment_results = []

        for exp_id in range(num_experiments):
            print(f'\nExperiment {exp_id + 1}:')

            # Create random generator for this experiment
            gen = RandomGenerator(seed=exp_id * 1000)

            with measure_time(f'  Experiment {exp_id + 1}'):
                # Monte Carlo π estimation
                inside_circle = 0
                sample_points = []

                for _ in range(samples_per_experiment):
                    x = gen.uniform(-1.0, 1.0)
                    y = gen.uniform(-1.0, 1.0)

                    distance_squared = x * x + y * y
                    sample_points.append((x, y, distance_squared))

                    if distance_squared <= 1.0:
                        inside_circle += 1

                # Calculate π estimate
                pi_estimate = 4.0 * inside_circle / samples_per_experiment
                error = abs(pi_estimate - 3.14159265359)

                # Analyze sample distribution
                distances = [point[2] for point in sample_points]
                sort_inplace(distances)

                min_dist, max_dist = find_min_max(distances)
                close_points = count_if(distances, lambda d: d < 0.1)
                far_points = count_if(distances, lambda d: d > 0.9)

                experiment_result = {
                    'experiment_id': exp_id + 1,
                    'pi_estimate': pi_estimate,
                    'error': error,
                    'inside_circle': inside_circle,
                    'total_samples': samples_per_experiment,
                    'min_distance': min_dist,
                    'max_distance': max_dist,
                    'close_points': close_points,
                    'far_points': far_points,
                }

                experiment_results.append(experiment_result)

                print(f'    π estimate: {pi_estimate:.6f}')
                print(f'    Error: {error:.6f}')
                print(f'    Points inside circle: {inside_circle}')
                print(f'    Distance range: {min_dist:.4f} - {max_dist:.4f}')

        # Aggregate analysis across experiments
        print(f'\nAggregate Analysis:')

        pi_estimates = [result['pi_estimate'] for result in experiment_results]
        errors = [result['error'] for result in experiment_results]

        sort_inplace(pi_estimates)
        sort_inplace(errors)

        mean_pi = sum(pi_estimates) / len(pi_estimates)
        mean_error = sum(errors) / len(errors)
        best_estimate = min(errors)
        worst_estimate = max(errors)

        print(f'  Mean π estimate: {mean_pi:.6f}')
        print(f'  Mean error: {mean_error:.6f}')
        print(f'  Best error: {best_estimate:.6f}')
        print(f'  Worst error: {worst_estimate:.6f}')

        # Consistency analysis
        pi_range = max(pi_estimates) - min(pi_estimates)
        print(f'  Estimate range: {pi_range:.6f}')

        consistent_experiments = count_if(errors, lambda e: e < 0.01)
        print(
            f'  Consistent experiments (error < 0.01): {consistent_experiments}/{num_experiments}'
        )

    print()


def performance_benchmark_suite() -> None:
    """Demonstrate comprehensive performance benchmarking.

    Shows how to benchmark different approaches using timing,
    containers, algorithms, and statistical analysis.
    """
    print('=== Performance Benchmark Suite ===')

    # Test different sorting algorithms
    test_sizes = [100, 500, 1000]
    algorithms_to_test = ['python_sort', 'manual_sort', 'container_sort']

    print('Benchmarking sorting algorithms:')

    benchmark_results = {}

    for size in test_sizes:
        print(f'\nTest size: {size} elements')

        # Generate test data
        gen = RandomGenerator(seed=42)
        test_data = gen.integers(1, 1000, size)

        size_results = {}

        # Python's built-in sort
        def python_sort_test():
            data = test_data.copy()
            data.sort()
            return data

        stats = benchmark_function(
            python_sort_test, iterations=50, name=f'Python sort ({size})'
        )
        size_results['python_sort'] = stats
        print(f'  Python sort: {stats["human_readable"]["mean"]}')

        # Manual implementation
        def manual_sort_test():
            data = test_data.copy()
            sort_inplace(data)
            return data

        stats = benchmark_function(
            manual_sort_test, iterations=50, name=f'Manual sort ({size})'
        )
        size_results['manual_sort'] = stats
        print(f'  Manual sort: {stats["human_readable"]["mean"]}')

        # Container-based approach
        def container_sort_test():
            container = create_container(test_data.copy())
            data = list(container)
            sort_inplace(data)
            return data

        stats = benchmark_function(
            container_sort_test, iterations=50, name=f'Container sort ({size})'
        )
        size_results['container_sort'] = stats
        print(f'  Container sort: {stats["human_readable"]["mean"]}')

        benchmark_results[size] = size_results

    # Analyze scaling behavior
    print(f'\nScaling Analysis:')

    for algorithm in algorithms_to_test:
        print(f'  {algorithm}:')

        prev_time = None
        for size in test_sizes:
            current_time = benchmark_results[size][algorithm]['time_ns']

            if prev_time is not None:
                ratio = current_time / prev_time
                size_ratio = size / prev_size
                print(
                    f'    {prev_size} -> {size}: {ratio:.2f}x slower ({size_ratio:.1f}x data)'
                )

            prev_time = current_time
            prev_size = size

    print()


def real_world_application_demo() -> None:
    """Demonstrate a real-world application scenario.

    Shows a complete application that processes user data,
    validates inputs, performs calculations, and handles errors gracefully.
    """
    print('=== Real-World Application Demo ===')

    # Simulate a geometric calculator application
    print('Geometric Calculator Application')

    with managed_resources() as manager:
        # Simulate user input data (normally from forms/files)
        user_inputs = [
            {'type': 'circle', 'params': [5.0], 'user_id': 'user_001'},
            {'type': 'rectangle', 'params': [4.0, 6.0], 'user_id': 'user_002'},
            {'type': 'circle', 'params': [-2.0], 'user_id': 'user_003'},  # Invalid
            {'type': 'rectangle', 'params': [3.0, 8.0], 'user_id': 'user_004'},
            {'type': 'square', 'params': [4.0], 'user_id': 'user_005'},
            {
                'type': 'invalid_shape',
                'params': [1.0],
                'user_id': 'user_006',
            },  # Invalid
        ]

        print(f'Processing {len(user_inputs)} user requests...')

        # Process each user input
        successful_calculations = []
        failed_calculations = []

        with measure_time('User request processing'):
            for request in user_inputs:
                user_id = request['user_id']
                shape_type = request['type']
                params = request['params']

                print(f'\nProcessing request from {user_id}:')
                print(f'  Requested: {shape_type} with params {params}')

                try:
                    # Validate and create shape
                    if shape_type == 'circle':
                        if len(params) == 1 and params[0] > 0:
                            shape = manager.create_circle(params[0])
                        else:
                            raise ValueError('Invalid circle parameters')
                    elif shape_type == 'rectangle':
                        if len(params) == 2 and all(p > 0 for p in params):
                            shape = manager.create_rectangle(params[0], params[1])
                        else:
                            raise ValueError('Invalid rectangle parameters')
                    elif shape_type == 'square':
                        if len(params) == 1 and params[0] > 0:
                            shape = manager.create_rectangle(params[0], params[0])
                        else:
                            raise ValueError('Invalid square parameters')
                    else:
                        raise ValueError(f'Unknown shape type: {shape_type}')

                    # Calculate properties
                    area = shape.get_area()
                    perimeter = shape.get_perimeter()
                    metrics = analyze_shape(shape)

                    # Store results
                    calculation_result = {
                        'user_id': user_id,
                        'shape_type': shape_type,
                        'params': params,
                        'area': area,
                        'perimeter': perimeter,
                        'efficiency': metrics.efficiency,
                        'aspect_ratio': metrics.aspect_ratio,
                        'status': 'success',
                    }

                    successful_calculations.append(calculation_result)

                    print(f'  ✓ Success: area={area:.2f}, perimeter={perimeter:.2f}')
                    print(f'    Efficiency: {metrics.efficiency:.4f}')

                except Exception as e:
                    error_result = {
                        'user_id': user_id,
                        'shape_type': shape_type,
                        'params': params,
                        'error': str(e),
                        'status': 'error',
                    }

                    failed_calculations.append(error_result)
                    print(f'  ✗ Error: {e}')

        # Generate summary report
        print(f'\n=== Summary Report ===')
        print(f'Total requests: {len(user_inputs)}')
        print(f'Successful: {len(successful_calculations)}')
        print(f'Failed: {len(failed_calculations)}')

        if successful_calculations:
            # Analyze successful calculations
            areas = [calc['area'] for calc in successful_calculations]
            perimeters = [calc['perimeter'] for calc in successful_calculations]
            efficiencies = [calc['efficiency'] for calc in successful_calculations]

            sort_inplace(areas)
            sort_inplace(efficiencies)

            print(f'\nSuccessful Calculations Analysis:')
            print(f'  Area range: {min(areas):.2f} - {max(areas):.2f}')
            print(f'  Average efficiency: {sum(efficiencies)/len(efficiencies):.4f}')
            print(f'  Most efficient: {max(efficiencies):.4f}')

            # Group by shape type
            shape_types = {}
            for calc in successful_calculations:
                shape_type = calc['shape_type']
                if shape_type not in shape_types:
                    shape_types[shape_type] = []
                shape_types[shape_type].append(calc)

            print(f'  By shape type:')
            for shape_type, calcs in shape_types.items():
                avg_area = sum(c['area'] for c in calcs) / len(calcs)
                print(
                    f'    {shape_type}: {len(calcs)} shapes, avg area: {avg_area:.2f}'
                )

        if failed_calculations:
            print(f'\nFailed Calculations:')
            for failure in failed_calculations:
                print(f'  {failure["user_id"]}: {failure["error"]}')

    print()


def main() -> int:
    """Run all integration examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Integration Example')
    print('==================')
    print()

    try:
        geometric_data_analysis()
        data_processing_pipeline()
        scientific_simulation()
        performance_benchmark_suite()
        real_world_application_demo()

        print('All integration examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running integration examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
