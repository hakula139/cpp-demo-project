"""Tests for Python algorithms module.

Comprehensive tests following C++ test patterns for algorithm functionality.
"""

import pytest

from python import algorithms, containers


class TestSortInplace:
    """Test in-place sorting functionality."""

    def test_sort_empty_list(self) -> None:
        """Test sorting empty list."""
        data: list[int] = []
        algorithms.sort_inplace(data)
        assert data == []

    def test_sort_single_element(self) -> None:
        """Test sorting single element list."""
        data = [42]
        algorithms.sort_inplace(data)
        assert data == [42]

    def test_sort_integers(self) -> None:
        """Test sorting integers in ascending order."""
        data = [42, 17, 89, 3, 56, 23, 78, 12, 95, 34]
        algorithms.sort_inplace(data)
        expected = [3, 12, 17, 23, 34, 42, 56, 78, 89, 95]
        assert data == expected

    def test_sort_strings(self) -> None:
        """Test sorting strings lexicographically."""
        data = ['cherry', 'banana', 'elderberry', 'date', 'apple']
        algorithms.sort_inplace(data)
        expected = ['apple', 'banana', 'cherry', 'date', 'elderberry']
        assert data == expected

    def test_sort_already_sorted(self) -> None:
        """Test sorting already sorted list."""
        data = [1, 2, 3, 4, 5]
        algorithms.sort_inplace(data)
        assert data == [1, 2, 3, 4, 5]

    def test_sort_reverse_sorted(self) -> None:
        """Test sorting reverse sorted list."""
        data = [5, 4, 3, 2, 1]
        algorithms.sort_inplace(data)
        assert data == [1, 2, 3, 4, 5]

    def test_sort_with_duplicates(self) -> None:
        """Test sorting list with duplicates."""
        data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
        algorithms.sort_inplace(data)
        expected = [1, 1, 2, 3, 3, 4, 5, 5, 6, 9]
        assert data == expected

    def test_sort_container(self) -> None:
        """Test sorting container."""
        container = containers.Container(int, [3, 1, 4, 1, 5])
        algorithms.sort_inplace(container)
        assert list(container) == [1, 1, 3, 4, 5]

    def test_sort_unsupported_type(self) -> None:
        """Test sorting unsupported type raises error."""
        with pytest.raises(TypeError, match='Unsupported type for sorting'):
            algorithms.sort_inplace({'a': 1, 'b': 2})


class TestCountIf:
    """Test counting with predicates."""

    def test_count_empty_container(self) -> None:
        """Test counting in empty container."""
        result = algorithms.count_if([], lambda x: x > 0)
        assert result == 0

    def test_count_even_numbers(self) -> None:
        """Test counting even numbers."""
        data = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        even_count = algorithms.count_if(data, lambda x: x % 2 == 0)
        assert even_count == 4

    def test_count_odd_numbers(self) -> None:
        """Test counting odd numbers."""
        data = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        odd_count = algorithms.count_if(data, lambda x: x % 2 == 1)
        assert odd_count == 5

    def test_count_greater_than_threshold(self) -> None:
        """Test counting numbers greater than threshold."""
        data = [10, 25, 30, 45, 50, 75, 80, 95]
        large_count = algorithms.count_if(data, lambda x: x > 50)
        assert large_count == 3

    def test_count_no_matches(self) -> None:
        """Test counting with no matches."""
        data = [1, 3, 5, 7, 9]
        even_count = algorithms.count_if(data, lambda x: x % 2 == 0)
        assert even_count == 0

    def test_count_all_matches(self) -> None:
        """Test counting with all matches."""
        data = [2, 4, 6, 8, 10]
        even_count = algorithms.count_if(data, lambda x: x % 2 == 0)
        assert even_count == 5

    def test_count_strings_by_length(self) -> None:
        """Test counting strings by length."""
        data = ['a', 'hello', 'world', 'test', 'algorithm']
        long_words = algorithms.count_if(data, lambda word: len(word) > 4)
        assert long_words == 3

    def test_count_container(self) -> None:
        """Test counting in container."""
        container = containers.Container(int, [1, 2, 3, 4, 5, 6])
        even_count = algorithms.count_if(container, lambda x: x % 2 == 0)
        assert even_count == 3

    def test_count_tuple(self) -> None:
        """Test counting in tuple."""
        data = (1, 2, 3, 4, 5, 6, 7, 8, 9)
        even_count = algorithms.count_if(data, lambda x: x % 2 == 0)
        assert even_count == 4

    def test_count_generator(self) -> None:
        """Test counting in generator."""
        data = (x for x in range(1, 10))
        even_count = algorithms.count_if(data, lambda x: x % 2 == 0)
        assert even_count == 4


class TestTransform:
    """Test transformation operations."""

    def test_transform_empty_container(self) -> None:
        """Test transforming empty container."""
        result = algorithms.transform([], lambda x: x * 2)
        assert result == []

    def test_square_numbers(self) -> None:
        """Test squaring numbers."""
        data = [1, 2, 3, 4, 5]
        squares = algorithms.transform(data, lambda x: x * x)
        expected = [1, 4, 9, 16, 25]
        assert squares == expected

    def test_double_numbers(self) -> None:
        """Test doubling numbers."""
        data = [10, 20, 30, 40, 50]
        doubled = algorithms.transform(data, lambda x: x * 2)
        expected = [20, 40, 60, 80, 100]
        assert doubled == expected

    def test_convert_to_string(self) -> None:
        """Test converting numbers to strings."""
        data = [1, 2, 3, 4, 5]
        strings = algorithms.transform(data, str)
        expected = ['1', '2', '3', '4', '5']
        assert strings == expected

    def test_string_to_uppercase(self) -> None:
        """Test converting strings to uppercase."""
        data = ['hello', 'world', 'test']
        uppercased = algorithms.transform(data, str.upper)
        expected = ['HELLO', 'WORLD', 'TEST']
        assert uppercased == expected

    def test_transform_container(self) -> None:
        """Test transforming container."""
        container = containers.Container(int, [1, 2, 3, 4, 5])
        squares = algorithms.transform(container, lambda x: x * x)
        assert squares == [1, 4, 9, 16, 25]

    def test_transform_tuple(self) -> None:
        """Test transforming tuple."""
        data = (1, 2, 3, 4, 5)
        doubled = algorithms.transform(data, lambda x: x * 2)
        assert doubled == [2, 4, 6, 8, 10]

    def test_transform_generator(self) -> None:
        """Test transforming generator."""
        data = (x for x in range(1, 6))
        squares = algorithms.transform(data, lambda x: x * x)
        assert squares == [1, 4, 9, 16, 25]


class TestFindMinMax:
    """Test finding minimum and maximum values."""

    def test_find_minmax_integers(self) -> None:
        """Test finding min/max in integer sequence."""
        data = [3, 1, 4, 1, 5, 9, 2, 6]
        min_val, max_val = algorithms.find_min_max(data)
        assert min_val == 1
        assert max_val == 9

    def test_find_minmax_single_element(self) -> None:
        """Test finding min/max in single element sequence."""
        data = [42]
        min_val, max_val = algorithms.find_min_max(data)
        assert min_val == 42
        assert max_val == 42

    def test_find_minmax_same_elements(self) -> None:
        """Test finding min/max with all same elements."""
        data = [5, 5, 5, 5, 5]
        min_val, max_val = algorithms.find_min_max(data)
        assert min_val == 5
        assert max_val == 5

    def test_find_minmax_negative_numbers(self) -> None:
        """Test finding min/max in negative numbers."""
        data = [-10, -5, -20, -1, -15]
        min_val, max_val = algorithms.find_min_max(data)
        assert min_val == -20
        assert max_val == -1

    def test_find_minmax_mixed_numbers(self) -> None:
        """Test finding min/max in mixed positive/negative."""
        data = [-5, 10, -15, 20, 0]
        min_val, max_val = algorithms.find_min_max(data)
        assert min_val == -15
        assert max_val == 20

    def test_find_minmax_strings(self) -> None:
        """Test finding min/max in string sequence."""
        data = ['cherry', 'banana', 'elderberry', 'date', 'apple']
        min_word, max_word = algorithms.find_min_max(data)
        assert min_word == 'apple'
        assert max_word == 'elderberry'

    def test_find_minmax_container(self) -> None:
        """Test finding min/max in container."""
        container = containers.Container(int, [3, 1, 4, 1, 5, 9, 2, 6])
        min_val, max_val = algorithms.find_min_max(container)
        assert min_val == 1
        assert max_val == 9

    def test_find_minmax_tuple(self) -> None:
        """Test finding min/max in tuple."""
        data = (3, 1, 4, 1, 5, 9, 2, 6)
        min_val, max_val = algorithms.find_min_max(data)
        assert min_val == 1
        assert max_val == 9


class TestPipeline:
    """Test function pipeline functionality."""

    def test_simple_pipeline(self) -> None:
        """Test simple function pipeline."""
        process = algorithms.pipeline(
            lambda x: [i * 2 for i in x],
            lambda x: [i for i in x if i > 5],
            sum
        )

        result = process([1, 2, 3, 4, 5])
        # [1,2,3,4,5] -> [2,4,6,8,10] -> [6,8,10] -> 24
        assert result == 24

    def test_string_pipeline(self) -> None:
        """Test pipeline with string operations."""
        process = algorithms.pipeline(
            str.upper,
            lambda s: s.replace(' ', '_'),
            lambda s: f'PREFIX_{s}'
        )

        result = process('hello world')
        assert result == 'PREFIX_HELLO_WORLD'

    def test_empty_pipeline(self) -> None:
        """Test empty pipeline returns input unchanged."""
        process = algorithms.pipeline()
        assert process(42) == 42

    def test_single_function_pipeline(self) -> None:
        """Test pipeline with single function."""
        process = algorithms.pipeline(lambda x: x * 2)
        assert process(5) == 10


class TestFunctionalChain:
    """Test functional chain operations."""

    def test_basic_chain(self) -> None:
        """Test basic functional chain."""
        result = (algorithms.functional_chain([1, 2, 3, 4, 5, 6])
                 .filter(lambda x: x % 2 == 0)
                 .map(lambda x: x * x)
                 .collect())

        assert result == [4, 16, 36]

    def test_chain_with_sort(self) -> None:
        """Test chain with sorting."""
        result = (algorithms.functional_chain([3, 1, 4, 1, 5])
                 .sort()
                 .collect())

        assert result == [1, 1, 3, 4, 5]

    def test_chain_with_reverse_sort(self) -> None:
        """Test chain with reverse sorting."""
        result = (algorithms.functional_chain([3, 1, 4, 1, 5])
                 .sort(reverse=True)
                 .collect())

        assert result == [5, 4, 3, 1, 1]

    def test_chain_with_key_sort(self) -> None:
        """Test chain with key-based sorting."""
        result = (algorithms.functional_chain(['apple', 'pie', 'a', 'cherry'])
                 .sort(key=len)
                 .collect())

        assert result == ['a', 'pie', 'apple', 'cherry']

    def test_chain_take_and_skip(self) -> None:
        """Test chain with take and skip operations."""
        result = (algorithms.functional_chain([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
                 .skip(3)
                 .take(4)
                 .collect())

        assert result == [4, 5, 6, 7]

    def test_chain_reduce(self) -> None:
        """Test chain with reduce operation."""
        result = (algorithms.functional_chain([1, 2, 3, 4, 5])
                 .reduce(lambda a, b: a + b))

        assert result == 15

    def test_chain_reduce_with_initial(self) -> None:
        """Test chain with reduce operation and initial value."""
        result = (algorithms.functional_chain([1, 2, 3, 4, 5])
                 .reduce(lambda a, b: a * b, 1))

        assert result == 120

    def test_complex_chain(self) -> None:
        """Test complex chain with multiple operations."""
        result = (algorithms.functional_chain(range(1, 21))
                 .filter(lambda x: x % 2 == 0)  # Even numbers
                 .map(lambda x: x ** 2)          # Square them
                 .filter(lambda x: x > 50)      # Only large squares
                 .sort(reverse=True)             # Sort descending
                 .take(3)                        # Take first 3
                 .collect())

        # Even numbers: 2,4,6,8,10,12,14,16,18,20
        # Squared: 4,16,36,64,100,144,196,256,324,400
        # > 50: 64,100,144,196,256,324,400
        # Sorted desc: 400,324,256,196,144,100,64
        # Take 3: 400,324,256
        assert result == [400, 324, 256]


class TestAlgorithmCombinations:
    """Test combining different algorithms."""

    def test_transform_then_find_minmax(self) -> None:
        """Test transforming then finding min/max."""
        data = [-3, 2, -1, 4, -5]
        squares = algorithms.transform(data, lambda x: x * x)
        min_val, max_val = algorithms.find_min_max(squares)

        assert min_val == 1   # (-1)^2
        assert max_val == 25  # (-5)^2

    def test_sort_transform_count_pipeline(self) -> None:
        """Test full pipeline: sort, transform, count."""
        data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]

        # Sort in place
        algorithms.sort_inplace(data)

        # Transform to squares
        squares = algorithms.transform(data, lambda x: x * x)

        # Count large squares
        large_squares = algorithms.count_if(squares, lambda x: x > 10)

        assert large_squares == 5  # 16, 25, 25, 36, 81

    def test_filter_sort_transform_chain(self) -> None:
        """Test chaining filter, sort, and transform operations."""
        data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

        result = (algorithms.functional_chain(data)
                 .filter(lambda x: x % 2 == 0)    # [2, 4, 6, 8, 10]
                 .sort(reverse=True)               # [10, 8, 6, 4, 2]
                 .map(lambda x: x // 2)            # [5, 4, 3, 2, 1]
                 .collect())

        assert result == [5, 4, 3, 2, 1]

    def test_multiple_containers_algorithms(self) -> None:
        """Test algorithms on multiple containers."""
        container1 = containers.Container(int, [1, 3, 5, 7, 9])
        container2 = containers.Container(int, [2, 4, 6, 8, 10])

        # Transform both containers
        squares1 = algorithms.transform(container1, lambda x: x * x)
        squares2 = algorithms.transform(container2, lambda x: x * x)

        # Combine and find min/max
        combined = squares1 + squares2
        min_val, max_val = algorithms.find_min_max(combined)

        assert min_val == 1   # 1^2
        assert max_val == 100 # 10^2
