"""Example demonstrating the usage of the exceptions module."""

import sys
from pathlib import Path

python_dir = Path(__file__).parent.parent
sys.path.append(str(python_dir / 'src'))
sys.path.append(str(python_dir.parent / 'build' / 'binding'))

from demo.exceptions import (  # noqa: E402
    BaseException,
    CalculationException,
    ErrorSeverity,
    ResourceException,
    ValidationException,
)


def main() -> None:
    """Run all exceptions examples."""
    print('=== Exceptions Module Example ===')

    # Demonstrate BaseException with different severity levels
    try:
        raise BaseException('This is a warning', ErrorSeverity.WARNING)
    except BaseException as e:
        print(f'Caught BaseException: {e}')
        print(f'Severity: {e.severity}')

    # Demonstrate ValidationException
    try:
        raise ValidationException('Invalid email format', 'email')
    except ValidationException as e:
        print(f'Caught ValidationException: {e}')
        print(f'Field: {e.field_name}')

    # Demonstrate ResourceException
    try:
        raise ResourceException('File not found', '/path/to/config.txt')
    except ResourceException as e:
        print(f'Caught ResourceException: {e}')
        print(f'Resource: {e.resource_name}')

    # Demonstrate CalculationException
    try:
        raise CalculationException('Division by zero', 0.0)
    except CalculationException as e:
        print(f'Caught CalculationException: {e}')
        print(f'Input value: {e.input_value}')

    print('=== Exceptions Module Example Completed ===')


if __name__ == '__main__':
    main()
