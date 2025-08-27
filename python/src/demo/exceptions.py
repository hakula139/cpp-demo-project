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
    return _exceptions.severity_to_string(severity)


ValidationException: type[Exception] = _exceptions.ValidationException
ResourceException: type[Exception] = _exceptions.ResourceException
CalculationException: type[Exception] = _exceptions.CalculationException

__all__ = [
    'ErrorSeverity',
    'severity_to_string',
    'ValidationException',
    'ResourceException',
    'CalculationException',
]
