/**
 * @file memory_example.cpp
 * @brief Example demonstrating the usage of the memory module
 */

#include <exception>
#include <memory>
#include <print>
#include <stdexcept>
#include <utility>

#include "memory/resource_manager.hpp"
#include "memory/unique_resource.hpp"
#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"

using cpp_features::memory::MakeUniqueWithDeleter;
using cpp_features::memory::ResourceManager;
using cpp_features::memory::UniqueResource;
using cpp_features::shapes::Circle;
using cpp_features::shapes::Rectangle;

namespace {

/**
 * @brief Demonstrate basic smart pointer usage
 */
void DemonstrateSmartPointerBasics() {
  std::println("\n1. Smart Pointer Basics:");
  auto unique_circle = std::make_unique<Circle>(5.0);
  std::println("   Created unique_ptr<Circle> (r = {})", unique_circle->GetRadius());

  auto shared_rect = std::make_shared<Rectangle>(3.0, 4.0);
  std::println("   Created shared_ptr<Rectangle>, use_count: {}", shared_rect.use_count());

  std::weak_ptr<Rectangle> weak_rect = shared_rect;
  std::println("   Created weak_ptr, expired: {}", weak_rect.expired());
}

/**
 * @brief Demonstrate custom deleter functionality
 */
void DemonstrateCustomDeleters() {
  std::println("\n2. Custom Deleters:");
  auto custom_deleter = [](Circle *ptr) {
    std::println("   Custom deleter: cleaning up Circle (r = {})", ptr->GetRadius());
    delete ptr;
  };

  {
    auto circle_with_deleter = MakeUniqueWithDeleter(new Circle(7.0), custom_deleter);
    std::println("   Created unique_ptr<Circle> (r = {}) and custom deleter",
                 circle_with_deleter->GetRadius());
  }  // Custom deleter called here
}

/**
 * @brief Demonstrate ResourceManager automatic cleanup
 */
void DemonstrateResourceManager() {
  std::println("\n3. ResourceManager:");
  {
    ResourceManager manager;

    // Create resources through manager
    auto managed_circle = manager.CreateUnique<Circle>(2.0);
    auto managed_rect = manager.CreateShared<Rectangle>(5.0, 6.0);
    std::println("   Created resources through ResourceManager");

    // Register cleanup functions
    manager.RegisterCleanup<int>([]() { std::println("   Cleanup 1: Saving application state"); });
    manager.RegisterCleanup<int>(
        []() { std::println("   Cleanup 2: Closing database connections"); });

    std::println("   Registered cleanup functions");
  }  // Automatic cleanup happens here
}

/**
 * @brief Demonstrate manual cleanup with exception handling
 */
void DemonstrateManualCleanup() {
  std::println("\n4. Manual Cleanup:");
  {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    manager.RegisterCleanup<int>([]() { std::println("   Cleanup 1: Normal operation"); });
    manager.RegisterCleanup<int>([]() {
      std::println("   Cleanup 2: Simulating error");
      throw std::runtime_error("Cleanup error");
    });

    try {
      manager.ExecuteCleanup();
    } catch (const std::exception &e) {
      std::println("   Caught cleanup exception: {}", e.what());
    }

    // Demonstrate noexcept cleanup
    manager.RegisterCleanup<int>([]() { std::println("   Cleanup 3: NoExcept cleanup"); });
    manager.ExecuteCleanupNoexcept();
  }
}

/**
 * @brief Demonstrate UniqueResource wrapper functionality
 */
void DemonstrateUniqueResource() {
  std::println("\n5. UniqueResource:");
  {
    auto raw_unique = std::make_unique<Circle>(4.5);
    UniqueResource<Circle> wrapped_circle(std::move(raw_unique));

    std::println("   Wrapped Circle (r = {})", wrapped_circle->GetRadius());
    std::println("   Raw pointer: {}", reinterpret_cast<const void *>(wrapped_circle.Get()));

    auto released = wrapped_circle.Release();
    std::println("   Released wrapped resource");
  }
}

}  // namespace

auto main() -> int {
  std::println("=== Memory Module Example ===");

  DemonstrateSmartPointerBasics();
  DemonstrateCustomDeleters();
  DemonstrateResourceManager();
  DemonstrateManualCleanup();
  DemonstrateUniqueResource();

  std::println("\n=== Memory Module Example Completed ===");
  return 0;
}
