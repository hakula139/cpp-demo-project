"""Example demonstrating the usage of the timing module."""

import sys
from pathlib import Path
from time import sleep

python_dir = Path(__file__).parent.parent
sys.path.append(str(python_dir / 'src'))
sys.path.append(str(python_dir.parent / 'build' / 'binding'))

from demo.timing import (  # noqa: E402
    Timer,
    benchmark,
    measure_time,
    time_function,
    to_human_readable,
)


def demonstrate_basic_timer() -> None:
    """Demonstrate basic timer usage with manual start / stop."""
    print('\n1. Basic Timer Usage:')

    timer = Timer()
    print('   Timer started...')

    sleep(0.05)

    timer.stop()
    print(f'   Timer stopped after {timer.elapsed_str}')
    print(f'   Elapsed nanoseconds: {timer.elapsed_ns}')
    print(f'   Elapsed microseconds: {timer.elapsed_us}')
    print(f'   Elapsed milliseconds: {timer.elapsed_ms}')

    timer.reset()
    print('   Timer reset')

    sleep(0.025)
    print(f'   New elapsed time: {timer.elapsed_str}')


def demonstrate_scoped_timer() -> None:
    """Demonstrate context-manager-based scoped timer."""
    print('\n2. Scoped Timer Usage:')

    with measure_time('Scoped operation') as timer:
        sleep(0.03)
        print('   Doing some work inside scoped timer...')

    # You can also access the timer after the scope ends.
    print(f'   Elapsed time: {timer.elapsed_str}')


def demonstrate_benchmarking() -> None:
    """Demonstrate benchmarking."""
    print('\n3. Benchmarking:')

    def simple_task() -> int:
        total = 0
        for i in range(1000):
            total += i
        return total

    result = benchmark('Simple task', simple_task, iterations=1000)
    result.print()

    def list_ops() -> None:
        data = list(range(1000))
        data.sort(reverse=True)

    list_result = benchmark('List Operations', list_ops, iterations=500)
    list_result.print()


def demonstrate_utility_functions() -> None:
    """Demonstrate utility functions."""
    print('\n4. Utility Functions:')

    elapsed = time_function(lambda: sleep(0.0005))
    elapsed_str = to_human_readable(elapsed)
    print(f'   time_function result: {elapsed}ns ({elapsed_str})')


def main() -> None:
    """Run all timing examples."""
    print('=== Timing Module Example ===')

    demonstrate_basic_timer()
    demonstrate_scoped_timer()
    demonstrate_benchmarking()
    demonstrate_utility_functions()

    print('\n=== Timing Module Example Completed ===')


if __name__ == '__main__':
    main()
