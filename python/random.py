"""Modern Python wrapper for the random module.

Type-safe random number generation with enhanced Python integration.
"""

import random as _random
from typing import Generic, TypeVar

import cpp_features.random as _random_cpp

T = TypeVar('T')


class RandomGenerator:
    """Enhanced random generator with additional Python functionality.

    Parameters
    ----------
    seed : int, optional
        Seed value for reproducible results
    """

    def __init__(self, seed: int | None = None):
        if seed is not None:
            self._generator = _random_cpp.RandomGenerator(seed)
        else:
            self._generator = _random_cpp.RandomGenerator()

    def randint(self, min_val: int, max_val: int) -> int:
        """Generate random integer in range [min_val, max_val].

        Parameters
        ----------
        min_val : int
            Minimum value (inclusive)
        max_val : int
            Maximum value (inclusive)

        Returns
        -------
        int
            Random integer in the specified range
        """
        return self._generator.generate_int(min_val, max_val)

    def randlong(self, min_val: int, max_val: int) -> int:
        """Generate random long integer in range [min_val, max_val].

        Parameters
        ----------
        min_val : int
            Minimum value (inclusive)
        max_val : int
            Maximum value (inclusive)

        Returns
        -------
        int
            Random long integer in the specified range
        """
        return self._generator.generate_long(min_val, max_val)

    def random(self) -> float:
        """Generate random float in range [0.0, 1.0).

        Returns
        -------
        float
            Random float between 0.0 and 1.0
        """
        return self._generator.generate_real(0.0, 1.0)

    def uniform(self, min_val: float, max_val: float) -> float:
        """Generate random float in range [min_val, max_val).

        Parameters
        ----------
        min_val : float
            Minimum value (inclusive)
        max_val : float
            Maximum value (exclusive)

        Returns
        -------
        float
            Random float in the specified range
        """
        return self._generator.generate_real(min_val, max_val)

    def randfloat(self, min_val: float, max_val: float) -> float:
        """Generate random float32 value.

        Parameters
        ----------
        min_val : float
            Minimum value (inclusive)
        max_val : float
            Maximum value (exclusive)

        Returns
        -------
        float
            Random float32 in the specified range
        """
        return self._generator.generate_float(min_val, max_val)

    def choice(self, probability: float = 0.5) -> bool:
        """Generate random boolean with given probability.

        Parameters
        ----------
        probability : float, default=0.5
            Probability of returning True

        Returns
        -------
        bool
            Random boolean value
        """
        return self._generator.generate_bool(probability)

    def normal(self, mean: float = 0.0, stddev: float = 1.0) -> float:
        """Generate random value from normal distribution.

        Parameters
        ----------
        mean : float, default=0.0
            Mean of the distribution
        stddev : float, default=1.0
            Standard deviation of the distribution

        Returns
        -------
        float
            Random value from normal distribution
        """
        return self._generator.generate_normal(mean, stddev)

    def normal_float(self, mean: float = 0.0, stddev: float = 1.0) -> float:
        """Generate random float32 from normal distribution.

        Parameters
        ----------
        mean : float, default=0.0
            Mean of the distribution
        stddev : float, default=1.0
            Standard deviation of the distribution

        Returns
        -------
        float
            Random float32 from normal distribution
        """
        return self._generator.generate_normal_float(mean, stddev)

    def integers(self, min_val: int, max_val: int, count: int) -> list[int]:
        """Generate list of random integers.

        Parameters
        ----------
        min_val : int
            Minimum value (inclusive)
        max_val : int
            Maximum value (inclusive)
        count : int
            Number of integers to generate

        Returns
        -------
        list[int]
            List of random integers
        """
        return self._generator.generate_int_vector(min_val, max_val, count)

    def floats(self, min_val: float, max_val: float, count: int) -> list[float]:
        """Generate list of random floats.

        Parameters
        ----------
        min_val : float
            Minimum value (inclusive)
        max_val : float
            Maximum value (exclusive)
        count : int
            Number of floats to generate

        Returns
        -------
        list[float]
            List of random floats
        """
        return self._generator.generate_real_vector(min_val, max_val, count)

    def seed(self, seed: int) -> None:
        """Set seed for reproducible results.

        Parameters
        ----------
        seed : int
            Seed value for the generator
        """
        self._generator.seed(seed)

    def seed_with_time(self) -> None:
        """Seed with current time for non-deterministic results."""
        self._generator.seed_with_time()


def shuffle(container: list[T]) -> None:
    """Shuffle container in-place.

    Parameters
    ----------
    container : list[T]
        List to shuffle in place
    """
    match container:
        case list() if all(isinstance(x, int) for x in container):
            _random_cpp.shuffle_container(container)
        case list() if all(isinstance(x, float) for x in container):
            _random_cpp.shuffle_container(container)
        case list() if all(isinstance(x, str) for x in container):
            _random_cpp.shuffle_container(container)
        case _:
            _random.shuffle(container)


def sample(population: list[T], k: int) -> list[T]:
    """Sample k elements from population without replacement.

    Parameters
    ----------
    population : list[T]
        Population to sample from
    k : int
        Number of elements to sample

    Returns
    -------
    list[T]
        Sampled elements
    """
    if k > len(population):
        raise ValueError('Sample size cannot exceed population size')
    match population:
        case list() if all(isinstance(x, int) for x in population):
            return _random_cpp.sample_from_range(population, k)
        case list() if all(isinstance(x, float) for x in population):
            return _random_cpp.sample_from_range(population, k)
        case list() if all(isinstance(x, str) for x in population):
            return _random_cpp.sample_from_range(population, k)
        case _:
            return _random.sample(population, k)


def sample_string(text: str, k: int) -> list[str]:
    """Sample k characters from string.

    Parameters
    ----------
    text : str
        String to sample from
    k : int
        Number of characters to sample

    Returns
    -------
    list[str]
        List of sampled characters
    """
    return _random_cpp.sample_from_range(text, k)


class Distribution(Generic[T]):
    """Base class for probability distributions.

    Parameters
    ----------
    generator : RandomGenerator
        Random generator to use for sampling
    """

    def __init__(self, generator: RandomGenerator):
        self.generator = generator

    def sample(self) -> T:
        """Generate a single sample.

        Returns
        -------
        T
            Single sample from the distribution
        """
        raise NotImplementedError

    def samples(self, count: int) -> list[T]:
        """Generate multiple samples.

        Parameters
        ----------
        count : int
            Number of samples to generate

        Returns
        -------
        list[T]
            List of samples from the distribution
        """
        return [self.sample() for _ in range(count)]


class UniformInt(Distribution[int]):
    """Uniform integer distribution.

    Parameters
    ----------
    generator : RandomGenerator
        Random generator to use
    min_val : int
        Minimum value (inclusive)
    max_val : int
        Maximum value (inclusive)
    """

    def __init__(self, generator: RandomGenerator, min_val: int, max_val: int):
        super().__init__(generator)
        self.min_val = min_val
        self.max_val = max_val

    def sample(self) -> int:
        """Generate random integer.

        Returns
        -------
        int
            Random integer from uniform distribution
        """
        return self.generator.randint(self.min_val, self.max_val)


class UniformFloat(Distribution[float]):
    """Uniform float distribution.

    Parameters
    ----------
    generator : RandomGenerator
        Random generator to use
    min_val : float
        Minimum value (inclusive)
    max_val : float
        Maximum value (exclusive)
    """

    def __init__(self, generator: RandomGenerator, min_val: float, max_val: float):
        super().__init__(generator)
        self.min_val = min_val
        self.max_val = max_val

    def sample(self) -> float:
        """Generate random float.

        Returns
        -------
        float
            Random float from uniform distribution
        """
        return self.generator.uniform(self.min_val, self.max_val)


class Normal(Distribution[float]):
    """Normal (Gaussian) distribution.

    Parameters
    ----------
    generator : RandomGenerator
        Random generator to use
    mean : float, default=0.0
        Mean of the distribution
    stddev : float, default=1.0
        Standard deviation of the distribution
    """

    def __init__(self, generator: RandomGenerator, mean: float = 0.0, stddev: float = 1.0):
        super().__init__(generator)
        self.mean = mean
        self.stddev = stddev

    def sample(self) -> float:
        """Generate random value from normal distribution.

        Returns
        -------
        float
            Random value from normal distribution
        """
        return self.generator.normal(self.mean, self.stddev)


# Convenience functions
_default_generator = RandomGenerator()


def randint(min_val: int, max_val: int) -> int:
    """Generate random integer using default generator.

    Parameters
    ----------
    min_val : int
        Minimum value (inclusive)
    max_val : int
        Maximum value (inclusive)

    Returns
    -------
    int
        Random integer
    """
    return _default_generator.randint(min_val, max_val)


def random() -> float:
    """Generate random float using default generator.

    Returns
    -------
    float
        Random float between 0.0 and 1.0
    """
    return _default_generator.random()


def uniform(min_val: float, max_val: float) -> float:
    """Generate random float in range using default generator.

    Parameters
    ----------
    min_val : float
        Minimum value (inclusive)
    max_val : float
        Maximum value (exclusive)

    Returns
    -------
    float
        Random float in range
    """
    return _default_generator.uniform(min_val, max_val)


def choice(probability: float = 0.5) -> bool:
    """Generate random boolean using default generator.

    Parameters
    ----------
    probability : float, default=0.5
        Probability of returning True

    Returns
    -------
    bool
        Random boolean value
    """
    return _default_generator.choice(probability)


def normal(mean: float = 0.0, stddev: float = 1.0) -> float:
    """Generate normal random value using default generator.

    Parameters
    ----------
    mean : float, default=0.0
        Mean of the distribution
    stddev : float, default=1.0
        Standard deviation of the distribution

    Returns
    -------
    float
        Random value from normal distribution
    """
    return _default_generator.normal(mean, stddev)


# Re-export C++ class
CppRandomGenerator = _random_cpp.RandomGenerator

__all__ = [
    'RandomGenerator',
    'Distribution',
    'UniformInt',
    'UniformFloat',
    'Normal',
    'CppRandomGenerator',
    'shuffle',
    'sample',
    'sample_string',
    'randint',
    'random',
    'uniform',
    'choice',
    'normal',
]
