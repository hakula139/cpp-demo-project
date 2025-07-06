/**
 * @file custom_exception.cpp
 * @brief Implementation of custom exception classes
 */

#include "exceptions/custom_exception.hpp"

#include <string_view>

namespace cpp_features::exceptions {

auto SeverityToString(ErrorSeverity severity) -> std::string_view {
  switch (severity) {
    case ErrorSeverity::kTrace:
      return "TRACE";
    case ErrorSeverity::kDebug:
      return "DEBUG";
    case ErrorSeverity::kInfo:
      return "INFO";
    case ErrorSeverity::kWarning:
      return "WARNING";
    case ErrorSeverity::kError:
      return "ERROR";
    case ErrorSeverity::kFatal:
      return "FATAL";
  }
}

}  // namespace cpp_features::exceptions
