"""Tests for Python shapes module.

Comprehensive tests following C++ test patterns for shapes functionality.
"""

import math

import pytest

from python import shapes


class TestCircle:
    """Test Circle creation and basic properties."""

    def test_circle_creation(self) -> None:
        """Test basic circle creation."""
        circle = shapes.create_shape('circle', 5.0)

        assert circle.get_name() == 'Circle'
        assert circle.get_area() == pytest.approx(math.pi * 25.0)
        assert circle.get_perimeter() == pytest.approx(math.pi * 10.0)

    def test_circle_factory_function(self) -> None:
        """Test circle factory function."""
        circle = shapes.create_shape(shapes.ShapeType.CIRCLE, 3.0)

        assert circle.get_name() == 'Circle'
        assert circle.get_area() == pytest.approx(math.pi * 9.0)

    def test_circle_invalid_radius(self) -> None:
        """Test circle with invalid radius throws exception."""
        with pytest.raises(Exception):  # C++ ValidationException
            shapes.create_shape('circle', 0.0)

        with pytest.raises(Exception):
            shapes.create_shape('circle', -3.0)

    def test_circle_comparison(self) -> None:
        """Test circle comparison operations."""
        circle1 = shapes.Circle(5.0)
        circle2 = shapes.Circle(5.0)
        circle3 = shapes.Circle(3.0)

        assert circle1 == circle2
        assert not (circle1 == circle3)
        assert circle3 < circle1
        assert circle1 > circle3


class TestRectangle:
    """Test Rectangle creation and basic properties."""

    def test_rectangle_creation(self) -> None:
        """Test basic rectangle creation."""
        rect = shapes.create_shape('rectangle', 4.0, 6.0)

        assert rect.get_name() == 'Rectangle'
        assert rect.get_area() == 24.0
        assert rect.get_perimeter() == 20.0
        assert not rect.is_square()

    def test_square_creation(self) -> None:
        """Test square creation."""
        square = shapes.create_shape('square', 5.0)

        assert square.get_name() == 'Rectangle'
        assert square.get_area() == 25.0
        assert square.get_perimeter() == 20.0
        assert square.is_square()

    def test_rectangle_invalid_dimensions(self) -> None:
        """Test rectangle with invalid dimensions throws exception."""
        with pytest.raises(Exception):
            shapes.create_shape('rectangle', 0.0, 5.0)

        with pytest.raises(Exception):
            shapes.create_shape('rectangle', 5.0, -3.0)

    def test_rectangle_comparison(self) -> None:
        """Test rectangle comparison operations."""
        rect1 = shapes.Rectangle(4.0, 3.0)
        rect2 = shapes.Rectangle(4.0, 3.0)
        rect3 = shapes.Rectangle(2.0, 3.0)

        assert rect1 == rect2
        assert not (rect1 == rect3)
        assert rect3 < rect1


class TestShapeAnalysis:
    """Test shape analysis functions."""

    def test_analyze_shape(self) -> None:
        """Test shape analysis function."""
        circle = shapes.create_shape('circle', 5.0)
        metrics = shapes.analyze_shape(circle)

        assert metrics.name == 'Circle'
        assert metrics.area == pytest.approx(math.pi * 25.0)
        assert metrics.perimeter == pytest.approx(math.pi * 10.0)
        assert metrics.aspect_ratio > 0

    def test_compare_shapes(self) -> None:
        """Test shape comparison function."""
        circle = shapes.create_shape('circle', 3.0)
        rectangle = shapes.create_shape('rectangle', 4.0, 5.0)
        square = shapes.create_shape('square', 2.0)

        comparison = shapes.compare_shapes(circle, rectangle, square)

        assert comparison['count'] == 3
        assert comparison['total_area'] > 0
        assert comparison['total_perimeter'] > 0
        assert 'largest_by_area' in comparison
        assert 'smallest_by_area' in comparison
        assert 'average_area' in comparison
        assert len(comparison['metrics']) == 3

    def test_compare_shapes_empty(self) -> None:
        """Test comparison with no shapes raises error."""
        with pytest.raises(ValueError, match='At least one shape is required'):
            shapes.compare_shapes()


class TestShapeFactory:
    """Test shape factory functions."""

    def test_factory_with_string_types(self) -> None:
        """Test factory with string shape types."""
        circle = shapes.create_shape('circle', 5.0)
        rectangle = shapes.create_shape('rectangle', 3.0, 4.0)
        square = shapes.create_shape('square', 6.0)

        assert circle.get_name() == 'Circle'
        assert rectangle.get_name() == 'Rectangle'
        assert square.get_name() == 'Rectangle'
        assert square.is_square()

    def test_factory_with_enum_types(self) -> None:
        """Test factory with enum shape types."""
        circle = shapes.create_shape(shapes.ShapeType.CIRCLE, 5.0)
        rectangle = shapes.create_shape(shapes.ShapeType.RECTANGLE, 3.0, 4.0)
        square = shapes.create_shape(shapes.ShapeType.SQUARE, 6.0)

        assert circle.get_name() == 'Circle'
        assert rectangle.get_name() == 'Rectangle'
        assert square.is_square()

    def test_factory_invalid_arguments(self) -> None:
        """Test factory with invalid arguments."""
        with pytest.raises(ValueError, match='Circle requires exactly 1 argument'):
            shapes.create_shape('circle', 1.0, 2.0)

        with pytest.raises(ValueError, match='Rectangle requires exactly 2 arguments'):
            shapes.create_shape('rectangle', 1.0)

        with pytest.raises(ValueError, match='Unknown shape type'):
            shapes.create_shape('triangle', 1.0)


class TestShapeMetrics:
    """Test ShapeMetrics dataclass."""

    def test_shape_metrics_creation(self) -> None:
        """Test creating ShapeMetrics."""
        metrics = shapes.ShapeMetrics(area=25.0, perimeter=20.0, name='Square')

        assert metrics.area == 25.0
        assert metrics.perimeter == 20.0
        assert metrics.name == 'Square'
        assert metrics.aspect_ratio == pytest.approx(25.0 / 400.0)

    def test_shape_metrics_immutable(self) -> None:
        """Test that ShapeMetrics is immutable."""
        metrics = shapes.ShapeMetrics(area=25.0, perimeter=20.0, name='Square')

        with pytest.raises(AttributeError):
            metrics.area = 30.0  # Should fail - frozen dataclass


class TestShapeIntegration:
    """Integration tests for shape functionality."""

    def test_polymorphic_behavior(self) -> None:
        """Test polymorphic behavior with different shapes."""
        shapes_list = [
            shapes.create_shape('circle', 3.0),
            shapes.create_shape('rectangle', 4.0, 5.0),
            shapes.create_shape('square', 2.0)
        ]

        expected_names = ['Circle', 'Rectangle', 'Rectangle']
        expected_areas = [math.pi * 9.0, 20.0, 4.0]

        for shape, name, area in zip(shapes_list, expected_names, expected_areas):
            assert shape.get_name() == name
            assert shape.get_area() == pytest.approx(area)

    def test_shape_collection_analysis(self) -> None:
        """Test analyzing a collection of shapes."""
        test_shapes = [
            shapes.create_shape('circle', 1.0),
            shapes.create_shape('circle', 2.0),
            shapes.create_shape('rectangle', 2.0, 3.0),
            shapes.create_shape('square', 2.0)
        ]

        total_area = sum(shape.get_area() for shape in test_shapes)
        circle_area_1 = math.pi * 1.0
        circle_area_2 = math.pi * 4.0
        rect_area = 6.0
        square_area = 4.0

        expected_total = circle_area_1 + circle_area_2 + rect_area + square_area
        assert total_area == pytest.approx(expected_total)

    def test_shape_sorting_by_area(self) -> None:
        """Test sorting shapes by area."""
        test_shapes = [
            shapes.create_shape('circle', 2.0),      # π * 4 ≈ 12.57
            shapes.create_shape('rectangle', 2.0, 3.0),  # 6.0
            shapes.create_shape('square', 4.0),          # 16.0
            shapes.create_shape('circle', 1.0)       # π ≈ 3.14
        ]

        sorted_shapes = sorted(test_shapes, key=lambda s: s.get_area())
        areas = [shape.get_area() for shape in sorted_shapes]

        # Should be in ascending order
        for i in range(len(areas) - 1):
            assert areas[i] <= areas[i + 1]
