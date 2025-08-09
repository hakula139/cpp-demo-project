"""Modern Python wrapper for the algorithms module.

Functional programming utilities with enhanced Python integration.
"""

from collections.abc import Iterable, Sequence
from typing import Any, Callable, Generic, TypeVar

import cpp_features.algorithms as _algorithms

from .containers import Container

T = TypeVar('T')
U = TypeVar('U')


def sort_inplace(data: list[T] | Container[T]) -> None:
    """Sort container or list in place.

    Parameters
    ----------
    data : list[T] or Container[T]
        The data structure to sort

    Raises
    ------
    TypeError
        If data type is not supported for sorting
    """
    match data:
        case list():
            _algorithms.sort_container(data)
        case Container():
            _algorithms.sort_container(data._container)
        case _:
            raise TypeError(f'Unsupported type for sorting: {type(data)}')


def count_if(data: Iterable[T], predicate: Callable[[T], bool]) -> int:
    """Count elements matching predicate.

    Parameters
    ----------
    data : Iterable[T]
        The data to search through
    predicate : Callable[[T], bool]
        Function to test each element

    Returns
    -------
    int
        Number of elements matching the predicate
    """
    match data:
        case list() | tuple():
            return _algorithms.count_if(list(data), predicate)
        case Container():
            return _algorithms.count_if(data._container, predicate)
        case _:
            return sum(1 for item in data if predicate(item))


def transform(data: Iterable[T], func: Callable[[T], U]) -> list[U]:
    """Transform sequence using function.

    Parameters
    ----------
    data : Iterable[T]
        The data to transform
    func : Callable[[T], U]
        Function to apply to each element

    Returns
    -------
    list[U]
        List of transformed elements
    """
    match data:
        case list() | tuple():
            return _algorithms.transform_to_vector(list(data), func)
        case Container():
            return _algorithms.transform_to_vector(data._container, func)
        case _:
            return [func(item) for item in data]


def find_min_max(data: Sequence[T]) -> tuple[T, T]:
    """Find minimum and maximum values.

    Parameters
    ----------
    data : Sequence[T]
        The sequence to search

    Returns
    -------
    tuple[T, T]
        Tuple containing (minimum, maximum) values
    """
    match data:
        case list() | tuple():
            return _algorithms.find_min_max(list(data))
        case Container():
            return _algorithms.find_min_max(data._container)
        case _:
            items = list(data)
            return _algorithms.find_min_max(items)


def pipeline(*functions: Callable[[Any], Any]) -> Callable[[Any], Any]:
    """Create a function pipeline using modern Python features.

    Parameters
    ----------
    *functions : Callable[[Any], Any]
        Functions to compose in pipeline order

    Returns
    -------
    Callable[[Any], Any]
        Composed function that applies all functions in sequence

    Examples
    --------
    >>> process = pipeline(
    ...     lambda x: [i * 2 for i in x],
    ...     lambda x: [i for i in x if i > 5],
    ...     sum
    ... )
    >>> process([1, 2, 3, 4, 5])
    30
    """

    def composed(data: Any) -> Any:
        result = data
        for func in functions:
            result = func(result)
        return result

    return composed


def functional_chain(data: Iterable[T]) -> 'FunctionalChain[T]':
    """Create a functional chain for method chaining.

    Parameters
    ----------
    data : Iterable[T]
        Initial data for the chain

    Returns
    -------
    FunctionalChain[T]
        Functional chain for method chaining operations
    """
    return FunctionalChain(data)


class FunctionalChain(Generic[T]):
    """Functional programming chain with method chaining.

    Parameters
    ----------
    data : Iterable[T]
        Initial data for the chain
    """

    def __init__(self, data: Iterable[T]):
        self._data = list(data)

    def filter(self, predicate: Callable[[T], bool]) -> 'FunctionalChain[T]':
        """Filter elements.

        Parameters
        ----------
        predicate : Callable[[T], bool]
            Function to test each element

        Returns
        -------
        FunctionalChain[T]
            New chain with filtered elements
        """
        return FunctionalChain(item for item in self._data if predicate(item))

    def map(self, func: Callable[[T], U]) -> 'FunctionalChain[U]':
        """Transform elements.

        Parameters
        ----------
        func : Callable[[T], U]
            Function to apply to each element

        Returns
        -------
        FunctionalChain[U]
            New chain with transformed elements
        """
        return FunctionalChain(func(item) for item in self._data)

    def sort(
        self,
        *,
        key: Callable[[T], Any] | None = None,
        reverse: bool = False,
    ) -> 'FunctionalChain[T]':
        """Sort elements.

        Parameters
        ----------
        key : Callable[[T], Any], optional
            Function to extract comparison key from each element
        reverse : bool, default=False
            Whether to sort in reverse order

        Returns
        -------
        FunctionalChain[T]
            New chain with sorted elements
        """
        sorted_data = sorted(self._data, key=key, reverse=reverse)
        return FunctionalChain(sorted_data)

    def take(self, n: int) -> 'FunctionalChain[T]':
        """Take first n elements.

        Parameters
        ----------
        n : int
            Number of elements to take

        Returns
        -------
        FunctionalChain[T]
            New chain with first n elements
        """
        return FunctionalChain(self._data[:n])

    def skip(self, n: int) -> 'FunctionalChain[T]':
        """Skip first n elements.

        Parameters
        ----------
        n : int
            Number of elements to skip

        Returns
        -------
        FunctionalChain[T]
            New chain with remaining elements after skipping n
        """
        return FunctionalChain(self._data[n:])

    def collect(self) -> list[T]:
        """Collect results into list.

        Returns
        -------
        list[T]
            List containing all elements in the chain
        """
        return list(self._data)

    def reduce(self, func: Callable[[T, T], T], initial: T | None = None) -> T:
        """Reduce to single value.

        Parameters
        ----------
        func : Callable[[T, T], T]
            Binary function to apply for reduction
        initial : T, optional
            Initial value for reduction

        Returns
        -------
        T
            Single reduced value
        """
        from functools import reduce as _reduce

        if initial is not None:
            return _reduce(func, self._data, initial)
        return _reduce(func, self._data)


__all__ = [
    'sort_inplace',
    'count_if',
    'transform',
    'find_min_max',
    'pipeline',
    'functional_chain',
    'FunctionalChain',
]
