"""Python wrapper for the random module."""

import random
from typing import TypeVar

import cpp_features.random as _random

from .containers import Container

T = TypeVar('T')


class RandomGenerator:
    """A random number generator."""

    def __init__(self, seed: int | None = None) -> None:
        """Initialize the random number generator.

        Parameters
        ----------
        seed : int, optional
            Seed value for reproducible results

        Examples
        --------
        >>> RandomGenerator()
        <RandomGenerator(seed=None) at 0x13911eaa0>
        >>> RandomGenerator(seed=12345)
        <RandomGenerator(seed=12345) at 0x13911eaa0>
        """
        if seed is not None:
            self._generator = _random.RandomGenerator(seed)
        else:
            self._generator = _random.RandomGenerator()

    def rand_int(self, min_val: int, max_val: int) -> int:
        """Generate a random integer within a range.

        Generates a uniformly distributed random integral value within the specified range.

        Parameters
        ----------
        min_val : int
            Minimum value (inclusive)
        max_val : int
            Maximum value (inclusive)

        Returns
        -------
        int
            Random integer in the range [min_val, max_val]

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.rand_int(1, 10)
        5
        """
        return self._generator.rand_int(min_val, max_val)

    def rand_float(self, min_val: float, max_val: float) -> float:
        """Generate a random floating-point value within a range.

        Generates a uniformly distributed random floating-point value within the specified range.

        Parameters
        ----------
        min_val : float
            Minimum value (inclusive)
        max_val : float
            Maximum value (exclusive)

        Returns
        -------
        float
            Random floating-point value in the range [min_val, max_val)

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.rand_float(0.0, 1.0)
        0.5
        """
        return self._generator.rand_float(min_val, max_val)

    def rand_ints(self, min_val: int, max_val: int, count: int) -> list[int]:
        """Generate a list of random integral values.

        Efficiently generates a vector of uniformly distributed random integral values.
        Each value is independently generated within the specified range.

        Parameters
        ----------
        min_val : int
            Minimum value for each element (inclusive)
        max_val : int
            Maximum value for each element (inclusive)
        count : int
            Number of random values to generate

        Returns
        -------
        list[int]
            A list of random integral values

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.rand_ints(1, 10, 5)
        [3, 8, 2, 6, 4]
        """
        if count < 0:
            raise ValueError('Count must be non-negative')
        return self._generator.rand_ints(min_val, max_val, count)

    def rand_floats(self, min_val: float, max_val: float, count: int) -> list[float]:
        """Generate a list of random floating-point values.

        Efficiently generates a vector of uniformly distributed random floating-point values.
        Each value is independently generated within the specified range.

        Parameters
        ----------
        min_val : float
            Minimum value for each element (inclusive)
        max_val : float
            Maximum value for each element (exclusive)
        count : int
            Number of random values to generate

        Returns
        -------
        list[float]
            A list of random floating-point values

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.rand_floats(0.0, 1.0, 5)
        [0.234, 0.765, 0.123, 0.890, 0.456]
        """
        if count < 0:
            raise ValueError('Count must be non-negative')
        return self._generator.rand_floats(min_val, max_val, count)

    def rand_bool(self, probability: float = 0.5) -> bool:
        """Generate a random boolean value with specified probability.

        Generates a random boolean value using a Bernoulli distribution with the specified probability.
        A probability of 0.5 creates a fair coin flip, while other values bias the outcome accordingly.

        Parameters
        ----------
        probability : float, default=0.5
            Probability of returning True

        Returns
        -------
        bool
            Random boolean value

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.rand_bool()     # 50% chance of True
        False
        >>> rg.rand_bool(0.7)  # 70% chance of True
        True
        """
        return self._generator.rand_bool(probability)

    def normal(self, mean: float = 0.0, stddev: float = 1.0) -> float:
        """Generate a random value from a normal (Gaussian) distribution.

        Generates a random value from a normal (Gaussian) distribution with the specified mean and
        standard deviation. This is useful for generating naturally distributed data, noise, or
        measurements with known statistics.

        Parameters
        ----------
        mean : float, default=0.0
            Mean (center) of the distribution
        stddev : float, default=1.0
            Standard deviation of the distribution

        Returns
        -------
        float
            A random value from the normal distribution

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.normal()
        0.5
        >>> rg.normal(100.0, 15.0)
        105.0
        """
        return self._generator.normal(mean, stddev)

    def seed(self, seed: int) -> None:
        """Manually seed the random number generator.

        Sets a specific seed for the random number generator. This allows for reproducible random
        sequences, which is useful for testing, debugging, or when deterministic behavior is required.

        Parameters
        ----------
        seed : int
            Seed value for the generator

        Examples
        --------
        >>> rg1 = RandomGenerator()
        >>> rg1.seed(42)
        >>> rg1.rand_int(1, 10)
        5
        >>> rg2 = RandomGenerator()
        >>> rg2.seed(42)
        >>> rg2.rand_int(1, 10)
        5
        """
        self._generator.seed(seed)

    def seed_with_time(self) -> None:
        """Seed the generator using current time.

        Re-seeds the random number generator using the current high-resolution time.
        This provides non-deterministic behavior and is useful when you want to reset the generator
        with a new unpredictable seed.

        Examples
        --------
        >>> rg = RandomGenerator()
        >>> rg.seed_with_time()
        """
        self._generator.seed_with_time()


def shuffle(data: list[T] | Container[T]) -> None:
    """Randomly shuffle elements in a container.

    Randomly reorders the elements in the provided range using the Fisher-Yates shuffle algorithm.

    Parameters
    ----------
    data : list[T] | Container[T]
        The container to shuffle in-place

    Examples
    --------
    >>> deck = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    >>> shuffle(deck)
    >>> deck
    [6, 10, 8, 3, 1, 4, 2, 7, 9, 5]
    >>> container = Container(str, ['apple', 'banana', 'cherry', 'date', 'elderberry'])
    >>> shuffle(container)
    >>> list(container)
    ['cherry', 'banana', 'elderberry', 'date', 'apple']
    """
    match data:
        case Container():
            _random.shuffle(data._container)
        case _:
            random.shuffle(data)


def sample(data: list[T] | Container[T], count: int) -> list[T]:
    """Randomly sample elements from a range.

    Selects a random subset of elements from the input range without replacement.
    If count exceeds the range size, all elements are returned. The relative order of sampled
    elements is preserved.

    Parameters
    ----------
    data : list[T] | Container[T]
        The source container to sample from
    count : int
        Number of elements to sample

    Returns
    -------
    list[T]
        A list of randomly selected elements from the range

    Examples
    --------
    >>> deck = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    >>> sample(deck, 5)
    [6, 10, 8, 3, 1]
    >>> container = Container(str, ['apple', 'banana', 'cherry', 'date', 'elderberry'])
    >>> sample(container, 3)
    ['cherry', 'banana', 'elderberry']
    """
    match data:
        case Container():
            return _random.sample(data._container, count)
        case _:
            return random.sample(data, count)  # nosec B311


__all__ = [
    'RandomGenerator',
    'shuffle',
    'sample',
]
