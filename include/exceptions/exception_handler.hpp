/**
 * @file exception_handler.hpp
 * @brief Exception handler utility class with safe execution and logging capabilities
 *
 * This module provides a utility class for safe exception handling and logging.
 * It supports both simple execution and execution with default return values.
 */

#pragma once

#include <exception>
#include <type_traits>
#include <utility>

#include "concepts/callable_concepts.hpp"
#include "custom_exception.hpp"

namespace cpp_features::exceptions {

/**
 * @brief Utility class for safe exception handling and logging
 *
 * Provides static methods for safely executing functions with automatic exception handling and
 * logging. Supports both simple execution and execution with default return values.
 *
 * @code
 * // Safe execution with boolean result
 * bool success = ExceptionHandler::SafeExecute([]() {
 *   // Some risky operation
 *   throw ValidationException{"Something went wrong"};
 * });
 *
 * // Safe execution with default return value
 * double result = ExceptionHandler::SafeExecuteWithDefault(
 *   []() -> double { throw CalculationException{"Division by zero", 0.0}; },
 *   -1.0  // default value
 * );
 * @endcode
 */
class ExceptionHandler {
 public:
  /**
   * @brief Safely executes a function with automatic exception handling
   *
   * Catches and logs all exceptions, returning true on success or false
   * if any exception was thrown and handled.
   *
   * @tparam Func Callable type (function, lambda, functor)
   * @param func Function to execute safely
   * @return true if execution succeeded, false if exception was caught
   */
  template <concepts::NullaryCallable Func>
  static auto SafeExecute(Func &&func) noexcept -> bool {
    try {
      static_cast<void>(func());
      return true;
    } catch (const BaseException &e) {
      LogException(e);
      return false;
    } catch (const std::exception &e) {
      LogStandardException(e);
      return false;
    } catch (...) {
      LogUnknownException();
      return false;
    }
  }

  /**
   * @brief Safely executes a function with a default return value on exception
   *
   * Executes the provided function and returns its result on success.
   * If any exception is thrown, returns the provided default value instead.
   *
   * @tparam Func Callable type that returns a value
   * @tparam DefaultValue Type of the default value
   * @param func Function to execute safely
   * @param default_value Value to return if exception occurs
   * @return Function result on success, default_value on exception
   */
  template <concepts::NullaryCallable Func, typename DefaultValue>
  static auto SafeExecuteWithDefault(Func &&func, DefaultValue &&default_value)
      -> std::decay_t<DefaultValue> {
    try {
      return func();
    } catch (...) {
      return std::forward<DefaultValue>(default_value);
    }
  }

  /**
   * @brief Logs a custom BaseException with formatted output
   * @param exception The BaseException to log
   */
  static void LogException(const BaseException &exception);

  /**
   * @brief Logs a standard library exception
   * @param exception The std::exception to log
   */
  static void LogStandardException(const std::exception &exception);

  /**
   * @brief Logs an unknown exception (catch-all)
   */
  static void LogUnknownException();
};

}  // namespace cpp_features::exceptions
