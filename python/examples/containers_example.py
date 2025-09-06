"""Example demonstrating the usage of the containers module."""

import sys
from pathlib import Path

python_dir = Path(__file__).parent.parent
sys.path.append(str(python_dir / 'src'))
sys.path.append(str(python_dir.parent / 'build' / 'binding'))

from demo.containers import Container


def main() -> None:
    """Run all container examples."""
    print('=== Containers Module Example ===')

    # Demonstrate empty container
    empty_container = Container(int)
    print(f'Empty container: {empty_container}')

    # Demonstrate initialization with integers
    numbers = Container(int, [1, 2, 3, 4, 5])
    print(f'Integer container: {numbers}')

    # Demonstrate adding elements
    numbers.add(6)
    numbers.add(7)
    print(f'After adding elements: {numbers}')

    # Demonstrate removing elements
    value = 3
    removed_count = numbers.remove(value)
    print(f'Removed {removed_count} elements of value {value}')
    print(f'After removing elements: {numbers}')

    # Demonstrate safe access to elements
    index = 3
    try:
        element = numbers[index]
        print(f'Element at index {index}: {element}')
    except IndexError:
        print(f'Error accessing element at index {index}')

    # Demonstrate filtering
    even_numbers = numbers.filter(lambda x: x % 2 == 0)
    print(f'Even numbers: {even_numbers}')

    # Demonstrate transformation
    doubled_numbers = numbers.transform(lambda n: n * 2)
    print(f'Doubled numbers: {doubled_numbers}')

    # Demonstrate initialization with strings
    words = Container(str, ['hello', 'modern', 'python', 'world'])
    print(f'String container: {words}')

    # Demonstrate initialization with doubles
    prices = Container(float, [19.999, 29.95, 5.50])
    print(f'Floating point container: {prices}')

    print('=== Containers Module Example Completed ===')


if __name__ == '__main__':
    main()
