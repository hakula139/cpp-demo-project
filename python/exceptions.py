"""Modern Python wrapper for the exceptions module.

Enhanced error handling with Result types and custom exceptions.
"""

from enum import Enum
from typing import Any, Callable, Generic, TypeVar

import cpp_features.exceptions as _exceptions

T = TypeVar('T')
U = TypeVar('U')


class ErrorSeverity(Enum):
    """Error severity levels."""

    TRACE = _exceptions.ErrorSeverity.TRACE
    DEBUG = _exceptions.ErrorSeverity.DEBUG
    INFO = _exceptions.ErrorSeverity.INFO
    WARNING = _exceptions.ErrorSeverity.WARNING
    ERROR = _exceptions.ErrorSeverity.ERROR
    FATAL = _exceptions.ErrorSeverity.FATAL


class Result(Generic[T]):
    """Generic Result wrapper for functional error handling.

    Provides a more Pythonic interface to the C++ Result types.

    Parameters
    ----------
    cpp_result : Any
        The underlying C++ Result object
    """

    def __init__(self, cpp_result: Any):
        self._result = cpp_result

    @property
    def is_ok(self) -> bool:
        """Check if result contains a value.

        Returns
        -------
        bool
            True if result contains a value, False if error
        """
        return self._result.has_value()

    @property
    def is_err(self) -> bool:
        """Check if result contains an error.

        Returns
        -------
        bool
            True if result contains an error, False if value
        """
        return not self._result.has_value()

    def unwrap(self) -> T:
        """Get the value or raise exception.

        Returns
        -------
        T
            The contained value

        Raises
        ------
        Exception
            If the result contains an error
        """
        return self._result.get_value()

    def unwrap_or(self, default: T) -> T:
        """Get the value or return default.

        Parameters
        ----------
        default : T
            Default value to return if result contains error

        Returns
        -------
        T
            The contained value or default
        """
        return self._result.get_value() if self.is_ok else default

    def unwrap_or_else(self, func: Callable[[], T]) -> T:
        """Get the value or compute default.

        Parameters
        ----------
        func : Callable[[], T]
            Function to compute default value

        Returns
        -------
        T
            The contained value or computed default
        """
        return self._result.get_value() if self.is_ok else func()

    def map(self, func: Callable[[T], U]) -> 'Result[U]':
        """Transform the contained value if present.

        Parameters
        ----------
        func : Callable[[T], U]
            Function to apply to the value

        Returns
        -------
        Result[U]
            New Result with transformed value or original error
        """
        if self.is_ok:
            try:
                new_value = func(self.unwrap())
                # Create appropriate Result type based on value type
                match new_value:
                    case int():
                        return Result(_exceptions.IntResult(new_value))
                    case float():
                        return Result(_exceptions.DoubleResult(new_value))
                    case str():
                        return Result(_exceptions.StringResult(new_value))
                    case _:
                        raise ValueError(f'Unsupported result type: {type(new_value)}')
            except Exception as e:
                return Result.error(str(e))
        else:
            return Result(self._result)

    def and_then(self, func: Callable[[T], 'Result[U]']) -> 'Result[U]':
        """Chain Result operations.

        Parameters
        ----------
        func : Callable[[T], Result[U]]
            Function that returns a Result

        Returns
        -------
        Result[U]
            Result from function or original error
        """
        if self.is_ok:
            return func(self.unwrap())
        else:
            return Result(self._result)

    def or_else(self, func: Callable[[Exception], 'Result[T]']) -> 'Result[T]':
        """Handle error case.

        Parameters
        ----------
        func : Callable[[Exception], Result[T]]
            Function to handle the error

        Returns
        -------
        Result[T]
            Original result if Ok, or result from error handler
        """
        if self.is_err:
            return func(self._result.get_exception())
        else:
            return self

    @staticmethod
    def ok(value: T) -> 'Result[T]':
        """Create a successful Result.

        Parameters
        ----------
        value : T
            The value to wrap

        Returns
        -------
        Result[T]
            Successful Result containing the value

        Raises
        ------
        ValueError
            If value type is not supported
        """
        match value:
            case int():
                return Result(_exceptions.IntResult(value))
            case float():
                return Result(_exceptions.DoubleResult(value))
            case str():
                return Result(_exceptions.StringResult(value))
            case _:
                raise ValueError(f'Unsupported result type: {type(value)}')

    @staticmethod
    def error(message: str, severity: ErrorSeverity = ErrorSeverity.ERROR) -> 'Result[Any]':
        """Create an error Result.

        Parameters
        ----------
        message : str
            Error message
        severity : ErrorSeverity, default=ErrorSeverity.ERROR
            Error severity level

        Returns
        -------
        Result[Any]
            Error Result containing the exception
        """
        exception = _exceptions.BaseException(message, severity.value)
        return Result(_exceptions.IntResult(exception))

    def __bool__(self) -> bool:
        """Boolean conversion - True if Ok.

        Returns
        -------
        bool
            True if result contains value, False if error
        """
        return self.is_ok

    def __str__(self) -> str:
        """String representation.

        Returns
        -------
        str
            String representation of the result
        """
        return str(self._result)

    def __repr__(self) -> str:
        """Debug representation.

        Returns
        -------
        str
            Debug representation of the result
        """
        return f'Result({self._result})'


def safe_divide(a: float, b: float) -> Result[float]:
    """Safe division that returns Result.

    Parameters
    ----------
    a : float
        Dividend
    b : float
        Divisor

    Returns
    -------
    Result[float]
        Result containing quotient or division by zero error
    """
    if b == 0.0:
        return Result.error('Division by zero')
    return Result.ok(a / b)


def safe_sqrt(x: float) -> Result[float]:
    """Safe square root that returns Result.

    Parameters
    ----------
    x : float
        Value to take square root of

    Returns
    -------
    Result[float]
        Result containing square root or negative number error
    """
    if x < 0:
        return Result.error('Square root of negative number')
    return Result.ok(x**0.5)


def chain_operations(
    *operations: Callable[[Any], Result[Any]],
) -> Callable[[Any], Result[Any]]:
    """Chain multiple Result-returning operations.

    Parameters
    ----------
    *operations : Callable[[Any], Result[Any]]
        Operations to chain together

    Returns
    -------
    Callable[[Any], Result[Any]]
        Function that applies all operations in sequence
    """

    def chained(initial_value: Any) -> Result[Any]:
        result = Result.ok(initial_value)
        for operation in operations:
            if result.is_err:
                break
            result = result.and_then(operation)
        return result

    return chained


# Re-export C++ classes
BaseException = _exceptions.BaseException
ValidationException = _exceptions.ValidationException
ResourceException = _exceptions.ResourceException
CalculationException = _exceptions.CalculationException
IntResult = _exceptions.IntResult
DoubleResult = _exceptions.DoubleResult
StringResult = _exceptions.StringResult

__all__ = [
    'ErrorSeverity',
    'Result',
    'BaseException',
    'ValidationException',
    'ResourceException',
    'CalculationException',
    'IntResult',
    'DoubleResult',
    'StringResult',
    'safe_divide',
    'safe_sqrt',
    'chain_operations',
]
