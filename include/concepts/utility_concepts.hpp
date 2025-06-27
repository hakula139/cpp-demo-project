#pragma once

#include <concepts>
#include <ostream>

namespace cpp_features::concepts {

template <typename T>
concept PrintableType = requires(T value, std::ostream& os) {
  { os << value } -> std::convertible_to<std::ostream&>;
};

template <typename T>
concept ComparableType = std::totally_ordered<T>;

template <typename T>
concept DefaultConstructibleType = std::default_initializable<T>;

template <typename T>
concept CopyableType = std::copyable<T>;

template <typename T>
concept MovableType = std::movable<T>;

}  // namespace cpp_features::concepts
