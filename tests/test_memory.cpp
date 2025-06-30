#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "memory/resource_management.hpp"

namespace {

using namespace cpp_features::memory;

class TestResource {
 public:
  explicit TestResource(int value) : value_{value} {}
  explicit TestResource(int value, std::string name) : value_{value}, name_{std::move(name)} {}

  [[nodiscard]] auto GetValue() const noexcept -> int { return value_; }
  [[nodiscard]] auto GetName() const noexcept -> std::string_view { return name_; }

 private:
  int value_;
  std::string name_{"default"};
};

class CustomDeleterTestResource {
 public:
  explicit CustomDeleterTestResource(int value) : value_{value} { ++instance_count; }

  ~CustomDeleterTestResource() { --instance_count; }

  [[nodiscard]] auto GetValue() const noexcept -> int { return value_; }

  static auto GetInstanceCount() noexcept -> int { return instance_count; }
  static void ResetInstanceCount() noexcept { instance_count = 0; }

 private:
  static int instance_count;
  int value_;
};

int CustomDeleterTestResource::instance_count = 0;

// Helper functions for testing memory module
namespace test_helpers {

auto CreateThrowingCleanup() -> std::function<void()> {
  return []() { throw std::runtime_error("Cleanup error"); };
}

auto CreateCountingCleanup(int *counter) -> std::function<void()> {
  return [counter]() { ++(*counter); };
}

}  // namespace test_helpers

TEST_CASE("ResourceManager construction and basic properties", "[memory][resource_manager]") {
  SECTION("Default construction") {
    ResourceManager manager;
    // Basic properties should be satisfied
    REQUIRE_NOTHROW(manager.DisableAutomaticCleanup());
  }

  SECTION("Move construction") {
    ResourceManager original_manager;
    original_manager.RegisterCleanup<int>([]() {});

    ResourceManager moved_manager{std::move(original_manager)};
    REQUIRE_NOTHROW(moved_manager.DisableAutomaticCleanup());
  }

  SECTION("Move assignment") {
    ResourceManager original_manager;
    original_manager.RegisterCleanup<int>([]() {});

    ResourceManager assigned_manager;
    assigned_manager = std::move(original_manager);
    REQUIRE_NOTHROW(assigned_manager.DisableAutomaticCleanup());
  }

  SECTION("Copy operations are deleted") {
    static_assert(!std::is_copy_constructible_v<ResourceManager>);
    static_assert(!std::is_copy_assignable_v<ResourceManager>);
  }
}

TEST_CASE("ResourceManager smart pointer creation", "[memory][resource_manager][smart_pointers]") {
  SECTION("CreateUnique with single parameter") {
    ResourceManager manager;
    auto value = 42;
    auto unique_resource = manager.CreateUnique<TestResource>(value);

    REQUIRE(unique_resource != nullptr);
    REQUIRE(unique_resource->GetValue() == value);
    REQUIRE(unique_resource->GetName() == "default");
  }

  SECTION("CreateUnique with multiple parameters") {
    ResourceManager manager;
    auto value = 100;
    std::string name{"test_name"};
    auto unique_resource = manager.CreateUnique<TestResource>(value, name);

    REQUIRE(unique_resource != nullptr);
    REQUIRE(unique_resource->GetValue() == value);
    REQUIRE(unique_resource->GetName() == name);
  }

  SECTION("CreateShared with single parameter") {
    ResourceManager manager;
    auto value = 75;
    auto shared_resource = manager.CreateShared<TestResource>(value);

    REQUIRE(shared_resource != nullptr);
    REQUIRE(shared_resource.use_count() == 1);
    REQUIRE(shared_resource->GetValue() == value);
    REQUIRE(shared_resource->GetName() == "default");
  }

  SECTION("CreateShared with multiple parameters") {
    ResourceManager manager;
    auto value = 200;
    std::string name{"shared_test"};
    auto shared_resource = manager.CreateShared<TestResource>(value, name);

    REQUIRE(shared_resource != nullptr);
    REQUIRE(shared_resource.use_count() == 1);
    REQUIRE(shared_resource->GetValue() == value);
    REQUIRE(shared_resource->GetName() == name);
  }

  SECTION("CreateShared reference counting") {
    ResourceManager manager;
    auto value = 50;
    std::string name{"shared_test"};
    auto shared1 = manager.CreateShared<TestResource>(value, name);
    auto shared2 = shared1;
    auto shared3 = shared1;

    REQUIRE(shared1.use_count() == 3);
    REQUIRE(shared1->GetValue() == value);
    REQUIRE(shared2.use_count() == 3);
    REQUIRE(shared2->GetValue() == value);
    REQUIRE(shared3.use_count() == 3);
    REQUIRE(shared3->GetValue() == value);

    shared2.reset();
    REQUIRE(shared1.use_count() == 2);
    REQUIRE(shared1->GetValue() == value);
    REQUIRE(shared3.use_count() == 2);
    REQUIRE(shared3->GetValue() == value);

    shared1.reset();
    REQUIRE(shared3.use_count() == 1);
    REQUIRE(shared3->GetValue() == value);
    REQUIRE(shared3->GetName() == name);
  }
}

TEST_CASE("ResourceManager cleanup registration", "[memory][resource_manager][cleanup]") {
  SECTION("Register single cleanup function") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    REQUIRE(cleanup_count == 0);
    manager.ExecuteCleanup();
    REQUIRE(cleanup_count == 1);
  }

  SECTION("Register multiple cleanup functions") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    REQUIRE(cleanup_count == 0);
    manager.ExecuteCleanup();
    REQUIRE(cleanup_count == 3);
  }

  SECTION("Cleanup functions execute in reverse order") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    std::vector<int> execution_order;
    manager.RegisterCleanup<int>([&execution_order]() { execution_order.push_back(1); });
    manager.RegisterCleanup<int>([&execution_order]() { execution_order.push_back(2); });
    manager.RegisterCleanup<int>([&execution_order]() { execution_order.push_back(3); });

    manager.ExecuteCleanup();

    REQUIRE(execution_order.size() == 3);
    REQUIRE(execution_order == std::vector<int>{3, 2, 1});
  }

  SECTION("Cleanup functions are cleared after execution") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    manager.ExecuteCleanup();
    REQUIRE(cleanup_count == 1);

    // Executing again should not call cleanup functions
    manager.ExecuteCleanup();
    REQUIRE(cleanup_count == 1);
  }
}

TEST_CASE("ResourceManager cleanup execution", "[memory][resource_manager][cleanup]") {
  SECTION("ExecuteCleanup with normal functions") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    REQUIRE_NOTHROW(manager.ExecuteCleanup());
    REQUIRE(cleanup_count == 1);
  }

  SECTION("ExecuteCleanup with throwing function") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    manager.RegisterCleanup<int>(test_helpers::CreateThrowingCleanup());

    REQUIRE_THROWS_AS(manager.ExecuteCleanup(), std::runtime_error);
  }

  SECTION("ExecuteCleanupNoexcept with normal functions") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    REQUIRE_NOTHROW(manager.ExecuteCleanupNoexcept());
    REQUIRE(cleanup_count == 1);
  }

  SECTION("ExecuteCleanupNoexcept with throwing function") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));
    manager.RegisterCleanup<int>(test_helpers::CreateThrowingCleanup());
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    // Should not throw and should continue executing other cleanup functions
    REQUIRE_NOTHROW(manager.ExecuteCleanupNoexcept());
    REQUIRE(cleanup_count == 2);
  }

  SECTION("DisableAutomaticCleanup prevents destructor cleanup") {
    auto cleanup_count = 0;
    {
      ResourceManager manager;
      manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));
      manager.DisableAutomaticCleanup();
    }  // Destructor called here, but cleanup disabled

    REQUIRE(cleanup_count == 0);
  }

  SECTION("Automatic cleanup in destructor") {
    auto cleanup_count = 0;
    {
      ResourceManager manager;
      manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));
      // Automatic cleanup enabled by default
    }  // Destructor called here with automatic cleanup

    REQUIRE(cleanup_count == 1);
  }
}

TEST_CASE("UniqueResource basic operations", "[memory][unique_resource]") {
  SECTION("Construction from unique_ptr") {
    auto value = 42;
    auto unique_ptr = std::make_unique<TestResource>(value);
    UniqueResource<TestResource> wrapped{std::move(unique_ptr)};

    REQUIRE(wrapped.Get() != nullptr);
    REQUIRE(wrapped->GetValue() == value);
  }

  SECTION("Get method returns raw pointer") {
    auto value = 100;
    auto unique_ptr = std::make_unique<TestResource>(value);
    auto *raw_ptr = unique_ptr.get();
    UniqueResource<TestResource> wrapped{std::move(unique_ptr)};

    REQUIRE(wrapped.Get() == raw_ptr);
    REQUIRE(wrapped.Get()->GetValue() == value);
  }

  SECTION("Release method transfers ownership") {
    auto value = 200;
    auto unique_ptr = std::make_unique<TestResource>(value);
    UniqueResource<TestResource> wrapped{std::move(unique_ptr)};

    auto released = wrapped.Release();
    REQUIRE(released != nullptr);
    REQUIRE(released->GetValue() == value);
    REQUIRE(wrapped.Get() == nullptr);
  }

  SECTION("Operator-> provides pointer access") {
    auto value = 300;
    std::string name{"arrow_test"};
    auto unique_ptr = std::make_unique<TestResource>(value, name);
    UniqueResource<TestResource> wrapped{std::move(unique_ptr)};

    REQUIRE(wrapped->GetValue() == value);
    REQUIRE(wrapped->GetName() == name);
  }

  SECTION("Operator* provides reference access") {
    auto value = 400;
    std::string name{"deref_test"};
    auto unique_ptr = std::make_unique<TestResource>(value, name);
    UniqueResource<TestResource> wrapped{std::move(unique_ptr)};

    auto &ref = *wrapped;
    REQUIRE(ref.GetValue() == value);
    REQUIRE(ref.GetName() == name);
  }
}

TEST_CASE("MakeUniqueWithDeleter functionality", "[memory][custom_deleter]") {
  SECTION("Default deleter behavior") {
    CustomDeleterTestResource::ResetInstanceCount();

    {
      auto value = 42;
      auto ptr = MakeUniqueWithDeleter(new CustomDeleterTestResource(value));
      REQUIRE(CustomDeleterTestResource::GetInstanceCount() == 1);
      REQUIRE(ptr->GetValue() == value);
    }  // Default deleter called here

    REQUIRE(CustomDeleterTestResource::GetInstanceCount() == 0);
  }

  SECTION("Custom deleter behavior") {
    CustomDeleterTestResource::ResetInstanceCount();
    auto custom_deleter_called = false;

    auto custom_deleter = [&custom_deleter_called](CustomDeleterTestResource *ptr) {
      custom_deleter_called = true;
      delete ptr;
    };

    {
      auto value = 100;
      auto ptr = MakeUniqueWithDeleter(new CustomDeleterTestResource(value), custom_deleter);
      REQUIRE(CustomDeleterTestResource::GetInstanceCount() == 1);
      REQUIRE(ptr->GetValue() == value);
      REQUIRE_FALSE(custom_deleter_called);
    }  // Custom deleter called here

    REQUIRE(custom_deleter_called);
    REQUIRE(CustomDeleterTestResource::GetInstanceCount() == 0);
  }

  SECTION("Custom deleter with lambda capturing state") {
    CustomDeleterTestResource::ResetInstanceCount();
    auto deletion_count = 0;

    auto counting_deleter = [&deletion_count](CustomDeleterTestResource *ptr) {
      ++deletion_count;
      delete ptr;
    };

    {
      auto ptr1 = MakeUniqueWithDeleter(new CustomDeleterTestResource(1), counting_deleter);
      auto ptr2 = MakeUniqueWithDeleter(new CustomDeleterTestResource(2), counting_deleter);
      REQUIRE(CustomDeleterTestResource::GetInstanceCount() == 2);
    }  // Both custom deleters called here

    REQUIRE(deletion_count == 2);
    REQUIRE(CustomDeleterTestResource::GetInstanceCount() == 0);
  }
}

TEST_CASE("Memory module integration scenarios", "[memory][integration]") {
  SECTION("ResourceManager with UniqueResource") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    auto value = 500;
    std::string name{"integration_test"};
    auto unique_resource = manager.CreateUnique<TestResource>(value, name);
    UniqueResource<TestResource> wrapped{std::move(unique_resource)};

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>([&cleanup_count, &wrapped, value]() {
      REQUIRE(wrapped->GetValue() == value);
      ++cleanup_count;
    });

    manager.ExecuteCleanup();
    REQUIRE(cleanup_count == 1);
  }

  SECTION("Multiple ResourceManagers with overlapping lifetimes") {
    auto cleanup_count_1 = 0;
    auto cleanup_count_2 = 0;

    {
      ResourceManager manager1;
      manager1.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count_1));

      {
        ResourceManager manager2;
        manager2.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count_2));
      }  // manager2 destructor

      REQUIRE(cleanup_count_1 == 0);
      REQUIRE(cleanup_count_2 == 1);
    }  // manager1 destructor

    REQUIRE(cleanup_count_1 == 1);
    REQUIRE(cleanup_count_2 == 1);
  }

  SECTION("Exception safety in complex scenarios") {
    ResourceManager manager;
    manager.DisableAutomaticCleanup();

    manager.RegisterCleanup<int>(test_helpers::CreateThrowingCleanup());
    REQUIRE_THROWS(manager.ExecuteCleanup());

    auto cleanup_count = 0;
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));
    manager.RegisterCleanup<int>(test_helpers::CreateThrowingCleanup());
    manager.RegisterCleanup<int>(test_helpers::CreateCountingCleanup(&cleanup_count));

    REQUIRE_NOTHROW(manager.ExecuteCleanupNoexcept());
    REQUIRE(cleanup_count == 2);  // Both non-throwing cleanup functions executed
  }
}

}  // namespace
