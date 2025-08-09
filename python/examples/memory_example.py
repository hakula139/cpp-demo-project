#!/usr/bin/env python3
"""Memory module example demonstrating RAII and resource management patterns.

This example shows how to use the memory module for automatic resource
management, cleanup handling, and memory-safe operations in Python.
"""

import sys
from pathlib import Path
from typing import Any, Callable, List

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from memory import ResourceManager, managed_resources, scoped_resource


def basic_resource_management_demo() -> None:
    """Demonstrate basic resource manager usage.

    Shows how to create and manage resources with automatic cleanup
    using the resource manager context manager pattern.
    """
    print('=== Basic Resource Management Demo ===')

    cleanup_log = []

    def create_cleanup_function(name: str) -> Callable[[], None]:
        """Create a cleanup function that logs its execution."""

        def cleanup():
            cleanup_log.append(f'Cleaned up {name}')

        return cleanup

    # Using resource manager as context manager
    print('Creating resources with automatic cleanup:')

    with managed_resources() as manager:
        # Create shapes through resource manager
        circle = manager.create_circle(3.0)
        rectangle = manager.create_rectangle(4.0, 5.0)
        square_circle = manager.create_circle(2.5)

        print(f'Created circle: area={circle.get_area():.2f}')
        print(f'Created rectangle: area={rectangle.get_area():.2f}')
        print(f'Created square circle: area={square_circle.get_area():.2f}')

        # Register cleanup functions
        manager.register_cleanup(create_cleanup_function('circle'))
        manager.register_cleanup(create_cleanup_function('rectangle'))
        manager.register_cleanup(create_cleanup_function('square_circle'))

        print(f'Resources created and cleanup registered')

    print(f'After context manager exit:')
    print(f'Cleanup log: {cleanup_log}')
    print()


def scoped_resource_demo() -> None:
    """Demonstrate scoped resource management.

    Shows how to manage individual resources with automatic cleanup
    using scoped resource context managers.
    """
    print('=== Scoped Resource Demo ===')

    cleanup_log = []

    def resource_cleanup(name: str) -> Callable[[], None]:
        """Create a cleanup function for a named resource."""

        def cleanup():
            cleanup_log.append(f'Scoped cleanup: {name}')

        return cleanup

    # Single scoped resource
    print('Managing single scoped resource:')

    with scoped_resource('important_data', resource_cleanup('important_data')) as data:
        print(f'Using scoped resource: {data}')
        # Simulate work with the resource
        processed_data = f'processed_{data}'
        print(f'Processed data: {processed_data}')

    print(f'Scoped cleanup log: {cleanup_log}')

    # Multiple nested scoped resources
    cleanup_log.clear()
    print(f'\nNested scoped resources:')

    with scoped_resource('config', resource_cleanup('config')) as config:
        print(f'Loaded config: {config}')

        with scoped_resource('database', resource_cleanup('database')) as db:
            print(f'Connected to database: {db}')

            with scoped_resource('session', resource_cleanup('session')) as session:
                print(f'Created session: {session}')
                result = f'{config}+{db}+{session}'
                print(f'Combined result: {result}')

    print(f'Nested cleanup log: {cleanup_log}')
    print()


def complex_resource_hierarchy_demo() -> None:
    """Demonstrate complex resource hierarchies and dependencies.

    Shows how to manage interconnected resources with proper
    cleanup ordering and dependency management.
    """
    print('=== Complex Resource Hierarchy Demo ===')

    cleanup_log = []
    resource_count = 0

    def create_numbered_cleanup(resource_type: str, number: int) -> Callable[[], None]:
        """Create a numbered cleanup function for tracking order."""

        def cleanup():
            cleanup_log.append(f'{resource_type}_{number}')

        return cleanup

    with managed_resources() as manager:
        # Create a hierarchy of geometric shapes
        print('Creating geometric shape hierarchy:')

        # Base shapes
        base_circles = []
        for i in range(3):
            circle = manager.create_circle(float(i + 1))
            base_circles.append(circle)
            manager.register_cleanup(create_numbered_cleanup('circle', i + 1))
            resource_count += 1

        # Rectangles based on circle areas
        derived_rectangles = []
        for i, circle in enumerate(base_circles):
            width = circle.get_area() / 2
            height = 2.0
            rectangle = manager.create_rectangle(width, height)
            derived_rectangles.append(rectangle)
            manager.register_cleanup(create_numbered_cleanup('rectangle', i + 1))
            resource_count += 1

        # Additional processing resources
        for i in range(2):
            manager.register_cleanup(create_numbered_cleanup('processor', i + 1))
            resource_count += 1

        print(
            f'Created {len(base_circles)} circles and {len(derived_rectangles)} rectangles'
        )
        print(f'Total resources managed: {resource_count}')

        # Calculate some metrics
        total_circle_area = sum(c.get_area() for c in base_circles)
        total_rectangle_area = sum(r.get_area() for r in derived_rectangles)

        print(f'Circle areas: {[f"{c.get_area():.2f}" for c in base_circles]}')
        print(f'Rectangle areas: {[f"{r.get_area():.2f}" for r in derived_rectangles]}')
        print(f'Total circle area: {total_circle_area:.2f}')
        print(f'Total rectangle area: {total_rectangle_area:.2f}')

    print(f'Cleanup order: {cleanup_log}')
    print(f'Resources cleaned up: {len(cleanup_log)}')
    print()


def error_handling_demo() -> None:
    """Demonstrate error handling with resource management.

    Shows how resources are properly cleaned up even when
    exceptions occur during resource usage.
    """
    print('=== Error Handling Demo ===')

    cleanup_log = []

    def error_cleanup(name: str) -> Callable[[], None]:
        """Create cleanup function for error scenarios."""

        def cleanup():
            cleanup_log.append(f'Error cleanup: {name}')

        return cleanup

    # Successful resource management
    print('Successful resource management:')
    cleanup_log.clear()

    try:
        with managed_resources() as manager:
            circle = manager.create_circle(5.0)
            manager.register_cleanup(error_cleanup('success_circle'))

            # Successful operations
            area = circle.get_area()
            print(f'Circle area calculated: {area:.2f}')

    except Exception as e:
        print(f'Unexpected error: {e}')

    print(f'Success cleanup log: {cleanup_log}')

    # Resource management with exception
    print(f'\nResource management with exception:')
    cleanup_log.clear()

    try:
        with managed_resources() as manager:
            circle = manager.create_circle(3.0)
            rectangle = manager.create_rectangle(4.0, 2.0)

            manager.register_cleanup(error_cleanup('error_circle'))
            manager.register_cleanup(error_cleanup('error_rectangle'))

            print(f'Resources created successfully')

            # Simulate an error during processing
            raise ValueError('Simulated processing error')

    except ValueError as e:
        print(f'Caught expected error: {e}')
    except Exception as e:
        print(f'Unexpected error: {e}')

    print(f'Error cleanup log: {cleanup_log}')

    # Nested error handling
    print(f'\nNested error handling:')
    cleanup_log.clear()

    try:
        with scoped_resource('outer', error_cleanup('outer')) as outer:
            print(f'Outer resource: {outer}')

            try:
                with scoped_resource('inner', error_cleanup('inner')) as inner:
                    print(f'Inner resource: {inner}')
                    raise RuntimeError('Inner error')
            except RuntimeError as e:
                print(f'Inner error handled: {e}')

            print(f'Continuing with outer resource')

    except Exception as e:
        print(f'Outer error: {e}')

    print(f'Nested error cleanup log: {cleanup_log}')
    print()


def performance_and_scalability_demo() -> None:
    """Demonstrate performance characteristics of resource management.

    Shows how the resource manager performs with many resources
    and measures cleanup overhead.
    """
    print('=== Performance and Scalability Demo ===')

    import time

    # Test with many resources
    resource_counts = [10, 50, 100, 500]

    print('Performance testing with different resource counts:')

    for count in resource_counts:
        cleanup_log = []

        def perf_cleanup(i: int) -> Callable[[], None]:
            def cleanup():
                cleanup_log.append(f'resource_{i}')

            return cleanup

        # Measure creation and cleanup time
        start_time = time.perf_counter()

        with managed_resources() as manager:
            shapes = []

            # Create many shapes
            for i in range(count):
                if i % 2 == 0:
                    shape = manager.create_circle(float(i + 1))
                else:
                    shape = manager.create_rectangle(float(i + 1), float(i + 2))

                shapes.append(shape)
                manager.register_cleanup(perf_cleanup(i))

            creation_time = time.perf_counter() - start_time

            # Do some work with the shapes
            total_area = sum(shape.get_area() for shape in shapes)

        total_time = time.perf_counter() - start_time
        cleanup_time = total_time - creation_time

        print(f'  {count} resources:')
        print(f'    Creation time: {creation_time:.6f}s')
        print(f'    Cleanup time: {cleanup_time:.6f}s')
        print(f'    Total time: {total_time:.6f}s')
        print(f'    Total area: {total_area:.2f}')
        print(f'    Cleanup count: {len(cleanup_log)}')

        # Calculate per-resource overhead
        if count > 0:
            per_resource_time = total_time / count
            print(f'    Per-resource time: {per_resource_time:.8f}s')

    print()


def real_world_patterns_demo() -> None:
    """Demonstrate real-world resource management patterns.

    Shows practical examples of how to use resource management
    in typical application scenarios.
    """
    print('=== Real-World Patterns Demo ===')

    # Database-like connection pattern
    print('Database-like connection pattern:')

    cleanup_log = []

    def connection_cleanup(conn_id: str) -> Callable[[], None]:
        def cleanup():
            cleanup_log.append(f'Closed connection {conn_id}')

        return cleanup

    def transaction_cleanup(tx_id: str) -> Callable[[], None]:
        def cleanup():
            cleanup_log.append(f'Rolled back transaction {tx_id}')

        return cleanup

    with managed_resources() as manager:
        # Simulate database connections and transactions
        connections = []

        for i in range(3):
            conn_id = f'conn_{i}'
            # In real world, this would be actual connection
            connection = {'id': conn_id, 'active': True}
            connections.append(connection)
            manager.register_cleanup(connection_cleanup(conn_id))

            # Create transaction for each connection
            tx_id = f'tx_{i}'
            manager.register_cleanup(transaction_cleanup(tx_id))

        print(f'Created {len(connections)} connections with transactions')

        # Simulate work
        for conn in connections:
            print(f'  Working with {conn["id"]}')

    print(f'Connection cleanup log: {cleanup_log}')

    # File-like resource pattern
    print(f'\nFile-like resource pattern:')
    cleanup_log.clear()

    def file_cleanup(filename: str) -> Callable[[], None]:
        def cleanup():
            cleanup_log.append(f'Closed file {filename}')

        return cleanup

    def buffer_cleanup(buffer_id: str) -> Callable[[], None]:
        def cleanup():
            cleanup_log.append(f'Freed buffer {buffer_id}')

        return cleanup

    with managed_resources() as manager:
        # Simulate file operations
        files = ['data.txt', 'config.json', 'output.log']

        for filename in files:
            # Register file cleanup
            manager.register_cleanup(file_cleanup(filename))

            # Register associated buffer cleanup
            buffer_id = f'buffer_for_{filename}'
            manager.register_cleanup(buffer_cleanup(buffer_id))

            print(f'Opened {filename} with buffer {buffer_id}')

    print(f'File cleanup log: {cleanup_log}')

    # Thread-like resource pattern
    print(f'\nThread-like resource pattern:')
    cleanup_log.clear()

    def thread_cleanup(thread_id: str) -> Callable[[], None]:
        def cleanup():
            cleanup_log.append(f'Joined thread {thread_id}')

        return cleanup

    def lock_cleanup(lock_id: str) -> Callable[[], None]:
        def cleanup():
            cleanup_log.append(f'Released lock {lock_id}')

        return cleanup

    with managed_resources() as manager:
        # Simulate thread management
        threads = ['worker_1', 'worker_2', 'monitor']
        locks = ['data_lock', 'log_lock']

        for thread_id in threads:
            manager.register_cleanup(thread_cleanup(thread_id))
            print(f'Started thread {thread_id}')

        for lock_id in locks:
            manager.register_cleanup(lock_cleanup(lock_id))
            print(f'Acquired lock {lock_id}')

    print(f'Thread cleanup log: {cleanup_log}')
    print()


def main() -> int:
    """Run all memory management examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Memory Module Example')
    print('====================')
    print()

    try:
        basic_resource_management_demo()
        scoped_resource_demo()
        complex_resource_hierarchy_demo()
        error_handling_demo()
        performance_and_scalability_demo()
        real_world_patterns_demo()

        print('All memory management examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running memory examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
