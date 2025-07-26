#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

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

template <NullaryCallable T>
auto TestNullaryCallable(const T &callable) -> bool {
  callable();
  return true;
}

TEST_CASE("NullaryCallable concept", "[concepts][callable]") {
  SECTION("Valid nullary callables") {
    REQUIRE(NullaryCallable<void (*)()>);
    REQUIRE(NullaryCallable<decltype([]() {})>);
    REQUIRE(NullaryCallable<decltype([]() { return 42; })>);
    REQUIRE(NullaryCallable<std::function<void()>>);
    REQUIRE(NullaryCallable<std::function<int()>>);

    auto void_lambda = []() {};
    auto int_lambda = []() { return 42; };
    std::function<void()> void_func = []() {};
    std::function<int()> int_func = []() { return 42; };

    REQUIRE(TestNullaryCallable(void_lambda));
    REQUIRE(TestNullaryCallable(int_lambda));
    REQUIRE(TestNullaryCallable(void_func));
    REQUIRE(TestNullaryCallable(int_func));
  }

  SECTION("Invalid nullary callables") {
    // Requires parameters
    REQUIRE_FALSE(NullaryCallable<void (*)(int)>);
    REQUIRE_FALSE(NullaryCallable<decltype([](int x) { return x; })>);
    REQUIRE_FALSE(NullaryCallable<std::function<void(int)>>);
    REQUIRE_FALSE(NullaryCallable<std::function<int(int)>>);

    // Not callable
    REQUIRE_FALSE(NullaryCallable<int>);
    REQUIRE_FALSE(NullaryCallable<std::string>);
    REQUIRE_FALSE(NullaryCallable<void *>);
  }
}

template <VoidNullaryCallable T>
auto TestVoidNullaryCallable(const T &callable) -> bool {
  callable();
  return true;
}

TEST_CASE("VoidNullaryCallable concept", "[concepts][callable]") {
  SECTION("Valid void nullary callables") {
    REQUIRE(VoidNullaryCallable<void (*)()>);
    REQUIRE(VoidNullaryCallable<decltype([]() {})>);
    REQUIRE(VoidNullaryCallable<std::function<void()>>);

    auto void_lambda = []() {};
    std::function<void()> void_func = []() {};

    REQUIRE(TestVoidNullaryCallable(void_lambda));
    REQUIRE(TestVoidNullaryCallable(void_func));
  }

  SECTION("Invalid void nullary callables") {
    // Non-void return types
    REQUIRE_FALSE(VoidNullaryCallable<decltype([]() { return 42; })>);
    REQUIRE_FALSE(VoidNullaryCallable<std::function<int()>>);
    REQUIRE_FALSE(VoidNullaryCallable<int (*)()>);

    // Requires parameters
    REQUIRE_FALSE(VoidNullaryCallable<void (*)(int)>);
    REQUIRE_FALSE(VoidNullaryCallable<decltype([](int) {})>);
    REQUIRE_FALSE(VoidNullaryCallable<std::function<void(int)>>);

    // Not callable
    REQUIRE_FALSE(VoidNullaryCallable<int>);
    REQUIRE_FALSE(VoidNullaryCallable<std::string>);
    REQUIRE_FALSE(VoidNullaryCallable<void *>);
  }
}

template <TimerCallback T>
auto TestTimerCallback(const T &callback) -> bool {
  callback(1000000);
  return true;
}

TEST_CASE("TimerCallback concept", "[concepts][callable][timer]") {
  SECTION("Valid timer callbacks") {
    REQUIRE(TimerCallback<void (*)(std::int64_t)>);
    REQUIRE(TimerCallback<decltype([](std::int64_t) {})>);
    REQUIRE(TimerCallback<std::function<void(std::int64_t)>>);

    auto lambda_callback = [](std::int64_t ns) {
      // Do something with the time
      static_cast<void>(ns);
    };

    std::function<void(std::int64_t)> func_callback = [](std::int64_t ns) {
      // Do something with the time
      static_cast<void>(ns);
    };

    REQUIRE(TestTimerCallback(lambda_callback));
    REQUIRE(TestTimerCallback(func_callback));
  }

  SECTION("Invalid timer callbacks") {
    // Wrong return type
    REQUIRE_FALSE(TimerCallback<int (*)(std::int64_t)>);
    REQUIRE_FALSE(TimerCallback<decltype([](std::int64_t) { return 42; })>);
    REQUIRE_FALSE(TimerCallback<std::function<int(std::int64_t)>>);

    // Wrong number of parameters
    REQUIRE_FALSE(TimerCallback<void (*)()>);
    REQUIRE_FALSE(TimerCallback<void (*)(std::int64_t, int)>);
    REQUIRE_FALSE(TimerCallback<decltype([]() {})>);
    REQUIRE_FALSE(TimerCallback<decltype([](std::int64_t, int) {})>);

    // Not callable
    REQUIRE_FALSE(TimerCallback<int>);
    REQUIRE_FALSE(TimerCallback<std::string>);
    REQUIRE_FALSE(TimerCallback<void *>);
  }
}

template <typename T, PredicateFor<T> Predicate>
auto TestPredicateFor(const T &value, Predicate predicate) -> bool {
  return predicate(value);
}

TEST_CASE("PredicateFor concept", "[concepts][callable][predicate]") {
  SECTION("Valid predicates") {
    REQUIRE(PredicateFor<decltype([](const int &n) { return n % 2 == 0; }), int>);
    REQUIRE(PredicateFor<decltype([](const int &n) { return n; }), int>);
    REQUIRE(PredicateFor<decltype([](int n) { return n > 0; }), int>);
    REQUIRE(PredicateFor<std::function<bool(const int &)>, int>);
    REQUIRE(PredicateFor<std::function<bool(const std::string &)>, std::string>);

    auto even_predicate = [](const int &n) { return n % 2 == 0; };
    auto string_predicate = [](const std::string &s) { return !s.empty(); };

    REQUIRE(TestPredicateFor(4, even_predicate));
    REQUIRE_FALSE(TestPredicateFor(3, even_predicate));
    REQUIRE(TestPredicateFor(std::string{"hello"}, string_predicate));
  }

  SECTION("Invalid predicates") {
    // Wrong parameter type
    REQUIRE_FALSE(PredicateFor<decltype([](const std::string &) { return true; }), int>);
    REQUIRE_FALSE(PredicateFor<decltype([](const int &) { return true; }), std::string>);

    // Wrong return type
    REQUIRE_FALSE(PredicateFor<decltype([](const int &) { return std::string{""}; }), int>);
    REQUIRE_FALSE(PredicateFor<decltype([](const int &) -> void {}), int>);

    // Wrong arity
    REQUIRE_FALSE(PredicateFor<decltype([]() { return true; }), int>);
    REQUIRE_FALSE(PredicateFor<decltype([](const int &, const int &) { return true; }), int>);

    // Not callable
    REQUIRE_FALSE(PredicateFor<int, int>);
    REQUIRE_FALSE(PredicateFor<std::string, int>);
    REQUIRE_FALSE(PredicateFor<void *, int>);
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

template <SortableContainer T>
auto TestSortableContainer(T container) -> bool {
  std::ranges::sort(container);
  return std::ranges::is_sorted(container);
}

TEST_CASE("SortableContainer concept", "[concepts][container][sortable]") {
  SECTION("Valid sortable containers") {
    // Random access containers that support sorting
    REQUIRE(SortableContainer<std::vector<int>>);
    REQUIRE(SortableContainer<std::array<int, 5>>);
    REQUIRE(SortableContainer<std::deque<int>>);
    REQUIRE(SortableContainer<std::string>);

    REQUIRE(TestSortableContainer(std::vector<int>{3, 1, 4, 1, 5}));
    REQUIRE(TestSortableContainer(std::array<int, 5>{9, 2, 6, 5, 3}));
    REQUIRE(TestSortableContainer(std::deque<int>{8, 7, 6, 5, 4}));
    REQUIRE(TestSortableContainer(std::string{"dcba"}));
  }

  SECTION("Invalid sortable containers") {
    // Non-random access containers
    REQUIRE_FALSE(SortableContainer<std::list<int>>);
    REQUIRE_FALSE(SortableContainer<std::set<int>>);
    REQUIRE_FALSE(SortableContainer<std::map<int, std::string>>);

    // Non-container types
    REQUIRE_FALSE(SortableContainer<int>);
    REQUIRE_FALSE(SortableContainer<double>);
    REQUIRE_FALSE(SortableContainer<void *>);
    REQUIRE_FALSE(SortableContainer<int *>);

    // Not sortable in-place
    REQUIRE_FALSE(SortableContainer<std::string_view>);
  }

  SECTION("Edge cases") {
    // Empty containers should still be sortable
    REQUIRE(TestSortableContainer(std::vector<int>{}));
    REQUIRE(TestSortableContainer(std::array<int, 0>{}));
    REQUIRE(TestSortableContainer(std::string{}));

    // Single element containers
    REQUIRE(TestSortableContainer(std::vector<int>{42}));
    REQUIRE(TestSortableContainer(std::string{"a"}));
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

  SECTION("DestructibleType") {
    REQUIRE(DestructibleType<int>);
    REQUIRE(DestructibleType<double>);
    REQUIRE(DestructibleType<std::string>);
    REQUIRE(DestructibleType<std::vector<int>>);
    REQUIRE(DestructibleType<std::unique_ptr<int>>);
    REQUIRE(DestructibleType<std::shared_ptr<int>>);

    struct DestructibleClass {
      ~DestructibleClass() = default;
    };
    REQUIRE(DestructibleType<DestructibleClass>);

    struct VirtualDestructor {
      virtual ~VirtualDestructor() = default;
    };
    REQUIRE(DestructibleType<VirtualDestructor>);
  }

  SECTION("NonDestructibleType") {
    struct NonDestructible {
      ~NonDestructible() = delete;
    };
    REQUIRE_FALSE(DestructibleType<NonDestructible>);

    struct PrivateDestructor {
     private:
      ~PrivateDestructor() = default;
    };
    REQUIRE_FALSE(DestructibleType<PrivateDestructor>);
  }
}

}  // namespace
