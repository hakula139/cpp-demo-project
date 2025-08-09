#!/usr/bin/env python3
"""Random module example demonstrating type-safe random number generation.

This example shows how to use the random module for generating random numbers,
working with distributions, and creating reproducible random sequences.
"""

import math
import sys
from pathlib import Path
from typing import Dict, List, Tuple

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from random import Normal, RandomGenerator, UniformInt, randint
from random import random as random_float
from random import sample, shuffle


def basic_random_demo() -> None:
    """Demonstrate basic random number generation.

    Shows how to create random generators and generate different
    types of random values with proper seeding for reproducibility.
    """
    print('=== Basic Random Demo ===')

    # Create random generator with seed for reproducible results
    gen = RandomGenerator(seed=42)

    print('Basic random value generation:')
    print(f'Random integer (1-100): {gen.randint(1, 100)}')
    print(f'Random float (0-1): {gen.random():.6f}')
    print(f'Random uniform (5-10): {gen.uniform(5.0, 10.0):.4f}')
    print(f'Random boolean: {gen.choice()}')
    print(f'Random boolean (70% chance): {gen.choice(0.7)}')
    print(f'Random normal (μ=0, σ=1): {gen.normal(mean=0.0, stddev=1.0):.4f}')

    # Generate with different seeds
    print(f'\nReproducibility with seeds:')

    generators = [
        RandomGenerator(seed=123),
        RandomGenerator(seed=123),
        RandomGenerator(seed=456),
    ]

    for i, generator in enumerate(generators):
        values = [generator.randint(1, 10) for _ in range(5)]
        print(f'  Generator {i+1} (seed {[123, 123, 456][i]}): {values}')

    # Unseeded generator (different each time)
    print(f'\nUnseeded generator (non-reproducible):')
    unseeded = RandomGenerator()
    unseeded_values = [unseeded.randint(1, 10) for _ in range(5)]
    print(f'  Unseeded values: {unseeded_values}')

    print()


def vector_generation_demo() -> None:
    """Demonstrate random vector generation.

    Shows how to generate arrays of random values efficiently
    for statistical analysis and data generation.
    """
    print('=== Vector Generation Demo ===')

    gen = RandomGenerator(seed=789)

    # Integer vectors
    print('Integer vector generation:')

    small_ints = gen.integers(1, 10, 8)
    large_ints = gen.integers(100, 1000, 5)
    negative_ints = gen.integers(-50, 50, 10)

    print(f'Small integers (1-10): {small_ints}')
    print(f'Large integers (100-1000): {large_ints}')
    print(f'Mixed integers (-50 to 50): {negative_ints}')

    # Float vectors
    print(f'\nFloat vector generation:')

    unit_floats = gen.floats(0.0, 1.0, 6)
    scaled_floats = gen.floats(10.0, 20.0, 5)
    precise_floats = gen.floats(-1.0, 1.0, 8)

    print(f'Unit floats (0-1): {[f"{x:.4f}" for x in unit_floats]}')
    print(f'Scaled floats (10-20): {[f"{x:.2f}" for x in scaled_floats]}')
    print(f'Precise floats (-1 to 1): {[f"{x:.6f}" for x in precise_floats]}')

    # Statistical properties
    print(f'\nStatistical properties of generated vectors:')

    large_sample = gen.floats(0.0, 1.0, 1000)

    mean = sum(large_sample) / len(large_sample)
    variance = sum((x - mean) ** 2 for x in large_sample) / len(large_sample)
    std_dev = math.sqrt(variance)

    print(f'Large sample (n=1000):')
    print(f'  Mean: {mean:.4f} (expected ~0.5)')
    print(f'  Std dev: {std_dev:.4f} (expected ~0.289)')
    print(f'  Min: {min(large_sample):.4f}')
    print(f'  Max: {max(large_sample):.4f}')

    print()


def distributions_demo() -> None:
    """Demonstrate different probability distributions.

    Shows how to use various distribution classes for generating
    random numbers from specific probability distributions.
    """
    print('=== Distributions Demo ===')

    gen = RandomGenerator(seed=456)

    # Uniform integer distribution
    print('Uniform integer distribution:')

    uniform_dist = UniformInt(gen, 10, 20)
    uniform_samples = uniform_dist.samples(10)

    print(f'  Range: 10-20')
    print(f'  Samples: {uniform_samples}')
    print(f'  Single sample: {uniform_dist.sample()}')

    # Normal distribution
    print(f'\nNormal distribution:')

    # Standard normal (μ=0, σ=1)
    standard_normal = Normal(gen, mean=0.0, stddev=1.0)
    std_samples = standard_normal.samples(8)

    print(f'  Standard normal (μ=0, σ=1):')
    print(f'    Samples: {[f"{x:.3f}" for x in std_samples]}')

    # Custom normal distribution
    custom_normal = Normal(gen, mean=100.0, stddev=15.0)
    custom_samples = custom_normal.samples(8)

    print(f'  Custom normal (μ=100, σ=15):')
    print(f'    Samples: {[f"{x:.1f}" for x in custom_samples]}')

    # Compare distribution properties
    print(f'\nDistribution properties analysis:')

    # Generate large samples for analysis
    large_uniform = uniform_dist.samples(1000)
    large_normal = standard_normal.samples(1000)

    # Uniform distribution analysis
    uniform_mean = sum(large_uniform) / len(large_uniform)
    uniform_range = max(large_uniform) - min(large_uniform)

    print(f'  Uniform distribution (n=1000):')
    print(f'    Mean: {uniform_mean:.2f} (expected: 15.0)')
    print(f'    Range: {uniform_range} (expected: 10)')
    print(f'    Min: {min(large_uniform)}, Max: {max(large_uniform)}')

    # Normal distribution analysis
    normal_mean = sum(large_normal) / len(large_normal)
    normal_var = sum((x - normal_mean) ** 2 for x in large_normal) / len(large_normal)
    normal_std = math.sqrt(normal_var)

    print(f'  Normal distribution (n=1000):')
    print(f'    Mean: {normal_mean:.4f} (expected: 0.0)')
    print(f'    Std dev: {normal_std:.4f} (expected: 1.0)')

    # Count values within 1, 2, 3 standard deviations
    within_1std = sum(1 for x in large_normal if abs(x) <= 1.0)
    within_2std = sum(1 for x in large_normal if abs(x) <= 2.0)
    within_3std = sum(1 for x in large_normal if abs(x) <= 3.0)

    print(f'    Within 1σ: {within_1std/10:.1f}% (expected: ~68%)')
    print(f'    Within 2σ: {within_2std/10:.1f}% (expected: ~95%)')
    print(f'    Within 3σ: {within_3std/10:.1f}% (expected: ~99.7%)')

    print()


def sampling_and_shuffling_demo() -> None:
    """Demonstrate sampling and shuffling operations.

    Shows how to randomly sample from collections and shuffle
    data while maintaining randomness properties.
    """
    print('=== Sampling and Shuffling Demo ===')

    # Create test data
    data = list(range(1, 21))  # 1 to 20
    names = ['Alice', 'Bob', 'Charlie', 'Diana', 'Eve', 'Frank', 'Grace', 'Henry']

    print(f'Original data: {data}')
    print(f'Original names: {names}')

    # Shuffling
    print(f'\nShuffling operations:')

    shuffled_data = data.copy()
    shuffle(shuffled_data)
    print(f'Shuffled numbers: {shuffled_data}')

    shuffled_names = names.copy()
    shuffle(shuffled_names)
    print(f'Shuffled names: {shuffled_names}')

    # Verify shuffling doesn't lose elements
    print(f'Data integrity check: {sorted(shuffled_data) == sorted(data)}')

    # Sampling without replacement
    print(f'\nSampling without replacement:')

    sample_sizes = [3, 5, 8]
    for size in sample_sizes:
        sampled = sample(data, size)
        print(f'  Sample {size} from numbers: {sorted(sampled)}')

    name_sample = sample(names, 4)
    print(f'  Sample 4 names: {name_sample}')

    # Multiple shuffles to show randomness
    print(f'\nMultiple shuffles (demonstrating randomness):')

    original = list(range(1, 11))
    shuffles = []

    for i in range(5):
        shuffled = original.copy()
        shuffle(shuffled)
        shuffles.append(shuffled)
        print(f'  Shuffle {i+1}: {shuffled}')

    # Check if all shuffles are different
    unique_shuffles = len(set(tuple(s) for s in shuffles))
    print(f'  Unique shuffles: {unique_shuffles}/{len(shuffles)}')

    print()


def statistical_simulation_demo() -> None:
    """Demonstrate statistical simulations and Monte Carlo methods.

    Shows how to use random generation for statistical simulations
    and Monte Carlo estimation techniques.
    """
    print('=== Statistical Simulation Demo ===')

    gen = RandomGenerator(seed=12345)

    # Dice rolling simulation
    print('Dice rolling simulation:')

    num_rolls = 10000
    dice_results = gen.integers(1, 6, num_rolls)

    # Count frequencies
    frequencies = {i: dice_results.count(i) for i in range(1, 7)}

    print(f'  {num_rolls} dice rolls:')
    for face, count in frequencies.items():
        percentage = (count / num_rolls) * 100
        print(f'    Face {face}: {count} times ({percentage:.1f}%)')

    # Expected is ~16.67% for fair dice
    expected_freq = num_rolls / 6
    chi_squared = sum(
        (count - expected_freq) ** 2 / expected_freq for count in frequencies.values()
    )
    print(f'    Chi-squared statistic: {chi_squared:.2f}')

    # Coin flipping simulation
    print(f'\nCoin flipping simulation:')

    num_flips = 5000
    heads_probability = 0.5

    heads_count = 0
    for _ in range(num_flips):
        if gen.choice(heads_probability):
            heads_count += 1

    heads_percentage = (heads_count / num_flips) * 100
    tails_count = num_flips - heads_count

    print(f'  {num_flips} coin flips:')
    print(f'    Heads: {heads_count} ({heads_percentage:.1f}%)')
    print(f'    Tails: {tails_count} ({100-heads_percentage:.1f}%)')

    # Monte Carlo π estimation
    print(f'\nMonte Carlo π estimation:')

    num_points = 100000
    inside_circle = 0

    # Generate random points in unit square and count those inside unit circle
    for _ in range(num_points):
        x = gen.uniform(-1.0, 1.0)
        y = gen.uniform(-1.0, 1.0)
        if x * x + y * y <= 1.0:
            inside_circle += 1

    pi_estimate = 4.0 * inside_circle / num_points
    error = abs(pi_estimate - math.pi)

    print(f'  Points tested: {num_points}')
    print(f'  Points inside circle: {inside_circle}')
    print(f'  Estimated π: {pi_estimate:.6f}')
    print(f'  Actual π: {math.pi:.6f}')
    print(f'  Error: {error:.6f}')

    # Random walk simulation
    print(f'\nRandom walk simulation:')

    steps = 1000
    position = 0
    positions = [position]

    for _ in range(steps):
        step = 1 if gen.choice() else -1
        position += step
        positions.append(position)

    max_pos = max(positions)
    min_pos = min(positions)
    final_pos = positions[-1]

    print(f'  Steps: {steps}')
    print(f'  Final position: {final_pos}')
    print(f'  Max position: {max_pos}')
    print(f'  Min position: {min_pos}')
    print(f'  Range: {max_pos - min_pos}')

    print()


def data_generation_demo() -> None:
    """Demonstrate random data generation for testing.

    Shows how to generate realistic test data for various
    applications including names, coordinates, and datasets.
    """
    print('=== Data Generation Demo ===')

    gen = RandomGenerator(seed=999)

    # Generate test coordinates
    print('Random coordinate generation:')

    # 2D coordinates
    num_points = 8
    coordinates_2d = [
        (gen.uniform(-10.0, 10.0), gen.uniform(-10.0, 10.0)) for _ in range(num_points)
    ]

    print(f'  2D coordinates:')
    for i, (x, y) in enumerate(coordinates_2d):
        print(f'    Point {i+1}: ({x:.2f}, {y:.2f})')

    # 3D coordinates
    coordinates_3d = [
        (gen.uniform(-5.0, 5.0), gen.uniform(-5.0, 5.0), gen.uniform(-5.0, 5.0))
        for _ in range(5)
    ]

    print(f'  3D coordinates:')
    for i, (x, y, z) in enumerate(coordinates_3d):
        print(f'    Point {i+1}: ({x:.2f}, {y:.2f}, {z:.2f})')

    # Generate test datasets
    print(f'\nTest dataset generation:')

    # Student grades dataset
    num_students = 10
    subjects = ['Math', 'Science', 'English', 'History']

    print(f'  Student grades dataset:')
    for i in range(num_students):
        student_id = f'S{i+1:03d}'
        grades = {subject: gen.randint(60, 100) for subject in subjects}
        avg_grade = sum(grades.values()) / len(grades)
        print(f'    {student_id}: {grades} (avg: {avg_grade:.1f})')

    # Sales data simulation
    print(f'\nSales data simulation:')

    months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun']
    products = ['Widget A', 'Widget B', 'Widget C']

    for month in months:
        monthly_sales = {}
        for product in products:
            # Simulate seasonal variation
            base_sales = gen.randint(100, 500)
            seasonal_factor = gen.uniform(0.8, 1.2)
            sales = int(base_sales * seasonal_factor)
            monthly_sales[product] = sales

        total_sales = sum(monthly_sales.values())
        print(f'  {month}: {monthly_sales} (total: {total_sales})')

    # Random configuration generation
    print(f'\nRandom configuration generation:')

    config_templates = [
        {'name': 'timeout', 'type': 'int', 'range': (1, 60)},
        {'name': 'retry_count', 'type': 'int', 'range': (1, 5)},
        {'name': 'success_rate', 'type': 'float', 'range': (0.8, 1.0)},
        {'name': 'buffer_size', 'type': 'int', 'range': (1024, 8192)},
        {'name': 'compression', 'type': 'bool', 'range': None},
    ]

    num_configs = 3
    for i in range(num_configs):
        config = {}
        for template in config_templates:
            name = template['name']
            if template['type'] == 'int':
                min_val, max_val = template['range']
                config[name] = gen.randint(min_val, max_val)
            elif template['type'] == 'float':
                min_val, max_val = template['range']
                config[name] = gen.uniform(min_val, max_val)
            elif template['type'] == 'bool':
                config[name] = gen.choice()

        print(f'  Config {i+1}: {config}')

    print()


def convenience_functions_demo() -> None:
    """Demonstrate convenience functions for quick random generation.

    Shows how to use module-level functions for simple random
    generation tasks without creating generator instances.
    """
    print('=== Convenience Functions Demo ===')

    print('Module-level convenience functions:')

    # Quick random generation without explicit generator
    print(f'Quick randint (1-10): {randint(1, 10)}')
    print(f'Quick random float: {random_float():.6f}')

    # Generate multiple values quickly
    quick_ints = [randint(1, 100) for _ in range(5)]
    quick_floats = [random_float() for _ in range(5)]

    print(f'Quick integers: {quick_ints}')
    print(f'Quick floats: {[f"{x:.4f}" for x in quick_floats]}')

    # Quick shuffling and sampling
    test_list = list(range(1, 11))
    print(f'Original list: {test_list}')

    shuffled_copy = test_list.copy()
    shuffle(shuffled_copy)
    print(f'Shuffled: {shuffled_copy}')

    sampled = sample(test_list, 4)
    print(f'Sample of 4: {sampled}')

    # Compare with generator approach
    print(f'\nComparison with explicit generator:')

    gen = RandomGenerator(seed=42)

    print('Convenience functions (different each time):')
    for i in range(3):
        values = [randint(1, 6) for _ in range(3)]
        print(f'  Run {i+1}: {values}')

    print('Generator with fixed seed (reproducible):')
    for i in range(3):
        gen = RandomGenerator(seed=42)  # Reset with same seed
        values = [gen.randint(1, 6) for _ in range(3)]
        print(f'  Run {i+1}: {values}')

    print()


def main() -> int:
    """Run all random generation examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Random Module Example')
    print('====================')
    print()

    try:
        basic_random_demo()
        vector_generation_demo()
        distributions_demo()
        sampling_and_shuffling_demo()
        statistical_simulation_demo()
        data_generation_demo()
        convenience_functions_demo()

        print('All random generation examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running random examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
