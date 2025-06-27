#pragma once

#include <concepts>

namespace cpp_features::concepts {

template <typename T>
concept ArithmeticType = std::integral<T> || std::floating_point<T>;

template <typename T>
concept AddableType = requires(T lhs, T rhs) {
  { lhs + rhs } -> std::same_as<T>;
};

template <typename T>
concept SubtractableType = requires(T lhs, T rhs) {
  { lhs - rhs } -> std::same_as<T>;
};

template <typename T>
concept NumericType = ArithmeticType<T> && AddableType<T> && SubtractableType<T>;

}  // namespace cpp_features::concepts
