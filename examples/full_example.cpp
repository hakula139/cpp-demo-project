/**
 * @file full_demo_example.cpp
 * @brief Example demonstrating the usage of multiple modules together
 */

#include <iostream>
#include <vector>

#include "algorithms/stl.hpp"
#include "containers/container.hpp"
#include "memory/smart_pointers.hpp"
#include "shapes/circle.hpp"

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

  // Use algorithms module utilities
  std::cout << "\n3. STL algorithms utilities:\n";
  std::vector<int> test_data = {5, 2, 8, 1, 9};
  algorithms::SortContainer(test_data);
  std::cout << "   Sorted data: ";
  for (const auto &n : test_data) {
    std::cout << n << " ";
  }
  std::cout << "\n   (See algorithms_example.cpp for full demonstrations)\n";

  // Use memory module
  std::cout << "\n4. Smart pointers demonstration:\n";
  memory::DemonstrateUniquePtr();

  std::cout << "\nFull demo example completed successfully!\n";
  return 0;
}
