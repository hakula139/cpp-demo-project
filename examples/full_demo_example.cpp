/**
 * @file full_demo_example.cpp
 * @brief Example demonstrating the usage of multiple modules together
 */

#include <iostream>
#include <vector>

#include "shapes/circle.hpp"
#include "containers/container.hpp"
#include "memory/smart_pointers.hpp"
#include "algorithms/stl_demos.hpp"

using namespace cpp_features;

int main() {
    std::cout << "=== Full Demo Example ===\n\n";

    // Use shapes module
    std::cout << "1. Creating shapes:\n";
    auto circle = shapes::CreateCircle(3.0);
    circle->Draw();
    if (auto area = circle->GetArea(); area) {
        std::cout << "   Circle area: " << area.value() << "\n";
    }

    // Use containers module
    std::cout << "\n2. Using modern containers:\n";
    containers::Container<double> measurements{1.1, 2.2, 3.3, 4.4, 5.5};
    measurements.Display();

    // Use algorithms module
    std::cout << "\n3. STL algorithms demonstration:\n";
    algorithms::DemonstrateStlAlgorithms();

    // Use memory module
    std::cout << "\n4. Smart pointers demonstration:\n";
    memory::DemonstrateUniquePtr();

    std::cout << "\nFull demo example completed successfully!\n";
    return 0;
}