#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "algorithms/stl.hpp"

namespace {

using namespace cpp_features::algorithms;

TEST_CASE("SortContainer basic operations", "[algorithms][sort]") {
  SECTION("Sort empty container") {
    std::vector<int> container;

    REQUIRE_NOTHROW(SortContainer(container));
    REQUIRE(container.empty());
  }

  SECTION("Sort single element") {
    std::vector<int> container{42};

    REQUIRE_NOTHROW(SortContainer(container));
    REQUIRE(container[0] == 42);
  }

  SECTION("Sort integers in ascending order") {
    std::vector<int> container{42, 17, 89, 3, 56, 23, 78, 12, 95, 34};

    REQUIRE_NOTHROW(SortContainer(container));
    std::vector<int> expected{3, 12, 17, 23, 34, 42, 56, 78, 89, 95};
    REQUIRE(container == expected);
  }

  SECTION("Sort strings lexicographically") {
    std::vector<std::string> container{"cherry", "banana", "elderberry", "date", "apple"};

    REQUIRE_NOTHROW(SortContainer(container));
    std::vector<std::string> expected{"apple", "banana", "cherry", "date", "elderberry"};
    REQUIRE(container == expected);
  }

  SECTION("Sort already sorted container") {
    std::vector<int> container{1, 2, 3, 4, 5};

    REQUIRE_NOTHROW(SortContainer(container));
    std::vector<int> expected{1, 2, 3, 4, 5};
    REQUIRE(container == expected);
  }

  SECTION("Sort reverse sorted container") {
    std::vector<int> container{5, 4, 3, 2, 1};

    REQUIRE_NOTHROW(SortContainer(container));
    std::vector<int> expected{1, 2, 3, 4, 5};
    REQUIRE(container == expected);
  }

  SECTION("Sort container with duplicates") {
    std::vector<int> container{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    REQUIRE_NOTHROW(SortContainer(container));
    std::vector<int> expected{1, 1, 2, 3, 3, 4, 5, 5, 6, 9};
    REQUIRE(container == expected);
  }
}

TEST_CASE("CountIf predicate operations", "[algorithms][count]") {
  SECTION("Count in empty container") {
    std::vector<int> container;

    auto count = CountIf(container, [](int n) { return n > 0; });
    REQUIRE(count == 0);
  }

  SECTION("Count even numbers") {
    std::vector<int> container{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto even_count = CountIf(container, [](int n) { return n % 2 == 0; });
    REQUIRE(even_count == 4);
  }

  SECTION("Count odd numbers") {
    std::vector<int> container{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto odd_count = CountIf(container, [](int n) { return n % 2 == 1; });
    REQUIRE(odd_count == 5);
  }

  SECTION("Count numbers greater than threshold") {
    std::vector<int> container{10, 25, 30, 45, 50, 75, 80, 95};

    auto large_count = CountIf(container, [](int n) { return n > 50; });
    REQUIRE(large_count == 3);
  }

  SECTION("Count with no matches") {
    std::vector<int> container{1, 3, 5, 7, 9};

    auto even_count = CountIf(container, [](int n) { return n % 2 == 0; });
    REQUIRE(even_count == 0);
  }

  SECTION("Count with all matches") {
    std::vector<int> container{2, 4, 6, 8, 10};

    auto even_count = CountIf(container, [](int n) { return n % 2 == 0; });
    REQUIRE(even_count == 5);
  }

  SECTION("Count strings by length") {
    std::vector<std::string> container{"a", "hello", "world", "test", "algorithm"};

    auto long_words = CountIf(container, [](const std::string &word) { return word.length() > 4; });
    REQUIRE(long_words == 3);
  }
}

TEST_CASE("TransformToVector operations", "[algorithms][transform]") {
  SECTION("Transform empty container") {
    std::vector<int> container;

    auto result = TransformToVector(container, [](int n) { return n * 2; });
    REQUIRE(result.empty());
  }

  SECTION("Square numbers") {
    std::vector<int> container{1, 2, 3, 4, 5};

    auto squares = TransformToVector(container, [](int n) { return n * n; });
    std::vector<int> expected{1, 4, 9, 16, 25};
    REQUIRE(squares == expected);
  }

  SECTION("Double numbers") {
    std::vector<int> container{10, 20, 30, 40, 50};

    auto doubled = TransformToVector(container, [](int n) { return n * 2; });
    std::vector<int> expected{20, 40, 60, 80, 100};
    REQUIRE(doubled == expected);
  }

  SECTION("Convert to string") {
    std::vector<int> container{1, 2, 3, 4, 5};

    auto strings = TransformToVector(container, [](int n) { return std::to_string(n); });
    std::vector<std::string> expected{"1", "2", "3", "4", "5"};
    REQUIRE(strings == expected);
  }

  SECTION("String to uppercase") {
    std::vector<std::string> container{"hello", "world", "test"};

    auto uppercased = TransformToVector(container, [](const std::string &word) {
      auto result = word;
      std::ranges::transform(result, result.begin(), ::toupper);
      return result;
    });
    std::vector<std::string> expected{"HELLO", "WORLD", "TEST"};
    REQUIRE(uppercased == expected);
  }
}

TEST_CASE("FindMinMax operations", "[algorithms][minmax]") {
  SECTION("Find min/max in integer container") {
    std::vector<int> container{3, 1, 4, 1, 5, 9, 2, 6};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == 1);
    REQUIRE(max_val == 9);
  }

  SECTION("Find min/max in single element") {
    std::vector<int> container{42};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == 42);
    REQUIRE(max_val == 42);
  }

  SECTION("Find min/max with all same elements") {
    std::vector<int> container{5, 5, 5, 5, 5};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == 5);
    REQUIRE(max_val == 5);
  }

  SECTION("Find min/max in negative numbers") {
    std::vector<int> container{-10, -5, -20, -1, -15};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == -20);
    REQUIRE(max_val == -1);
  }

  SECTION("Find min/max in mixed positive/negative") {
    std::vector<int> container{-5, 10, -15, 20, 0};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == -15);
    REQUIRE(max_val == 20);
  }

  SECTION("Find min/max in string container") {
    std::vector<std::string> container{"cherry", "banana", "elderberry", "date", "apple"};

    auto [min_word, max_word] = FindMinMax(container);
    REQUIRE(min_word == "apple");
    REQUIRE(max_word == "elderberry");
  }

  SECTION("Find min/max with string duplicates") {
    std::vector<std::string> container{"banana", "apple", "cherry", "apple", "banana"};

    auto [min_word, max_word] = FindMinMax(container);
    REQUIRE(min_word == "apple");
    REQUIRE(max_word == "cherry");
  }

  SECTION("Find min/max in sorted container") {
    std::vector<int> container{1, 2, 3, 4, 5};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == 1);
    REQUIRE(max_val == 5);
  }

  SECTION("Find min/max in reverse sorted container") {
    std::vector<int> container{5, 4, 3, 2, 1};

    auto [min_val, max_val] = FindMinMax(container);
    REQUIRE(min_val == 1);
    REQUIRE(max_val == 5);
  }
}

TEST_CASE("Algorithm combinations", "[algorithms][integration]") {
  SECTION("Transform then find min/max") {
    std::vector<int> container{-3, 2, -1, 4, -5};

    auto squares = TransformToVector(container, [](int n) { return n * n; });
    auto [min_val, max_val] = FindMinMax(squares);
    REQUIRE(min_val == 1);   // (-1)^2
    REQUIRE(max_val == 25);  // (-5)^2
  }

  SECTION("Full pipeline: sort, transform, count") {
    std::vector<int> container{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    REQUIRE_NOTHROW(SortContainer(container));
    auto squares = TransformToVector(container, [](int n) { return n * n; });
    auto large_squares = CountIf(squares, [](int n) { return n > 10; });
    REQUIRE(large_squares == 5);  // 16, 25, 25, 36, 81
  }
}

}  // namespace
