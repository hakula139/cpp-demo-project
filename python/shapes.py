"""Modern Python wrapper for the shapes module.

Demonstrates Python 3.13 features like pattern matching, enhanced error handling,
and modern type hints.
"""

from dataclasses import dataclass
from enum import Enum, auto
from typing import Any, Protocol

import cpp_features.shapes as _shapes


class ShapeProtocol(Protocol):
    """Protocol defining the shape interface."""

    def get_area(self) -> float:
        """Get the area of the shape."""
        ...

    def get_perimeter(self) -> float:
        """Get the perimeter of the shape."""
        ...

    def draw(self) -> None:
        """Draw the shape."""
        ...

    def get_name(self) -> str:
        """Get the name of the shape."""
        ...


class ShapeType(Enum):
    """Enumeration of available shape types."""

    CIRCLE = auto()
    RECTANGLE = auto()
    SQUARE = auto()


@dataclass(frozen=True)
class ShapeMetrics:
    """Immutable data class for shape measurements.

    Parameters
    ----------
    area : float
        The area of the shape
    perimeter : float
        The perimeter of the shape
    name : str
        The name of the shape
    """

    area: float
    perimeter: float
    name: str

    @property
    def aspect_ratio(self) -> float:
        """Calculate aspect ratio for applicable shapes.

        Returns
        -------
        float
            The aspect ratio (area / perimeter^2)
        """
        return self.area / (self.perimeter**2) if self.perimeter > 0 else 0.0


def create_shape(shape_type: str | ShapeType, *args: float) -> _shapes.Shape:
    """Factory function using modern Python 3.13 pattern matching.

    Parameters
    ----------
    shape_type : str or ShapeType
        Type of shape to create
    *args : float
        Arguments for shape construction

    Returns
    -------
    Shape
        Created shape instance

    Raises
    ------
    ValueError
        For invalid shape types or arguments
    """
    match shape_type:
        case ShapeType.CIRCLE | 'circle':
            if len(args) != 1:
                raise ValueError('Circle requires exactly 1 argument (radius)')
            return _shapes.create_circle(args[0])

        case ShapeType.RECTANGLE | 'rectangle':
            if len(args) != 2:
                raise ValueError(
                    'Rectangle requires exactly 2 arguments (width, height)'
                )
            return _shapes.create_rectangle(args[0], args[1])

        case ShapeType.SQUARE | 'square':
            if len(args) != 1:
                raise ValueError('Square requires exactly 1 argument (side)')
            return _shapes.create_square(args[0])

        case _:
            raise ValueError(f'Unknown shape type: {shape_type}')


def analyze_shape(shape: ShapeProtocol) -> ShapeMetrics:
    """Analyze a shape and return comprehensive metrics.

    Parameters
    ----------
    shape : ShapeProtocol
        The shape to analyze

    Returns
    -------
    ShapeMetrics
        Comprehensive metrics for the shape
    """
    return ShapeMetrics(
        area=shape.get_area(), perimeter=shape.get_perimeter(), name=shape.get_name()
    )


def compare_shapes(*shapes: ShapeProtocol) -> dict[str, Any]:
    """Compare multiple shapes using modern Python features.

    Parameters
    ----------
    *shapes : ShapeProtocol
        Variable number of shapes to compare

    Returns
    -------
    dict[str, Any]
        Dictionary with comparison results and statistics

    Raises
    ------
    ValueError
        If no shapes are provided
    """
    if not shapes:
        raise ValueError('At least one shape is required')

    metrics = [analyze_shape(shape) for shape in shapes]

    return {
        'count': len(shapes),
        'total_area': sum(m.area for m in metrics),
        'total_perimeter': sum(m.perimeter for m in metrics),
        'largest_by_area': max(metrics, key=lambda m: m.area),
        'smallest_by_area': min(metrics, key=lambda m: m.area),
        'average_area': sum(m.area for m in metrics) / len(metrics),
        'metrics': metrics,
    }


# Re-export the C++ classes for direct use
Circle = _shapes.Circle
Rectangle = _shapes.Rectangle
RectangleDimensions = _shapes.RectangleDimensions
Shape = _shapes.Shape

__all__ = [
    'Shape',
    'Circle',
    'Rectangle',
    'RectangleDimensions',
    'ShapeProtocol',
    'ShapeType',
    'ShapeMetrics',
    'create_shape',
    'analyze_shape',
    'compare_shapes',
]
