"""Tests for Python containers module.

Comprehensive tests following C++ test patterns for container functionality.
"""

import pytest
from demo import containers


class TestContainer:
    """Test Container creation and basic operations."""

    def test_int_container_creation(self) -> None:
        """Test creating integer container."""
        container = containers.Container(int, [1, 2, 3, 4, 5])

        assert len(container) == 5
        assert bool(container) is True
        assert container[0] == 1
        assert container[4] == 5

    def test_float_container_creation(self) -> None:
        """Test creating float container."""
        container = containers.Container(float, [1.5, 2.5, 3.5])

        assert len(container) == 3
        assert container[0] == 1.5
        assert container[2] == 3.5

    def test_string_container_creation(self) -> None:
        """Test creating string container."""
        container = containers.Container(str, ['hello', 'world'])

        assert len(container) == 2
        assert container[0] == 'hello'
        assert container[1] == 'world'

    def test_empty_container(self) -> None:
        """Test empty container behavior."""
        container = containers.Container(int)

        assert len(container) == 0
        assert bool(container) is False

    def test_unsupported_type(self) -> None:
        """Test creating container with unsupported type."""
        with pytest.raises(ValueError, match='Unsupported container type'):
            containers.Container(list, [])


class TestContainerOperations:
    """Test container operations."""

    def test_add_items(self) -> None:
        """Test adding items to container."""
        container = containers.Container(int, [1, 2, 3])
        container.add(4)
        container.add(5)

        assert len(container) == 5
        assert 4 in list(container)
        assert 5 in list(container)

    def test_remove_items(self) -> None:
        """Test removing items from container."""
        container = containers.Container(int, [1, 2, 3, 2, 4])
        removed_count = container.remove(2)

        assert removed_count == 2  # Should remove all occurrences
        assert 2 not in list(container)
        assert len(container) == 3

    def test_iteration(self) -> None:
        """Test container iteration."""
        data = [1, 3, 5, 7, 9]
        container = containers.Container(int, data)

        result = list(container)
        assert result == data

    def test_indexing(self) -> None:
        """Test container indexing."""
        container = containers.Container(str, ['a', 'b', 'c', 'd'])

        assert container[0] == 'a'
        assert container[1] == 'b'
        assert container[3] == 'd'

    def test_filter_operation(self) -> None:
        """Test filtering container elements."""
        container = containers.Container(int, [1, 2, 3, 4, 5, 6, 7, 8, 9])

        even_numbers = container.filter(lambda x: x % 2 == 0)
        assert even_numbers == [2, 4, 6, 8]

        large_numbers = container.filter(lambda x: x > 5)
        assert large_numbers == [6, 7, 8, 9]

    def test_transform_operation(self) -> None:
        """Test transforming container elements."""
        container = containers.Container(int, [1, 2, 3, 4, 5])

        squares = container.transform(lambda x: x * x)
        assert squares == [1, 4, 9, 16, 25]

        strings = container.transform(str)
        assert strings == ['1', '2', '3', '4', '5']

    def test_string_representation(self) -> None:
        """Test container string representation."""
        container = containers.Container(int, [1, 2, 3])
        repr_str = repr(container)

        assert 'Container[int]' in repr_str
        assert '[1, 2, 3]' in repr_str


class TestContainerFactory:
    """Test container factory function."""

    def test_create_int_container(self) -> None:
        """Test creating integer container via factory."""
        container = containers.create_container([1, 2, 3, 4])

        assert len(container) == 4
        assert container[0] == 1
        assert repr(container).startswith('Container[int]')

    def test_create_float_container(self) -> None:
        """Test creating float container via factory."""
        container = containers.create_container([1.5, 2.5, 3.5])

        assert len(container) == 3
        assert container[0] == 1.5
        assert repr(container).startswith('Container[float]')

    def test_create_string_container(self) -> None:
        """Test creating string container via factory."""
        container = containers.create_container(['hello', 'world'])

        assert len(container) == 2
        assert container[0] == 'hello'
        assert repr(container).startswith('Container[str]')

    def test_empty_data_error(self) -> None:
        """Test factory with empty data raises error."""
        with pytest.raises(ValueError, match='Cannot determine type from empty data'):
            containers.create_container([])

    def test_unsupported_type_error(self) -> None:
        """Test factory with unsupported type raises error."""
        with pytest.raises(ValueError, match='Unsupported data type'):
            containers.create_container([{'key': 'value'}])


class TestContainerTypes:
    """Test different container types."""

    def test_int_container_operations(self) -> None:
        """Test operations on integer container."""
        container = containers.Container(int, [5, 2, 8, 1, 9])

        # Test filtering
        large_nums = container.filter(lambda x: x > 5)
        assert set(large_nums) == {8, 9}

        # Test transformation
        doubled = container.transform(lambda x: x * 2)
        assert doubled == [10, 4, 16, 2, 18]

    def test_float_container_operations(self) -> None:
        """Test operations on float container."""
        container = containers.Container(float, [1.1, 2.2, 3.3, 4.4])

        # Test filtering
        large_nums = container.filter(lambda x: x > 2.5)
        assert large_nums == [3.3, 4.4]

        # Test transformation
        rounded = container.transform(lambda x: round(x))
        assert rounded == [1, 2, 3, 4]

    def test_string_container_operations(self) -> None:
        """Test operations on string container."""
        container = containers.Container(str, ['apple', 'banana', 'cherry', 'date'])

        # Test filtering
        long_words = container.filter(lambda x: len(x) > 5)
        assert long_words == ['banana', 'cherry']

        # Test transformation
        lengths = container.transform(len)
        assert lengths == [5, 6, 6, 4]

        uppercase = container.transform(str.upper)
        assert uppercase == ['APPLE', 'BANANA', 'CHERRY', 'DATE']


class TestContainerEdgeCases:
    """Test container edge cases and error conditions."""

    def test_container_with_duplicates(self) -> None:
        """Test container behavior with duplicate values."""
        container = containers.Container(int, [1, 2, 2, 3, 2, 4])

        assert len(container) == 6
        removed = container.remove(2)
        assert removed == 3  # All occurrences removed
        assert len(container) == 3

    def test_container_boundary_operations(self) -> None:
        """Test container operations at boundaries."""
        container = containers.Container(int, [42])

        # Single element container
        assert len(container) == 1
        assert container[0] == 42
        assert list(container) == [42]

        # Remove the only element
        removed = container.remove(42)
        assert removed == 1
        assert len(container) == 0
        assert bool(container) is False

    def test_filter_no_matches(self) -> None:
        """Test filtering with no matches."""
        container = containers.Container(int, [1, 3, 5, 7, 9])

        even_numbers = container.filter(lambda x: x % 2 == 0)
        assert even_numbers == []

    def test_filter_all_matches(self) -> None:
        """Test filtering where all elements match."""
        container = containers.Container(int, [2, 4, 6, 8, 10])

        even_numbers = container.filter(lambda x: x % 2 == 0)
        assert even_numbers == [2, 4, 6, 8, 10]


class TestContainerIntegration:
    """Integration tests for container functionality."""

    def test_container_pipeline(self) -> None:
        """Test chaining container operations."""
        container = containers.Container(int, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

        # Filter even numbers, then square them
        even_numbers = container.filter(lambda x: x % 2 == 0)
        squares = [x * x for x in even_numbers]

        assert even_numbers == [2, 4, 6, 8, 10]
        assert squares == [4, 16, 36, 64, 100]

    def test_multiple_containers_interaction(self) -> None:
        """Test interaction between multiple containers."""
        int_container = containers.Container(int, [1, 2, 3])
        str_container = containers.Container(str, ['a', 'b', 'c'])

        # Transform int container to strings and compare
        int_as_strings = int_container.transform(str)
        str_lengths = str_container.transform(len)

        assert int_as_strings == ['1', '2', '3']
        assert str_lengths == [1, 1, 1]

    def test_container_factory_vs_direct_creation(self) -> None:
        """Test that factory and direct creation produce equivalent results."""
        data = [1, 2, 3, 4, 5]

        factory_container = containers.create_container(data)
        direct_container = containers.Container(int, data)

        assert len(factory_container) == len(direct_container)
        assert list(factory_container) == list(direct_container)
        assert factory_container.filter(lambda x: x > 3) == direct_container.filter(
            lambda x: x > 3
        )
