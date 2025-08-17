"""Modern Python wrapper for the exceptions module.

Enhanced error handling with Result types and custom exceptions.
"""

from enum import Enum
from typing import Any, Callable, Generic, TypeVar

T = TypeVar('T')
U = TypeVar('U')


class ErrorSeverity(Enum):
    TRACE = 0
    DEBUG = 1
    INFO = 2
    WARNING = 3
    ERROR = 4
    FATAL = 5


class _PyResult(Generic[T]):
    def __init__(self, value: T | None = None, error: Exception | None = None):
        self._value = value
        self._error = error

    def has_value(self) -> bool:
        return self._error is None

    def get_value(self) -> T:
        if self._error is not None:
            raise self._error
        return self._value  # type: ignore[return-value]

    def get_exception(self) -> Exception:
        if self._error is None:
            raise RuntimeError('No exception in successful result')
        return self._error

    def __str__(self) -> str:
        return f'Ok({self._value})' if self.has_value() else f'Err({self._error})'


class Result(Generic[T]):
    def __init__(self, inner: _PyResult[T]):
        self._inner = inner

    @property
    def is_ok(self) -> bool:
        return self._inner.has_value()

    @property
    def is_err(self) -> bool:
        return not self.is_ok

    def unwrap(self) -> T:
        return self._inner.get_value()

    def unwrap_or(self, default: T) -> T:
        return self._inner.get_value() if self.is_ok else default

    def unwrap_or_else(self, func: Callable[[], T]) -> T:
        return self._inner.get_value() if self.is_ok else func()

    def map(self, func: Callable[[T], U]) -> 'Result[U]':
        if self.is_ok:
            try:
                return Result.ok(func(self.unwrap()))
            except Exception as e:
                return Result.error(str(e))
        return Result(_PyResult(error=self._inner.get_exception()))

    def and_then(self, func: Callable[[T], 'Result[U]']) -> 'Result[U]':
        if self.is_ok:
            return func(self.unwrap())
        return Result(_PyResult(error=self._inner.get_exception()))

    def or_else(self, func: Callable[[Exception], 'Result[T]']) -> 'Result[T]':
        if self.is_err:
            return func(self._inner.get_exception())
        return self

    @staticmethod
    def ok(value: T) -> 'Result[T]':
        return Result(_PyResult(value=value))

    @staticmethod
    def error(
        message: str, severity: ErrorSeverity = ErrorSeverity.ERROR
    ) -> 'Result[Any]':
        # Attach severity information via a simple Exception subclass
        class _Error(Exception):
            def __init__(self, msg: str, sev: ErrorSeverity):
                super().__init__(msg)
                self.severity = sev

        return Result(_PyResult(error=_Error(message, severity)))

    def __bool__(self) -> bool:
        return self.is_ok

    def __str__(self) -> str:
        return str(self._inner)

    def __repr__(self) -> str:
        return f'Result({self._inner})'


def safe_divide(a: float, b: float) -> Result[float]:
    if b == 0.0:
        return Result.error('Division by zero')
    return Result.ok(a / b)


def safe_sqrt(x: float) -> Result[float]:
    if x < 0:
        return Result.error('Square root of negative number')
    return Result.ok(x**0.5)


def chain_operations(
    *operations: Callable[[Any], Result[Any]]
) -> Callable[[Any], Result[Any]]:
    def chained(initial_value: Any) -> Result[Any]:
        result = Result.ok(initial_value)
        for operation in operations:
            if result.is_err:
                break
            result = result.and_then(operation)
        return result

    return chained


__all__ = [
    'ErrorSeverity',
    'Result',
    'safe_divide',
    'safe_sqrt',
    'chain_operations',
]
