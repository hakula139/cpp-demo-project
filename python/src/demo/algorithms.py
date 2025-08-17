"""Python wrapper for the algorithms module."""

from __future__ import annotations

from collections.abc import Iterable
from typing import TYPE_CHECKING, Any, Callable, TypeVar

if TYPE_CHECKING:
    from _typeshed import SupportsRichComparisonT

import cpp_features.algorithms as _algorithms

from .containers import Container

T = TypeVar('T')
U = TypeVar('U')


def sort_inplace(data: list[T] | Container[T]) -> None:
    """Sort a list or Container in place.

    Parameters
    ----------
    data : list[T] or Container[T]
        The data structure to sort

    Examples
    --------
    >>> sort_inplace([3, 1, 4, 1, 5, 9, 2, 6, 5, 3])
    [1, 1, 2, 3, 3, 4, 5, 5, 6, 9]
    >>> sort_inplace(Container(str, ['cherry', 'banana', 'elderberry', 'date', 'apple']))
    ['apple', 'banana', 'cherry', 'date', 'elderberry']
    """
    match data:
        case list():
            _algorithms.sort(data)
        case Container():
            _algorithms.sort(data._container)


def count_if(data: Iterable[T], predicate: Callable[[T], bool]) -> int:
    """Count elements in a sequence that satisfy a predicate.

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

    Examples
    --------
    >>> count_if([1, 2, 3, 4, 5], lambda x: x % 2 == 1)
    3
    >>> count_if(Container(str, ['apple', 'banana', 'cherry']), lambda x: len(x) > 5)
    2
    """
    match data:
        case list() | tuple():
            return _algorithms.count_if(list(data), predicate)
        case Container():
            return _algorithms.count_if(data._container, predicate)
        case _:
            return sum(1 for item in data if predicate(item))


def transform_to_list(data: Iterable[T], func: Callable[[T], U]) -> list[U]:
    """Transform a sequence into a list using a transformation function.

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

    Examples
    --------
    >>> transform_to_list([1, 2, 3, 4, 5], lambda x: x * 2)
    [2, 4, 6, 8, 10]
    >>> transform_to_list(Container(str, ['apple', 'banana', 'cherry']), lambda x: x.upper())
    ['APPLE', 'BANANA', 'CHERRY']
    """
    match data:
        case list() | tuple():
            return _algorithms.transform_to_list(list(data), func)
        case Container():
            return _algorithms.transform_to_list(data._container, func)
        case _:
            return [func(item) for item in data]


def find_min_max(
    data: Iterable[SupportsRichComparisonT],
) -> tuple[SupportsRichComparisonT, SupportsRichComparisonT]:
    """Find the minimum and maximum elements in a sequence.

    Parameters
    ----------
    data : Iterable[T]
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
            return min(data), max(data)


def pipeline(*functions: Callable[[Any], Any]) -> Callable[[Any], Any]:
    """Create a function pipeline.

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
    ...     lambda data: [x * 2 for x in data],
    ...     lambda data: [x for x in data if x > 5],
    ...     sum,
    ... )
    >>> process([1, 2, 3, 4, 5])
    24
    """

    def composed(data: Any) -> Any:
        result = data
        for f in functions:
            result = f(result)
        return result

    return composed


__all__ = [
    'sort_inplace',
    'count_if',
    'transform_to_list',
    'find_min_max',
    'pipeline',
]
