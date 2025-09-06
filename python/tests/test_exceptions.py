"""Tests for the exceptions module."""

import pytest
from demo.exceptions import *


class TestErrorSeverity:
    """Test severity enum and conversion."""

    @pytest.mark.parametrize(
        'severity',
        [
            ErrorSeverity.TRACE,
            ErrorSeverity.DEBUG,
            ErrorSeverity.INFO,
            ErrorSeverity.WARNING,
            ErrorSeverity.ERROR,
            ErrorSeverity.FATAL,
        ],
    )
    def test_str(self, severity: ErrorSeverity) -> None:
        """All severities convert to the expected string."""
        assert str(severity) == severity.name


class TestValidationException:
    """Test ValidationException functionality."""

    def test_without_field_name(self) -> None:
        """Test raising ValidationException without field name."""
        message = 'Invalid input'

        with pytest.raises(ValidationException, match=message):
            raise ValidationException(message)

    def test_with_field_name(self) -> None:
        """Test raising ValidationException with field name."""
        message = 'Must be a valid email'
        field_name = 'email'

        with pytest.raises(ValidationException, match=message) as exc_info:
            raise ValidationException(message, field_name)

        assert exc_info.value.field_name == field_name

    def test_derived_from_base_exception(self) -> None:
        """Test that ValidationException can be caught as BaseException."""
        message = 'Test validation error'

        with pytest.raises(BaseException, match=message):
            raise ValidationException(message)

    def test_throw_from_cpp(self) -> None:
        """Test throwing ValidationException from C++."""
        message = 'Test validation exception'
        field_name = 'test_field'

        with pytest.raises(ValidationException, match=message) as exc_info:
            _test_throw_validation_exception()

        assert exc_info.value.field_name == field_name


class TestResourceException:
    """Test ResourceException functionality."""

    def test_without_resource_name(self) -> None:
        """Test raising ResourceException without resource name."""
        message = 'Resource unavailable'

        with pytest.raises(ResourceException, match=message):
            raise ResourceException(message)

    def test_with_resource_name(self) -> None:
        """Test raising ResourceException with resource name."""
        message = 'File not found'
        resource_name = '/path/to/config.txt'

        with pytest.raises(ResourceException, match=message) as exc_info:
            raise ResourceException(message, resource_name)

        assert exc_info.value.resource_name == resource_name

    def test_derived_from_base_exception(self) -> None:
        """Test that ResourceException can be caught as BaseException."""
        message = 'Database connection failed'

        with pytest.raises(BaseException, match=message):
            raise ResourceException(message)

    def test_throw_from_cpp(self) -> None:
        """Test throwing ResourceException from C++."""
        message = 'Test resource exception'
        resource_name = 'test_resource'

        with pytest.raises(ResourceException, match=message) as exc_info:
            _test_throw_resource_exception()

        assert exc_info.value.resource_name == resource_name


class TestCalculationException:
    """Test CalculationException functionality."""

    def test_without_input_value(self) -> None:
        """Test raising CalculationException without input value."""
        message = 'Mathematical error'

        with pytest.raises(CalculationException, match=message):
            raise CalculationException(message)

    def test_with_input_value(self) -> None:
        """Test raising CalculationException with input value."""
        message = 'Cannot calculate square root of negative number'
        input_value = -4.0

        with pytest.raises(CalculationException, match=message) as exc_info:
            raise CalculationException(message, input_value)

        assert exc_info.value.input_value == input_value

    def test_derived_from_base_exception(self) -> None:
        """Test that CalculationException can be caught as BaseException."""
        message = 'Overflow error'

        with pytest.raises(BaseException, match=message):
            raise CalculationException(message)

    def test_throw_from_cpp(self) -> None:
        """Test throwing CalculationException from C++."""
        message = 'Test calculation exception'
        input_value = 1.0

        with pytest.raises(CalculationException, match=message) as exc_info:
            _test_throw_calculation_exception()

        assert exc_info.value.input_value == input_value


class TestBaseException:
    """Test BaseException functionality."""

    def test_throw_from_cpp(self) -> None:
        """Test throwing BaseException from C++."""
        message = 'Test base exception'

        with pytest.raises(BaseException, match=message) as exc_info:
            _test_throw_base_exception()

        assert exc_info.value.severity == ErrorSeverity.WARNING


class TestUnknownException:
    """Test UnknownException functionality."""

    def test_throw_from_cpp(self) -> None:
        """Test throwing UnknownException from C++."""
        message = 'Test unknown exception'

        with pytest.raises(RuntimeError, match=message):
            _test_throw_unknown_exception()
