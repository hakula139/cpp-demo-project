#!/usr/bin/env python3
"""Exceptions module example demonstrating safe error handling with Result types.

This example shows how to use the exceptions module for functional error
handling, avoiding exceptions in favor of explicit Result types.
"""

import math
import sys
from pathlib import Path
from typing import Callable, List, Optional

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from exceptions import Result, chain_operations, safe_divide, safe_sqrt


def basic_result_demo() -> None:
    """Demonstrate basic Result type usage.

    Shows how to create, check, and unwrap Result values for
    safe error handling without exceptions.
    """
    print('=== Basic Result Demo ===')

    # Creating successful Results
    success_result = Result.ok(42)
    print(f'Success result: {success_result}')
    print(f'Is success: {success_result.is_ok}')
    print(f'Value: {success_result.unwrap()}')

    # Creating error Results
    error_result = Result.error('Something went wrong')
    print(f'\nError result: {error_result}')
    print(f'Is error: {error_result.is_error}')
    print(f'Error message: {error_result.error_message}')

    # Safe unwrapping with defaults
    safe_value = error_result.unwrap_or(-1)
    print(f'Error result with default: {safe_value}')

    # Pattern matching style checking
    results = [success_result, error_result]

    print(f'\nProcessing multiple results:')
    for i, result in enumerate(results):
        if result.is_ok:
            print(f'  Result {i}: Success with value {result.unwrap()}')
        else:
            print(f'  Result {i}: Error - {result.error_message}')

    print()


def safe_operations_demo() -> None:
    """Demonstrate safe mathematical operations.

    Shows how to perform mathematical operations that can fail
    safely using Result types instead of raising exceptions.
    """
    print('=== Safe Operations Demo ===')

    # Safe division examples
    division_tests = [
        (10.0, 2.0),
        (15.0, 3.0),
        (7.0, 0.0),  # Division by zero
        (-8.0, 2.0),
        (0.0, 5.0),
    ]

    print('Safe division tests:')
    for dividend, divisor in division_tests:
        result = safe_divide(dividend, divisor)
        if result.is_ok:
            print(f'  {dividend} / {divisor} = {result.unwrap():.3f}')
        else:
            print(f'  {dividend} / {divisor} = Error: {result.error_message}')

    # Safe square root examples
    sqrt_tests = [16.0, 25.0, -4.0, 0.0, 2.5]

    print(f'\nSafe square root tests:')
    for value in sqrt_tests:
        result = safe_sqrt(value)
        if result.is_ok:
            print(f'  sqrt({value}) = {result.unwrap():.4f}')
        else:
            print(f'  sqrt({value}) = Error: {result.error_message}')

    print()


def result_chaining_demo() -> None:
    """Demonstrate Result chaining and transformation.

    Shows how to chain operations on Results and transform
    values while preserving error states.
    """
    print('=== Result Chaining Demo ===')

    # Basic chaining with map
    print('Result transformation with map:')

    success_num = Result.ok(5)
    doubled = success_num.map(lambda x: x * 2)
    squared = doubled.map(lambda x: x * x)

    print(f'Original: {success_num.unwrap()}')
    print(f'Doubled: {doubled.unwrap()}')
    print(f'Squared: {squared.unwrap()}')

    # Chaining with error propagation
    print(f'\nError propagation in chains:')

    error_num = Result.error('Invalid input')
    error_doubled = error_num.map(lambda x: x * 2)
    error_squared = error_doubled.map(lambda x: x * x)

    print(f'Error original: {error_num.error_message}')
    print(f'Error doubled: {error_doubled.error_message}')
    print(f'Error squared: {error_squared.error_message}')

    # Complex mathematical chains
    print(f'\nComplex mathematical chains:')

    test_values = [4.0, 16.0, -1.0, 0.0, 9.0]

    for value in test_values:
        # Chain: square root -> divide by 2 -> multiply by 3
        result = (
            safe_sqrt(value)
            .and_then(lambda x: safe_divide(x, 2.0))
            .map(lambda x: x * 3.0)
        )

        if result.is_ok:
            print(f'  Process({value}): sqrt -> /2 -> *3 = {result.unwrap():.4f}')
        else:
            print(f'  Process({value}): Error - {result.error_message}')

    print()


def operation_chaining_demo() -> None:
    """Demonstrate chaining of multiple operations.

    Shows how to compose multiple fallible operations into
    safe pipelines using the chain_operations function.
    """
    print('=== Operation Chaining Demo ===')

    # Define safe operations
    def safe_add_one(x: float) -> Result[float]:
        """Safely add one to a number."""
        return Result.ok(x + 1.0)

    def safe_multiply_by_two(x: float) -> Result[float]:
        """Safely multiply by two."""
        return Result.ok(x * 2.0)

    def safe_subtract_ten(x: float) -> Result[float]:
        """Safely subtract ten."""
        return Result.ok(x - 10.0)

    def safe_validate_positive(x: float) -> Result[float]:
        """Validate that a number is positive."""
        if x > 0:
            return Result.ok(x)
        else:
            return Result.error(f'Value {x} is not positive')

    # Simple chain
    print('Simple operation chain:')
    simple_chain = chain_operations(safe_add_one, safe_multiply_by_two)

    test_values = [5.0, 10.0, 0.0, -3.0]
    for value in test_values:
        result = simple_chain(value)
        if result.is_ok:
            print(f'  {value} -> +1 -> *2 = {result.unwrap()}')
        else:
            print(f'  {value} -> Error: {result.error_message}')

    # Complex chain with validation
    print(f'\nComplex chain with validation:')
    complex_chain = chain_operations(
        safe_add_one, safe_multiply_by_two, safe_subtract_ten, safe_validate_positive
    )

    for value in test_values:
        result = complex_chain(value)
        if result.is_ok:
            print(f'  {value} -> +1 -> *2 -> -10 -> validate = {result.unwrap()}')
        else:
            print(f'  {value} -> Error: {result.error_message}')

    # Mathematical chain with potential failures
    print(f'\nMathematical chain with potential failures:')

    def safe_reciprocal(x: float) -> Result[float]:
        """Safely calculate reciprocal."""
        if x == 0:
            return Result.error('Cannot calculate reciprocal of zero')
        return Result.ok(1.0 / x)

    def safe_logarithm(x: float) -> Result[float]:
        """Safely calculate natural logarithm."""
        if x <= 0:
            return Result.error(f'Cannot calculate log of {x}')
        return Result.ok(math.log(x))

    math_chain = chain_operations(safe_reciprocal, safe_logarithm)

    math_test_values = [1.0, 2.0, 0.5, 0.0, -1.0]
    for value in math_test_values:
        result = math_chain(value)
        if result.is_ok:
            print(f'  {value} -> 1/x -> ln(x) = {result.unwrap():.4f}')
        else:
            print(f'  {value} -> Error: {result.error_message}')

    print()


def error_accumulation_demo() -> None:
    """Demonstrate error accumulation and handling patterns.

    Shows different strategies for handling multiple potential
    errors and accumulating results or errors.
    """
    print('=== Error Accumulation Demo ===')

    # Processing lists with potential errors
    def safe_parse_number(s: str) -> Result[float]:
        """Safely parse a string as a number."""
        try:
            value = float(s)
            return Result.ok(value)
        except ValueError:
            return Result.error(f'Cannot parse "{s}" as number')

    def safe_sqrt_wrapper(x: float) -> Result[float]:
        """Wrapper for safe square root."""
        return safe_sqrt(x)

    # Test data with some invalid entries
    input_strings = ['4.0', '16.0', 'invalid', '25.0', '-1.0', '9.0', 'bad']

    print('Processing list with error accumulation:')

    # Collect successful results and errors separately
    successful_results = []
    errors = []

    for input_str in input_strings:
        parse_result = safe_parse_number(input_str)
        if parse_result.is_ok:
            sqrt_result = safe_sqrt_wrapper(parse_result.unwrap())
            if sqrt_result.is_ok:
                successful_results.append((input_str, sqrt_result.unwrap()))
            else:
                errors.append((input_str, sqrt_result.error_message))
        else:
            errors.append((input_str, parse_result.error_message))

    print(f'Input data: {input_strings}')
    print(f'Successful results: {successful_results}')
    print(f'Errors: {errors}')
    print(
        f'Success rate: {len(successful_results)}/{len(input_strings)} ({len(successful_results)/len(input_strings)*100:.1f}%)'
    )

    # Alternative: fail-fast approach
    print(f'\nFail-fast approach:')

    def process_all_or_fail(strings: List[str]) -> Result[List[float]]:
        """Process all strings or fail on first error."""
        results = []
        for s in strings:
            parse_result = safe_parse_number(s)
            if parse_result.is_error:
                return Result.error(f'Failed on "{s}": {parse_result.error_message}')

            sqrt_result = safe_sqrt_wrapper(parse_result.unwrap())
            if sqrt_result.is_error:
                return Result.error(f'Failed on "{s}": {sqrt_result.error_message}')

            results.append(sqrt_result.unwrap())

        return Result.ok(results)

    fail_fast_result = process_all_or_fail(input_strings)
    if fail_fast_result.is_ok:
        print(f'All processed successfully: {fail_fast_result.unwrap()}')
    else:
        print(f'Processing failed: {fail_fast_result.error_message}')

    # Process only valid inputs
    valid_inputs = ['4.0', '16.0', '25.0', '9.0']
    valid_result = process_all_or_fail(valid_inputs)
    print(
        f'Valid inputs result: {valid_result.unwrap() if valid_result.is_ok else valid_result.error_message}'
    )

    print()


def practical_patterns_demo() -> None:
    """Demonstrate practical error handling patterns.

    Shows real-world scenarios where Result types provide
    cleaner error handling than traditional exceptions.
    """
    print('=== Practical Patterns Demo ===')

    # Configuration parsing pattern
    def parse_config_value(key: str, value: str, value_type: str) -> Result[any]:
        """Parse a configuration value based on its expected type."""
        try:
            if value_type == 'int':
                return Result.ok(int(value))
            elif value_type == 'float':
                return Result.ok(float(value))
            elif value_type == 'bool':
                if value.lower() in ('true', '1', 'yes'):
                    return Result.ok(True)
                elif value.lower() in ('false', '0', 'no'):
                    return Result.ok(False)
                else:
                    return Result.error(f'Invalid boolean value: {value}')
            else:
                return Result.ok(value)  # String
        except ValueError:
            return Result.error(f'Cannot parse {key}="{value}" as {value_type}')

    print('Configuration parsing:')
    config_data = [
        ('port', '8080', 'int'),
        ('debug', 'true', 'bool'),
        ('timeout', '5.5', 'float'),
        ('host', 'localhost', 'string'),
        ('workers', 'invalid', 'int'),  # Error case
        ('ssl', 'maybe', 'bool'),  # Error case
    ]

    parsed_config = {}
    config_errors = []

    for key, value, value_type in config_data:
        result = parse_config_value(key, value, value_type)
        if result.is_ok:
            parsed_config[key] = result.unwrap()
            print(
                f'  {key}: {value} -> {parsed_config[key]} ({type(parsed_config[key]).__name__})'
            )
        else:
            config_errors.append((key, result.error_message))
            print(f'  {key}: Error - {result.error_message}')

    print(f'Parsed config: {parsed_config}')
    print(f'Config errors: {config_errors}')

    # Data validation pipeline
    print(f'\nData validation pipeline:')

    def validate_email(email: str) -> Result[str]:
        """Basic email validation."""
        if '@' in email and '.' in email.split('@')[1]:
            return Result.ok(email)
        return Result.error(f'Invalid email format: {email}')

    def validate_age(age: int) -> Result[int]:
        """Age validation."""
        if 0 <= age <= 150:
            return Result.ok(age)
        return Result.error(f'Invalid age: {age}')

    def validate_name(name: str) -> Result[str]:
        """Name validation."""
        if len(name.strip()) >= 2:
            return Result.ok(name.strip())
        return Result.error(f'Name too short: "{name}"')

    # Test user data
    users_data = [
        ('john.doe@email.com', 25, 'John Doe'),
        ('invalid-email', 30, 'Jane Smith'),
        ('bob@test.com', -5, 'Bob Wilson'),
        ('alice@example.org', 35, 'A'),  # Name too short
        ('valid@user.com', 28, 'Valid User'),
    ]

    print('User validation results:')
    for email, age, name in users_data:
        email_result = validate_email(email)
        age_result = validate_age(age)
        name_result = validate_name(name)

        if email_result.is_ok and age_result.is_ok and name_result.is_ok:
            print(
                f'  ✓ Valid user: {name_result.unwrap()} ({age_result.unwrap()}) - {email_result.unwrap()}'
            )
        else:
            errors = []
            if email_result.is_error:
                errors.append(email_result.error_message)
            if age_result.is_error:
                errors.append(age_result.error_message)
            if name_result.is_error:
                errors.append(name_result.error_message)
            print(f'  ✗ Invalid user: {"; ".join(errors)}')

    print()


def advanced_result_patterns_demo() -> None:
    """Demonstrate advanced Result usage patterns.

    Shows sophisticated error handling techniques including
    result combination, conditional processing, and recovery.
    """
    print('=== Advanced Result Patterns Demo ===')

    # Result combination
    def combine_results(r1: Result[float], r2: Result[float]) -> Result[float]:
        """Combine two results by adding their values."""
        if r1.is_ok and r2.is_ok:
            return Result.ok(r1.unwrap() + r2.unwrap())
        elif r1.is_error and r2.is_error:
            return Result.error(f'Both failed: {r1.error_message}; {r2.error_message}')
        elif r1.is_error:
            return Result.error(f'First failed: {r1.error_message}')
        else:
            return Result.error(f'Second failed: {r2.error_message}')

    print('Result combination:')
    test_pairs = [
        (Result.ok(5.0), Result.ok(3.0)),
        (Result.ok(2.0), Result.error('Second error')),
        (Result.error('First error'), Result.ok(4.0)),
        (Result.error('First error'), Result.error('Second error')),
    ]

    for i, (r1, r2) in enumerate(test_pairs):
        combined = combine_results(r1, r2)
        if combined.is_ok:
            print(f'  Pair {i+1}: {combined.unwrap()}')
        else:
            print(f'  Pair {i+1}: Error - {combined.error_message}')

    # Conditional processing
    print(f'\nConditional processing:')

    def process_if_positive(x: float) -> Result[float]:
        """Process number only if positive."""
        if x > 0:
            return safe_sqrt(x).map(lambda y: y * 2)
        else:
            return Result.ok(0.0)  # Default for non-positive

    conditional_tests = [4.0, -2.0, 0.0, 16.0, -9.0]
    for value in conditional_tests:
        result = process_if_positive(value)
        print(
            f'  Process({value}): {result.unwrap() if result.is_ok else result.error_message}'
        )

    # Error recovery
    print(f'\nError recovery patterns:')

    def divide_with_fallback(a: float, b: float, fallback: float) -> Result[float]:
        """Divide with fallback value on error."""
        result = safe_divide(a, b)
        if result.is_error:
            return Result.ok(fallback)
        return result

    recovery_tests = [(10.0, 2.0, 1.0), (8.0, 0.0, 1.0), (15.0, 3.0, 1.0)]
    for a, b, fallback in recovery_tests:
        result = divide_with_fallback(a, b, fallback)
        print(f'  {a}/{b} with fallback {fallback}: {result.unwrap()}')

    print()


def main() -> int:
    """Run all exception handling examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Exceptions Module Example')
    print('========================')
    print()

    try:
        basic_result_demo()
        safe_operations_demo()
        result_chaining_demo()
        operation_chaining_demo()
        error_accumulation_demo()
        practical_patterns_demo()
        advanced_result_patterns_demo()

        print('All exception handling examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running exception examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
