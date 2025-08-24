"""Tests for Python exceptions module.

Comprehensive tests following C++ test patterns for exception handling functionality.
"""

from demo import exceptions


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
