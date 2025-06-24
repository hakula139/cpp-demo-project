#pragma once

#include <concepts>
#include <type_traits>
#include <iostream>

namespace cpp_features::concepts {

template<typename T>
concept ArithmeticType = std::integral<T> || std::floating_point<T>;

template<typename T>
concept PrintableType = requires(T t, std::ostream& os) {
    { os << t } -> std::convertible_to<std::ostream&>;
};

template<typename T>
concept ComparableType = std::totally_ordered<T>;

template<typename T>
concept AddableType = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<typename T>
concept SubtractableType = requires(T a, T b) {
    { a - b } -> std::same_as<T>;
};

template<typename Container>
concept IterableContainer = requires(Container c) {
    { c.begin() } -> std::input_or_output_iterator;
    { c.end() } -> std::input_or_output_iterator;
    { c.size() } -> std::convertible_to<std::size_t>;
};

template<typename Container>
concept RangeContainer = std::ranges::range<Container>;

template<typename T>
concept StringLikeType = std::convertible_to<T, std::string_view>;

template<typename T>
concept NumericType = ArithmeticType<T> && AddableType<T> && SubtractableType<T>;

template<typename T>
concept DefaultConstructibleType = std::default_initializable<T>;

template<typename T>
concept CopyableType = std::copyable<T>;

template<typename T>
concept MovableType = std::movable<T>;

}  // namespace cpp_features::concepts