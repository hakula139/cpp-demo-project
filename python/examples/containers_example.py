#!/usr/bin/env python3
"""Containers module example demonstrating type-safe container operations.

This example shows how to use the container wrapper for type-safe operations
including filtering, transformation, and collection management.
"""

import sys
from pathlib import Path
from typing import Any, Callable, List

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from containers import Container, create_container


def basic_container_demo() -> None:
    """Demonstrate basic container creation and operations.

    Shows fundamental container operations including creation, addition,
    removal, and basic iteration.
    """
    print('=== Basic Container Demo ===')

    # Create containers with different types
    int_container = create_container([1, 3, 2, 5, 4])
    str_container = Container(str, ['apple', 'banana', 'cherry'])

    print(f'Integer container: {list(int_container)}')
    print(f'String container: {list(str_container)}')

    # Add and remove items
    int_container.add(6)
    removed = int_container.remove(3)
    print(f'After adding 6 and removing 3 (removed {removed}): {list(int_container)}')

    # Container properties
    print(f'Integer container size: {len(int_container)}')
    print(f'Is empty: {int_container.is_empty()}')
    print(f'Contains 5: {int_container.contains(5)}')

    print()


def filtering_demo() -> None:
    """Demonstrate container filtering capabilities.

    Shows various filtering patterns and how to chain filter operations
    for complex data selection.
    """
    print('=== Filtering Demo ===')

    # Create test data
    numbers = create_container(list(range(1, 21)))  # 1 to 20
    words = Container(str, ['apple', 'banana', 'cherry', 'date', 'elderberry', 'fig'])

    # Numeric filtering
    even_numbers = numbers.filter(lambda x: x % 2 == 0)
    large_numbers = numbers.filter(lambda x: x > 15)
    divisible_by_three = numbers.filter(lambda x: x % 3 == 0)

    print(f'Original numbers: {list(numbers)}')
    print(f'Even numbers: {even_numbers}')
    print(f'Large numbers (>15): {large_numbers}')
    print(f'Divisible by 3: {divisible_by_three}')

    # String filtering
    long_words = words.filter(lambda s: len(s) > 5)
    words_with_e = words.filter(lambda s: 'e' in s)
    words_starting_with_c = words.filter(lambda s: s.startswith('c'))

    print(f'\nOriginal words: {list(words)}')
    print(f'Long words (>5 chars): {long_words}')
    print(f'Words containing "e": {words_with_e}')
    print(f'Words starting with "c": {words_starting_with_c}')

    # Complex filtering with multiple conditions
    complex_filter = numbers.filter(lambda x: x % 2 == 0 and x > 10)
    print(f'Even numbers > 10: {complex_filter}')

    print()


def transformation_demo() -> None:
    """Demonstrate container transformation capabilities.

    Shows how to transform container elements using mapping functions
    and create new containers with modified data.
    """
    print('=== Transformation Demo ===')

    # Numeric transformations
    numbers = create_container([1, 2, 3, 4, 5])

    squared = numbers.transform(lambda x: x * x)
    doubled = numbers.transform(lambda x: x * 2)
    negated = numbers.transform(lambda x: -x)

    print(f'Original: {list(numbers)}')
    print(f'Squared: {squared}')
    print(f'Doubled: {doubled}')
    print(f'Negated: {negated}')

    # String transformations
    words = Container(str, ['hello', 'world', 'python', 'container'])

    lengths = words.transform(len)
    uppercase = words.transform(str.upper)
    reversed_words = words.transform(lambda s: s[::-1])
    first_chars = words.transform(lambda s: s[0] if s else '')

    print(f'\nOriginal words: {list(words)}')
    print(f'Word lengths: {lengths}')
    print(f'Uppercase: {uppercase}')
    print(f'Reversed: {reversed_words}')
    print(f'First characters: {first_chars}')

    # Complex transformations
    numbers_to_strings = numbers.transform(lambda x: f'Number: {x}')
    print(f'Complex transformation: {numbers_to_strings}')

    print()


def chained_operations_demo() -> None:
    """Demonstrate chaining of container operations.

    Shows how to combine filtering and transformation operations
    to create complex data processing pipelines.
    """
    print('=== Chained Operations Demo ===')

    # Create sample data
    data = create_container(list(range(1, 11)))
    print(f'Original data: {list(data)}')

    # Chain operations step by step
    step1 = data.filter(lambda x: x % 2 == 1)  # Keep odd numbers
    step2 = step1.transform(lambda x: x * x)  # Square them
    step3 = Container(int, step2).filter(lambda x: x < 50)  # Keep those < 50

    print(f'Step 1 - Odd numbers: {step1}')
    print(f'Step 2 - Squared: {step2}')
    print(f'Step 3 - Squares < 50: {step3}')

    # Working with strings - pipeline processing
    text_data = Container(
        str, ['The', 'quick', 'brown', 'fox', 'jumps', 'over', 'lazy', 'dog']
    )

    # Process: keep words > 3 chars, convert to uppercase, get lengths
    processed_text = text_data.filter(lambda w: len(w) > 3)
    upper_text = Container(str, processed_text).transform(str.upper)
    final_lengths = Container(str, upper_text).transform(len)

    print(f'\nText processing pipeline:')
    print(f'Original: {list(text_data)}')
    print(f'Words > 3 chars: {processed_text}')
    print(f'Uppercase: {upper_text}')
    print(f'Final lengths: {final_lengths}')

    print()


def type_safety_demo() -> None:
    """Demonstrate type safety features of containers.

    Shows how containers maintain type safety and provide
    compile-time and runtime type checking capabilities.
    """
    print('=== Type Safety Demo ===')

    # Homogeneous containers
    int_container = Container(int, [1, 2, 3])
    str_container = Container(str, ['a', 'b', 'c'])
    float_container = Container(float, [1.1, 2.2, 3.3])

    print(f'Integer container: {list(int_container)}')
    print(f'String container: {list(str_container)}')
    print(f'Float container: {list(float_container)}')

    # Type-specific operations
    print(f'\nType-specific operations:')

    # Integer operations
    int_sum = sum(int_container)
    int_max = max(int_container)
    print(f'Integer sum: {int_sum}, max: {int_max}')

    # String operations
    str_joined = ' '.join(str_container)
    str_total_length = sum(len(s) for s in str_container)
    print(f'Joined strings: "{str_joined}", total length: {str_total_length}')

    # Float operations with precision
    float_avg = sum(float_container) / len(float_container)
    print(f'Float average: {float_avg:.3f}')

    # Demonstrate type preservation in transformations
    int_squared = int_container.transform(lambda x: x * x)
    str_lengths = str_container.transform(len)
    float_rounded = float_container.transform(lambda x: round(x, 1))

    print(f'\nType-preserving transformations:')
    print(f'Integer squared: {int_squared}')
    print(f'String lengths: {str_lengths}')
    print(f'Float rounded: {float_rounded}')

    print()


def advanced_operations_demo() -> None:
    """Demonstrate advanced container operations.

    Shows complex use cases including batch processing, conditional
    operations, and advanced filtering patterns.
    """
    print('=== Advanced Operations Demo ===')

    # Batch processing with multiple containers
    datasets = [
        create_container([1, 2, 3, 4, 5]),
        create_container([6, 7, 8, 9, 10]),
        create_container([11, 12, 13, 14, 15]),
    ]

    print('Batch processing multiple containers:')
    for i, dataset in enumerate(datasets, 1):
        processed = dataset.filter(lambda x: x % 2 == 0).transform(lambda x: x * 2)
        print(f'  Dataset {i}: {list(dataset)} -> {processed}')

    # Conditional operations
    mixed_data = create_container([-3, -1, 0, 2, 5, 8, 12])

    # Separate positive and negative numbers
    positive = mixed_data.filter(lambda x: x > 0)
    negative = mixed_data.filter(lambda x: x < 0)
    zero_or_positive = mixed_data.filter(lambda x: x >= 0)

    print(f'\nConditional separation:')
    print(f'Original: {list(mixed_data)}')
    print(f'Positive: {positive}')
    print(f'Negative: {negative}')
    print(f'Zero or positive: {zero_or_positive}')

    # Statistical operations
    stats_data = create_container([1, 2, 2, 3, 4, 4, 4, 5, 6])

    unique_values = list(set(stats_data))
    value_counts = {val: list(stats_data).count(val) for val in unique_values}

    print(f'\nStatistical analysis:')
    print(f'Data: {list(stats_data)}')
    print(f'Unique values: {sorted(unique_values)}')
    print(f'Value counts: {value_counts}')

    # Find most frequent value
    most_frequent = max(value_counts.items(), key=lambda x: x[1])
    print(f'Most frequent value: {most_frequent[0]} (appears {most_frequent[1]} times)')

    print()


def performance_demo() -> None:
    """Demonstrate performance characteristics of container operations.

    Shows how different operations scale and provides insights into
    performance considerations for large datasets.
    """
    print('=== Performance Demo ===')

    # Create larger datasets for performance testing
    small_data = create_container(list(range(100)))
    medium_data = create_container(list(range(1000)))
    large_data = create_container(list(range(10000)))

    datasets = [
        ('Small (100 elements)', small_data),
        ('Medium (1000 elements)', medium_data),
        ('Large (10000 elements)', large_data),
    ]

    print('Performance comparison across dataset sizes:')

    for name, dataset in datasets:
        # Time filtering operation
        import time

        start_time = time.perf_counter()
        filtered = dataset.filter(lambda x: x % 10 == 0)
        filter_time = time.perf_counter() - start_time

        start_time = time.perf_counter()
        transformed = dataset.transform(lambda x: x * 2)
        transform_time = time.perf_counter() - start_time

        print(f'  {name}:')
        print(f'    Filter time: {filter_time:.6f}s, result size: {len(filtered)}')
        print(
            f'    Transform time: {transform_time:.6f}s, result size: {len(transformed)}'
        )

    # Memory efficiency demonstration
    print(f'\nMemory efficiency:')
    efficient_chain = large_data.filter(lambda x: x % 100 == 0).transform(
        lambda x: x // 100
    )
    print(
        f'Chained operations on large dataset: {len(efficient_chain)} elements processed'
    )

    print()


def main() -> int:
    """Run all container examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Containers Module Example')
    print('========================')
    print()

    try:
        basic_container_demo()
        filtering_demo()
        transformation_demo()
        chained_operations_demo()
        type_safety_demo()
        advanced_operations_demo()
        performance_demo()

        print('All container examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running container examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
