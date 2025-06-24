#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <ranges>
#include <algorithm>
#include <regex>
#include <format>
#include <concepts>
#include <charconv>
#include <expected>

namespace cpp_features::strings {

template<typename T>
concept StringLike = std::convertible_to<T, std::string_view>;

enum class StringError {
  kEmpty,
  kInvalidFormat,
  kConversionError,
  kPatternNotFound
};

class StringProcessor {
 public:
  static auto ToUpper(std::string_view input) -> std::string;
  static auto ToLower(std::string_view input) -> std::string;

  static auto Trim(std::string_view input) -> std::string_view;
  static auto TrimLeft(std::string_view input) -> std::string_view;
  static auto TrimRight(std::string_view input) -> std::string_view;

  static auto Split(std::string_view input, char delimiter) -> std::vector<std::string_view>;
  static auto Split(std::string_view input, std::string_view delimiter) -> std::vector<std::string_view>;

  static auto Join(const std::vector<std::string_view>& parts, std::string_view separator) -> std::string;

  template<StringLike T>
  static auto Contains(std::string_view haystack, T&& needle) -> bool {
    return haystack.find(std::string_view(needle)) != std::string_view::npos;
  }

  static auto Replace(std::string_view input, std::string_view from, std::string_view to) -> std::string;

  static auto ReplaceAll(std::string_view input, std::string_view from, std::string_view to) -> std::string;

  template<typename T>
    requires std::integral<T> || std::floating_point<T>
  static auto ToString(T value) -> std::string {
    return std::format("{}", value);
  }

  template<std::integral T>
  static auto ParseInt(std::string_view input) -> std::expected<T, StringError> {
    T result;
    auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result);
    if (ec == std::errc{} && ptr == input.data() + input.size()) {
      return result;
    }
    return std::unexpected(StringError::kConversionError);
  }

  template<std::floating_point T>
  static auto ParseFloat(std::string_view input) -> std::expected<T, StringError> {
    try {
      std::string str(input);
      T result;
      if constexpr (std::is_same_v<T, float>) {
        result = std::stof(str);
      } else if constexpr (std::is_same_v<T, double>) {
        result = std::stod(str);
      } else {
        result = std::stold(str);
      }
      return result;
    } catch (...) {
      return std::unexpected(StringError::kConversionError);
    }
  }

  static auto IsWhitespace(char c) -> bool;
  static auto IsAlphaNumeric(char c) -> bool;

  static auto CountOccurrences(std::string_view input, char target) -> std::size_t;
  static auto CountOccurrences(std::string_view input, std::string_view target) -> std::size_t;

  static auto StartsWith(std::string_view input, std::string_view prefix) -> bool;
  static auto EndsWith(std::string_view input, std::string_view suffix) -> bool;

  static auto Reverse(std::string_view input) -> std::string;

  static auto RemoveWhitespace(std::string_view input) -> std::string;

  static auto FindRegex(std::string_view input, const std::regex& pattern)
    -> std::vector<std::string>;

  static auto ValidateEmail(std::string_view email) -> bool;
  static auto ValidateUrl(std::string_view url) -> bool;
};

void DemonstrateBasicStringOps();

void DemonstrateStringViews();

void DemonstrateStringFormatting();

void DemonstrateStringConversions();

void DemonstrateRegexOperations();

template<std::ranges::input_range Range>
  requires StringLike<std::ranges::range_value_t<Range>>
auto ConcatenateStrings(Range&& strings, std::string_view separator = "") -> std::string {
  if (std::ranges::empty(strings)) {
    return {};
  }

  std::string result;
  auto it = std::ranges::begin(strings);
  result += std::string_view(*it);
  ++it;

  for (; it != std::ranges::end(strings); ++it) {
    result += separator;
    result += std::string_view(*it);
  }

  return result;
}

template<typename Predicate>
auto FilterStrings(const std::vector<std::string>& strings, Predicate predicate)
  -> std::vector<std::string> {
  std::vector<std::string> result;
  std::ranges::copy_if(strings, std::back_inserter(result), predicate);
  return result;
}

}  // namespace cpp_features::strings