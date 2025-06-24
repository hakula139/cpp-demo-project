#include <exception>
#include <string>
#include <print>

#include "exceptions/custom_exception.hpp"

namespace cpp_features::exceptions {

void ExceptionHandler::LogException(const BaseException& exception) {
  std::print("Custom Exception [{}]: {}\n",
            SeverityToString(exception.GetSeverity()),
            exception.GetFormattedMessage());
}

void ExceptionHandler::LogStandardException(const std::exception& exception) {
  std::print("Standard Exception: {}\n", exception.what());
}

void ExceptionHandler::LogUnknownException() {
  std::print("Unknown Exception: An unhandled exception occurred\n");
}

auto ExceptionHandler::SeverityToString(ErrorSeverity severity) -> std::string_view {
  switch (severity) {
    case ErrorSeverity::kInfo: return "INFO";
    case ErrorSeverity::kWarning: return "WARNING";
    case ErrorSeverity::kError: return "ERROR";
    case ErrorSeverity::kCritical: return "CRITICAL";
    default: return "UNKNOWN";
  }
}

void DemonstrateCustomExceptions() {
  std::print("\n=== Custom Exceptions Demonstration ===\n");

  try {
    throw ValidationException("Invalid input value", "username");
  } catch (const ValidationException& e) {
    std::print("Caught ValidationException for field '{}': {}\n",
              e.GetFieldName(), e.what());
    std::print("Location: {}:{}\n",
              e.GetLocation().file_name(), e.GetLocation().line());
  }

  try {
    throw ResourceException("Failed to open file", "config.txt");
  } catch (const ResourceException& e) {
    std::print("Caught ResourceException for resource '{}': {}\n",
              e.GetResourceName(), e.what());
  }

  try {
    throw CalculationException("Division by zero", 42.0);
  } catch (const CalculationException& e) {
    std::print("Caught CalculationException with input {}: {}\n",
              e.GetInputValue(), e.what());
  }
}

void DemonstrateExceptionHandling() {
  std::print("\n=== Exception Handling Demonstration ===\n");

  auto safe_divide = [](double a, double b) {
    if (b == 0.0) {
      throw CalculationException("Division by zero", b);
    }
    return a / b;
  };

  bool success = ExceptionHandler::SafeExecute([&]() {
    auto result = safe_divide(10.0, 2.0);
    std::print("Safe division result: {}\n", result);
  });
  std::print("Safe execution success: {}\n", success);

  success = ExceptionHandler::SafeExecute([&]() {
    [[maybe_unused]] auto result = safe_divide(10.0, 0.0);
    std::print("This shouldn't print\n");
  });
  std::print("Safe execution with error success: {}\n", success);

  auto default_result = ExceptionHandler::SafeExecuteWithDefault([&]() {
    return safe_divide(10.0, 0.0);
  }, -1.0);
  std::print("Safe execution with default returned: {}\n", default_result);
}

void DemonstrateSourceLocation() {
  std::print("\n=== Source Location Demonstration ===\n");

  auto create_exception = [](std::string_view message) {
    return BaseException(message, ErrorSeverity::kWarning);
  };

  auto exception = create_exception("Test exception with source location");
  std::print("Exception created at:\n");
  std::print("  File: {}\n", exception.GetLocation().file_name());
  std::print("  Line: {}\n", exception.GetLocation().line());
  std::print("  Function: {}\n", exception.GetLocation().function_name());
  std::print("  Formatted: {}\n", exception.GetFormattedMessage());
}

}  // namespace cpp_features::exceptions
