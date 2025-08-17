#!/usr/bin/env python3
"""Shapes module example demonstrating geometric shape creation and analysis.

This example shows how to create different shapes, calculate their properties,
and perform analysis using the C++ Python bindings.
"""

import sys
from pathlib import Path
from typing import List

# Add the python module to the path
sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

from shapes import Circle, Rectangle, analyze_shape, compare_shapes, create_shape


def basic_shapes_demo() -> None:
    """Demonstrate basic shape creation and property calculation.

    Creates different types of shapes and displays their basic properties
    like area and perimeter.
    """
    print('=== Basic Shapes Demo ===')

    # Create different shapes using factory functions
    circle = create_shape('circle', 5.0)
    rectangle = create_shape('rectangle', 4.0, 3.0)
    square = create_shape('square', 6.0)

    print(
        f'Circle: area={circle.get_area():.2f}, perimeter={circle.get_perimeter():.2f}'
    )
    print(
        f'Rectangle: area={rectangle.get_area():.2f}, perimeter={rectangle.get_perimeter():.2f}'
    )
    print(
        f'Square: area={square.get_area():.2f}, perimeter={square.get_perimeter():.2f}'
    )

    print()


def shape_analysis_demo() -> None:
    """Demonstrate advanced shape analysis capabilities.

    Shows how to analyze individual shapes and extract detailed metrics
    including aspect ratios and other derived properties.
    """
    print('=== Shape Analysis Demo ===')

    # Create shapes for analysis
    circle = create_shape('circle', 3.5)
    rectangle = create_shape('rectangle', 8.0, 2.5)

    # Analyze individual shapes
    circle_metrics = analyze_shape(circle)
    rectangle_metrics = analyze_shape(rectangle)

    print(f'Circle analysis:')
    print(f'  - Area: {circle_metrics.area:.2f}')
    print(f'  - Perimeter: {circle_metrics.perimeter:.2f}')
    print(f'  - Aspect ratio: {circle_metrics.aspect_ratio:.4f}')

    print(f'Rectangle analysis:')
    print(f'  - Area: {rectangle_metrics.area:.2f}')
    print(f'  - Perimeter: {rectangle_metrics.perimeter:.2f}')
    print(f'  - Aspect ratio: {rectangle_metrics.aspect_ratio:.4f}')

    print()


def shape_comparison_demo() -> None:
    """Demonstrate shape comparison and collection analysis.

    Shows how to compare multiple shapes and extract aggregate statistics
    from collections of geometric objects.
    """
    print('=== Shape Comparison Demo ===')

    # Create a collection of shapes
    shapes = [
        create_shape('circle', 4.0),
        create_shape('rectangle', 5.0, 3.0),
        create_shape('square', 4.5),
        create_shape('circle', 2.5),
        create_shape('rectangle', 6.0, 2.0),
    ]

    # Compare all shapes
    comparison = compare_shapes(*shapes)

    print(f'Shape Collection Analysis:')
    print(f'  - Total shapes: {len(shapes)}')
    print(f'  - Total area: {comparison["total_area"]:.2f}')
    print(f'  - Total perimeter: {comparison["total_perimeter"]:.2f}')
    print(f'  - Average area: {comparison["average_area"]:.2f}')
    print(f'  - Largest shape by area: {comparison["largest_by_area"].get_name()}')
    print(f'  - Smallest shape by area: {comparison["smallest_by_area"].get_name()}')

    # Show individual shape details
    print(f'\nIndividual Shape Details:')
    for i, shape in enumerate(shapes, 1):
        metrics = analyze_shape(shape)
        print(f'  {i}. {shape.get_name()}: area={metrics.area:.2f}')

    print()


def advanced_shape_operations() -> None:
    """Demonstrate advanced shape operations and edge cases.

    Shows handling of edge cases, validation, and advanced geometric
    calculations with error handling.
    """
    print('=== Advanced Shape Operations ===')

    # Test edge cases and validation
    test_cases = [
        ('Very small circle', 'circle', 0.1),
        ('Large rectangle', 'rectangle', 100.0, 50.0),
        ('Square', 'square', 7.5),
        ('Thin rectangle', 'rectangle', 20.0, 0.5),
    ]

    print('Testing various shape configurations:')
    for description, shape_type, *params in test_cases:
        try:
            shape = create_shape(shape_type, *params)
            metrics = analyze_shape(shape)

            print(f'  {description}:')
            print(f'    - Type: {shape.get_name()}')
            print(f'    - Area: {metrics.area:.4f}')
            print(f'    - Perimeter: {metrics.perimeter:.4f}')
            print(f'    - Aspect ratio: {metrics.aspect_ratio:.4f}')

            # Check for interesting properties
            if metrics.aspect_ratio > 5.0:
                print(
                    f'    - Note: Very elongated shape (aspect ratio: {metrics.aspect_ratio:.2f})'
                )
            if metrics.efficiency > 0.5:
                print(
                    f'    - Note: Highly efficient shape (efficiency: {metrics.efficiency:.4f})'
                )

        except Exception as e:
            print(f'  {description}: Error - {e}')

    print()


def shape_factory_patterns() -> None:
    """Demonstrate various shape creation patterns.

    Shows different ways to create shapes using factory methods and
    direct constructors with parameter validation.
    """
    print('=== Shape Factory Patterns ===')

    # Direct constructor usage
    print('Direct constructors:')
    direct_circle = Circle(radius=3.0)
    direct_rectangle = Rectangle(width=4.0, height=6.0)

    print(f'  Circle (direct): area={direct_circle.get_area():.2f}')
    print(f'  Rectangle (direct): area={direct_rectangle.get_area():.2f}')

    # Factory function usage with different patterns
    print('\nFactory function patterns:')

    # Standard shapes
    unit_circle = create_shape('circle', 1.0)
    unit_square = create_shape('square', 1.0)

    print(f'  Unit circle: area={unit_circle.get_area():.4f}')
    print(f'  Unit square: area={unit_square.get_area():.4f}')

    # Golden ratio rectangle
    golden_ratio = 1.618033988749
    golden_rectangle = create_shape('rectangle', golden_ratio, 1.0)
    golden_metrics = analyze_shape(golden_rectangle)

    print(f'  Golden rectangle: aspect_ratio={golden_metrics.aspect_ratio:.6f}')

    # Array of similar shapes for batch processing
    radii = [1.0, 1.5, 2.0, 2.5, 3.0]
    circles = [create_shape('circle', r) for r in radii]
    areas = [c.get_area() for c in circles]

    print(f'  Circle areas for radii {radii}: {[f"{a:.2f}" for a in areas]}')

    print()


def main() -> int:
    """Run all shape examples.

    Returns
    -------
    int
        Exit code (0 for success, 1 for error)
    """
    print('Shapes Module Example')
    print('====================')
    print()

    try:
        basic_shapes_demo()
        shape_analysis_demo()
        shape_comparison_demo()
        advanced_shape_operations()
        shape_factory_patterns()

        print('All shape examples completed successfully!')
        return 0

    except Exception as e:
        print(f'Error running shape examples: {e}')
        import traceback

        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
