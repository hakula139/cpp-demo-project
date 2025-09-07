"""Tests for the shapes module."""

import math

import pytest

from demo.exceptions import ValidationException
from demo.shapes import Circle, Rectangle, Shape


class TestCircle:
    """Test Circle functionality."""

    def test_creation(self) -> None:
        """Test creation."""
        circle = Circle(5.0)

        assert circle.radius == 5.0

    @pytest.mark.parametrize('radius', [0.0, -3.0])
    def test_creation_with_invalid_radius(self, radius: float) -> None:
        """Test creation with invalid radius."""
        with pytest.raises(
            ValidationException, match='Circle radius must be positive'
        ) as exc_info:
            Circle(radius)

        assert exc_info.value.field_name == 'radius'

    def test_get_area(self) -> None:
        """Test area calculation."""
        circle = Circle(5.0)
        area = circle.get_area()

        assert area == pytest.approx(math.pi * 25.0)

    def test_get_perimeter(self) -> None:
        """Test perimeter calculation."""
        circle = Circle(5.0)
        perimeter = circle.get_perimeter()

        assert perimeter == pytest.approx(math.pi * 10.0)

    def test_draw(self) -> None:
        """Test draw functionality."""
        circle = Circle(5.0)
        circle.draw()

    def test_equality_comparison(self) -> None:
        """Test equality comparison."""
        circle1 = Circle(5.0)
        circle2 = Circle(5.0)
        circle3 = Circle(3.0)

        assert circle1 == circle2
        assert not (circle1 != circle2)

        assert circle1 != circle3
        assert not (circle1 == circle3)

    def test_three_way_comparison(self) -> None:
        """Test three-way comparison."""
        small_circle = Circle(3.0)
        large_circle = Circle(5.0)

        assert small_circle < large_circle
        assert small_circle <= large_circle
        assert large_circle > small_circle
        assert large_circle >= small_circle
        assert not (small_circle > large_circle)
        assert not (small_circle >= large_circle)
        assert not (large_circle < small_circle)
        assert not (large_circle <= small_circle)

    def test_str(self) -> None:
        """Test string representation."""
        circle = Circle(5.0)

        assert str(circle) == 'Circle (r = 5.00)'

    def test_repr(self) -> None:
        """Test string representation for debugging."""
        circle = Circle(5.0)

        assert repr(circle).startswith('<Circle(radius=5.00) at ')


class TestRectangle:
    """Test Rectangle functionality."""

    @pytest.mark.parametrize(
        'width, height',
        [
            (4.0, 6.0),
            (5.0, None),  # square
        ],
    )
    def test_creation(self, width: float, height: float | None) -> None:
        """Test creation."""
        rect = Rectangle(width, height)

        assert rect.width == width
        if height is None:
            assert rect.height == width
        else:
            assert rect.height == height
        assert rect.is_square == (rect.width == rect.height)

    @pytest.mark.parametrize(
        'width, height',
        [
            (0.0, 5.0),
            (5.0, 0.0),
            (-3.0, 5.0),
            (5.0, -3.0),
        ],
    )
    def test_creation_with_invalid_dimensions(
        self, width: float, height: float
    ) -> None:
        """Test creation with invalid dimensions."""
        with pytest.raises(
            ValidationException, match='Rectangle dimensions must be positive'
        ):
            Rectangle(width, height)

    @pytest.mark.parametrize(
        'width, height',
        [
            (4.0, 6.0),
            (5.0, None),  # square
        ],
    )
    def test_get_area(self, width: float, height: float | None) -> None:
        """Test area calculation."""
        rect = Rectangle(width, height)
        area = rect.get_area()

        if height is None:
            assert area == width * width
        else:
            assert area == width * height

    @pytest.mark.parametrize(
        'width, height',
        [
            (4.0, 6.0),
            (5.0, None),  # square
        ],
    )
    def test_get_perimeter(self, width: float, height: float | None) -> None:
        """Test perimeter calculation."""
        rect = Rectangle(width, height)
        perimeter = rect.get_perimeter()

        if height is None:
            assert perimeter == 4 * width
        else:
            assert perimeter == 2 * (width + height)

    def test_draw(self) -> None:
        """Test draw functionality."""
        rect = Rectangle(4.0, 3.0)
        rect.draw()

    def test_equality_comparison(self) -> None:
        """Test equality comparison."""
        rect1 = Rectangle(4.0, 3.0)
        rect2 = Rectangle(4.0, 3.0)
        rect3 = Rectangle(3.0, 4.0)

        assert rect1 == rect2
        assert rect1 != rect3
        assert not (rect1 == rect3)

    def test_three_way_comparison_by_area(self) -> None:
        """Test three-way comparison by area."""
        small_rect = Rectangle(2.0, 3.0)  # area = 6.0
        large_rect = Rectangle(4.0, 5.0)  # area = 20.0

        assert small_rect < large_rect
        assert small_rect <= large_rect
        assert large_rect > small_rect
        assert large_rect >= small_rect
        assert not (small_rect > large_rect)
        assert not (small_rect >= large_rect)
        assert not (large_rect < small_rect)
        assert not (large_rect <= small_rect)

    def test_three_way_comparison_with_same_area(self) -> None:
        """Test three-way comparison with same area."""
        rect1 = Rectangle(2.0, 6.0)  # area = 12.0
        rect2 = Rectangle(3.0, 4.0)  # area = 12.0

        # Should compare by width when areas are equal
        assert rect1 < rect2
        assert rect2 > rect1
        assert not (rect1 == rect2)

    def test_three_way_comparison_with_same_width(self) -> None:
        """Test three-way comparison with same width."""
        rect1 = Rectangle(3.0, 4.0)  # area = 12.0
        rect2 = Rectangle(3.0, 5.0)  # area = 15.0

        # Should compare by area first
        assert rect1 < rect2
        assert rect2 > rect1
        assert not (rect1 == rect2)

    def test_str(self) -> None:
        """Test string representation."""
        rect = Rectangle(4.0, 3.0)

        assert str(rect) == 'Rectangle (w = 4.00, h = 3.00)'

    def test_repr(self) -> None:
        """Test string representation for debugging."""
        rect = Rectangle(4.0, 3.0)

        assert repr(rect).startswith('<Rectangle(width=4.00, height=3.00) at ')


class TestShapePolymorphism:
    """Test Shape polymorphism."""

    def test_derived_from_base_class(self) -> None:
        """Test that shapes are derived from base class."""
        assert issubclass(Circle, Shape)
        assert issubclass(Rectangle, Shape)

    def test_polymorphic_behavior(self) -> None:
        """Test polymorphic behavior."""
        circle = Circle(3.0)
        rectangle = Rectangle(4.0, 5.0)
        square = Rectangle(2.0)

        shapes: list[Shape] = [circle, rectangle, square]
        expected_results: list[tuple[type[Shape], float, float]] = [
            (Circle, math.pi * 9.0, math.pi * 6.0),
            (Rectangle, 20.0, 18.0),
            (Rectangle, 4.0, 8.0),
        ]

        for shape, (cls, area, perimeter) in zip(
            shapes, expected_results, strict=False
        ):
            assert isinstance(shape, cls)
            assert isinstance(shape, Shape)
            assert shape.get_area() == pytest.approx(area)
            assert shape.get_perimeter() == pytest.approx(perimeter)

    def test_invalid_comparison(self) -> None:
        """Test invalid comparison between different shapes."""
        circle = Circle(3.0)
        rectangle = Rectangle(4.0, 5.0)

        assert circle != rectangle
        assert not (circle == rectangle)

        with pytest.raises(TypeError):
            _ = circle < rectangle

        with pytest.raises(TypeError):
            _ = circle <= rectangle

        shape1 = Shape(circle._shape)
        shape2 = Shape(rectangle._shape)

        with pytest.raises(TypeError):
            _ = shape1 == shape2
