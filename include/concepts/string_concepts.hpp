#pragma once

#include <concepts>
#include <string_view>

namespace cpp_features::concepts {

template <typename T>
concept StringLikeType = std::convertible_to<T, std::string_view>;

}  // namespace cpp_features::concepts
