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


def sort(
    data: list[SupportsRichComparisonT] | Container[SupportsRichComparisonT],
) -> None:
    """Sort a container in-place.

    Sorts the elements in the container in ascending order.

    Parameters
    ----------
    data : list[SupportsRichComparisonT] | Container[SupportsRichComparisonT]
        The container to sort in-place

    Examples
    --------
    >>> data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
    >>> sort(data)
    >>> data
    [1, 1, 2, 3, 3, 4, 5, 5, 6, 9]
    >>> container = Container(str, ['cherry', 'banana', 'elderberry', 'date', 'apple'])
    >>> sort(container)
    >>> list(container)
    ['apple', 'banana', 'cherry', 'date', 'elderberry']
    """
    match data:
        case Container():
            _algorithms.sort(data._container)
        case _:
            data.sort()


def count_if(data: Iterable[T], predicate: Callable[[T], bool]) -> int:
    """Count elements in a sequence that satisfy a predicate.

    Counts the number of elements in the sequence for which the predicate
    returns true.

    Parameters
    ----------
    data : Iterable[T]
        The input sequence to examine
    predicate : Callable[[T], bool]
        The predicate function to test each element

    Returns
    -------
    int
        The number of elements that satisfy the predicate

    Examples
    --------
    >>> count_if([1, 2, 3, 4, 5], lambda x: x % 2 == 1)
    3
    >>> count_if(Container(str, ['apple', 'banana', 'cherry']), lambda x: len(x) > 5)
    2
    """
    match data:
        case Container():
            return _algorithms.count_if(data._container, predicate)
        case _:
            return sum(1 for item in data if predicate(item))


def transform_to_list(data: Iterable[T], func: Callable[[T], U]) -> list[U]:
    """Transform a sequence into a list using a transformation function.

    Applies the transformation function to each element in the input sequence
    and collects the results in a new list.

    Parameters
    ----------
    data : Iterable[T]
        The input sequence to transform
    func : Callable[[T], U]
        The transformation function to apply to each element

    Returns
    -------
    list[U]
        A list containing the transformed elements

    Examples
    --------
    >>> transform_to_list([1, 2, 3, 4, 5], lambda x: x * 2)
    [2, 4, 6, 8, 10]
    >>> transform_to_list(Container(str, ['apple', 'banana', 'cherry']), lambda x: x.upper())
    ['APPLE', 'BANANA', 'CHERRY']
    """
    match data:
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
    data : Iterable[SupportsRichComparisonT]
        The input sequence to search for minimum and maximum elements

    Returns
    -------
    tuple[SupportsRichComparisonT, SupportsRichComparisonT]
        A tuple containing the minimum and maximum elements (min, max)
    """
    match data:
        case Container():
            return _algorithms.find_min_max(data._container)
        case _:
            return min(data), max(data)


def pipeline(*functions: Callable[[Any], Any]) -> Callable[[Any], Any]:
    """Create a function pipeline.

    Creates a function that applies a sequence of functions in a pipeline.

    Parameters
    ----------
    *functions : Callable[[Any], Any]
        The functions to compose in pipeline order

    Returns
    -------
    Callable[[Any], Any]
        A composed function that applies all functions in sequence

    Examples
    --------
    >>> process = pipeline(
    ...     lambda data: [x * 2 for x in data],
    ...     lambda data: [x for x in data if x > 5],
    ...     sum,
    ... )
    ...
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
    'sort',
    'count_if',
    'transform_to_list',
    'find_min_max',
    'pipeline',
]
