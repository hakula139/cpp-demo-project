"""Python wrapper for the exceptions module."""

from enum import Enum

import cpp_features.exceptions as _exceptions


class ErrorSeverity(Enum):
    """Error severity levels."""

    TRACE = _exceptions.ErrorSeverity.TRACE
    DEBUG = _exceptions.ErrorSeverity.DEBUG
    INFO = _exceptions.ErrorSeverity.INFO
    WARNING = _exceptions.ErrorSeverity.WARNING
    ERROR = _exceptions.ErrorSeverity.ERROR
    FATAL = _exceptions.ErrorSeverity.FATAL


def severity_to_string(severity: ErrorSeverity) -> str:
    """Convert error severity to string.

    Parameters
    ----------
    severity : ErrorSeverity
        The error severity to convert to a string

    Returns
    -------
    str
        The string representation of the error severity
    """
    return _exceptions.severity_to_string(severity.value)


class BaseException(_exceptions.BaseException):
    """Base exception class."""

    def __init__(
        self,
        message: str,
        severity: ErrorSeverity | _exceptions.ErrorSeverity = ErrorSeverity.ERROR,
    ) -> None:
        """Constructs a BaseException.

        Parameters
        ----------
        message : str
            Error message describing what went wrong
        severity : ErrorSeverity | _exceptions.ErrorSeverity, default=ErrorSeverity.ERROR
            Severity level of the error
        """
        if isinstance(severity, ErrorSeverity):
            self.severity = severity.value
        else:
            self.severity = severity
        super().__init__(message, self.severity)


class ValidationException(_exceptions.ValidationException, BaseException):
    """Exception for validation and input errors.

    Specialized exception for validation failures, with optional field name to identify which input
    field caused the validation error.

    Examples
    --------
    >>> raise ValidationException('Must be a valid email address', 'email')
    >>> raise ValidationException('Input validation failed')
    """

    def __init__(self, message: str, field_name: str | None = None) -> None:
        """Constructs a ValidationException.

        Parameters
        ----------
        message : str
            Validation error message
        field_name : str, optional
            Optional name of the field that failed validation
        """
        self.severity = ErrorSeverity.ERROR
        self.field_name = field_name
        super().__init__(message, self.field_name)


class ResourceException(_exceptions.ResourceException, BaseException):
    """Exception for resource-related errors.

    Used for errors related to external resources such as files, network connections, databases, or
    any other system resources.

    Examples
    --------
    >>> raise ResourceException('File not found', '/path/to/config.txt')
    >>> raise ResourceException('Connection timeout', 'api.example.com:443')
    """

    def __init__(self, message: str, resource_name: str | None = None) -> None:
        """Constructs a ResourceException.

        Parameters
        ----------
        message : str
            Resource error message
        resource_name : str, optional
            Optional name of the resource
        """
        self.severity = ErrorSeverity.ERROR
        self.resource_name = resource_name
        super().__init__(message, self.resource_name)


class CalculationException(_exceptions.CalculationException, BaseException):
    """Exception for mathematical and calculation errors.

    Specialized exception for mathematical operations, calculations, and numerical processing errors.
    Stores the input value that caused the error.

    Examples
    --------
    >>> raise CalculationException('Division by zero', 0.0)
    >>> raise CalculationException('Square root of negative number', -4.0)
    """

    def __init__(self, message: str, input_value: float = 0.0) -> None:
        """Constructs a CalculationException.

        Parameters
        ----------
        message : str
            Calculation error message
        input_value : float, default=0.0
            The input value that caused the calculation error
        """
        self.severity = ErrorSeverity.ERROR
        self.input_value = input_value
        super().__init__(message, self.input_value)


__all__ = [
    'ErrorSeverity',
    'severity_to_string',
    'BaseException',
    'ValidationException',
    'ResourceException',
    'CalculationException',
]
