#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "concepts/concepts.hpp"

using namespace cpp_features::concepts;

template <ArithmeticType T>
auto TestArithmetic(T a, T b) -> T {
  return a + b;
}

template <PrintableType T>
auto TestPrintable([[maybe_unused]] const T& value) -> bool {
  return true;
}

template <ComparableType T>
auto TestComparable(const T& a, const T& b) -> bool {
  return a < b;
}

TEST_CASE("ArithmeticType concept", "[concepts][arithmetic]") {
  SECTION("Valid arithmetic types") {
    REQUIRE(ArithmeticType<int>);
    REQUIRE(ArithmeticType<float>);
    REQUIRE(ArithmeticType<double>);
    REQUIRE(ArithmeticType<long>);
    REQUIRE(ArithmeticType<short>);
    REQUIRE(ArithmeticType<unsigned int>);

    REQUIRE(TestArithmetic(5, 3) == 8);
    REQUIRE(TestArithmetic(2.5, 1.5) == 4.0);
  }

  SECTION("Invalid arithmetic types") {
    REQUIRE_FALSE(ArithmeticType<std::string>);
    REQUIRE_FALSE(ArithmeticType<std::vector<int>>);
    REQUIRE_FALSE(ArithmeticType<void*>);
  }
}

TEST_CASE("PrintableType concept", "[concepts][printable]") {
  SECTION("Valid printable types") {
    REQUIRE(PrintableType<int>);
    REQUIRE(PrintableType<double>);
    REQUIRE(PrintableType<std::string>);
    REQUIRE(PrintableType<const char*>);

    REQUIRE(TestPrintable(42));
    REQUIRE(TestPrintable(3.14));
    REQUIRE(TestPrintable(std::string("hello")));
  }
}

TEST_CASE("ComparableType concept", "[concepts][comparable]") {
  SECTION("Valid comparable types") {
    REQUIRE(ComparableType<int>);
    REQUIRE(ComparableType<double>);
    REQUIRE(ComparableType<std::string>);

    REQUIRE(TestComparable(1, 2));
    REQUIRE(TestComparable(1.0, 2.0));
    REQUIRE(TestComparable(std::string("a"), std::string("b")));
  }
}

TEST_CASE("AddableType concept", "[concepts][addable]") {
  SECTION("Valid addable types") {
    REQUIRE(AddableType<int>);
    REQUIRE(AddableType<double>);
    REQUIRE(AddableType<std::string>);

    static_assert(AddableType<int>);
    static_assert(AddableType<double>);
  }
}

TEST_CASE("NumericType concept", "[concepts][numeric]") {
  SECTION("Valid numeric types") {
    REQUIRE(NumericType<int>);
    REQUIRE(NumericType<float>);
    REQUIRE(NumericType<double>);

    static_assert(NumericType<int>);
    static_assert(NumericType<double>);
  }

  SECTION("Invalid numeric types") {
    REQUIRE_FALSE(NumericType<std::string>);
    REQUIRE_FALSE(NumericType<std::vector<int>>);
  }
}

TEST_CASE("Type trait concepts", "[concepts][traits]") {
  SECTION("DefaultConstructibleType") {
    REQUIRE(DefaultConstructibleType<int>);
    REQUIRE(DefaultConstructibleType<std::string>);
    REQUIRE(DefaultConstructibleType<std::vector<int>>);
  }

  SECTION("CopyableType") {
    REQUIRE(CopyableType<int>);
    REQUIRE(CopyableType<std::string>);
    REQUIRE(CopyableType<std::vector<int>>);
  }

  SECTION("MovableType") {
    REQUIRE(MovableType<int>);
    REQUIRE(MovableType<std::string>);
    REQUIRE(MovableType<std::vector<int>>);
    REQUIRE(MovableType<std::unique_ptr<int>>);
  }
}

TEMPLATE_TEST_CASE("Concept template tests", "[concepts][template]", int, float, double) {
  SECTION("ArithmeticType with different types") {
    REQUIRE(ArithmeticType<TestType>);
    REQUIRE(NumericType<TestType>);
    REQUIRE(AddableType<TestType>);
    REQUIRE(SubtractableType<TestType>);
    REQUIRE(ComparableType<TestType>);
  }
}
