/**
 * @file string_concepts.hpp
 * @brief String-related concepts for C++ template constraints
 *
 * This file contains concept definitions for string-like types, providing
 * compile-time constraints for template parameters that need to work with
 * string-like objects that can be converted to string views.
 */

#pragma once

#include <concepts>
#include <string_view>

namespace cpp_features::concepts {

/**
 * @brief Concept for string-like types
 *
 * @tparam T The type to check for string-like properties
 *
 * This concept ensures that a type can be converted to std::string_view,
 * making it usable in contexts that expect string-like behavior. This includes
 * types such as:
 *
 * - std::string
 * - std::string_view
 * - const char *
 * - char arrays
 * - Any custom string type with appropriate conversion operators
 *
 * This concept is particularly useful for function templates that need to
 * accept various string representations without forcing a specific string type.
 *
 * @code
 * template <StringLikeType T>
 * auto StartsWithHello(const T &text) -> bool {
 *   std::string_view view{text};
 *   return view.starts_with("hello");
 * }
 *
 * auto result1 = StartsWithHello(std::string{"hello world"});  // std::string
 * auto result2 = StartsWithHello(std::string_view{"hello!"});  // std::string_view
 * auto result3 = StartsWithHello("hello there");               // const char *
 * @endcode
 */
template <typename T>
concept StringLikeType = std::convertible_to<T, std::string_view>;

}  // namespace cpp_features::concepts
