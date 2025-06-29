#include <iostream>

#include "algorithms/stl_demos.hpp"
#include "containers/container.hpp"
#include "memory/smart_pointers.hpp"
#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "strings/string_ops.hpp"

using namespace cpp_features;

auto main() -> int {
  std::cout << "=== C++23 Feature Demonstration Program ===\n";
  std::cout << "Testing various modern C++ features and libraries.\n\n";

  // Demonstrate shapes with error handling
  std::cout << "=== Shape Demonstrations ===\n";
  auto circle = shapes::CreateCircle(5.0);
  auto rectangle = shapes::CreateRectangle(4.0, 6.0);

  circle->Draw();
  if (auto area = circle->GetArea(); area) {
    std::cout << "Circle area: " << area.value() << "\n";
  }

  rectangle->Draw();
  if (auto area = rectangle->GetArea(); area) {
    std::cout << "Rectangle area: " << area.value() << "\n";
  }

  // Demonstrate modern containers
  std::cout << "\n=== Container Demonstrations ===\n";
  containers::Container<int> numbers{1, 2, 3, 4, 5};
  std::cout << "Container contents: ";
  for (const auto &n : numbers) {
    std::cout << n << " ";
  }
  std::cout << "\n";

  // Demonstrate smart pointers
  std::cout << "\n=== Smart Pointer Demonstrations ===\n";
  memory::DemonstrateUniquePtr();
  memory::DemonstrateSharedPtr();

  // Note: STL algorithms demonstrations are now in examples/algorithms_example.cpp
  std::cout << "\n=== STL Algorithms Module ===\n";
  std::cout << "STL algorithms utilities are available for use.\n";
  std::cout << "Run examples/algorithms_example for detailed demonstrations.\n";

  // Demonstrate string operations
  strings::DemonstrateBasicStringOps();

  std::cout << "\nProgram completed successfully!\n";
  return 0;
}
