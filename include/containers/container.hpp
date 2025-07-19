/**
 * @file container.hpp
 * @brief Modern C++ container implementation with concepts and ranges support
 *
 * This file contains a modern C++ container implementation that demonstrates
 * the use of concepts, ranges, and modern C++ features. The Container class
 * provides a wrapper around std::vector with additional functionality.
 */

#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <format>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "concepts/concepts.hpp"

namespace cpp_features::containers {

/**
 * @brief Error codes for container operations
 *
 * Enumeration defining possible error conditions that can occur during container operations.
 */
enum class ContainerError : std::uint8_t {
  kEmpty = 1,            ///< Container is empty when operation requires elements
  kOutOfBounds = 2,      ///< Index is out of valid range
  kInvalidOperation = 3  ///< Operation is not valid in current state
};

/**
 * @brief A modern C++ container wrapper with enhanced functionality
 *
 * @tparam T The type of elements stored in the container. Must satisfy CopyableType concept.
 *
 * This class provides a modern wrapper around std::vector with additional features:
 *
 * - Concept-based type safety
 * - Range and view support
 * - Error handling with std::expected
 * - Modern C++ iterator interface
 * - Custom formatting support
 *
 * The container satisfies both IterableContainer and RangeContainer concepts,
 * making it compatible with STL algorithms and ranges.
 *
 * @code
 * Container<int> numbers{1, 2, 3, 4, 5};
 * numbers.Add(6);
 * @endcode
 */
template <concepts::CopyableType T>
class Container {
 public:
  using value_type = T;                                ///< Type of elements stored
  using size_type = std::size_t;                       ///< Type used for sizes and indices
  using iterator = typename std::vector<T>::iterator;  ///< Iterator type
  using const_iterator = typename std::vector<T>::const_iterator;  ///< Const iterator type

  /**
   * @brief Default constructor
   *
   * Creates an empty container with no elements.
   */
  Container() = default;

  /**
   * @brief Construct container with specified initial size
   *
   * @param initial_size The number of default-constructed elements to create
   *
   * Creates a container with the specified number of default-constructed elements.
   */
  explicit Container(size_type initial_size) : data_(initial_size) {}

  /**
   * @brief Construct container from initializer list
   *
   * @param init Initializer list containing elements to copy into the container
   *
   * Creates a container and initializes it with the elements from the
   * provided initializer list.
   *
   * @code
   * Container<int> numbers{1, 2, 3, 4, 5};
   * @endcode
   */
  Container(std::initializer_list<T> init) : data_(init) {}

  /**
   * @brief Construct container from any input range
   *
   * @tparam Range Type of the input range
   * @param range Input range to copy elements from
   *
   * Creates a container and initializes it with elements from the provided range.
   * The range element type must be convertible to T.
   *
   * @code
   * std::vector<int> source{1, 2, 3};
   * Container<int> container(source);
   * @endcode
   */
  template <std::ranges::input_range Range>
    requires std::convertible_to<std::ranges::range_value_t<Range>, T> &&
             (!std::same_as<std::decay_t<Range>, std::initializer_list<T>>) &&
             (!std::convertible_to<Range, T>)
  explicit Container(Range &&range) : data_(std::ranges::begin(range), std::ranges::end(range)) {}

  /**
   * @brief Add an element to the container (copy)
   *
   * @param item The element to add (will be copied)
   *
   * Adds a copy of the specified element to the end of the container.
   */
  void Add(const T &item) { data_.push_back(item); }

  /**
   * @brief Add an element to the container (move)
   *
   * @param item The element to add (will be moved)
   *
   * Adds the specified element to the end of the container using move semantics.
   */
  void Add(T &&item) { data_.push_back(std::move(item)); }

  /**
   * @brief Construct element in-place at the end of the container
   *
   * @tparam Args Types of arguments to forward to the element constructor
   * @param args Arguments to forward to the element constructor
   *
   * Constructs a new element directly in the container using the provided arguments.
   * This avoids unnecessary copy or move operations.
   *
   * @code
   * Container<std::string> words;
   * words.Emplace(5, '!');  // Creates "!!!!!"
   * @endcode
   */
  template <typename... Args>
  void Emplace(Args &&...args) {
    data_.emplace_back(std::forward<Args>(args)...);
  }

  /**
   * @brief Remove all occurrences of a specific item
   *
   * @param item The item to remove from the container
   * @return The number of elements that were removed
   *
   * Removes all elements that compare equal to the specified item.
   * The container size is reduced by the number of removed elements.
   *
   * @code
   * Container<int> numbers{1, 2, 3, 2, 4, 2};
   * auto removed_count = numbers.Remove(2);  // removed_count == 3
   * @endcode
   */
  auto Remove(const T &item) -> size_type {
    auto ret = std::ranges::remove(data_, item);
    auto count = static_cast<size_type>(std::distance(ret.begin(), ret.end()));
    data_.erase(ret.begin(), ret.end());
    return count;
  }

  /**
   * @brief Get the number of elements in the container
   *
   * @return The number of elements currently stored in the container
   */
  [[nodiscard]] auto GetSize() const noexcept -> size_type { return data_.size(); }

  /**
   * @brief Get the number of elements in the container (STL-compatible)
   *
   * @return The number of elements currently stored in the container
   *
   * This method provides STL-compatible naming for concept compliance.
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto size() const noexcept -> size_type { return data_.size(); }

  /**
   * @brief Check if the container is empty
   *
   * @return true if the container contains no elements, false otherwise
   */
  [[nodiscard]] auto IsEmpty() const noexcept -> bool { return data_.empty(); }

  /**
   * @brief Access element at specified index (const version)
   *
   * @param index The index of the element to access
   * @return Expected containing reference wrapper to the element, or ContainerError on failure
   *
   * Provides safe access to elements with bounds checking.
   * Returns an std::expected that contains either a reference wrapper to the requested element,
   * or an error code if the index is out of bounds.
   *
   * @code
   * Container<int> numbers{1, 2, 3};
   * auto result = numbers.At(1);
   * if (result) {
   *   std::print("Element: {}", result->get());  // Prints: Element: 2
   * }
   * @endcode
   */
  [[nodiscard]] auto At(size_type index) const
      -> std::expected<std::reference_wrapper<const T>, ContainerError> {
    if (index >= data_.size()) {
      return std::unexpected(ContainerError::kOutOfBounds);
    }
    return std::cref(data_[index]);
  }

  /**
   * @brief Access element at specified index (non-const version)
   *
   * @param index The index of the element to access
   * @return Expected containing reference wrapper to the element, or ContainerError on failure
   *
   * Provides safe access to elements with bounds checking.
   * Returns an std::expected that contains either a reference wrapper to the requested element,
   * or an error code if the index is out of bounds.
   */
  [[nodiscard]] auto At(size_type index)
      -> std::expected<std::reference_wrapper<T>, ContainerError> {
    if (index >= data_.size()) {
      return std::unexpected(ContainerError::kOutOfBounds);
    }
    return std::ref(data_[index]);
  }

  /**
   * @brief Get a view of all elements
   *
   * @return A ranges view of all elements in the container
   *
   * Returns a view that can be used with ranges algorithms and operations.
   */
  [[nodiscard]] auto GetView() const { return std::views::all(data_); }

  /**
   * @brief Get a filtered view of elements
   *
   * @param predicate Function that returns true for elements to include
   * @return A ranges view containing only elements that satisfy the predicate
   *
   * Returns a lazy-evaluated view that contains only elements for which the predicate returns true.
   *
   * @code
   * auto even_numbers = container.GetFilteredView([](int n) { return n % 2 == 0; });
   * @endcode
   */
  [[nodiscard]] auto GetFilteredView(std::function<bool(const T &)> predicate) const
      -> std::ranges::view auto {
    return data_ | std::views::filter(predicate);
  }

  /**
   * @brief Get a transformed view of elements
   *
   * @tparam Func Type of the transformation function
   * @param transform_func Function to apply to each element
   * @return A ranges view containing transformed elements
   *
   * Returns a lazy-evaluated view where each element is transformed by the provided function.
   *
   * @code
   * auto doubled = container.GetTransformedView([](int n) { return n * 2; });
   * @endcode
   */
  template <typename Func>
  [[nodiscard]] auto GetTransformedView(Func transform_func) const {
    return data_ | std::views::transform(transform_func);
  }

  /**
   * @brief Get iterator to the beginning
   * @return Iterator pointing to the first element
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto begin() noexcept -> iterator { return data_.begin(); }

  /**
   * @brief Get iterator to the end
   * @return Iterator pointing past the last element
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto end() noexcept -> iterator { return data_.end(); }

  /**
   * @brief Get const iterator to the beginning
   * @return Const iterator pointing to the first element
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto begin() const noexcept -> const_iterator { return data_.begin(); }

  /**
   * @brief Get const iterator to the end
   * @return Const iterator pointing past the last element
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto end() const noexcept -> const_iterator { return data_.end(); }

  /**
   * @brief Get const iterator to the beginning
   * @return Const iterator pointing to the first element
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto cbegin() const noexcept -> const_iterator { return data_.cbegin(); }

  /**
   * @brief Get const iterator to the end
   * @return Const iterator pointing past the last element
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] auto cend() const noexcept -> const_iterator { return data_.cend(); }

  /**
   * @brief Three-way comparison operator
   * @param other The other container to compare with
   * @return Comparison result
   */
  [[nodiscard]] auto operator<=>(const Container &other) const = default;

 private:
  std::vector<T> data_;  ///< Internal storage for container elements
};

static_assert(cpp_features::concepts::IterableContainer<Container<int>>);
static_assert(cpp_features::concepts::RangeContainer<Container<int>>);

}  // namespace cpp_features::containers

// Class Template Argument Deduction (CTAD) guides for Container
namespace cpp_features::containers {

/**
 * @brief CTAD guide for initializer_list constructor
 *
 * Allows automatic type deduction when constructing from initializer list.
 *
 * @code
 * Container numbers{1, 2, 3, 4, 5};  // Deduces Container<int>
 * @endcode
 */
template <typename T>
Container(std::initializer_list<T>) -> Container<T>;

/**
 * @brief CTAD guide for range constructor
 *
 * Allows automatic type deduction when constructing from ranges.
 *
 * @code
 * std::vector<int> source{1, 2, 3};
 * Container container(source);  // Deduces Container<int>
 * @endcode
 */
template <std::ranges::input_range Range>
Container(Range) -> Container<std::ranges::range_value_t<Range>>;

}  // namespace cpp_features::containers

/**
 * @brief Custom formatter for Container<T> to work with std::format and std::print
 *
 * @tparam T Type of elements in the container
 *
 * Provides formatting support for Container objects, allowing them to be used directly with
 * std::format, std::print, and related formatting functions.
 *
 * The container is formatted as a comma-separated list enclosed in square brackets.
 *
 * @code
 * Container<int> numbers{1, 2, 3};
 * std::print("{}", numbers); // Prints: [1, 2, 3]
 * @endcode
 */
template <cpp_features::concepts::CopyableType T>
struct std::formatter<cpp_features::containers::Container<T>> : std::formatter<std::string_view> {
  /**
   * @brief Format the container for output
   *
   * @param container The container to format
   * @param ctx The format context
   * @return Iterator pointing past the formatted output
   */
  auto format(const cpp_features::containers::Container<T> &container,
              std::format_context &ctx) const {
    std::string temp;
    if (container.IsEmpty()) {
      temp = "[]";
    } else {
      temp = "[";
      auto first = true;
      for (const auto &item : container) {
        if (!first) {
          temp += ", ";
        }
        std::format_to(std::back_inserter(temp), "{}", item);
        first = false;
      }
      temp += "]";
    }
    return std::formatter<std::string_view>::format(temp, ctx);
  }
};
