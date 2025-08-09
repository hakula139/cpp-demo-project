"""Modern Python wrapper for the memory module.

RAII and resource management utilities.
"""

from contextlib import contextmanager
from typing import Any, Callable, ContextManager

import cpp_features.memory as _memory

from .shapes import Circle, Rectangle


class ResourceManager:
    """Enhanced resource manager with context manager support.

    Provides RAII-like behavior in Python using context managers.
    """

    def __init__(self):
        self._manager = _memory.ResourceManager()
        self._cleanup_functions = []

    def create_circle(self, radius: float) -> Circle:
        """Create a Circle using the resource manager.

        Parameters
        ----------
        radius : float
            Radius of the circle

        Returns
        -------
        Circle
            Created circle instance
        """
        return self._manager.create_unique_circle(radius)

    def create_rectangle(self, width: float, height: float) -> Rectangle:
        """Create a Rectangle using the resource manager.

        Parameters
        ----------
        width : float
            Width of the rectangle
        height : float
            Height of the rectangle

        Returns
        -------
        Rectangle
            Created rectangle instance
        """
        return self._manager.create_unique_rectangle(width, height)

    def create_shared_circle(self, radius: float) -> Circle:
        """Create a shared Circle.

        Parameters
        ----------
        radius : float
            Radius of the circle

        Returns
        -------
        Shared circle instance
        """
        return self._manager.create_shared_circle(radius)

    def create_shared_rectangle(self, width: float, height: float) -> Rectangle:
        """Create a shared Rectangle.

        Parameters
        ----------
        width : float
            Width of the rectangle
        height : float
            Height of the rectangle

        Returns
        -------
        Shared rectangle instance
        """
        return self._manager.create_shared_rectangle(width, height)

    def register_cleanup(self, cleanup_func: Callable[[], None]) -> None:
        """Register a cleanup function.

        Parameters
        ----------
        cleanup_func : Callable[[], None]
            Function to call during cleanup
        """
        self._cleanup_functions.append(cleanup_func)
        self._manager.register_cleanup(cleanup_func)

    def execute_cleanup(self) -> None:
        """Execute all cleanup functions."""
        self._manager.execute_cleanup()
        self._cleanup_functions.clear()

    def __enter__(self) -> 'ResourceManager':
        """Context manager entry.

        Returns
        -------
        ResourceManager
            This resource manager instance
        """
        return self

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Context manager exit with automatic cleanup.

        Parameters
        ----------
        exc_type : Any
            Exception type (if any)
        exc_val : Any
            Exception value (if any)
        exc_tb : Any
            Exception traceback (if any)
        """
        self.execute_cleanup()


@contextmanager
def managed_resources() -> ContextManager[ResourceManager]:
    """Context manager for automatic resource cleanup.

    Yields
    ------
    ResourceManager
        Resource manager with automatic cleanup

    Examples
    --------
    >>> with managed_resources() as manager:
    ...     circle = manager.create_circle(5.0)
    ...     # Resources cleaned up automatically
    """
    manager = ResourceManager()
    try:
        yield manager
    finally:
        manager.execute_cleanup()


class ScopedResource:
    """RAII-style resource wrapper using Python context managers.

    Automatically calls cleanup function when scope ends.

    Parameters
    ----------
    resource : Any
        The resource to manage
    cleanup_func : Callable[[], None], optional
        Function to call for cleanup
    """

    def __init__(self, resource: Any, cleanup_func: Callable[[], None] | None = None):
        self.resource = resource
        self._cleanup = cleanup_func

    def __enter__(self) -> Any:
        """Return the resource.

        Returns
        -------
        Any
            The managed resource
        """
        return self.resource

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Execute cleanup.

        Parameters
        ----------
        exc_type : Any
            Exception type (if any)
        exc_val : Any
            Exception value (if any)
        exc_tb : Any
            Exception traceback (if any)
        """
        if self._cleanup:
            self._cleanup()


def scoped_resource(
    resource: Any, cleanup_func: Callable[[], None] | None = None
) -> ScopedResource:
    """Factory function for creating scoped resources.

    Parameters
    ----------
    resource : Any
        The resource to manage
    cleanup_func : Callable[[], None], optional
        Function to call for cleanup

    Returns
    -------
    ScopedResource
        Scoped resource wrapper

    Examples
    --------
    >>> with scoped_resource(file_handle, lambda: file_handle.close()) as f:
    ...     # Use file_handle
    ...     pass  # file_handle.close() called automatically
    """
    return ScopedResource(resource, cleanup_func)


# Re-export C++ class
CppResourceManager = _memory.ResourceManager

__all__ = [
    'ResourceManager',
    'ScopedResource',
    'CppResourceManager',
    'managed_resources',
    'scoped_resource',
]
