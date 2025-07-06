/**
 * @file exceptions_example.cpp
 * @brief Example demonstrating the usage of the exceptions module
 */

#include <cmath>
#include <print>
#include <type_traits>

#include "exceptions/custom_exception.hpp"
#include "exceptions/exception_handler.hpp"
#include "exceptions/result.hpp"

using cpp_features::exceptions::BaseException;
using cpp_features::exceptions::CalculationException;
using cpp_features::exceptions::ErrorSeverity;
using cpp_features::exceptions::ExceptionHandler;
using cpp_features::exceptions::ResourceException;
using cpp_features::exceptions::Result;
using cpp_features::exceptions::ValidationException;

auto main() -> int {
  std::println("=== Exceptions Module Example ===");

  // Demonstrate BaseException with different severity levels
  try {
    throw BaseException{"This is a warning", ErrorSeverity::kWarning};
  } catch (const BaseException &e) {
    std::println("Caught BaseException: {}", e.what());
    std::println("Severity: {}", SeverityToString(e.GetSeverity()));
    std::println("Location: {}:{}", e.GetLocation().file_name(), e.GetLocation().line());
    std::println("Formatted: {}", e.GetFormattedMessage());
  }

  // Demonstrate ValidationException
  try {
    throw ValidationException{"Invalid email format", "email"};
  } catch (const ValidationException &e) {
    std::println("Caught ValidationException: {}", e.what());
    std::println("Field: {}", e.GetFieldName().value_or("unknown"));
  }

  // Demonstrate ResourceException
  try {
    throw ResourceException{"File not found", "/path/to/config.txt"};
  } catch (const ResourceException &e) {
    std::println("Caught ResourceException: {}", e.what());
    std::println("Resource: {}", e.GetResourceName().value_or("unknown"));
  }

  // Demonstrate CalculationException
  try {
    throw CalculationException{"Division by zero", 0.0};
  } catch (const CalculationException &e) {
    std::println("Caught CalculationException: {}", e.what());
    std::println("Input value: {}", e.GetInputValue());
  }

  // Demonstrate ExceptionHandler::SafeExecute
  auto risky_operation = []() { throw ValidationException{"Something went wrong"}; };
  auto success = ExceptionHandler::SafeExecute(risky_operation);
  std::println("SafeExecute result: {}", success);

  // Demonstrate ExceptionHandler::SafeExecuteWithDefault
  auto divide_by_zero = []() -> double { throw CalculationException{"Division by zero", 0.0}; };
  auto result = ExceptionHandler::SafeExecuteWithDefault(divide_by_zero, 0.0);
  std::println("SafeExecuteWithDefault result: {}", result);

  // Demonstrate Result type
  auto safe_divide = [](double a, double b) -> Result<double> {
    if (b == 0.0) {
      return Result<double>{CalculationException{"Division by zero", b}};
    }
    return Result<double>{a / b};
  };

  // Successful Result
  auto result1 = safe_divide(10.0, 2.0);
  if (result1.HasValue()) {
    std::println("10 / 2 = {}", result1.GetValue());
  } else {
    std::println("10 / 2 failed: {}", result1.GetException().what());
  }

  // Failed Result
  auto result2 = safe_divide(10.0, 0.0);
  if (result2.HasValue()) {
    std::println("10 / 0 = {}", result2.GetValue());
  } else {
    std::println("10 / 0 failed: {}", result2.GetException().what());
  }

  // Demonstrate Result::Visit
  auto result3 = safe_divide(15.0, 3.0);
  result3.Visit([](const auto &value) {
    if constexpr (std::is_same_v<std::decay_t<decltype(value)>, double>) {
      std::println("15 / 3 = {}", value);
    } else {
      std::println("15 / 3 failed: {}", value.what());
    }
  });

  // Demonstrate Result::Map
  auto mapped = safe_divide(8.0, 2.0).Map([](double x) { return x * 3.0; }).Map([](double x) {
    return static_cast<int>(x);
  });

  if (mapped.HasValue()) {
    std::println("8 / 2 * 3 = {}", mapped.GetValue());
  }

  // Demonstrate Result::Then (chaining operations that can fail)
  auto safe_sqrt = [](double x) -> Result<double> {
    if (x < 0.0) {
      return Result<double>{
          CalculationException{"Cannot calculate square root of negative number", x},
      };
    }
    return Result<double>{std::sqrt(x)};
  };

  auto chained = safe_divide(16.0, 4.0)
                     .Then([&safe_sqrt](double x) { return safe_sqrt(x); })
                     .Map([](double x) { return x + 1.0; });

  if (chained.HasValue()) {
    std::println("sqrt(16 / 4) + 1 = {}", chained.GetValue());
  }

  // Demonstrate error propagation in Result chains
  auto error_chain =
      safe_divide(10.0, 0.0).Map([](double x) { return x * 2.0; }).Then([&safe_sqrt](double x) {
        return safe_sqrt(x);
      });

  if (error_chain.HasValue()) {
    std::println("10 / 0 * 2 = {}", error_chain.GetValue());
  } else {
    std::println("10 / 0 * 2 failed: {}", error_chain.GetException().what());
  }

  std::println("=== Exceptions Module Example Completed ===");
  return 0;
}
