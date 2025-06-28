#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <list>
#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "concepts/concepts.hpp"

namespace {

using namespace cpp_features::concepts;

template <ArithmeticType T>
auto TestArithmetic(T a, T b) -> T {
  return a + b;
}

TEST_CASE("ArithmeticType concept", "[concepts][arithmetic]") {
  SECTION("Valid arithmetic types") {
    REQUIRE(ArithmeticType<int>);
    REQUIRE(ArithmeticType<float>);
    REQUIRE(ArithmeticType<double>);
    REQUIRE(ArithmeticType<char>);
    REQUIRE(ArithmeticType<std::int64_t>);
    REQUIRE(ArithmeticType<std::uint64_t>);
    REQUIRE(ArithmeticType<std::ptrdiff_t>);
    REQUIRE(ArithmeticType<std::size_t>);

    REQUIRE(TestArithmetic(5, 3) == 8);
    REQUIRE(TestArithmetic(2.5, 1.5) == 4.0);
  }

  SECTION("Invalid arithmetic types") {
    REQUIRE_FALSE(ArithmeticType<std::string>);
    REQUIRE_FALSE(ArithmeticType<std::vector<int>>);
    REQUIRE_FALSE(ArithmeticType<void *>);
    REQUIRE_FALSE(ArithmeticType<int *>);
    REQUIRE_FALSE(ArithmeticType<int &>);
  }
}

TEST_CASE("AddableType concept", "[concepts][addable]") {
  SECTION("Valid addable types") {
    REQUIRE(AddableType<int>);
    REQUIRE(AddableType<double>);
    REQUIRE(AddableType<std::string>);
  }

  SECTION("Invalid addable types") {
    REQUIRE_FALSE(AddableType<char>);
    REQUIRE_FALSE(AddableType<void *>);
    REQUIRE_FALSE(AddableType<int *>);
    REQUIRE_FALSE(AddableType<std::vector<int>>);
  }
}

TEST_CASE("NumericType concept", "[concepts][numeric]") {
  SECTION("Valid numeric types") {
    REQUIRE(NumericType<int>);
    REQUIRE(NumericType<double>);
  }

  SECTION("Invalid numeric types") {
    REQUIRE_FALSE(NumericType<std::string>);
    REQUIRE_FALSE(NumericType<std::vector<int>>);
  }
}

template <IterableContainer T>
auto TestIterableContainer(const T &container) -> std::size_t {
  std::size_t count = 0;
  for (const auto &_ : container) {
    ++count;
  }
  return count;
}

TEST_CASE("IterableContainer concept", "[concepts][container][iterable]") {
  SECTION("Valid iterable containers") {
    REQUIRE(IterableContainer<std::vector<int>>);
    REQUIRE(IterableContainer<std::array<int, 5>>);
    REQUIRE(IterableContainer<std::list<int>>);
    REQUIRE(IterableContainer<std::deque<int>>);
    REQUIRE(IterableContainer<std::set<int>>);
    REQUIRE(IterableContainer<std::map<int, std::string>>);
    REQUIRE(IterableContainer<std::string>);

    REQUIRE(TestIterableContainer(std::vector{1, 2, 3}) == 3);
    REQUIRE(TestIterableContainer(std::array{4, 5, 6}) == 3);
    REQUIRE(TestIterableContainer(std::list{7, 8, 9}) == 3);
    REQUIRE(TestIterableContainer(std::set{10, 11, 12}) == 3);
    REQUIRE(TestIterableContainer(std::map<int, std::string>{{13, "13"}, {14, "14"}}) == 2);
    REQUIRE(TestIterableContainer(std::string{"hello"}) == 5);
  }

  SECTION("Invalid iterable containers") {
    REQUIRE_FALSE(IterableContainer<int>);
    REQUIRE_FALSE(IterableContainer<double>);
    REQUIRE_FALSE(IterableContainer<void *>);
    REQUIRE_FALSE(IterableContainer<int *>);
  }
}

template <RangeContainer T>
auto TestRangeContainer(const T &container) -> bool {
  return !std::ranges::empty(container) || std::ranges::empty(container);
}

TEST_CASE("RangeContainer concept", "[concepts][container][range]") {
  SECTION("Valid range containers") {
    REQUIRE(RangeContainer<std::vector<int>>);
    REQUIRE(RangeContainer<std::array<int, 5>>);
    REQUIRE(RangeContainer<std::list<int>>);
    REQUIRE(RangeContainer<std::deque<int>>);
    REQUIRE(RangeContainer<std::set<int>>);
    REQUIRE(RangeContainer<std::map<int, std::string>>);
    REQUIRE(RangeContainer<std::string>);
    REQUIRE(RangeContainer<std::string_view>);

    REQUIRE(TestRangeContainer(std::vector<int>{1, 2, 3}));
    REQUIRE(TestRangeContainer(std::array<int, 0>{}));
    REQUIRE(TestRangeContainer(std::set<int>{}));
    REQUIRE(TestRangeContainer(std::string{"hello"}));
  }

  SECTION("Invalid range containers") {
    REQUIRE_FALSE(RangeContainer<int>);
    REQUIRE_FALSE(RangeContainer<double>);
    REQUIRE_FALSE(RangeContainer<void *>);
    REQUIRE_FALSE(RangeContainer<int *>);
  }
}

template <StringLikeType T>
auto TestStringLike(const T &value) -> std::string_view {
  return std::string_view(value);
}

TEST_CASE("StringLikeType concept", "[concepts][string]") {
  SECTION("Valid string-like types") {
    REQUIRE(StringLikeType<std::string>);
    REQUIRE(StringLikeType<std::string_view>);
    REQUIRE(StringLikeType<const char *>);
    REQUIRE(StringLikeType<char *>);

    std::string str = "hello";
    REQUIRE(TestStringLike(str) == "hello");
    std::string_view sv = "world";
    REQUIRE(TestStringLike(sv) == "world");
    const char *cstr = "test";
    REQUIRE(TestStringLike(cstr) == "test");
  }

  SECTION("Invalid string-like types") {
    REQUIRE_FALSE(StringLikeType<int>);
    REQUIRE_FALSE(StringLikeType<double>);
    REQUIRE_FALSE(StringLikeType<void *>);
    REQUIRE_FALSE(StringLikeType<std::string *>);
    REQUIRE_FALSE(StringLikeType<std::vector<char>>);
    REQUIRE_FALSE(StringLikeType<std::vector<std::byte>>);
  }
}

template <PrintableType T>
auto TestPrintable([[maybe_unused]] const T &value) -> bool {
  return true;
}

TEST_CASE("PrintableType concept", "[concepts][printable]") {
  SECTION("Valid printable types") {
    REQUIRE(PrintableType<int>);
    REQUIRE(PrintableType<double>);
    REQUIRE(PrintableType<char>);
    REQUIRE(PrintableType<bool>);
    REQUIRE(PrintableType<std::string>);
    REQUIRE(PrintableType<std::string_view>);
    REQUIRE(PrintableType<const char *>);

    REQUIRE(TestPrintable(42));
    REQUIRE(TestPrintable(3.14));
    REQUIRE(TestPrintable('a'));
    REQUIRE(TestPrintable(true));
    REQUIRE(TestPrintable(std::string{"hello"}));
    REQUIRE(TestPrintable(std::string_view{"world"}));
    REQUIRE(TestPrintable("test"));
  }

  SECTION("Invalid printable types") {
    REQUIRE_FALSE(PrintableType<std::vector<int>>);
    struct NonPrintable {};
    REQUIRE_FALSE(PrintableType<NonPrintable>);
  }
}

template <ComparableType T>
auto TestComparable(const T &a, const T &b) -> bool {
  return a < b;
}

TEST_CASE("ComparableType concept", "[concepts][comparable]") {
  SECTION("Valid comparable types") {
    REQUIRE(ComparableType<int>);
    REQUIRE(ComparableType<double>);
    REQUIRE(ComparableType<std::string>);
    REQUIRE(ComparableType<std::string_view>);
    REQUIRE(ComparableType<std::vector<int>>);

    REQUIRE(TestComparable(1, 2));
    REQUIRE(TestComparable(1.0, 2.0));
    REQUIRE(TestComparable(std::string{"a"}, std::string{"b"}));
    REQUIRE(TestComparable(std::string_view{"a"}, std::string_view{"b"}));
    REQUIRE(TestComparable(std::vector<int>{1, 2, 3}, std::vector<int>{4, 5, 6}));
  }

  SECTION("Invalid comparable types") {
    struct NonComparable {};
    REQUIRE_FALSE(ComparableType<NonComparable>);
    REQUIRE_FALSE(ComparableType<std::vector<NonComparable>>);
  }
}

TEST_CASE("Type trait concepts", "[concepts][traits]") {
  SECTION("DefaultConstructibleType") {
    REQUIRE(DefaultConstructibleType<int>);
    REQUIRE(DefaultConstructibleType<std::string>);
    REQUIRE(DefaultConstructibleType<std::vector<int>>);
    REQUIRE(DefaultConstructibleType<std::unique_ptr<int>>);
    REQUIRE(DefaultConstructibleType<std::shared_ptr<int>>);
  }

  SECTION("NonDefaultConstructibleType") {
    struct NonDefaultConstructible {
      NonDefaultConstructible() = delete;
    };
    REQUIRE_FALSE(DefaultConstructibleType<NonDefaultConstructible>);
  }

  SECTION("CopyableType") {
    REQUIRE(CopyableType<int>);
    REQUIRE(CopyableType<std::string>);
    REQUIRE(CopyableType<std::vector<int>>);
    REQUIRE(CopyableType<std::shared_ptr<int>>);
  }

  SECTION("NonCopyableType") {
    REQUIRE_FALSE(CopyableType<std::unique_ptr<int>>);
    struct NonCopyable {
      NonCopyable(const NonCopyable &) = delete;
      auto operator=(const NonCopyable &) -> NonCopyable & = delete;
    };
    REQUIRE_FALSE(CopyableType<NonCopyable>);
  }

  SECTION("MovableType") {
    REQUIRE(MovableType<int>);
    REQUIRE(MovableType<std::string>);
    REQUIRE(MovableType<std::vector<int>>);
    REQUIRE(MovableType<std::unique_ptr<int>>);
    REQUIRE(MovableType<std::shared_ptr<int>>);
  }

  SECTION("NonMovableType") {
    struct NonMovable {
      NonMovable(NonMovable &&) = delete;
      auto operator=(NonMovable &&) -> NonMovable & = delete;
    };
    REQUIRE_FALSE(MovableType<NonMovable>);
  }
}

}  // namespace
