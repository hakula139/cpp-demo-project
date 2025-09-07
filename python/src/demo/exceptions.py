"""Python wrapper for the exceptions module."""

from enum import Enum

from .cpp_features import exceptions as _exceptions


class ErrorSeverity(Enum):
    """Error severity levels."""

    TRACE = _exceptions.ErrorSeverity.TRACE
    DEBUG = _exceptions.ErrorSeverity.DEBUG
    INFO = _exceptions.ErrorSeverity.INFO
    WARNING = _exceptions.ErrorSeverity.WARNING
    ERROR = _exceptions.ErrorSeverity.ERROR
    FATAL = _exceptions.ErrorSeverity.FATAL

    def __str__(self) -> str:
        """String representation."""
        return _exceptions.severity_to_string(self.value)


class BaseException(Exception):
    """Base exception class."""

    def __init__(
        self,
        message: str,
        severity: ErrorSeverity = ErrorSeverity.ERROR,
    ) -> None:
        """Constructs a BaseException.

        Parameters
        ----------
        message : str
            Error message describing what went wrong
        severity : ErrorSeverity, default=ErrorSeverity.ERROR
            Severity level of the error

        Examples
        --------
        >>> raise BaseException('This is an error')
        >>> raise BaseException('This is a warning', ErrorSeverity.WARNING)
        """
        super().__init__(message)
        self.message = message
        self.severity = severity


class ValidationException(BaseException):
    """Exception for validation and input errors.

    Specialized exception for validation failures, with optional field name to identify
    which input field caused the validation error.

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
        super().__init__(message)
        self.field_name = field_name


class ResourceException(BaseException):
    """Exception for resource-related errors.

    Used for errors related to external resources such as files, network connections,
    databases, or any other system resources.

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
        self.resource_name = resource_name
        super().__init__(message)


class CalculationException(BaseException):
    """Exception for mathematical and calculation errors.

    Specialized exception for mathematical operations, calculations, and numerical
    processing errors. Stores the input value that caused the error.

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
        super().__init__(message)
        self.input_value = input_value


def _test_throw_validation_exception() -> None:
    """Test throwing ValidationException from C++."""
    _exceptions.test_throw_validation_exception()


def _test_throw_resource_exception() -> None:
    """Test throwing ResourceException from C++."""
    _exceptions.test_throw_resource_exception()


def _test_throw_calculation_exception() -> None:
    """Test throwing CalculationException from C++."""
    _exceptions.test_throw_calculation_exception()


def _test_throw_base_exception() -> None:
    """Test throwing BaseException from C++."""
    _exceptions.test_throw_base_exception()


def _test_throw_unknown_exception() -> None:
    """Test throwing unknown exception from C++."""
    _exceptions.test_throw_unknown_exception()


__all__ = [
    'ErrorSeverity',
    'BaseException',
    'ValidationException',
    'ResourceException',
    'CalculationException',
    '_test_throw_validation_exception',
    '_test_throw_resource_exception',
    '_test_throw_calculation_exception',
    '_test_throw_base_exception',
    '_test_throw_unknown_exception',
]
