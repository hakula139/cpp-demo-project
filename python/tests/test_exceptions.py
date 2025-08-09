"""Tests for Python exceptions module.

Comprehensive tests following C++ test patterns for exception handling functionality.
"""

import pytest

from python import exceptions


class TestErrorSeverity:
    """Test ErrorSeverity enum."""

    def test_error_severity_values(self) -> None:
        """Test all error severity levels."""
        assert exceptions.ErrorSeverity.TRACE is not None
        assert exceptions.ErrorSeverity.DEBUG is not None
        assert exceptions.ErrorSeverity.INFO is not None
        assert exceptions.ErrorSeverity.WARNING is not None
        assert exceptions.ErrorSeverity.ERROR is not None
        assert exceptions.ErrorSeverity.FATAL is not None

    def test_error_severity_ordering(self) -> None:
        """Test error severity ordering if applicable."""
        # Test that we can access all values
        severities = [
            exceptions.ErrorSeverity.TRACE,
            exceptions.ErrorSeverity.DEBUG,
            exceptions.ErrorSeverity.INFO,
            exceptions.ErrorSeverity.WARNING,
            exceptions.ErrorSeverity.ERROR,
            exceptions.ErrorSeverity.FATAL,
        ]
        assert len(severities) == 6


class TestResult:
    """Test Result wrapper functionality."""

    def test_result_ok_creation(self) -> None:
        """Test creating successful Result."""
        result = exceptions.Result.ok(42)

        assert result.is_ok
        assert not result.is_err
        assert result.unwrap() == 42

    def test_result_ok_different_types(self) -> None:
        """Test creating successful Results with different types."""
        int_result = exceptions.Result.ok(123)
        float_result = exceptions.Result.ok(3.14)
        str_result = exceptions.Result.ok('hello')

        assert int_result.unwrap() == 123
        assert float_result.unwrap() == 3.14
        assert str_result.unwrap() == 'hello'

    def test_result_error_creation(self) -> None:
        """Test creating error Result."""
        result = exceptions.Result.error('Something went wrong')

        assert not result.is_ok
        assert result.is_err

    def test_result_unwrap_error(self) -> None:
        """Test unwrapping error Result raises exception."""
        result = exceptions.Result.error('Error message')

        with pytest.raises(Exception):
            result.unwrap()

    def test_result_unwrap_or(self) -> None:
        """Test unwrap_or with default values."""
        ok_result = exceptions.Result.ok(42)
        err_result = exceptions.Result.error('Error')

        assert ok_result.unwrap_or(0) == 42
        assert err_result.unwrap_or(0) == 0

    def test_result_unwrap_or_else(self) -> None:
        """Test unwrap_or_else with function."""
        ok_result = exceptions.Result.ok(42)
        err_result = exceptions.Result.error('Error')

        assert ok_result.unwrap_or_else(lambda: 0) == 42
        assert err_result.unwrap_or_else(lambda: -1) == -1

    def test_result_map_success(self) -> None:
        """Test mapping successful Result."""
        result = exceptions.Result.ok(5)
        mapped = result.map(lambda x: x * 2)

        assert mapped.is_ok
        assert mapped.unwrap() == 10

    def test_result_map_error(self) -> None:
        """Test mapping error Result."""
        result = exceptions.Result.error('Error')
        mapped = result.map(lambda x: x * 2)

        assert mapped.is_err

    def test_result_and_then_success(self) -> None:
        """Test chaining successful Results."""

        def double_if_positive(x: int) -> exceptions.Result[int]:
            if x > 0:
                return exceptions.Result.ok(x * 2)
            else:
                return exceptions.Result.error('Negative number')

        result = exceptions.Result.ok(5)
        chained = result.and_then(double_if_positive)

        assert chained.is_ok
        assert chained.unwrap() == 10

    def test_result_and_then_error(self) -> None:
        """Test chaining with error Result."""

        def double_if_positive(x: int) -> exceptions.Result[int]:
            return exceptions.Result.ok(x * 2)

        result = exceptions.Result.error('Initial error')
        chained = result.and_then(double_if_positive)

        assert chained.is_err

    def test_result_or_else_success(self) -> None:
        """Test or_else with successful Result."""

        def handle_error(e: Exception) -> exceptions.Result[int]:
            return exceptions.Result.ok(0)

        result = exceptions.Result.ok(42)
        handled = result.or_else(handle_error)

        assert handled.is_ok
        assert handled.unwrap() == 42

    def test_result_or_else_error(self) -> None:
        """Test or_else with error Result."""

        def handle_error(e: Exception) -> exceptions.Result[int]:
            return exceptions.Result.ok(-1)

        result = exceptions.Result.error('Error')
        handled = result.or_else(handle_error)

        assert handled.is_ok
        assert handled.unwrap() == -1

    def test_result_boolean_conversion(self) -> None:
        """Test Result boolean conversion."""
        ok_result = exceptions.Result.ok(42)
        err_result = exceptions.Result.error('Error')

        assert bool(ok_result) is True
        assert bool(err_result) is False

    def test_result_string_representation(self) -> None:
        """Test Result string representation."""
        ok_result = exceptions.Result.ok(42)
        err_result = exceptions.Result.error('Error')

        assert str(ok_result) is not None
        assert str(err_result) is not None
        assert repr(ok_result).startswith('Result(')
        assert repr(err_result).startswith('Result(')


class TestSafeDivide:
    """Test safe division function."""

    def test_safe_divide_normal(self) -> None:
        """Test normal division."""
        result = exceptions.safe_divide(10.0, 2.0)

        assert result.is_ok
        assert result.unwrap() == 5.0

    def test_safe_divide_by_zero(self) -> None:
        """Test division by zero."""
        result = exceptions.safe_divide(10.0, 0.0)

        assert result.is_err

    def test_safe_divide_negative_numbers(self) -> None:
        """Test division with negative numbers."""
        result = exceptions.safe_divide(-10.0, 2.0)

        assert result.is_ok
        assert result.unwrap() == -5.0

    def test_safe_divide_float_precision(self) -> None:
        """Test division with float precision."""
        result = exceptions.safe_divide(1.0, 3.0)

        assert result.is_ok
        assert result.unwrap() == pytest.approx(0.3333333333333333)


class TestSafeSqrt:
    """Test safe square root function."""

    def test_safe_sqrt_positive(self) -> None:
        """Test square root of positive number."""
        result = exceptions.safe_sqrt(16.0)

        assert result.is_ok
        assert result.unwrap() == 4.0

    def test_safe_sqrt_zero(self) -> None:
        """Test square root of zero."""
        result = exceptions.safe_sqrt(0.0)

        assert result.is_ok
        assert result.unwrap() == 0.0

    def test_safe_sqrt_negative(self) -> None:
        """Test square root of negative number."""
        result = exceptions.safe_sqrt(-1.0)

        assert result.is_err

    def test_safe_sqrt_float_precision(self) -> None:
        """Test square root with float precision."""
        result = exceptions.safe_sqrt(2.0)

        assert result.is_ok
        assert result.unwrap() == pytest.approx(1.4142135623730951)


class TestChainOperations:
    """Test chaining Result operations."""

    def test_chain_operations_success(self) -> None:
        """Test successful chain of operations."""

        def add_one(x: float) -> exceptions.Result[float]:
            return exceptions.Result.ok(x + 1.0)

        def multiply_by_two(x: float) -> exceptions.Result[float]:
            return exceptions.Result.ok(x * 2.0)

        chained = exceptions.chain_operations(add_one, multiply_by_two)
        result = chained(5.0)

        assert result.is_ok
        assert result.unwrap() == 12.0  # (5 + 1) * 2

    def test_chain_operations_early_error(self) -> None:
        """Test chain stops at first error."""

        def fail_operation(x: float) -> exceptions.Result[float]:
            return exceptions.Result.error('Operation failed')

        def never_called(x: float) -> exceptions.Result[float]:
            return exceptions.Result.ok(x * 100.0)

        chained = exceptions.chain_operations(fail_operation, never_called)
        result = chained(5.0)

        assert result.is_err

    def test_chain_operations_complex(self) -> None:
        """Test complex chain with safe operations."""

        def safe_divide_by_two(x: float) -> exceptions.Result[float]:
            return exceptions.safe_divide(x, 2.0)

        def safe_sqrt_result(x: float) -> exceptions.Result[float]:
            return exceptions.safe_sqrt(x)

        # Chain: divide by 2, then square root
        chained = exceptions.chain_operations(safe_divide_by_two, safe_sqrt_result)

        # Test with 8.0: 8.0 / 2.0 = 4.0, sqrt(4.0) = 2.0
        result = chained(8.0)
        assert result.is_ok
        assert result.unwrap() == 2.0

        # Test with negative number
        result_negative = chained(-8.0)
        assert result_negative.is_err  # Will fail at sqrt step

    def test_empty_chain_operations(self) -> None:
        """Test empty chain of operations."""
        chained = exceptions.chain_operations()
        result = chained(42.0)

        assert result.is_ok
        assert result.unwrap() == 42.0


class TestResultMapChaining:
    """Test Result map method chaining."""

    def test_result_map_chain_success(self) -> None:
        """Test chaining map operations on successful Result."""
        result = (
            exceptions.Result.ok(5)
            .map(lambda x: x * 2)
            .map(lambda x: x + 3)
            .map(lambda x: x // 2)
        )

        assert result.is_ok
        assert result.unwrap() == 6  # ((5 * 2) + 3) // 2 = 6

    def test_result_map_chain_with_error(self) -> None:
        """Test map chaining when intermediate operation fails."""

        def might_fail(x: int) -> int:
            if x > 10:
                raise ValueError('Too large')
            return x * 2

        result = (
            exceptions.Result.ok(5).map(lambda x: x * 3).map(might_fail)
        )  # 15  # Should fail

        assert result.is_err

    def test_result_type_transformations(self) -> None:
        """Test Result with different type transformations."""
        # Start with int, transform to string, then to length
        result = exceptions.Result.ok(42).map(str).map(len)  # '42'  # 2

        assert result.is_ok
        assert result.unwrap() == 2


class TestExceptionIntegration:
    """Integration tests for exception handling."""

    def test_complex_calculation_pipeline(self) -> None:
        """Test complex calculation with error handling."""

        def complex_calculation(x: float) -> exceptions.Result[float]:
            # Chain: sqrt -> divide by 2 -> add 1
            return (
                exceptions.safe_sqrt(x)
                .and_then(lambda v: exceptions.safe_divide(v, 2.0))
                .map(lambda v: v + 1.0)
            )

        # Test with valid input
        result_valid = complex_calculation(16.0)
        assert result_valid.is_ok
        assert result_valid.unwrap() == 3.0  # sqrt(16)/2 + 1 = 4/2 + 1 = 3

        # Test with invalid input
        result_invalid = complex_calculation(-4.0)
        assert result_invalid.is_err

    def test_result_error_recovery(self) -> None:
        """Test error recovery patterns."""

        def might_fail(x: int) -> exceptions.Result[int]:
            if x < 0:
                return exceptions.Result.error('Negative number')
            return exceptions.Result.ok(x * 2)

        def recover_from_error(e: Exception) -> exceptions.Result[int]:
            return exceptions.Result.ok(0)  # Default value

        # Test successful case
        success_result = might_fail(5).or_else(recover_from_error)
        assert success_result.is_ok
        assert success_result.unwrap() == 10

        # Test error recovery
        error_result = might_fail(-5).or_else(recover_from_error)
        assert error_result.is_ok
        assert error_result.unwrap() == 0

    def test_multiple_result_combinations(self) -> None:
        """Test combining multiple Result values."""
        result1 = exceptions.Result.ok(10)
        result2 = exceptions.Result.ok(20)
        result3 = exceptions.Result.error('Error')

        # Combine successful results
        if result1.is_ok and result2.is_ok:
            combined = exceptions.Result.ok(result1.unwrap() + result2.unwrap())
            assert combined.unwrap() == 30

        # Handle error in combination
        if result1.is_ok and result3.is_err:
            # Can't combine due to error
            assert result3.is_err

    def test_result_with_unsupported_type(self) -> None:
        """Test Result creation with unsupported type."""
        with pytest.raises(ValueError, match='Unsupported result type'):
            exceptions.Result.ok({'key': 'value'})

    def test_exception_hierarchy_integration(self) -> None:
        """Test integration with C++ exception hierarchy."""
        # Test that we can access C++ exception classes
        assert exceptions.BaseException is not None
        assert exceptions.ValidationException is not None
        assert exceptions.ResourceException is not None
        assert exceptions.CalculationException is not None

        # Test that we can access C++ Result types
        assert exceptions.IntResult is not None
        assert exceptions.DoubleResult is not None
        assert exceptions.StringResult is not None
