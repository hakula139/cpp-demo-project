/**
 * @file custom_exception.cpp
 * @brief Implementation of custom exception classes and handlers
 */

#include "exceptions/custom_exception.hpp"

#include <exception>
#include <print>
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

void ExceptionHandler::LogException(const BaseException &exception) {
  std::println("Custom Exception [{}]: {}", SeverityToString(exception.GetSeverity()),
               exception.GetFormattedMessage());
}

void ExceptionHandler::LogStandardException(const std::exception &exception) {
  std::println("Standard Exception: {}", exception.what());
}

void ExceptionHandler::LogUnknownException() {
  std::println("Unknown Exception: An unhandled exception occurred");
}

}  // namespace cpp_features::exceptions
