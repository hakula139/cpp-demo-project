"""Tests for the exceptions module."""

import pytest
from demo.exceptions import *


class TestErrorSeverity:
    """Test severity enum and conversion."""

    @pytest.mark.parametrize(
        'severity, expected_str',
        [
            (ErrorSeverity.TRACE, 'TRACE'),
            (ErrorSeverity.DEBUG, 'DEBUG'),
            (ErrorSeverity.INFO, 'INFO'),
            (ErrorSeverity.WARNING, 'WARNING'),
            (ErrorSeverity.ERROR, 'ERROR'),
            (ErrorSeverity.FATAL, 'FATAL'),
        ],
    )
    def test_severity_to_string(
        self, severity: ErrorSeverity, expected_str: str
    ) -> None:
        """All severities convert to the expected string."""
        assert severity_to_string(severity) == expected_str


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
