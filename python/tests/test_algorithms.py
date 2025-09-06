"""Tests for the algorithms module."""

import pytest

from demo.algorithms import *
from demo.containers import Container


class TestSort:
    """Test in-place sorting functionality."""

    def test_sort_empty(self) -> None:
        """Test sorting empty list."""
        data: list[int] = []
        sort(data)
        assert data == []

    def test_sort_single_element(self) -> None:
        """Test sorting single element list."""
        data = [42]
        sort(data)
        assert data == [42]

    def test_sort_integers(self) -> None:
        """Test sorting integers in ascending order."""
        data = [42, 17, 89, 3, 56, 23, 78, 12, 95, 34]
        sort(data)
        assert data == [3, 12, 17, 23, 34, 42, 56, 78, 89, 95]

    def test_sort_strings(self) -> None:
        """Test sorting strings lexicographically."""
        data = ['cherry', 'banana', 'elderberry', 'date', 'apple']
        sort(data)
        assert data == ['apple', 'banana', 'cherry', 'date', 'elderberry']

    def test_sort_already_sorted(self) -> None:
        """Test sorting already sorted list."""
        data = [1, 2, 3, 4, 5]
        sort(data)
        assert data == [1, 2, 3, 4, 5]

    def test_sort_reverse_sorted(self) -> None:
        """Test sorting reverse sorted list."""
        data = [5, 4, 3, 2, 1]
        sort(data)
        assert data == [1, 2, 3, 4, 5]

    def test_sort_with_duplicates(self) -> None:
        """Test sorting list with duplicates."""
        data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
        sort(data)
        assert data == [1, 1, 2, 3, 3, 4, 5, 5, 6, 9]

    def test_sort_container(self) -> None:
        """Test sorting container."""
        data = Container(int, [3, 1, 4, 1, 5, 9, 2, 6, 5, 3])
        sort(data)
        assert list(data) == [1, 1, 2, 3, 3, 4, 5, 5, 6, 9]

    def test_sort_unsupported_type(self) -> None:
        """Test sorting unsupported type."""
        data = [1, 'a']
        with pytest.raises(TypeError):
            sort(data)

        class UnsupportedType:
            pass

        data = [UnsupportedType(), UnsupportedType()]
        with pytest.raises(TypeError):
            sort(data)  # type: ignore


class TestCountIf:
    """Test counting with predicates."""

    def test_count_empty(self) -> None:
        """Test counting in empty list."""
        data: list[int] = []
        result = count_if(data, lambda x: x > 0)
        assert result == 0

    def test_count_even_numbers(self) -> None:
        """Test counting even numbers."""
        data = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        even_count = count_if(data, lambda x: x % 2 == 0)
        assert even_count == 4

    def test_count_odd_numbers(self) -> None:
        """Test counting odd numbers."""
        data = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        odd_count = count_if(data, lambda x: x % 2 == 1)
        assert odd_count == 5

    def test_count_greater_than_threshold(self) -> None:
        """Test counting numbers greater than threshold."""
        data = [10, 25, 30, 45, 50, 75, 80, 95]
        large_count = count_if(data, lambda x: x > 50)
        assert large_count == 3

    def test_count_no_matches(self) -> None:
        """Test counting with no matches."""
        data = [1, 3, 5, 7, 9]
        even_count = count_if(data, lambda x: x % 2 == 0)
        assert even_count == 0

    def test_count_all_matches(self) -> None:
        """Test counting with all matches."""
        data = [2, 4, 6, 8, 10]
        even_count = count_if(data, lambda x: x % 2 == 0)
        assert even_count == 5

    def test_count_strings_by_length(self) -> None:
        """Test counting strings by length."""
        data = ['a', 'hello', 'world', 'test', 'algorithm']
        long_words = count_if(data, lambda word: len(word) > 4)
        assert long_words == 3

    def test_count_container(self) -> None:
        """Test counting in container."""
        container = Container(int, [1, 2, 3, 4, 5, 6, 7, 8, 9])
        even_count = count_if(container, lambda x: x % 2 == 0)
        assert even_count == 4

    def test_count_tuple(self) -> None:
        """Test counting in tuple."""
        data = (1, 2, 3, 4, 5, 6, 7, 8, 9)
        even_count = count_if(data, lambda x: x % 2 == 0)
        assert even_count == 4

    def test_count_generator(self) -> None:
        """Test counting in generator."""
        data = (x for x in range(1, 10))
        even_count = count_if(data, lambda x: x % 2 == 0)
        assert even_count == 4


class TestTransformToList:
    """Test transforming to list."""

    def test_transform_empty(self) -> None:
        """Test transforming empty list."""
        data: list[int] = []
        result = transform_to_list(data, lambda x: x * 2)
        assert result == []

    def test_square_numbers(self) -> None:
        """Test squaring numbers."""
        data = [1, 2, 3, 4, 5]
        squares = transform_to_list(data, lambda x: x * x)
        assert squares == [1, 4, 9, 16, 25]

    def test_double_numbers(self) -> None:
        """Test doubling numbers."""
        data = [10, 20, 30, 40, 50]
        doubled = transform_to_list(data, lambda x: x * 2)
        assert doubled == [20, 40, 60, 80, 100]

    def test_convert_to_string(self) -> None:
        """Test converting numbers to strings."""
        data = [1, 2, 3, 4, 5]
        strings = transform_to_list(data, str)
        assert strings == ['1', '2', '3', '4', '5']

    def test_convert_to_int(self) -> None:
        """Test converting strings to ints."""
        data = ['1', '2', '3', '4', '5']
        ints = transform_to_list(data, int)
        assert ints == [1, 2, 3, 4, 5]

    def test_convert_to_float(self) -> None:
        """Test converting strings to floats."""
        data = ['1.1', '2.2', '3.3', '4.4', '5.5']
        floats = transform_to_list(data, float)
        assert floats == [1.1, 2.2, 3.3, 4.4, 5.5]

    def test_string_to_uppercase(self) -> None:
        """Test converting strings to uppercase."""
        data = ['hello', 'world', 'test']
        uppercased = transform_to_list(data, str.upper)
        assert uppercased == ['HELLO', 'WORLD', 'TEST']

    def test_transform_container(self) -> None:
        """Test transforming container."""
        container = Container(int, [1, 2, 3, 4, 5])
        square_strings = transform_to_list(container, lambda x: str(x * x))
        assert square_strings == ['1', '4', '9', '16', '25']

    def test_transform_tuple(self) -> None:
        """Test transforming tuple."""
        data = (1.1, 2.2, 3.3, 4.4, 5.5)
        doubled_ints = transform_to_list(data, lambda x: int(x * 2))
        assert doubled_ints == [2, 4, 6, 8, 11]

    def test_transform_generator(self) -> None:
        """Test transforming generator."""
        data = ['apple', 'banana', 'cherry', 'date', 'elderberry']
        lengths = transform_to_list(data, len)
        assert lengths == [5, 6, 6, 4, 10]


class TestFindMinMax:
    """Test finding minimum and maximum values."""

    def test_find_min_max_integers(self) -> None:
        """Test finding min/max in integer sequence."""
        data = [3, 1, 4, 1, 5, 9, 2, 6]
        min_val, max_val = find_min_max(data)
        assert min_val == 1
        assert max_val == 9

    def test_find_min_max_single_element(self) -> None:
        """Test finding min/max in single element sequence."""
        data = [42]
        min_val, max_val = find_min_max(data)
        assert min_val == 42
        assert max_val == 42

    def test_find_min_max_same_elements(self) -> None:
        """Test finding min/max with all same elements."""
        data = [5, 5, 5, 5, 5]
        min_val, max_val = find_min_max(data)
        assert min_val == 5
        assert max_val == 5

    def test_find_min_max_negative_numbers(self) -> None:
        """Test finding min/max in negative numbers."""
        data = [-10, -5, -20, -1, -15]
        min_val, max_val = find_min_max(data)
        assert min_val == -20
        assert max_val == -1

    def test_find_min_max_mixed_numbers(self) -> None:
        """Test finding min/max in mixed positive/negative."""
        data = [-5, 10, -15, 20, 0]
        min_val, max_val = find_min_max(data)
        assert min_val == -15
        assert max_val == 20

    def test_find_min_max_strings(self) -> None:
        """Test finding min/max in string sequence."""
        data = ['cherry', 'banana', 'elderberry', 'date', 'apple']
        min_word, max_word = find_min_max(data)
        assert min_word == 'apple'
        assert max_word == 'elderberry'

    def test_find_min_max_container(self) -> None:
        """Test finding min/max in container."""
        container = Container(int, [3, 1, 4, 1, 5, 9, 2, 6])
        min_val, max_val = find_min_max(container)
        assert min_val == 1
        assert max_val == 9

    def test_find_min_max_tuple(self) -> None:
        """Test finding min/max in tuple."""
        data = (3, 1, 4, 1, 5, 9, 2, 6)
        min_val, max_val = find_min_max(data)
        assert min_val == 1
        assert max_val == 9


class TestPipeline:
    """Test function pipeline functionality."""

    def test_simple_pipeline(self) -> None:
        """Test simple function pipeline."""
        process = pipeline(
            lambda data: [x * 2 for x in data],
            lambda data: [x for x in data if x > 5],
            sum,
        )

        result = process([1, 2, 3, 4, 5])
        # [1, 2, 3, 4, 5] -> [2, 4, 6, 8, 10] -> [6, 8, 10] -> 24
        assert result == 24

    def test_string_pipeline(self) -> None:
        """Test pipeline with string operations."""
        process = pipeline(
            str.upper,
            lambda s: s.replace(' ', '_'),
            lambda s: f'PREFIX_{s}',
        )

        result = process('hello world')
        # 'hello world' -> 'HELLO WORLD' -> 'HELLO_WORLD' -> 'PREFIX_HELLO_WORLD'
        assert result == 'PREFIX_HELLO_WORLD'

    def test_empty_pipeline(self) -> None:
        """Test empty pipeline returns input unchanged."""
        process = pipeline()
        assert process([1, 2, 3, 4, 5]) == [1, 2, 3, 4, 5]

    def test_single_function_pipeline(self) -> None:
        """Test pipeline with single function."""
        process = pipeline(lambda x: x * 2)
        assert process(5) == 10


class TestAlgorithmCombinations:
    """Test combining different algorithms."""

    def test_transform_then_find_min_max(self) -> None:
        """Test transforming then finding min/max."""
        data = [-3, 2, -1, 4, -5]

        process = pipeline(
            lambda data: [x * x for x in data],
            find_min_max,
        )
        min_val, max_val = process(data)
        # [-3, 2, -1, 4, -5] -> [9, 4, 1, 16, 25] -> (1, 25)
        assert min_val == 1
        assert max_val == 25

    def test_sort_transform_count_pipeline(self) -> None:
        """Test full pipeline: sort, transform, count."""
        data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]

        sort(data)
        # [1, 1, 2, 3, 3, 4, 5, 5, 6, 9]
        squares = transform_to_list(data, lambda x: x * x)
        # [1, 1, 4, 9, 9, 16, 25, 25, 36, 81]
        large_squares = count_if(squares, lambda x: x > 10)
        assert large_squares == 5  # 16, 25, 25, 36, 81

    def test_multiple_containers_algorithms(self) -> None:
        """Test algorithms on multiple containers."""
        container1 = Container(int, [1, 3, 5, 7, 9])
        container2 = Container(int, [2, 4, 6, 8, 10])

        squares1 = transform_to_list(container1, lambda x: x * x)
        # [1, 9, 25, 49, 81]
        squares2 = transform_to_list(container2, lambda x: x * x)
        # [4, 16, 36, 64, 100]
        combined = squares1 + squares2
        # [1, 9, 25, 49, 81, 4, 16, 36, 64, 100]
        min_val, max_val = find_min_max(combined)
        # (1, 100)
        assert min_val == 1
        assert max_val == 100
