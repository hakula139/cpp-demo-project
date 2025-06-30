#include "memory/smart_pointers.hpp"

#include <iostream>
#include <memory>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"

namespace cpp_features::memory {

void ResourceManager::ExecuteCleanup() {
  std::cout << "Executing " << cleanup_functions_.size() << " cleanup functions\n";
  for (auto &cleanup_func : cleanup_functions_) {
    cleanup_func();
  }
  cleanup_functions_.clear();
}

void DemonstrateUniquePtr() {
  std::cout << "\n=== Unique Pointer Demonstration ===\n";

  auto circle = std::make_unique<shapes::Circle>(5.0);
  std::cout << "Created unique_ptr to Circle with radius " << circle->GetRadius() << "\n";

  circle->Draw();
  auto area = circle->GetArea();
  if (area) {
    std::cout << "Circle area: " << area.value() << "\n";
  }

  auto rectangle = std::make_unique<shapes::Rectangle>(4.0, 6.0);
  auto rect_area = rectangle->GetArea();
  if (rect_area) {
    std::cout << "Rectangle area: " << rect_area.value() << "\n";
  }

  auto moved_circle = std::move(circle);
  std::cout << "Moved circle ownership\n";

  if (!circle) {
    std::cout << "Original circle pointer is now null\n";
  }

  if (moved_circle) {
    std::cout << "Moved circle is valid with radius " << moved_circle->GetRadius() << "\n";
  }
}

void DemonstrateSharedPtr() {
  std::cout << "\n=== Shared Pointer Demonstration ===\n";

  auto shared_circle = std::make_shared<shapes::Circle>(3.0);
  std::cout << "Created shared_ptr, reference count: " << shared_circle.use_count() << "\n";

  {
    auto another_ref = shared_circle;
    std::cout << "Added another reference, count: " << shared_circle.use_count() << "\n";

    auto third_ref = shared_circle;
    std::cout << "Added third reference, count: " << shared_circle.use_count() << "\n";
  }

  std::cout << "Exited scope, reference count: " << shared_circle.use_count() << "\n";

  std::vector<std::shared_ptr<shapes::Circle>> circles;
  circles.push_back(shared_circle);
  circles.push_back(shared_circle);

  std::cout << "Added to vector twice, reference count: " << shared_circle.use_count() << "\n";
}

void DemonstrateWeakPtr() {
  std::cout << "\n=== Weak Pointer Demonstration ===\n";

  std::weak_ptr<shapes::Rectangle> weak_rect;

  {
    auto shared_rect = std::make_shared<shapes::Rectangle>(2.0, 3.0);
    weak_rect = shared_rect;

    std::cout << "Shared pointer exists, weak_ptr expired: " << weak_rect.expired() << "\n";

    if (auto locked = weak_rect.lock()) {
      std::cout << "Successfully locked weak_ptr, area: " << locked->GetArea().value_or(0.0)
                << "\n";
    }
  }

  std::cout << "Shared pointer destroyed, weak_ptr expired: " << weak_rect.expired() << "\n";

  if (auto locked = weak_rect.lock()) {
    std::cout << "Unexpectedly locked expired weak_ptr with area: "
              << locked->GetArea().value_or(0.0) << "\n";
  } else {
    std::cout << "Cannot lock expired weak_ptr as expected\n";
  }
}

void DemonstrateCustomDeleter() {
  std::cout << "\n=== Custom Deleter Demonstration ===\n";

  auto custom_deleter = [](shapes::Circle *ptr) {
    std::cout << "Custom deleter called for Circle with radius " << ptr->GetRadius() << "\n";
    delete ptr;
  };

  {
    static constexpr double kDemoCircleRadius = 7.0;
    auto circle_ptr = MakeUniqueWithDeleter(new shapes::Circle(kDemoCircleRadius), custom_deleter);
    std::cout << "Created unique_ptr with custom deleter\n";
    circle_ptr->Draw();
  }
  std::cout << "unique_ptr went out of scope\n";

  auto logging_deleter = [](shapes::Rectangle *ptr) {
    auto area = ptr->GetArea();
    std::cout << "Destroying Rectangle with area " << area.value_or(0.0) << "\n";
    delete ptr;
  };

  {
    std::unique_ptr<shapes::Rectangle, decltype(logging_deleter)> rect_ptr(
        new shapes::Rectangle(5.0, 8.0), logging_deleter);
    std::cout << "Created Rectangle with logging deleter\n";
  }
}

void DemonstrateResourceManagement() {
  std::cout << "\n=== Resource Management Demonstration ===\n";

  ResourceManager manager;

  static constexpr double kManagedCircleRadius = 4.0;
  static constexpr double kManagedRectWidth = 3.0;
  static constexpr double kManagedRectHeight = 5.0;

  auto circle = manager.CreateUnique<shapes::Circle>(kManagedCircleRadius);
  auto rectangle = manager.CreateShared<shapes::Rectangle>(kManagedRectWidth, kManagedRectHeight);

  std::cout << "Created resources through ResourceManager\n";

  manager.RegisterCleanup<int>([]() { std::cout << "Cleanup function 1 executed\n"; });

  manager.RegisterCleanup<int>([]() { std::cout << "Cleanup function 2 executed\n"; });

  circle->Draw();
  rectangle->Draw();

  manager.ExecuteCleanup();

  UniqueResource<shapes::Circle> wrapped_circle(std::make_unique<shapes::Circle>(6.0));

  std::cout << "Wrapped circle radius: " << wrapped_circle->GetRadius() << "\n";

  auto released = wrapped_circle.Release();
  std::cout << "Released wrapped resource\n";
}

}  // namespace cpp_features::memory
