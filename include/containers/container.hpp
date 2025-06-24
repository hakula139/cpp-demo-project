#pragma once

#include <vector>
#include <algorithm>
#include <ranges>
#include <print>
#include <expected>
#include <functional>

#include "concepts/arithmetic_concepts.hpp"

namespace cpp_features::containers {

enum class ContainerError {
    kEmpty,
    kOutOfBounds,
    kInvalidOperation
};

template<cpp_features::concepts::CopyableType T>
class Container {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  Container() = default;
  explicit Container(size_type initial_size) : data_(initial_size) {}
  Container(std::initializer_list<T> init) : data_(init) {}

  template<std::ranges::input_range Range>
    requires std::convertible_to<std::ranges::range_value_t<Range>, T>
  explicit Container(Range&& range)
      : data_(std::ranges::begin(range), std::ranges::end(range)) {}

  void Add(const T& item) {
    data_.push_back(item);
  }

  void Add(T&& item) {
    data_.push_back(std::move(item));
  }

  template<typename... Args>
  void Emplace(Args&&... args) {
    data_.emplace_back(std::forward<Args>(args)...);
  }

  auto Remove(const T& item) -> size_type {
    auto new_end = std::ranges::remove(data_, item);
    auto count = std::distance(new_end.begin(), data_.end());
    data_.erase(new_end.begin(), data_.end());
    return count;
  }

  void Display() const {
    if (data_.empty()) {
      std::print("Container is empty\n");
      return;
    }

    std::print("Container contents: ");
    for (const auto& item : data_) {
      std::print("{} ", item);
    }
    std::print("\n");
  }

  [[nodiscard]] auto GetSize() const noexcept -> size_type {
    return data_.size();
  }

  [[nodiscard]] auto IsEmpty() const noexcept -> bool {
    return data_.empty();
  }

  [[nodiscard]] auto At(size_type index) const -> std::expected<const T*, ContainerError> {
    if (index >= data_.size()) {
      return std::unexpected(ContainerError::kOutOfBounds);
    }
    return &data_[index];
  }

  [[nodiscard]] auto At(size_type index) -> std::expected<T*, ContainerError> {
    if (index >= data_.size()) {
      return std::unexpected(ContainerError::kOutOfBounds);
    }
    return &data_[index];
  }

  auto GetView() const {
    return std::views::all(data_);
  }

  auto GetFilteredView(std::function<bool(const T&)> predicate) const {
    return data_ | std::views::filter(predicate);
  }

  template<typename Func>
  auto GetTransformedView(Func transform_func) const {
    return data_ | std::views::transform(transform_func);
  }

  [[nodiscard]] auto begin() noexcept -> iterator { return data_.begin(); }
  [[nodiscard]] auto end() noexcept -> iterator { return data_.end(); }
  [[nodiscard]] auto begin() const noexcept -> const_iterator { return data_.begin(); }
  [[nodiscard]] auto end() const noexcept -> const_iterator { return data_.end(); }
  [[nodiscard]] auto cbegin() const noexcept -> const_iterator { return data_.cbegin(); }
  [[nodiscard]] auto cend() const noexcept -> const_iterator { return data_.cend(); }

  auto operator<=>(const Container& other) const = default;

 private:
  std::vector<T> data_;
};

template<cpp_features::concepts::CopyableType T>
[[nodiscard]] auto CreateContainer() -> Container<T> {
  return Container<T>{};
}

template<cpp_features::concepts::CopyableType T>
[[nodiscard]] auto CreateContainer(std::initializer_list<T> init) -> Container<T> {
  return Container<T>{init};
}

}  // namespace cpp_features::containers