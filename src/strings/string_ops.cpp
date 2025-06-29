#include "strings/string_ops.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <print>
#include <string>
#include <string_view>

namespace cpp_features::strings {

auto StringProcessor::ToUpper(std::string_view input) -> std::string {
  std::string result(input);
  std::transform(result.begin(), result.end(), result.begin(), ::toupper);
  return result;
}

auto StringProcessor::ToLower(std::string_view input) -> std::string {
  std::string result(input);
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

auto StringProcessor::Trim(std::string_view input) -> std::string_view {
  return TrimLeft(TrimRight(input));
}

auto StringProcessor::TrimLeft(std::string_view input) -> std::string_view {
  auto start = input.find_first_not_of(" \t\n\r\f\v");
  return (start == std::string_view::npos) ? std::string_view{} : input.substr(start);
}

auto StringProcessor::TrimRight(std::string_view input) -> std::string_view {
  auto end = input.find_last_not_of(" \t\n\r\f\v");
  return (end == std::string_view::npos) ? std::string_view{} : input.substr(0, end + 1);
}

auto StringProcessor::Split(std::string_view input, char delimiter)
    -> std::vector<std::string_view> {
  std::vector<std::string_view> result;
  std::size_t start = 0;

  for (std::size_t i = 0; i <= input.length(); ++i) {
    if (i == input.length() || input[i] == delimiter) {
      if (i > start) {
        result.push_back(input.substr(start, i - start));
      }
      start = i + 1;
    }
  }

  return result;
}

auto StringProcessor::Split(std::string_view input, std::string_view delimiter)
    -> std::vector<std::string_view> {
  std::vector<std::string_view> result;
  std::size_t start = 0;
  std::size_t pos = 0;

  while ((pos = input.find(delimiter, start)) != std::string_view::npos) {
    if (pos > start) {
      result.push_back(input.substr(start, pos - start));
    }
    start = pos + delimiter.length();
  }

  if (start < input.length()) {
    result.push_back(input.substr(start));
  }

  return result;
}

auto StringProcessor::Join(const std::vector<std::string_view>& parts, std::string_view separator)
    -> std::string {
  if (parts.empty()) {
    return {};
  }

  std::string result;
  result += parts[0];

  for (std::size_t i = 1; i < parts.size(); ++i) {
    result += separator;
    result += parts[i];
  }

  return result;
}

auto StringProcessor::Replace(std::string_view input, std::string_view from, std::string_view to)
    -> std::string {
  std::string result(input);
  auto pos = result.find(from);
  if (pos != std::string::npos) {
    result.replace(pos, from.length(), to);
  }
  return result;
}

auto StringProcessor::ReplaceAll(std::string_view input, std::string_view from, std::string_view to)
    -> std::string {
  std::string result(input);
  std::size_t pos = 0;

  while ((pos = result.find(from, pos)) != std::string::npos) {
    result.replace(pos, from.length(), to);
    pos += to.length();
  }

  return result;
}

auto StringProcessor::IsWhitespace(char c) -> bool {
  return std::isspace(static_cast<unsigned char>(c));
}

auto StringProcessor::IsAlphaNumeric(char c) -> bool {
  return std::isalnum(static_cast<unsigned char>(c));
}

auto StringProcessor::CountOccurrences(std::string_view input, char target) -> std::size_t {
  return std::count(input.begin(), input.end(), target);
}

auto StringProcessor::CountOccurrences(std::string_view input, std::string_view target)
    -> std::size_t {
  std::size_t count = 0;
  std::size_t pos = 0;

  while ((pos = input.find(target, pos)) != std::string_view::npos) {
    ++count;
    pos += target.length();
  }

  return count;
}

auto StringProcessor::StartsWith(std::string_view input, std::string_view prefix) -> bool {
  return input.substr(0, prefix.length()) == prefix;
}

auto StringProcessor::EndsWith(std::string_view input, std::string_view suffix) -> bool {
  if (suffix.length() > input.length()) {
    return false;
  }
  return input.substr(input.length() - suffix.length()) == suffix;
}

auto StringProcessor::Reverse(std::string_view input) -> std::string {
  std::string result(input);
  std::reverse(result.begin(), result.end());
  return result;
}

auto StringProcessor::RemoveWhitespace(std::string_view input) -> std::string {
  std::string result;
  std::copy_if(input.begin(), input.end(), std::back_inserter(result),
               [](char c) { return !IsWhitespace(c); });
  return result;
}

auto StringProcessor::FindRegex(std::string_view input, const std::regex& pattern)
    -> std::vector<std::string> {
  std::vector<std::string> matches;
  std::string input_str(input);
  std::sregex_iterator iter(input_str.begin(), input_str.end(), pattern);
  std::sregex_iterator end;

  for (; iter != end; ++iter) {
    matches.push_back(iter->str());
  }

  return matches;
}

auto StringProcessor::ValidateEmail(std::string_view email) -> bool {
  std::regex email_pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
  std::string email_str(email);
  return std::regex_match(email_str, email_pattern);
}

auto StringProcessor::ValidateUrl(std::string_view url) -> bool {
  std::regex url_pattern(R"(^https?:\/\/[^\s\/$.?#].[^\s]*$)");
  std::string url_str(url);
  return std::regex_match(url_str, url_pattern);
}

void DemonstrateBasicStringOps() {
  std::print("\n=== Basic String Operations ===\n");

  std::string text = "  Hello, World!  ";
  std::print("Original: '{}'\n", text);
  std::print("Trimmed: '{}'\n", StringProcessor::Trim(text));
  std::print("Upper: '{}'\n", StringProcessor::ToUpper(text));
  std::print("Lower: '{}'\n", StringProcessor::ToLower(text));
  std::print("Reversed: '{}'\n", StringProcessor::Reverse(text));

  std::string sentence = "The quick brown fox jumps over the lazy dog";
  auto words = StringProcessor::Split(sentence, ' ');
  std::print("Split into {} words: ", words.size());
  for (const auto& word : words) {
    std::print("'{}' ", word);
  }
  std::print("\n");

  auto joined = StringProcessor::Join(words, " | ");
  std::print("Joined with ' | ': {}\n", joined);
}

void DemonstrateStringViews() {
  std::print("\n=== String Views Demonstration ===\n");

  std::string large_string = "This is a very long string that we want to process efficiently";
  std::string_view view = large_string;

  std::print("Full string: {}\n", view);
  std::print("Substring (5-15): {}\n", view.substr(5, 10));

  std::print("Starts with 'This': {}\n", StringProcessor::StartsWith(view, "This"));
  std::print("Ends with 'efficiently': {}\n", StringProcessor::EndsWith(view, "efficiently"));

  std::print("Contains 'long': {}\n", StringProcessor::Contains(view, "long"));
  std::print("Count of 'e': {}\n", StringProcessor::CountOccurrences(view, 'e'));
}

void DemonstrateStringFormatting() {
  std::print("\n=== String Formatting Demonstration ===\n");

  int number = 42;
  double pi = 3.14159;

  std::print("Integer: {}\n", StringProcessor::ToString(number));
  std::print("Float: {}\n", StringProcessor::ToString(pi));

  auto parse_result = StringProcessor::ParseInt<int>("123");
  if (parse_result) {
    std::print("Parsed integer: {}\n", parse_result.value());
  }

  auto parse_float = StringProcessor::ParseFloat<double>("3.14159");
  if (parse_float) {
    std::print("Parsed float: {:.3f}\n", parse_float.value());
  }
}

void DemonstrateStringConversions() {
  std::print("\n=== String Conversions Demonstration ===\n");

  std::string text = "Hello World 123 Test";
  std::print("Original: {}\n", text);
  std::print("Without whitespace: {}\n", StringProcessor::RemoveWhitespace(text));

  std::string replaced = StringProcessor::Replace(text, "World", "Universe");
  std::print("Replace 'World' with 'Universe': {}\n", replaced);

  std::string multi_replace = StringProcessor::ReplaceAll("aaa bbb aaa ccc aaa", "aaa", "XXX");
  std::print("Replace all 'aaa' with 'XXX': {}\n", multi_replace);
}

void DemonstrateRegexOperations() {
  std::print("\n=== Regex Operations Demonstration ===\n");

  std::string text = "Contact us at user@example.com or admin@test.org for support";
  std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

  auto emails = StringProcessor::FindRegex(text, email_regex);
  std::print("Found {} email addresses:\n", emails.size());
  for (const auto& email : emails) {
    std::print("  - {}\n", email);
    std::print("    Valid: {}\n", StringProcessor::ValidateEmail(email));
  }

  std::vector<std::string> test_urls = {"https://www.example.com", "http://test.org/path",
                                        "invalid-url", "ftp://not-http.com"};

  std::print("URL validation:\n");
  for (const auto& url : test_urls) {
    std::print("  {} -> {}\n", url, StringProcessor::ValidateUrl(url) ? "Valid" : "Invalid");
  }
}

}  // namespace cpp_features::strings
