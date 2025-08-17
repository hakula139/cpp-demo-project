"""Example demonstrating the usage of the algorithms module."""

from demo.algorithms import (
    count_if,
    find_min_max,
    pipeline,
    sort_inplace,
    transform_to_list,
)
from demo.containers import Container


def main() -> None:
    """Run all algorithm examples."""
    print('=== Algorithms Module Example ===')

    # Demonstrate sort_inplace with integers
    numbers = Container(int, [42, 17, 89, 3, 56, 23, 78, 12, 95, 34])
    print(f'Original numbers: {numbers}')

    sort_inplace(numbers)
    print(f'Sorted numbers: {numbers}')

    # Demonstrate count_if
    even_count = count_if(numbers, lambda n: n % 2 == 0)
    print(f'Count of even numbers: {even_count}')

    large_count = count_if(numbers, lambda n: n > 50)
    print(f'Count of numbers > 50: {large_count}')

    # Demonstrate transform_to_list
    squared = transform_to_list(numbers, lambda n: n * n)
    print(f'Squared numbers: {squared}')

    # Demonstrate find_min_max
    min_val, max_val = find_min_max(numbers)
    print(f'Min: {min_val}, Max: {max_val}')

    # Demonstrate with strings
    words = Container(str, ['cherry', 'banana', 'elderberry', 'date', 'apple'])
    print(f'Original words: {words}')

    sort_inplace(words)
    print(f'Sorted words: {words}')

    long_words = count_if(words, lambda word: len(word) > 5)
    print(f'Count of words > 5 characters: {long_words}')

    uppercased = transform_to_list(words, lambda word: word.upper())
    print(f'Uppercased words: {uppercased}')

    min_word, max_word = find_min_max(words)
    print(f'Min: {min_word}, Max: {max_word} (Lexicographically)')

    # Demonstrate pipeline
    process = pipeline(
        lambda data: transform_to_list(data, lambda x: x * x),
        find_min_max,
    )
    pipeline_result = process(numbers)
    print(f'Pipeline result: {pipeline_result}')

    print('=== Algorithms Module Example Completed ===')


if __name__ == '__main__':
    main()
