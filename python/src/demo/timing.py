"""Python wrapper for the timing module."""

from contextlib import contextmanager
from types import TracebackType
from typing import Any, Callable, Generator

import cpp_features.timing as _timing


class Timer:
    """High-resolution timer class for measuring elapsed time."""

    def __init__(self) -> None:
        """Construct a timer and start timing immediately.

        Creates a new timer instance and captures the current time as the start point.
        The timer begins measuring elapsed time immediately upon construction.
        """
        self._timer = _timing.Timer()

    def start(self) -> None:
        """Start or restart the timer.

        Captures the current time as the start point for timing measurements.
        If the timer was already running, this restarts the measurement.
        """
        self._timer.start()

    def stop(self) -> None:
        """Stop the timer.

        Captures the current time as the end point for timing measurements.
        After calling stop(), get_elapsed() methods will return the fixed
        duration between start() and stop() calls.
        """
        self._timer.stop()

    def reset(self) -> None:
        """Reset the timer to start a new measurement.

        Resets the timer by capturing the current time as the new start point and
        clearing any previously recorded end time. This allows reusing the same
        timer instance for multiple measurements.
        """
        self._timer.reset()

    @property
    def elapsed_ns(self) -> int:
        """Get the elapsed time in nanoseconds.

        Returns
        -------
        int
            The elapsed time in nanoseconds
        """
        return self._timer.get_elapsed_ns()

    @property
    def elapsed_us(self) -> int:
        """Get the elapsed time in microseconds.

        Returns
        -------
        int
            The elapsed time in microseconds
        """
        return self._timer.get_elapsed_us()

    @property
    def elapsed_ms(self) -> int:
        """Get the elapsed time in milliseconds.

        Returns
        -------
        int
            The elapsed time in milliseconds
        """
        return self._timer.get_elapsed_ms()

    @property
    def elapsed_s(self) -> int:
        """Get the elapsed time in seconds.

        Returns
        -------
        int
            The elapsed time in seconds
        """
        return self._timer.get_elapsed_s()

    @property
    def elapsed_str(self) -> str:
        """Get the elapsed time as a formatted string with appropriate units.

        Returns
        -------
        str
            A formatted string representing the elapsed time with units
        """
        return self._timer.get_elapsed_str()

    def __enter__(self) -> 'Timer':
        """Context manager entry.

        Starts the timer and returns the instance.
        This allows the timer to be used as a context manager.

        Returns
        -------
        Timer
            The timer instance
        """
        self.start()
        return self

    def __exit__(
        self,
        exc_type: type[BaseException] | None,
        exc_value: BaseException | None,
        exc_traceback: TracebackType | None,
    ) -> None:
        """Context manager exit.

        Stops the timer. Allows any exceptions to propagate normally.

        Parameters
        ----------
        exc_type : type[BaseException], optional
            Exception type (if any)
        exc_value : BaseException, optional
            Exception value (if any).
        exc_traceback : TracebackType, optional
            Exception traceback (if any)
        """
        self.stop()


@contextmanager
def measure_time(name: str | None = None) -> Generator[Timer, None, None]:
    """Context manager for measuring the execution time of a block of code.

    Parameters
    ----------
    name : str, optional
        Descriptive name for the timed operation

    Yields
    ------
    Timer
        Timer instance for the measurement

    Examples
    --------
    >>> from time import sleep
    >>> with measure_time('Scoped operation') as timer:
    ...     sleep(3)
    ...     print('Doing some work inside scoped timer...')
    ...
    Scoped operation: 3s
    """
    timer = Timer()
    if not name:
        name = 'Elapsed'
    try:
        yield timer
    finally:
        timer.stop()
        print(f'{name}: {timer.elapsed_str}')


def to_human_readable(ns: int) -> str:
    """Convert a duration in nanoseconds to a human-readable string.

    Automatically selects the most appropriate unit (ns, μs, ms, s) based on the
    magnitude of the duration for optimal readability.

    Parameters
    ----------
    ns : int
        The duration in nanoseconds

    Returns
    -------
    str
        A formatted string representing the duration with appropriate units

    Examples
    --------
    >>> to_human_readable(123_456_789)
    '123.46ms'
    """
    return _timing.to_human_readable(ns)


def time_function(func: Callable[[], Any]) -> int:
    """Utility function to time a single function execution.

    Parameters
    ----------
    func : Callable[[], Any]
        Function to time

    Returns
    -------
    int
        Elapsed time in nanoseconds
    """
    return _timing.time_function(func)


class BenchmarkResult:
    """Structure containing benchmark results and statistics."""

    def __init__(self, result: _timing.BenchmarkResult) -> None:
        """Initialize BenchmarkResult wrapper.

        Parameters
        ----------
        result : _timing.BenchmarkResult
            The underlying C++ BenchmarkResult object
        """
        self._result = result

    @property
    def name(self) -> str:
        """Name of the benchmark."""
        return self._result.name

    @property
    def iterations(self) -> int:
        """Number of iterations executed."""
        return self._result.iterations

    @property
    def total_ns(self) -> int:
        """Total execution time in nanoseconds."""
        return self._result.total_ns

    @property
    def avg_ns(self) -> int:
        """Average execution time in nanoseconds."""
        return self._result.avg_ns

    @property
    def min_ns(self) -> int:
        """Minimum execution time in nanoseconds."""
        return self._result.min_ns

    @property
    def max_ns(self) -> int:
        """Maximum execution time in nanoseconds."""
        return self._result.max_ns

    def print(self) -> None:
        """Print formatted benchmark results.

        Prints comprehensive benchmark statistics in a human-readable format with
        appropriate units and formatting.
        """
        _timing.BenchmarkRunner.print_result(self._result)


def benchmark(
    name: str, func: Callable[[], Any], *, iterations: int = 1000
) -> BenchmarkResult:
    """Utility function to benchmark a function with a given number of iterations.

    Parameters
    ----------
    name : str
        Descriptive name for the benchmark
    func : Callable[[], Any]
        Function to benchmark
    iterations : int, default=1000
        Number of times to execute the function

    Returns
    -------
    BenchmarkResult
        Structure containing timing statistics

    Examples
    --------
    >>> from random import randint
    >>> def sort_data() -> None:
    ...     data = [randint(1, 100) for _ in range(1000)]
    ...     data.sort()
    ...
    >>> result = benchmark('Sorting algorithm', sort_data, 1000)
    >>> result.print()
    Benchmark: Sorting algorithm
    - Iterations: 1000
    - Total time: 12.3ms
    - Average: 12.3μs
    - Min: 10.1μs
    - Max: 15.7μs
    """
    result = _timing.BenchmarkRunner.benchmark(name, func, iterations)
    return BenchmarkResult(result)


__all__ = [
    'Timer',
    'measure_time',
    'to_human_readable',
    'time_function',
    'BenchmarkResult',
    'benchmark',
]
