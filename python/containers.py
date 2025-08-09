"""Modern Python wrapper for the containers module.

Enhanced container functionality with Python 3.13 features.
"""

from collections.abc import Iterable
from typing import Any, Callable, Generic, Iterator, TypeVar

import cpp_features.containers as _containers

T = TypeVar('T')


class Container(Generic[T]):
    """Generic container wrapper with enhanced Python integration.

    Provides a more Pythonic interface to the C++ Container class.

    Parameters
    ----------
    container_type : type[T]
        The type of elements to store in the container
    data : Iterable[T], optional
        Initial data to populate the container
    """

    def __init__(self, container_type: type[T], data: Iterable[T] | None = None):
        """Initialize container with specific type."""
        self._type = container_type

        match container_type:
            case int:
                self._container = _containers.IntContainer(list(data) if data else [])
            case float:
                self._container = _containers.DoubleContainer(
                    list(data) if data else []
                )
            case str:
                self._container = _containers.StringContainer(
                    list(data) if data else []
                )
            case _:
                raise ValueError(f'Unsupported container type: {container_type}')

    def add(self, item: T) -> None:
        """Add item to container.

        Parameters
        ----------
        item : T
            The item to add to the container
        """
        self._container.add(item)

    def remove(self, item: T) -> int:
        """Remove all occurrences of item.

        Parameters
        ----------
        item : T
            The item to remove from the container

        Returns
        -------
        int
            Number of items removed
        """
        return self._container.remove(item)

    def __len__(self) -> int:
        """Get container size.

        Returns
        -------
        int
            Number of elements in the container
        """
        return len(self._container)

    def __bool__(self) -> bool:
        """Check if container is not empty.

        Returns
        -------
        bool
            True if container has elements, False otherwise
        """
        return bool(self._container)

    def __iter__(self) -> Iterator[T]:
        """Iterate over container.

        Returns
        -------
        Iterator[T]
            Iterator over container elements
        """
        return iter(self._container)

    def __getitem__(self, index: int) -> T:
        """Get item at index.

        Parameters
        ----------
        index : int
            Index of the item to retrieve

        Returns
        -------
        T
            The item at the specified index
        """
        return self._container[index]

    def filter(self, predicate: Callable[[T], bool]) -> list[T]:
        """Filter container elements.

        Parameters
        ----------
        predicate : Callable[[T], bool]
            Function to test each element

        Returns
        -------
        list[T]
            List of elements that satisfy the predicate
        """
        return [item for item in self._container if predicate(item)]

    def transform(self, func: Callable[[T], Any]) -> list[Any]:
        """Transform container elements.

        Parameters
        ----------
        func : Callable[[T], Any]
            Function to apply to each element

        Returns
        -------
        list[Any]
            List of transformed elements
        """
        return [func(item) for item in self._container]

    def __repr__(self) -> str:
        """String representation.

        Returns
        -------
        str
            String representation of the container
        """
        return f'Container[{self._type.__name__}]({list(self._container)})'


def create_container(data: Iterable[T]) -> Container[T]:
    """Factory function to create appropriately typed container.

    Uses pattern matching to determine container type.

    Parameters
    ----------
    data : Iterable[T]
        Data to populate the container

    Returns
    -------
    Container[T]
        Container with appropriate type

    Raises
    ------
    ValueError
        If data is empty or contains unsupported types
    """
    if not data:
        raise ValueError('Cannot determine type from empty data')

    # Get first item to determine type
    first_item = next(iter(data))

    match first_item:
        case int():
            return Container(int, data)
        case float():
            return Container(float, data)
        case str():
            return Container(str, data)
        case _:
            raise ValueError(f'Unsupported data type: {type(first_item)}')


# Re-export C++ classes
IntContainer = _containers.IntContainer
DoubleContainer = _containers.DoubleContainer
StringContainer = _containers.StringContainer
ContainerError = _containers.ContainerError

__all__ = [
    'Container',
    'IntContainer',
    'DoubleContainer',
    'StringContainer',
    'ContainerError',
    'create_container',
]
