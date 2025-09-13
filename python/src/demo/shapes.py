"""Python wrapper for the shapes module."""

from types import NotImplementedType

from .cpp_features import shapes as _shapes


class Shape:
    """Base class for geometric shapes."""

    def __init__(self, shape: _shapes.Shape) -> None:
        """Initialize shape wrapper.

        Parameters
        ----------
        shape : _shapes.Shape
            The underlying C++ shape object
        """
        self._shape = shape

    def get_area(self) -> float:
        """Calculate the area of the shape.

        Returns
        -------
        float
            Area of the shape

        Examples
        --------
        >>> circle = Circle(5.0)
        >>> print(f'Area: {circle.get_area():.2f}')
        Area: 78.54
        >>> rectangle = Rectangle(4.0, 3.0)
        >>> print(f'Area: {rectangle.get_area():.2f}')
        Area: 12.00
        >>> square = Rectangle.square(5.0)
        >>> print(f'Area: {square.get_area():.2f}')
        Area: 25.00
        """
        return self._shape.get_area()

    def get_perimeter(self) -> float:
        """Calculate the perimeter of the shape.

        Returns
        -------
        float
            Perimeter of the shape

        Examples
        --------
        >>> circle = Circle(5.0)
        >>> print(f'Perimeter: {circle.get_perimeter():.2f}')
        Perimeter: 31.42
        >>> rectangle = Rectangle(4.0, 3.0)
        >>> print(f'Perimeter: {rectangle.get_perimeter():.2f}')
        Perimeter: 14.00
        >>> square = Rectangle.square(5.0)
        >>> print(f'Perimeter: {square.get_perimeter():.2f}')
        Perimeter: 20.00
        """
        return self._shape.get_perimeter()

    def draw(self) -> None:
        """Draw the shape.

        The current implementation prints a message with the shape's information.

        Examples
        --------
        >>> circle = Circle(5.0)
        >>> circle.draw()
        Drawing Circle (r = 5.00)
        >>> rectangle = Rectangle(4.0, 3.0)
        >>> rectangle.draw()
        Drawing Rectangle (w = 4.00, h = 3.00)
        >>> square = Rectangle.square(5.0)
        >>> square.draw()
        Drawing Rectangle (w = 5.00, h = 5.00)
        """
        self._shape.draw()

    def __eq__(self, other: object) -> bool:
        """Check equality with another shape."""
        if not isinstance(other, type(self)):
            return False
        return self._shape == other._shape

    def __lt__(self, other: object) -> bool | NotImplementedType:
        """Check if this shape is smaller than another."""
        if not isinstance(other, type(self)):
            return NotImplemented
        return self._shape < other._shape

    def __le__(self, other: object) -> bool | NotImplementedType:
        """Check if this shape is smaller than or equal to another."""
        if not isinstance(other, type(self)):
            return NotImplemented
        return self._shape <= other._shape

    def __gt__(self, other: object) -> bool | NotImplementedType:
        """Check if this shape is larger than another."""
        if not isinstance(other, type(self)):
            return NotImplemented
        return self._shape > other._shape

    def __ge__(self, other: object) -> bool | NotImplementedType:
        """Check if this shape is larger than or equal to another."""
        if not isinstance(other, type(self)):
            return NotImplemented
        return self._shape >= other._shape

    def __str__(self) -> str:
        """String representation."""
        return str(self._shape)

    def __repr__(self) -> str:
        """String representation (for debugging)."""
        return repr(self._shape)


class Circle(Shape):
    """Circle shape implementation with radius-based geometry."""

    def __init__(self, radius: float) -> None:
        """Construct a circle with the specified radius.

        Parameters
        ----------
        radius : float
            Radius of the circle

        Raises
        ------
        ValidationException
            If the radius is not positive

        Examples
        --------
        >>> circle = Circle(5.0)
        """
        shape = _shapes.Circle(radius)
        super().__init__(shape)

    @property
    def radius(self) -> float:
        """Get the radius of the circle."""
        return self._shape.get_radius()


class Rectangle(Shape):
    """Rectangle shape implementation with width and height geometry."""

    def __init__(self, width: float, height: float | None = None) -> None:
        """Construct a rectangle with the specified width and height.

        Parameters
        ----------
        width : float
            Width of the rectangle
        height : float, optional
            Height of the rectangle. If not provided, defaults to the width.

        Raises
        ------
        ValidationException
            If width or height is not positive

        Examples
        --------
        >>> rectangle = Rectangle(4.0, 3.0)
        >>> square = Rectangle(5.0)  # 5.0 x 5.0 square
        """
        if height is not None:
            shape = _shapes.Rectangle(width, height)
        else:
            shape = _shapes.Rectangle(width)

        super().__init__(shape)

    @property
    def width(self) -> float:
        """Get the width of the rectangle."""
        return self._shape.get_width()

    @property
    def height(self) -> float:
        """Get the height of the rectangle."""
        return self._shape.get_height()

    @property
    def is_square(self) -> bool:
        """Check if the rectangle is a square."""
        return self._shape.is_square()


__all__ = [
    'Circle',
    'Rectangle',
]
