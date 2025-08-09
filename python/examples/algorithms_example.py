#!/usr/bin/env python3
"""Algorithms module example demonstrating functional programming and data processing.

This example shows how to use the algorithms module for sorting, transforming,
filtering, and functional programming patterns with C++ performance.
"""

import sys
from pathlib import Path
from typing import Any, Callable, List, Tuple

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from algorithms import (
    count_if,
    find_min_max,
    functional_chain,
    pipeline,
    sort_inplace,
    transform,
)


def sorting_demo() -> None:
    """Demonstrate sorting capabilities with various data types.

    Shows in-place sorting of different data structures and
    performance characteristics with various input patterns.
    """
    print('=== Sorting Demo ===')

    # Basic sorting examples
    random_data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
    print(f'Original data: {random_data}')

    sort_inplace(random_data)
    print(f'Sorted data: {random_data}')

    # Different data patterns
    test_cases = {
        'Random integers': [64, 34, 25, 12, 22, 11, 90],
        'Already sorted': [1, 2, 3, 4, 5, 6, 7],
        'Reverse sorted': [7, 6, 5, 4, 3, 2, 1],
        'Mostly sorted': [1, 2, 3, 5, 4, 6, 7],
        'Duplicates': [5, 2, 8, 2, 9, 1, 5, 5],
        'Single element': [42],
        'Empty list': [],
    }

    print(f'\nSorting different patterns:')
    for description, data in test_cases.items():
        original = data.copy()
        sort_inplace(data)
        print(f'  {description}: {original} -> {data}')

    # Float sorting
    float_data = [3.14, 2.71, 1.41, 1.73, 0.57]
    print(f'\nFloat sorting: {float_data}')
    sort_inplace(float_data)
    print(f'Sorted floats: {float_data}')

    print()


def transformation_demo() -> None:
    """Demonstrate data transformation operations.

    Shows how to apply functions to transform data collections
    with various mathematical and string operations.
    """
    print('=== Transformation Demo ===')

    # Numeric transformations
    numbers = [1, 2, 3, 4, 5]

    squared = transform(numbers, lambda x: x * x)
    cubed = transform(numbers, lambda x: x**3)
    doubled_plus_one = transform(numbers, lambda x: x * 2 + 1)

    print(f'Original numbers: {numbers}')
    print(f'Squared: {squared}')
    print(f'Cubed: {cubed}')
    print(f'Doubled + 1: {doubled_plus_one}')

    # String transformations
    words = ['hello', 'world', 'python', 'algorithms']

    lengths = transform(words, len)
    uppercase = transform(words, str.upper)
    first_chars = transform(words, lambda s: s[0] if s else '')
    reversed_words = transform(words, lambda s: s[::-1])

    print(f'\nString transformations:')
    print(f'Original: {words}')
    print(f'Lengths: {lengths}')
    print(f'Uppercase: {uppercase}')
    print(f'First chars: {first_chars}')
    print(f'Reversed: {reversed_words}')

    # Complex transformations
    coordinates = [(1, 2), (3, 4), (5, 6)]
    distances = transform(coordinates, lambda p: (p[0] ** 2 + p[1] ** 2) ** 0.5)

    print(f'\nComplex transformations:')
    print(f'Coordinates: {coordinates}')
    print(f'Distances from origin: {[f"{d:.2f}" for d in distances]}')

    print()


def filtering_and_counting_demo() -> None:
    """Demonstrate filtering and counting operations.

    Shows conditional counting and advanced filtering patterns
    for data analysis and selection.
    """
    print('=== Filtering and Counting Demo ===')

    # Basic counting
    data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    even_count = count_if(data, lambda x: x % 2 == 0)
    odd_count = count_if(data, lambda x: x % 2 == 1)
    large_count = count_if(data, lambda x: x > 5)
    perfect_squares = count_if(data, lambda x: int(x**0.5) ** 2 == x)

    print(f'Data: {data}')
    print(f'Even numbers: {even_count}')
    print(f'Odd numbers: {odd_count}')
    print(f'Numbers > 5: {large_count}')
    print(f'Perfect squares: {perfect_squares}')

    # String filtering and counting
    text_data = ['apple', 'banana', 'cherry', 'date', 'elderberry', 'fig']

    long_words = count_if(text_data, lambda s: len(s) > 5)
    words_with_a = count_if(text_data, lambda s: 'a' in s)
    short_words = count_if(text_data, lambda s: len(s) <= 4)

    print(f'\nText analysis:')
    print(f'Words: {text_data}')
    print(f'Long words (>5 chars): {long_words}')
    print(f'Words containing "a": {words_with_a}')
    print(f'Short words (<=4 chars): {short_words}')

    # Complex conditions
    numbers_data = [15, 23, 8, 42, 16, 4, 35, 19, 27]

    divisible_by_3_and_odd = count_if(numbers_data, lambda x: x % 3 == 0 and x % 2 == 1)
    between_10_and_30 = count_if(numbers_data, lambda x: 10 <= x <= 30)

    print(f'\nComplex conditions:')
    print(f'Numbers: {numbers_data}')
    print(f'Divisible by 3 AND odd: {divisible_by_3_and_odd}')
    print(f'Between 10 and 30: {between_10_and_30}')

    print()


def min_max_analysis_demo() -> None:
    """Demonstrate min/max finding and statistical analysis.

    Shows efficient finding of extrema and basic statistical
    operations on data collections.
    """
    print('=== Min/Max Analysis Demo ===')

    # Basic min/max
    test_datasets = [
        [64, 34, 25, 12, 22, 11, 90],
        [3.14, 2.71, 1.41, 1.73, 0.57, 4.47],
        [-5, -2, 10, 3, -8, 15],
        [100],  # Single element
    ]

    print('Min/Max analysis for different datasets:')
    for i, dataset in enumerate(test_datasets, 1):
        min_val, max_val = find_min_max(dataset)
        range_val = max_val - min_val
        print(f'  Dataset {i}: {dataset}')
        print(f'    Min: {min_val}, Max: {max_val}, Range: {range_val}')

    # Statistical analysis using min/max
    scores = [85, 92, 78, 96, 88, 76, 94, 89, 91, 83]
    min_score, max_score = find_min_max(scores)

    print(f'\nTest scores analysis:')
    print(f'Scores: {scores}')
    print(f'Lowest score: {min_score}')
    print(f'Highest score: {max_score}')
    print(f'Score range: {max_score - min_score}')

    # Performance categories
    high_performers = count_if(scores, lambda x: x >= 90)
    low_performers = count_if(scores, lambda x: x < 80)

    print(f'High performers (>=90): {high_performers}')
    print(f'Low performers (<80): {low_performers}')

    # Finding indices of extrema (custom analysis)
    max_index = scores.index(max_score)
    min_index = scores.index(min_score)

    print(f'Best score position: {max_index + 1}')
    print(f'Worst score position: {min_index + 1}')

    print()


def functional_chain_demo() -> None:
    """Demonstrate functional programming chains.

    Shows how to chain operations together for elegant
    data processing pipelines with lazy evaluation.
    """
    print('=== Functional Chain Demo ===')

    # Basic functional chain
    data = list(range(1, 21))  # 1 to 20

    result = (
        functional_chain(data)
        .filter(lambda x: x % 2 == 0)  # Even numbers
        .map(lambda x: x * x)  # Square them
        .filter(lambda x: x < 100)  # Keep those < 100
        .take(5)  # Take first 5
        .collect()
    )

    print(f'Basic chain example:')
    print(f'  Input: {data}')
    print(f'  Chain: filter(even) -> map(square) -> filter(<100) -> take(5)')
    print(f'  Result: {result}')

    # Complex functional chain with strings
    words = ['The', 'quick', 'brown', 'fox', 'jumps', 'over', 'the', 'lazy', 'dog']

    processed = (
        functional_chain(words)
        .filter(lambda w: len(w) > 3)  # Words longer than 3 chars
        .map(str.lower)  # Convert to lowercase
        .filter(lambda w: 'o' in w)  # Contains 'o'
        .map(lambda w: w.upper())  # Convert to uppercase
        .sort()  # Sort alphabetically
        .collect()
    )

    print(f'\nString processing chain:')
    print(f'  Input: {words}')
    print(f'  Chain: filter(len>3) -> lower -> filter(contains "o") -> upper -> sort')
    print(f'  Result: {processed}')

    # Mathematical processing chain
    numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    math_result = (
        functional_chain(numbers)
        .filter(lambda x: x % 2 == 1)  # Odd numbers
        .map(lambda x: x**2)  # Square them
        .map(lambda x: x + 1)  # Add 1
        .filter(lambda x: x < 50)  # Keep < 50
        .sort(reverse=True)  # Sort descending
        .take(3)  # Take top 3
        .collect()
    )

    print(f'\nMathematical chain:')
    print(f'  Input: {numbers}')
    print(
        f'  Chain: filter(odd) -> square -> add(1) -> filter(<50) -> sort(desc) -> take(3)'
    )
    print(f'  Result: {math_result}')

    print()


def pipeline_composition_demo() -> None:
    """Demonstrate pipeline composition patterns.

    Shows how to compose reusable processing pipelines
    for modular and maintainable data transformations.
    """
    print('=== Pipeline Composition Demo ===')

    # Simple pipeline
    data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    simple_pipeline = pipeline(
        lambda lst: [x for x in lst if x > 3],  # Filter > 3
        lambda lst: [x * 2 for x in lst],  # Double
        lambda lst: [x + 1 for x in lst],  # Add 1
        sum,  # Sum all
    )

    result = simple_pipeline(data)
    print(f'Simple pipeline:')
    print(f'  Input: {data}')
    print(f'  Pipeline: filter(>3) -> double -> add(1) -> sum')
    print(f'  Result: {result}')

    # Text processing pipeline
    text_pipeline = pipeline(
        lambda words: [w for w in words if len(w) > 2],  # Filter short words
        lambda words: [w.title() for w in words],  # Title case
        lambda words: [w for w in words if w[0] in 'AEIOU'],  # Starts with vowel
        lambda words: ' '.join(words),  # Join to string
    )

    text_data = ['a', 'quick', 'brown', 'fox', 'jumps', 'over', 'an', 'excited', 'dog']
    text_result = text_pipeline(text_data)

    print(f'\nText processing pipeline:')
    print(f'  Input: {text_data}')
    print(f'  Pipeline: filter(len>2) -> title_case -> filter(starts_vowel) -> join')
    print(f'  Result: "{text_result}"')

    # Mathematical analysis pipeline
    analysis_pipeline = pipeline(
        lambda nums: [x for x in nums if x != 0],  # Remove zeros
        lambda nums: [abs(x) for x in nums],  # Absolute values
        lambda nums: sorted(nums),  # Sort
        lambda nums: {
            'count': len(nums),
            'min': min(nums) if nums else 0,
            'max': max(nums) if nums else 0,
            'sum': sum(nums),
            'avg': sum(nums) / len(nums) if nums else 0,
        },
    )

    math_data = [-5, 3, 0, -2, 8, 0, 4, -1, 6]
    math_result = analysis_pipeline(math_data)

    print(f'\nMathematical analysis pipeline:')
    print(f'  Input: {math_data}')
    print(f'  Pipeline: remove_zeros -> abs -> sort -> statistics')
    print(f'  Result: {math_result}')

    # Reusable pipeline components
    def filter_positive(lst):
        return [x for x in lst if x > 0]

    def square_elements(lst):
        return [x * x for x in lst]

    def take_top_n(n):
        return lambda lst: sorted(lst, reverse=True)[:n]

    modular_pipeline = pipeline(filter_positive, square_elements, take_top_n(3))

    mixed_data = [-3, 5, -1, 8, 2, -4, 9, 1]
    modular_result = modular_pipeline(mixed_data)

    print(f'\nModular pipeline:')
    print(f'  Input: {mixed_data}')
    print(f'  Pipeline: filter_positive -> square -> take_top_3')
    print(f'  Result: {modular_result}')

    print()


def performance_comparison_demo() -> None:
    """Demonstrate performance characteristics of different approaches.

    Shows timing comparisons between functional and imperative
    approaches for various data processing tasks.
    """
    print('=== Performance Comparison Demo ===')

    import time

    # Generate test data
    large_data = list(range(1, 10001))  # 1 to 10000

    print(f'Performance comparison on {len(large_data)} elements:')

    # Functional approach with chains
    start_time = time.perf_counter()
    functional_result = (
        functional_chain(large_data)
        .filter(lambda x: x % 2 == 0)
        .map(lambda x: x * x)
        .filter(lambda x: x < 1000000)
        .take(100)
        .collect()
    )
    functional_time = time.perf_counter() - start_time

    # Imperative approach
    start_time = time.perf_counter()
    imperative_result = []
    for x in large_data:
        if x % 2 == 0:
            squared = x * x
            if squared < 1000000:
                imperative_result.append(squared)
                if len(imperative_result) >= 100:
                    break
    imperative_time = time.perf_counter() - start_time

    # Pipeline approach
    start_time = time.perf_counter()
    pipeline_proc = pipeline(
        lambda lst: [x for x in lst if x % 2 == 0],
        lambda lst: [x * x for x in lst],
        lambda lst: [x for x in lst if x < 1000000],
        lambda lst: lst[:100],
    )
    pipeline_result = pipeline_proc(large_data)
    pipeline_time = time.perf_counter() - start_time

    print(
        f'  Functional chain: {functional_time:.6f}s, result size: {len(functional_result)}'
    )
    print(
        f'  Imperative loop:  {imperative_time:.6f}s, result size: {len(imperative_result)}'
    )
    print(
        f'  Pipeline:         {pipeline_time:.6f}s, result size: {len(pipeline_result)}'
    )

    # Verify results are equivalent
    results_match = functional_result == imperative_result == pipeline_result
    print(f'  Results match: {results_match}')

    if results_match:
        fastest = min(
            [
                ('Functional', functional_time),
                ('Imperative', imperative_time),
                ('Pipeline', pipeline_time),
            ],
            key=lambda x: x[1],
        )
        print(f'  Fastest approach: {fastest[0]} ({fastest[1]:.6f}s)')

    print()


def main() -> int:
    """Run all algorithm examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Algorithms Module Example')
    print('========================')
    print()

    try:
        sorting_demo()
        transformation_demo()
        filtering_and_counting_demo()
        min_max_analysis_demo()
        functional_chain_demo()
        pipeline_composition_demo()
        performance_comparison_demo()

        print('All algorithm examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running algorithm examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
