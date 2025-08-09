"""Tests for Python timing module.

Comprehensive tests following C++ test patterns for timing functionality.
"""

import time

import pytest

from python import timing


class TestTimer:
    """Test Timer functionality."""

    def test_timer_creation(self) -> None:
        """Test basic timer creation."""
        timer = timing.Timer()
        assert timer is not None

    def test_timer_start_stop(self) -> None:
        """Test timer start and stop."""
        timer = timing.Timer()
        timer.start()
        time.sleep(0.01)  # Sleep for 10ms
        timer.stop()

        assert timer.elapsed_ns > 0
        assert timer.elapsed_us > 0
        assert timer.elapsed_ms >= 0  # Might be 0 for very short times

    def test_timer_reset(self) -> None:
        """Test timer reset functionality."""
        timer = timing.Timer()
        timer.start()
        time.sleep(0.01)
        timer.stop()

        initial_elapsed = timer.elapsed_ns
        assert initial_elapsed > 0

        timer.reset()
        # After reset, elapsed time should be minimal
        assert timer.elapsed_ns < initial_elapsed

    def test_timer_properties(self) -> None:
        """Test timer property accessors."""
        timer = timing.Timer()
        timer.start()
        time.sleep(0.01)
        timer.stop()

        # Test all time unit properties
        ns = timer.elapsed_ns
        us = timer.elapsed_us
        ms = timer.elapsed_ms
        s = timer.elapsed_s

        assert ns > 0
        assert us > 0
        assert ns >= us  # ns should be >= us (1000x conversion)
        assert us >= ms  # us should be >= ms (1000x conversion)
        assert ms >= s   # ms should be >= s (1000x conversion)

    def test_timer_string_representation(self) -> None:
        """Test timer string representation."""
        timer = timing.Timer()
        timer.start()
        time.sleep(0.001)
        timer.stop()

        elapsed_str = timer.elapsed_string
        str_repr = str(timer)

        assert isinstance(elapsed_str, str)
        assert isinstance(str_repr, str)
        assert elapsed_str == str_repr

    def test_timer_context_manager(self) -> None:
        """Test timer as context manager."""
        with timing.Timer() as timer:
            time.sleep(0.01)

        assert timer.elapsed_ns > 0

    def test_timer_multiple_measurements(self) -> None:
        """Test multiple measurements with same timer."""
        timer = timing.Timer()

        # First measurement
        timer.start()
        time.sleep(0.005)
        timer.stop()
        first_elapsed = timer.elapsed_ns

        # Reset and second measurement
        timer.reset()
        timer.start()
        time.sleep(0.01)
        timer.stop()
        second_elapsed = timer.elapsed_ns

        assert first_elapsed > 0
        assert second_elapsed > 0
        assert second_elapsed > first_elapsed


class TestMeasureTime:
    """Test measure_time context manager."""

    def test_measure_time_basic(self, capsys) -> None:
        """Test basic measure_time usage."""
        with timing.measure_time('test operation') as timer:
            time.sleep(0.01)

        assert timer.elapsed_ns > 0

        # Check that start/finish messages were printed
        captured = capsys.readouterr()
        assert 'Starting: test operation' in captured.out
        assert 'Finished test operation:' in captured.out

    def test_measure_time_no_name(self) -> None:
        """Test measure_time without name."""
        with timing.measure_time() as timer:
            time.sleep(0.005)

        assert timer.elapsed_ns > 0

    def test_measure_time_with_exception(self, capsys) -> None:
        """Test measure_time with exception."""
        try:
            with timing.measure_time('failing operation') as timer:
                time.sleep(0.005)
                raise ValueError('Test error')
        except ValueError:
            pass

        # Timer should still have measured time
        assert timer.elapsed_ns > 0

        # Finish message should still be printed
        captured = capsys.readouterr()
        assert 'Finished failing operation:' in captured.out


class TestBenchmark:
    """Test Benchmark functionality."""

    def test_benchmark_creation(self) -> None:
        """Test benchmark creation."""
        benchmark = timing.Benchmark('test_benchmark')
        assert benchmark.name == 'test_benchmark'
        assert benchmark.measurements == []

    def test_benchmark_default_name(self) -> None:
        """Test benchmark with default name."""
        benchmark = timing.Benchmark()
        assert benchmark.name == 'Benchmark'

    def test_benchmark_run_simple(self) -> None:
        """Test running simple benchmark."""
        def simple_function() -> int:
            return sum(range(100))

        benchmark = timing.Benchmark('sum_test')
        stats = benchmark.run(simple_function, iterations=5)

        assert stats['name'] == 'sum_test'
        assert stats['iterations'] == 5
        assert stats['total_time_ms'] > 0
        assert stats['mean_ms'] > 0
        assert stats['min_ms'] >= 0
        assert stats['max_ms'] >= stats['min_ms']
        assert len(stats['measurements_ns']) == 5

    def test_benchmark_statistics(self) -> None:
        """Test benchmark statistics calculation."""
        def test_function() -> None:
            time.sleep(0.001)  # 1ms sleep

        benchmark = timing.Benchmark('sleep_test')
        stats = benchmark.run(test_function, iterations=3)

        # Verify statistical measures
        assert 'mean_ms' in stats
        assert 'median_ms' in stats
        assert 'stdev_ms' in stats
        assert 'human_readable' in stats

        # Check human readable format
        human = stats['human_readable']
        assert 'mean' in human
        assert 'min' in human
        assert 'max' in human

    def test_benchmark_get_statistics_empty(self) -> None:
        """Test getting statistics from empty benchmark."""
        benchmark = timing.Benchmark()
        stats = benchmark.get_statistics()

        assert stats == {}

    def test_benchmark_multiple_runs(self) -> None:
        """Test multiple benchmark runs."""
        def test_function() -> int:
            return len([x for x in range(50)])

        benchmark = timing.Benchmark('multi_test')

        # First run
        stats1 = benchmark.run(test_function, iterations=3)
        assert len(stats1['measurements_ns']) == 3

        # Second run (should clear previous measurements)
        stats2 = benchmark.run(test_function, iterations=2)
        assert len(stats2['measurements_ns']) == 2
        assert len(benchmark.measurements) == 2

    def test_benchmark_compare_with(self) -> None:
        """Test comparing benchmarks."""
        def fast_function() -> int:
            return 42

        def slow_function() -> int:
            return sum(range(1000))

        fast_bench = timing.Benchmark('fast')
        slow_bench = timing.Benchmark('slow')

        fast_bench.run(fast_function, iterations=5)
        slow_bench.run(slow_function, iterations=5)

        comparison = fast_bench.compare_with(slow_bench)

        assert 'benchmarks' in comparison
        assert comparison['benchmarks'] == ['fast', 'slow']
        assert 'ratio' in comparison
        assert 'faster' in comparison
        assert 'speedup' in comparison
        assert 'difference_ms' in comparison

        # Fast function should be faster
        assert comparison['faster'] == 'fast'
        assert comparison['speedup'] >= 1.0

    def test_benchmark_compare_error(self) -> None:
        """Test benchmark comparison with no measurements."""
        benchmark1 = timing.Benchmark('empty1')
        benchmark2 = timing.Benchmark('empty2')

        with pytest.raises(ValueError, match='Both benchmarks must have measurements'):
            benchmark1.compare_with(benchmark2)


class TestBenchmarkFunction:
    """Test benchmark_function utility."""

    def test_benchmark_function_basic(self) -> None:
        """Test basic benchmark_function usage."""
        def test_func() -> list[int]:
            return [x * x for x in range(10)]

        stats = timing.benchmark_function(test_func, iterations=3)

        assert stats['name'] == 'test_func'
        assert stats['iterations'] == 3
        assert stats['mean_ms'] >= 0

    def test_benchmark_function_with_name(self) -> None:
        """Test benchmark_function with custom name."""
        def test_func() -> int:
            return 42

        stats = timing.benchmark_function(test_func, iterations=2, name='custom_name')

        assert stats['name'] == 'custom_name'
        assert stats['iterations'] == 2

    def test_benchmark_function_lambda(self) -> None:
        """Test benchmark_function with lambda."""
        stats = timing.benchmark_function(lambda: sum(range(50)), iterations=3)

        assert stats['name'] == '<lambda>'
        assert stats['iterations'] == 3


class TestTimeFunction:
    """Test time_function utility."""

    def test_time_function_basic(self) -> None:
        """Test basic time_function usage."""
        def test_func() -> str:
            return 'hello' * 100

        elapsed_str = timing.time_function(test_func)

        assert isinstance(elapsed_str, str)
        # Should contain time unit (ns, us, ms, or s)
        assert any(unit in elapsed_str for unit in ['ns', 'us', 'ms', 's'])

    def test_time_function_lambda(self) -> None:
        """Test time_function with lambda."""
        elapsed_str = timing.time_function(lambda: [x for x in range(100)])

        assert isinstance(elapsed_str, str)

    def test_time_function_different_complexities(self) -> None:
        """Test time_function with different complexity functions."""
        def simple_func() -> int:
            return 1 + 1

        def complex_func() -> int:
            return sum(x * x for x in range(1000))

        simple_time = timing.time_function(simple_func)
        complex_time = timing.time_function(complex_func)

        # Both should return valid time strings
        assert isinstance(simple_time, str)
        assert isinstance(complex_time, str)


class TestTimingIntegration:
    """Integration tests for timing functionality."""

    def test_timer_benchmark_integration(self) -> None:
        """Test using Timer within benchmark context."""
        def timed_operation() -> None:
            with timing.Timer() as timer:
                time.sleep(0.001)
            return timer.elapsed_ns

        benchmark = timing.Benchmark('timer_integration')
        stats = benchmark.run(timed_operation, iterations=3)

        assert stats['iterations'] == 3
        assert stats['mean_ms'] > 0

    def test_multiple_timing_methods_comparison(self) -> None:
        """Test comparing different timing methods."""
        def test_operation() -> list[int]:
            return sorted([x for x in range(100, 0, -1)])

        # Method 1: Direct timing
        elapsed_direct = timing.time_function(test_operation)

        # Method 2: Benchmark with single iteration
        benchmark_stats = timing.benchmark_function(test_operation, iterations=1)

        # Method 3: Manual timer
        with timing.Timer() as manual_timer:
            test_operation()

        # All methods should return valid results
        assert isinstance(elapsed_direct, str)
        assert benchmark_stats['iterations'] == 1
        assert manual_timer.elapsed_ns > 0

    def test_performance_measurement_workflow(self) -> None:
        """Test complete performance measurement workflow."""
        # Define test functions with different performance characteristics
        def linear_operation(n: int = 100) -> int:
            return sum(range(n))

        def quadratic_operation(n: int = 50) -> int:
            return sum(i * j for i in range(n) for j in range(n))

        # Benchmark both operations
        linear_bench = timing.Benchmark('linear')
        quadratic_bench = timing.Benchmark('quadratic')

        linear_stats = linear_bench.run(linear_operation, iterations=5)
        quadratic_stats = quadratic_bench.run(quadratic_operation, iterations=5)

        # Compare performance
        comparison = linear_bench.compare_with(quadratic_bench)

        # Linear should generally be faster than quadratic
        assert comparison['faster'] == 'linear'
        assert comparison['speedup'] > 1.0

        # Verify statistics structure
        for stats in [linear_stats, quadratic_stats]:
            assert 'mean_ms' in stats
            assert 'human_readable' in stats
            assert len(stats['measurements_ns']) == 5

    def test_timing_with_exceptions(self) -> None:
        """Test timing behavior with exceptions."""
        def failing_function() -> None:
            time.sleep(0.001)
            raise RuntimeError('Intentional failure')

        # Timer should still measure time even if function fails
        try:
            with timing.Timer() as timer:
                failing_function()
        except RuntimeError:
            pass

        assert timer.elapsed_ns > 0

        # measure_time should also handle exceptions
        try:
            with timing.measure_time('failing_op') as timer:
                failing_function()
        except RuntimeError:
            pass

        assert timer.elapsed_ns > 0

    def test_precision_and_accuracy(self) -> None:
        """Test timing precision and accuracy."""
        # Test very short operations
        def micro_operation() -> int:
            return 1 + 1

        # Test multiple measurements for consistency
        measurements = []
        for _ in range(10):
            with timing.Timer() as timer:
                micro_operation()
            measurements.append(timer.elapsed_ns)

        # All measurements should be positive
        assert all(m > 0 for m in measurements)

        # Test longer operation for comparison
        def longer_operation() -> list[int]:
            return [x * x for x in range(1000)]

        with timing.Timer() as long_timer:
            longer_operation()

        # Longer operation should take more time than micro operation
        assert long_timer.elapsed_ns > min(measurements)
