"""Example demonstrating the usage of the shapes module."""

from demo.shapes import Circle, Rectangle, Shape


def demonstrate_shape_creation() -> None:
    """Demonstrate basic shape creation using constructors."""
    print('\n1. Creating shapes:')

    # Create a circle
    circle = Circle(5.0)
    print(f'   {circle}')

    # Create a rectangle
    rectangle = Rectangle(4.0, 3.0)
    print(f'   {rectangle}, is square: {rectangle.is_square}')

    # Create a square
    square = Rectangle(2.5)
    print(f'   {square}, is square: {square.is_square}')


def demonstrate_polymorphic_behavior() -> None:
    """Demonstrate polymorphic behavior with shape calculations."""
    print('\n2. Polymorphic calculations:')

    shapes: list[Shape] = [
        Circle(5.0),
        Rectangle(4.0, 3.0),
        Rectangle(2.5),
    ]

    for shape in shapes:
        shape.draw()
        area = shape.get_area()
        perimeter = shape.get_perimeter()
        print(f'   Area: {area:.2f}, Perimeter: {perimeter:.2f}')


def demonstrate_shape_comparisons() -> None:
    """Demonstrate shape comparison operations."""
    print('\n3. Comparisons:')

    rect1 = Rectangle(2.0, 7.0)
    rect2 = Rectangle(8.0, 3.0)
    assert rect1 < rect2
    print(f'   {rect1} < {rect2}')

    rect3 = Rectangle(4.0, 6.0)
    assert rect2 > rect3
    print(f'   {rect2} > {rect3}')

    rect4 = Rectangle(8.0, 3.0)
    assert rect2 == rect4
    print(f'   {rect2} == {rect4}')


def main() -> None:
    """Run all shape examples."""
    print('=== Shapes Module Example ===')

    demonstrate_shape_creation()
    demonstrate_polymorphic_behavior()
    demonstrate_shape_comparisons()

    print('\n=== Shapes Module Example Completed ===')


if __name__ == '__main__':
    main()
