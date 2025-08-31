"""Tests for the containers module."""

import pytest
from demo.containers import *


class TestContainerBasic:
    """Test Container creation and basic operations."""

    def test_empty_container_creation(self) -> None:
        """Test creating empty container."""
        container = Container(int)

        assert len(container) == 0
        assert bool(container) is False
        assert list(container) == []

    def test_int_container_creation(self) -> None:
        """Test creating integer container."""
        container = Container(int, [1, 2, 3, 4, 5])

        assert len(container) == 5
        assert bool(container) is True
        assert list(container) == [1, 2, 3, 4, 5]

    def test_float_container_creation(self) -> None:
        """Test creating float container."""
        container = Container(float, [1.5, 2.5, 3.5])

        assert len(container) == 3
        assert list(container) == [1.5, 2.5, 3.5]

    def test_string_container_creation(self) -> None:
        """Test creating string container."""
        container = Container(str, ['hello', 'world'])

        assert len(container) == 2
        assert list(container) == ['hello', 'world']

    def test_unsupported_type_creation(self) -> None:
        """Test creating container with unsupported type."""
        with pytest.raises(ValueError, match='Unsupported container type: list'):
            Container(list, [[1, 2, 3]])


class TestContainerOperations:
    """Test container operations."""

    def test_add(self) -> None:
        """Test adding items to container."""
        container = Container(int, [1, 2, 3])

        container.add(4)
        container.add(5)
        assert len(container) == 5
        assert list(container) == [1, 2, 3, 4, 5]

    def test_remove(self) -> None:
        """Test removing items from container."""
        container = Container(int, [1, 2, 3, 2, 4])

        removed_count = container.remove(2)
        assert removed_count == 2
        assert 2 not in list(container)
        assert len(container) == 3
        assert list(container) == [1, 3, 4]

    def test_access(self) -> None:
        """Test accessing items in container."""
        container = Container(int, [1, 2, 3, 4, 5])

        assert container[0] == 1
        assert container[4] == 5
        assert container[-2] == 4
        with pytest.raises(IndexError, match='Index out of bounds'):
            container[5]

    def test_iteration(self) -> None:
        """Test iterating over container."""
        container = Container(int, [1, 2, 3, 4])

        iterator = iter(container)
        assert next(iterator) == 1
        assert next(iterator) == 2
        assert next(iterator) == 3
        assert next(iterator) == 4
        with pytest.raises(StopIteration):
            next(iterator)

        for i, item in enumerate(container):
            assert item == i + 1

    def test_filter(self) -> None:
        """Test filtering container elements."""
        container = Container(int, [1, 2, 3, 4, 5, 6, 7])

        even_values = container.filter(lambda x: x % 2 == 0)
        assert len(even_values) == 3
        assert even_values == [2, 4, 6]

        large_values = container.filter(lambda x: x > 5)
        assert len(large_values) == 2
        assert large_values == [6, 7]

    def test_transform(self) -> None:
        """Test transforming container elements."""
        container = Container(int, [1, 2, 3, 4, 5])

        squares = container.transform(lambda x: x * x)
        assert len(squares) == 5
        assert squares == [1, 4, 9, 16, 25]

        strings = container.transform(str)
        assert len(strings) == 5
        assert strings == ['1', '2', '3', '4', '5']


class TestContainerRepresentation:
    """Test container representation."""

    def test_str(self) -> None:
        """Test string representation."""
        container = Container(int, [1, 2, 3, 4, 5])

        assert str(container) == '[1, 2, 3, 4, 5]'

    def test_repr(self) -> None:
        """Test string representation for debugging."""
        container = Container(str, ['hello', 'world'])

        assert repr(container).startswith('<StringContainer(size=2) at ')
