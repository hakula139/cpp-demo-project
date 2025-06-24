/**
 * @file containers_example.cpp
 * @brief Example demonstrating the usage of the containers module
 */

#include <iostream>
#include <string>

#include "containers/container.hpp"

using namespace cpp_features::containers;

int main() {
    std::cout << "=== Containers Module Example ===\n\n";

    // Create container with integers
    Container<int> numbers{1, 2, 3, 4, 5};
    std::cout << "Original container:\n";
    numbers.Display();

    // Add more elements
    numbers.Add(6);
    numbers.Emplace(7);
    std::cout << "\nAfter adding elements:\n";
    numbers.Display();

    // Demonstrate views and filters
    std::cout << "\nFiltered view (even numbers):\n";
    auto even_view = numbers.GetFilteredView([](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: ";
    for (const auto& num : even_view) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Demonstrate transformation
    std::cout << "\nTransformed view (doubled):\n";
    auto doubled_view = numbers.GetTransformedView([](int n) { return n * 2; });
    std::cout << "Doubled numbers: ";
    for (const auto& num : doubled_view) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Test with strings
    std::cout << "\n=== String Container Example ===\n";
    Container<std::string> words{"hello", "modern", "cpp", "world"};
    words.Display();

    std::cout << "\nContainer example completed successfully!\n";
    return 0;
}