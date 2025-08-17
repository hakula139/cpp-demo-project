"""Modern Python wrapper for the timing module.

High-resolution timing and benchmarking utilities.
"""

import statistics
from contextlib import contextmanager
from typing import Any, Callable, ContextManager

import cpp_features.timing as _timing


class Timer:
    """Enhanced timer with additional Python functionality."""

    def __init__(self):
        self._timer = _timing.Timer()

    def start(self) -> None:
        """Start the timer."""
        self._timer.start()

    def stop(self) -> None:
        """Stop the timer."""
        self._timer.stop()

    def reset(self) -> None:
        """Reset the timer."""
        self._timer.reset()

    @property
    def elapsed_ns(self) -> int:
        """Get elapsed time in nanoseconds.

        Returns
        -------
        int
            Elapsed time in nanoseconds
        """
        return self._timer.get_elapsed_ns()

    @property
    def elapsed_us(self) -> int:
        """Get elapsed time in microseconds.

        Returns
        -------
        int
            Elapsed time in microseconds
        """
        return self._timer.get_elapsed_us()

    @property
    def elapsed_ms(self) -> int:
        """Get elapsed time in milliseconds.

        Returns
        -------
        int
            Elapsed time in milliseconds
        """
        return self._timer.get_elapsed_ms()

    @property
    def elapsed_s(self) -> int:
        """Get elapsed time in seconds.

        Returns
        -------
        int
            Elapsed time in seconds
        """
        return self._timer.get_elapsed_s()

    @property
    def elapsed_string(self) -> str:
        """Get human-readable elapsed time.

        Returns
        -------
        str
            Human-readable elapsed time string
        """
        return self._timer.get_elapsed_string()

    def __str__(self) -> str:
        """String representation.

        Returns
        -------
        str
            Human-readable elapsed time
        """
        return self.elapsed_string

    def __enter__(self) -> 'Timer':
        """Context manager entry.

        Returns
        -------
        Timer
            This timer instance
        """
        self.start()
        return self

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Context manager exit.

        Parameters
        ----------
        exc_type : Any
            Exception type (if any)
        exc_val : Any
            Exception value (if any)
        exc_tb : Any
            Exception traceback (if any)
        """
        self.stop()


@contextmanager
def measure_time(name: str | None = None) -> ContextManager[Timer]:
    """Context manager for measuring execution time.

    Parameters
    ----------
    name : str, optional
        Name to display during timing

    Yields
    ------
    Timer
        Timer instance for the measurement

    Examples
    --------
    >>> with measure_time('database query') as timer:
    ...     # Do work
    ...     pass
    >>> print(f'Elapsed: {timer.elapsed_string}')
    """
    timer = Timer()
    if name:
        print(f'Starting: {name}')
    try:
        yield timer
    finally:
        if name:
            print(f'Finished {name}: {timer.elapsed_string}')


class Benchmark:
    """Benchmarking utilities with statistical analysis.

    Parameters
    ----------
    name : str, default='Benchmark'
        Name of the benchmark
    """

    def __init__(self, name: str = 'Benchmark'):
        self.name = name
        self.measurements = []

    def run(self, func: Callable[[], Any], iterations: int = 10) -> dict[str, Any]:
        """Run benchmark for specified iterations.

        Parameters
        ----------
        func : Callable[[], Any]
            Function to benchmark
        iterations : int, default=10
            Number of iterations to run

        Returns
        -------
        dict[str, Any]
            Statistical summary of measurements
        """
        self.measurements.clear()

        for _ in range(iterations):
            elapsed_ns = _timing.time_function(func)
            self.measurements.append(elapsed_ns)

        return self.get_statistics()

    def get_statistics(self) -> dict[str, Any]:
        """Get statistical summary of measurements.

        Returns
        -------
        dict[str, Any]
            Dictionary containing statistical measures
        """
        if not self.measurements:
            return {}

        measurements_ms = [ns / 1_000_000 for ns in self.measurements]

        return {
            'name': self.name,
            'iterations': len(self.measurements),
            'total_time_ms': sum(measurements_ms),
            'mean_ms': statistics.mean(measurements_ms),
            'median_ms': statistics.median(measurements_ms),
            'min_ms': min(measurements_ms),
            'max_ms': max(measurements_ms),
            'stdev_ms': (
                statistics.stdev(measurements_ms) if len(measurements_ms) > 1 else 0.0
            ),
            'measurements_ns': self.measurements.copy(),
            'human_readable': {
                'mean': _timing.to_human_readable(
                    int(statistics.mean(self.measurements))
                ),
                'min': _timing.to_human_readable(min(self.measurements)),
                'max': _timing.to_human_readable(max(self.measurements)),
            },
        }

    def compare_with(self, other: 'Benchmark') -> dict[str, Any]:
        """Compare this benchmark with another.

        Parameters
        ----------
        other : Benchmark
            Other benchmark to compare with

        Returns
        -------
        dict[str, Any]
            Comparison results

        Raises
        ------
        ValueError
            If either benchmark has no measurements
        """
        if not self.measurements or not other.measurements:
            raise ValueError('Both benchmarks must have measurements')

        self_mean = statistics.mean(self.measurements)
        other_mean = statistics.mean(other.measurements)

        return {
            'benchmarks': [self.name, other.name],
            'ratio': self_mean / other_mean,
            'faster': other.name if self_mean > other_mean else self.name,
            'speedup': max(self_mean, other_mean) / min(self_mean, other_mean),
            'difference_ms': abs(self_mean - other_mean) / 1_000_000,
        }


def benchmark_function(
    func: Callable[[], Any], iterations: int = 10, name: str | None = None
) -> dict[str, Any]:
    """Benchmark a function and return statistics.

    Parameters
    ----------
    func : Callable[[], Any]
        Function to benchmark
    iterations : int, default=10
        Number of iterations to run
    name : str, optional
        Name for the benchmark

    Returns
    -------
    dict[str, Any]
        Statistical summary of the benchmark

    Examples
    --------
    >>> stats = benchmark_function(lambda: expensive_operation(), iterations=5)
    >>> print(f"Mean time: {stats['human_readable']['mean']}")
    """
    bench = Benchmark(name or func.__name__)
    return bench.run(func, iterations)


def time_function(func: Callable[[], Any]) -> str:
    """Time a single function execution.

    Parameters
    ----------
    func : Callable[[], Any]
        Function to time

    Returns
    -------
    str
        Human-readable elapsed time
    """
    elapsed_ns = _timing.time_function(func)
    return _timing.to_human_readable(elapsed_ns)


__all__ = [
    'Timer',
    'Benchmark',
    'measure_time',
    'benchmark_function',
    'time_function',
]
