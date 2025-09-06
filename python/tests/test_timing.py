"""Tests for the timing module."""

from time import sleep

import pytest
from demo.timing import (
    BenchmarkResult,
    Timer,
    benchmark,
    measure_time,
    time_function,
    to_human_readable,
)


class TestTimer:
    """Test Timer functionality."""

    def test_immediate_timing(self) -> None:
        """Test creation and immediate timing."""
        timer = Timer()
        sleep(0.1)

        elapsed_ms = timer.elapsed_ms
        print(elapsed_ms)

        assert 100 <= elapsed_ms < 300

    def test_start_and_stop(self) -> None:
        """Test start and stop."""
        timer = Timer()
        timer.start()
        sleep(0.1)
        timer.stop()

        first_elapsed = timer.elapsed_ms
        print(first_elapsed)

        assert 100 <= first_elapsed < 300

        sleep(0.1)

        second_elapsed = timer.elapsed_ms
        print(second_elapsed)

        assert first_elapsed == second_elapsed  # Should be same after stop

    def test_reset(self) -> None:
        """Test reset."""
        timer = Timer()
        sleep(0.2)
        timer.reset()

        elapsed_ms = timer.elapsed_ms
        print(elapsed_ms)

        assert 0 <= elapsed_ms < 100

    def test_multiple_start(self) -> None:
        """Test multiple start calls."""
        timer = Timer()
        timer.start()
        sleep(0.4)
        timer.start()  # Should reset the start time
        sleep(0.1)
        timer.stop()

        elapsed_ms = timer.elapsed_ms
        print(elapsed_ms)

        assert 100 <= elapsed_ms < 300

    def test_multiple_stop_calls(self) -> None:
        """Test multiple stop calls."""
        timer = Timer()
        timer.start()
        sleep(0.01)
        timer.stop()
        first_elapsed = timer.elapsed_ms
        timer.stop()  # Second stop should have no effect
        second_elapsed = timer.elapsed_ms

        assert first_elapsed == second_elapsed


class TestTimerDuration:
    """Test Timer duration types."""

    def test_different_types(self) -> None:
        """Test different duration type conversions."""
        timer = Timer()
        sleep(0.1)
        timer.stop()

        ns = timer.elapsed_ns
        us = timer.elapsed_us
        ms = timer.elapsed_ms
        s = timer.elapsed_s
        print(ns, us, ms, s)

        assert 100_000_000 <= ns < 300_000_000
        assert 100_000 <= us < 300_000
        assert 100 <= ms < 300
        assert s == 0

    def test_string_representation(self) -> None:
        """Test string representation."""
        timer = Timer()
        sleep(0.01)
        timer.stop()

        time_str = timer.elapsed_str
        print(time_str)

        assert len(time_str) > 0
        assert 'ms' in time_str


class TestScopedTimer:
    """Test scoped Timer functionality."""

    def test_context_manager(self) -> None:
        """Test context manager."""
        with Timer() as timer:
            sleep(0.1)

        elapsed_ns = timer.elapsed_ns
        print(elapsed_ns)

        assert 100_000_000 <= elapsed_ns < 300_000_000

    def test_measure_time(self) -> None:
        """Test measure_time function."""
        with measure_time('Scoped operation') as timer:
            sleep(0.1)

        elapsed_ns = timer.elapsed_ns
        print(elapsed_ns)

        assert 100_000_000 <= elapsed_ns < 300_000_000


class TestUtilityFunctions:
    """Test utility functions."""

    def test_to_human_readable(self) -> None:
        """Test conversion to human readable string."""
        assert to_human_readable(0) == '0ns'
        assert to_human_readable(123) == '123ns'
        assert to_human_readable(123_456) == '123.46μs'
        assert to_human_readable(123_456_789) == '123.46ms'
        assert to_human_readable(123_456_789_000) == '123.46s'
        assert to_human_readable(123_456_789_000_000) == '123456.79s'

    def test_time_function(self) -> None:
        """Test timing a function."""
        elapsed = time_function(lambda: sleep(0.1))
        print(elapsed)

        assert 100_000_000 <= elapsed < 300_000_000


class TestBenchmark:
    """Test benchmark functionality."""

    @pytest.mark.parametrize(
        'iterations',
        [5, 20],
    )
    def test_benchmark(self, iterations: int) -> None:
        """Test basic benchmarking."""

        def task() -> None:
            sleep(0.05)

        name = f'Task with {iterations} iterations'
        result = benchmark(name, task, iterations=iterations)

        assert result.name == name
        assert result.iterations == iterations
        assert result.total_ns >= 50_000_000 * iterations
        assert 50_000_000 <= result.min_ns <= result.avg_ns <= result.max_ns

    def test_print_result(self) -> None:
        """Test printing benchmark result."""
        result = benchmark('Test output', lambda: print('Test output'), iterations=5)
        result.print()
