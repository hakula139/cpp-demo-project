/**
 * @file shapes_example.cpp
 * @brief Example demonstrating the usage of the shapes module
 */

#include <iostream>
#include <vector>
#include <memory>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"

using namespace cpp_features::shapes;

int main() {
    std::cout << "=== Shapes Module Example ===\n\n";

    // Create shapes using factory functions
    auto circle = CreateCircle(5.0);
    auto rectangle = CreateRectangle(4.0, 6.0);

    // Store shapes in a container for polymorphic usage
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(circle));
    shapes.push_back(std::move(rectangle));

    // Demonstrate polymorphic behavior
    std::cout << "Processing shapes polymorphically:\n";
    for (const auto& shape : shapes) {
        shape->Draw();

        if (auto area = shape->GetArea(); area) {
            std::cout << "  Area: " << area.value() << "\n";
        }

        if (auto perimeter = shape->GetPerimeter(); perimeter) {
            std::cout << "  Perimeter: " << perimeter.value() << "\n";
        }

        std::cout << "\n";
    }

    std::cout << "Shapes example completed successfully!\n";
    return 0;
}