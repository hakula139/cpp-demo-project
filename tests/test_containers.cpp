#include <expected>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "containers/container.hpp"

namespace {

using namespace cpp_features::containers;

TEST_CASE("Container basic operations", "[containers][basic]") {
  SECTION("Empty container") {
    Container<int> container;

    REQUIRE(container.IsEmpty());
    REQUIRE(container.GetSize() == 0);
    REQUIRE(container.size() == 0);
  }

  SECTION("Initializer list construction") {
    Container<int> container{1, 2, 3, 4, 5};

    REQUIRE_FALSE(container.IsEmpty());
    REQUIRE(container.GetSize() == 5);
    REQUIRE(container.size() == 5);
  }

  SECTION("Range construction") {
    std::vector<int> source{1, 2, 3, 4, 5};
    Container<int> container(source);

    REQUIRE_FALSE(container.IsEmpty());
    REQUIRE(container.GetSize() == 5);
    REQUIRE(container.size() == 5);
  }

  SECTION("CTAD guide") {
    Container container{1, 2, 3, 4, 5};

    REQUIRE_FALSE(container.IsEmpty());
    REQUIRE(container.GetSize() == 5);
    REQUIRE(container.size() == 5);
  }

  SECTION("Add elements") {
    Container<std::string> container;

    container.Add("Hello");
    std::string world = "World";
    container.Add(std::move(world));

    REQUIRE_FALSE(container.IsEmpty());
    REQUIRE(container.GetSize() == 2);
    REQUIRE(container.size() == 2);
  }

  SECTION("Emplace elements") {
    Container<std::string> container;

    container.Emplace("Hello");
    container.Emplace(5, '!');

    REQUIRE(container.GetSize() == 2);

    auto val0 = container.At(0);
    REQUIRE(val0.has_value());
    REQUIRE(val0->get() == "Hello");

    auto val1 = container.At(1);
    REQUIRE(val1.has_value());
    REQUIRE(val1->get() == "!!!!!");
  }

  SECTION("Access elements") {
    Container<int> container{10, 20};

    auto val0 = container.At(0);
    REQUIRE(val0.has_value());
    REQUIRE(val0->get() == 10);

    auto val1 = container.At(1);
    REQUIRE(val1.has_value());
    REQUIRE(val1->get() == 20);

    auto val2 = container.At(2);
    REQUIRE_FALSE(val2.has_value());
    REQUIRE_THROWS_AS(val2.value(), std::bad_expected_access<ContainerError>);
    REQUIRE(val2.error() == ContainerError::kOutOfBounds);
  }

  SECTION("Remove elements") {
    Container<int> container{1, 2, 3, 2, 4, 2};

    REQUIRE(container.GetSize() == 6);
    auto removed_count = container.Remove(2);
    REQUIRE(removed_count == 3);
    REQUIRE(container.GetSize() == 3);

    auto val0 = container.At(0);
    REQUIRE(val0.has_value());
    REQUIRE(val0->get() == 1);

    auto val1 = container.At(1);
    REQUIRE(val1.has_value());
    REQUIRE(val1->get() == 3);

    auto val2 = container.At(2);
    REQUIRE(val2.has_value());
    REQUIRE(val2->get() == 4);

    auto val3 = container.At(3);
    REQUIRE_FALSE(val3.has_value());
    REQUIRE_THROWS_AS(val3.value(), std::bad_expected_access<ContainerError>);
    REQUIRE(val3.error() == ContainerError::kOutOfBounds);
  }
}

TEST_CASE("Container views and ranges", "[containers][ranges]") {
  SECTION("Filtered view") {
    Container<int> container{1, 2, 3, 4, 5, 6};

    auto even_view = container.GetFilteredView([](int n) { return n % 2 == 0; });
    auto even_values = std::ranges::to<std::vector<int>>(even_view);

    REQUIRE(even_values.size() == 3);
    REQUIRE(even_values == std::vector<int>{2, 4, 6});
  }

  SECTION("Transformed view") {
    Container<int> container{1, 2, 3, 4, 5};

    auto squared_view = container.GetTransformedView([](int n) { return n * n; });
    auto squared_values = std::ranges::to<std::vector<int>>(squared_view);

    REQUIRE(squared_values.size() == 5);
    REQUIRE(squared_values == std::vector<int>{1, 4, 9, 16, 25});
  }

  SECTION("Iterator support") {
    Container<int> container{1, 2, 3};

    auto it = container.begin();
    REQUIRE(*it == 1);

    ++it;
    REQUIRE(*it == 2);

    ++it;
    REQUIRE(*it == 3);

    ++it;
    REQUIRE(it == container.end());

    --it;
    REQUIRE(*it == 3);
  }
}

}  // namespace
