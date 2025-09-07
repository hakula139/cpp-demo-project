"""Python wrapper for the containers module."""

import builtins
from collections.abc import Iterable
from typing import Callable, Generic, Iterator, TypeVar

from .cpp_features import containers as _containers

T = TypeVar('T')
U = TypeVar('U')


class Container(Generic[T]):
    """A generic container wrapper with enhanced functionality."""

    def __init__(
        self, container_type: type[T], data: Iterable[T] | None = None
    ) -> None:
        """Initialize container with specific type.

        Parameters
        ----------
        container_type : type[T]
            The type of elements stored in the container
        data : Iterable[T], optional
            Initial data to populate the container

        Examples
        --------
        >>> Container(int, [1, 2, 3])
        <IntContainer(size=3) at 0x13911eaa0>
        >>> Container(float, [1.1, 2.2, 3.3])
        <FloatContainer(size=3) at 0x13911eaa0>
        >>> Container(str, ['a', 'b', 'c'])
        <StringContainer(size=3) at 0x13911eaa0>
        """
        self._type = container_type

        match container_type:
            case builtins.int:
                cls = _containers.IntContainer
            case builtins.float:
                cls = _containers.FloatContainer
            case builtins.str:
                cls = _containers.StringContainer
            case _:
                raise ValueError(
                    f'Unsupported container type: {container_type.__name__}'
                )

        self._container = cls(list(data)) if data else cls()

    def add(self, item: T) -> None:
        """Add an element to the container.

        Adds a copy of the specified element to the end of the container.

        Parameters
        ----------
        item : T
            The element to add

        Examples
        --------
        >>> container = Container(int, [1, 2, 3])
        >>> container.add(4)
        >>> list(container)
        [1, 2, 3, 4]
        """
        self._container.add(item)

    def remove(self, item: T) -> int:
        """Remove all occurrences of a specific item.

        Removes all elements that compare equal to the specified item.
        The container size is reduced by the number of removed elements.

        Parameters
        ----------
        item : T
            The item to remove from the container

        Returns
        -------
        int
            The number of elements that were removed

        Examples
        --------
        >>> container = Container(int, [1, 2, 3, 2, 4, 2])
        >>> container.remove(2)
        3
        >>> list(container)
        [1, 3, 4]
        """
        return self._container.remove(item)

    def __len__(self) -> int:
        """Get the number of elements in the container.

        Returns
        -------
        int
            The number of elements currently stored in the container

        Examples
        --------
        >>> container = Container(int, [1, 2, 3])
        >>> len(container)
        3
        """
        return len(self._container)

    def __bool__(self) -> bool:
        """Check if the container is not empty.

        Returns
        -------
        bool
            True if the container contains elements, False otherwise

        Examples
        --------
        >>> container = Container(int, [1, 2, 3])
        >>> bool(container)
        True
        >>> container = Container(int, [])
        >>> bool(container)
        False
        """
        return bool(self._container)

    def __iter__(self) -> Iterator[T]:
        """Iterate over the container.

        Iterates over the elements in the container in the order they were added.

        Returns
        -------
        Iterator[T]
            Iterator over container elements

        Examples
        --------
        >>> container = Container(int, [1, 2, 3])
        >>> for item in container:
        ...     print(item)
        ...
        1
        2
        3
        """
        return iter(self._container)

    def __getitem__(self, index: int) -> T:
        """Access the element at specified index.

        Returns the element at the specified index.

        Parameters
        ----------
        index : int
            The index of the element to access

        Returns
        -------
        T
            The element at the specified index

        Raises
        ------
        IndexError
            If the index is out of bounds

        Examples
        --------
        >>> container = Container(int, [1, 2, 3])
        >>> container[1]
        2
        >>> container[-1]
        3
        """
        if index < 0:
            index %= len(self._container)
        return self._container[index]

    def filter(self, predicate: Callable[[T], bool]) -> list[T]:
        """Filter container elements.

        Returns a list of elements that satisfy the predicate.

        Parameters
        ----------
        predicate : Callable[[T], bool]
            The predicate function that returns true for elements to include.

        Returns
        -------
        list[T]
            A list of elements that satisfy the predicate

        Examples
        --------
        >>> container = Container(int, [1, 2, 3, 4, 5])
        >>> container.filter(lambda x: x % 2 == 0)
        [2, 4]
        """
        return self._container.filter(predicate)

    def transform(self, func: Callable[[T], U]) -> list[U]:
        """Transform container elements.

        Returns a list of elements that have been transformed by the provided function.

        Parameters
        ----------
        func : Callable[[T], U]
            The function to apply to each element

        Returns
        -------
        list[U]
            A list of transformed elements

        Examples
        --------
        >>> container = Container(int, [1, 2, 3, 4, 5])
        >>> container.transform(lambda x: x * 2)
        [2, 4, 6, 8, 10]
        """
        return self._container.transform(func)

    def __str__(self) -> str:
        """String representation."""
        return str(self._container)

    def __repr__(self) -> str:
        """String representation (for debugging)."""
        return repr(self._container)


__all__ = [
    'Container',
]
