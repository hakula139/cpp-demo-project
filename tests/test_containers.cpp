#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <string>
#include <vector>

#include "containers/container.hpp"

using namespace cpp_features::containers;

TEST_CASE("Container basic operations", "[containers][basic]") {
    SECTION("Empty container") {
        Container<int> container;

        REQUIRE(container.IsEmpty());
        REQUIRE(container.GetSize() == 0);
    }

    SECTION("Adding elements") {
        Container<std::string> container;

        container.Add("Hello");
        container.Add("World");

        REQUIRE_FALSE(container.IsEmpty());
        REQUIRE(container.GetSize() == 2);
    }

    SECTION("Initializer list construction") {
        Container<int> container{1, 2, 3, 4, 5};

        REQUIRE(container.GetSize() == 5);
        REQUIRE_FALSE(container.IsEmpty());
    }

    SECTION("Element access") {
        Container<int> container{10, 20, 30};

        auto result1 = container.At(0);
        REQUIRE(result1.has_value());
        REQUIRE(**result1 == 10);

        auto result2 = container.At(1);
        REQUIRE(result2.has_value());
        REQUIRE(**result2 == 20);

        auto result3 = container.At(10);  // Out of bounds
        REQUIRE_FALSE(result3.has_value());
        REQUIRE(result3.error() == ContainerError::kOutOfBounds);
    }
}

TEST_CASE("Container advanced operations", "[containers][advanced]") {
    SECTION("Remove elements") {
        Container<int> container{1, 2, 3, 2, 4, 2};

        auto removed_count = container.Remove(2);
        REQUIRE(removed_count == 3);
        REQUIRE(container.GetSize() == 3);

        auto val1 = container.At(0);
        auto val2 = container.At(1);
        auto val3 = container.At(2);

        REQUIRE(val1.has_value());
        REQUIRE(val2.has_value());
        REQUIRE(val3.has_value());

        REQUIRE(**val1 == 1);
        REQUIRE(**val2 == 3);
        REQUIRE(**val3 == 4);
    }

    SECTION("Emplace elements") {
        Container<std::string> container;

        container.Emplace("Hello");
        container.Emplace("World", 5, '!');  // std::string constructor

        REQUIRE(container.GetSize() == 2);

        auto val1 = container.At(0);
        auto val2 = container.At(1);

        REQUIRE(val1.has_value());
        REQUIRE(val2.has_value());
        REQUIRE(**val1 == "Hello");
        REQUIRE(**val2 == "!!!!!");
    }
}

TEST_CASE("Container views and ranges", "[containers][ranges]") {
    SECTION("Filtered view") {
        Container<int> container{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        auto even_view = container.GetFilteredView([](int n) { return n % 2 == 0; });

        std::vector<int> even_values;
        for (const auto& val : even_view) {
            even_values.push_back(val);
        }

        REQUIRE(even_values.size() == 5);
        REQUIRE(even_values == std::vector<int>{2, 4, 6, 8, 10});
    }

    SECTION("Transformed view") {
        Container<int> container{1, 2, 3, 4, 5};

        auto squared_view = container.GetTransformedView([](int n) { return n * n; });

        std::vector<int> squared_values;
        for (const auto& val : squared_view) {
            squared_values.push_back(val);
        }

        REQUIRE(squared_values.size() == 5);
        REQUIRE(squared_values == std::vector<int>{1, 4, 9, 16, 25});
    }

    SECTION("Iterator support") {
        Container<int> container{10, 20, 30};

        std::vector<int> values;
        for (auto it = container.begin(); it != container.end(); ++it) {
            values.push_back(*it);
        }

        REQUIRE(values == std::vector<int>{10, 20, 30});

        std::vector<int> range_values;
        for (const auto& val : container) {
            range_values.push_back(val);
        }

        REQUIRE(range_values == std::vector<int>{10, 20, 30});
    }
}

TEMPLATE_TEST_CASE("Container with different types", "[containers][template]",
                   int, double, std::string) {
    Container<TestType> container;

    REQUIRE(container.IsEmpty());
    REQUIRE(container.GetSize() == 0);

    if constexpr (std::is_same_v<TestType, int>) {
        container.Add(42);
        auto val = container.At(0);
        REQUIRE(val.has_value());
        REQUIRE(**val == 42);
    } else if constexpr (std::is_same_v<TestType, double>) {
        container.Add(3.14);
        auto val = container.At(0);
        REQUIRE(val.has_value());
        REQUIRE(**val == 3.14);
    } else if constexpr (std::is_same_v<TestType, std::string>) {
        container.Add("test");
        auto val = container.At(0);
        REQUIRE(val.has_value());
        REQUIRE(**val == "test");
    }

    REQUIRE(container.GetSize() == 1);
}

TEST_CASE("Container factory functions", "[containers][factory]") {
    SECTION("CreateContainer empty") {
        auto container = CreateContainer<int>();
        REQUIRE(container.IsEmpty());
        REQUIRE(container.GetSize() == 0);
    }

    SECTION("CreateContainer with initializer list") {
        auto container = CreateContainer<double>({1.1, 2.2, 3.3});
        REQUIRE(container.GetSize() == 3);

        auto val = container.At(1);
        REQUIRE(val.has_value());
        REQUIRE(**val == 2.2);
    }
}