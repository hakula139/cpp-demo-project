/**
 * @file custom_exception.hpp
 * @brief Custom exception classes with enhanced error handling capabilities
 *
 * This module provides a comprehensive exception hierarchy with additional context
 * information such as source location, error severity levels, and specialized
 * exception types for common error scenarios.
 *
 * @code
 * try {
 *   throw ValidationException("Invalid email format", "email_field");
 * } catch (const ValidationException &e) {
 *   std::println("Field '{}' error: {}", e.GetFieldName(), e.what());
 * }
 * @endcode
 */

#pragma once

#include <cstdint>
#include <exception>
#include <format>
#include <optional>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace cpp_features::exceptions {

/**
 * @brief Enumeration representing different error severity levels
 *
 * Used to categorize exceptions by their impact and urgency.
 */
enum class ErrorSeverity : std::uint8_t {
  kTrace = 0,    ///< Trace-level messages for development
  kDebug = 1,    ///< Debug-level messages for development
  kInfo = 2,     ///< Informational messages
  kWarning = 3,  ///< Warning conditions that don't prevent operation
  kError = 4,    ///< Error conditions that prevent normal operation
  kFatal = 5,    ///< Fatal errors that cannot be recovered from
};

/**
 * @brief Converts ErrorSeverity enum to string representation
 * @param severity The severity level to convert
 * @return String view containing the severity name
 */
[[nodiscard]] auto SeverityToString(ErrorSeverity severity) -> std::string_view;

/**
 * @brief Base exception class with source location and severity information
 *
 * Provides enhanced exception handling with automatic source location capture, severity levels, and
 * formatted error messages. All custom exceptions in this module derive from this base class.
 *
 * This class is designed to be nothrow copy constructible in compliance with SEI CERT ERR60-CPP.
 *
 * @code
 * // Automatic source location capture
 * throw BaseException("Database connection failed", ErrorSeverity::kFatal);
 *
 * // Manual source location (rarely needed)
 * throw BaseException("Error message", ErrorSeverity::kError, std::source_location::current());
 * @endcode
 */
class BaseException : public std::exception {
 public:
  /**
   * @brief Constructs a BaseException with message and optional parameters
   *
   * @param message Error message describing what went wrong
   * @param severity Severity level of the error (default: kError)
   * @param location Source location where exception was thrown (auto-captured)
   */
  explicit BaseException(std::string_view message, ErrorSeverity severity = ErrorSeverity::kError,
                         std::source_location location = std::source_location::current())
      : runtime_error_{std::string{message}}, severity_{severity}, location_{location} {}

  /**
   * @brief Copy constructor explicitly declared noexcept
   */
  BaseException(const BaseException &) noexcept = default;

  /**
   * @brief Move constructor explicitly declared noexcept
   */
  BaseException(BaseException &&) noexcept = default;

  /**
   * @brief Copy assignment operator explicitly declared noexcept
   */
  auto operator=(const BaseException &) noexcept -> BaseException & = default;

  /**
   * @brief Move assignment operator explicitly declared noexcept
   */
  auto operator=(BaseException &&) noexcept -> BaseException & = default;

  /**
   * @brief Returns the error message
   * @return C-style string containing the error message
   */
  [[nodiscard]] auto what() const noexcept -> const char * override {
    return runtime_error_.what();
  }

  /**
   * @brief Gets the severity level of this exception
   * @return ErrorSeverity enumeration value
   */
  [[nodiscard]] auto GetSeverity() const noexcept -> ErrorSeverity { return severity_; }

  /**
   * @brief Gets the source location where this exception was thrown
   * @return Reference to the source_location object
   */
  [[nodiscard]] auto GetLocation() const noexcept -> const std::source_location & {
    return location_;
  }

  /**
   * @brief Creates a formatted message including source location information
   * @return Formatted string with file, line, function, and message
   *
   * Output format: "file.cpp:42 - MyFunction(): Error message"
   */
  [[nodiscard]] auto GetFormattedMessage() const -> std::string {
    return std::format("{}:{} - {}: {}", location_.file_name(), location_.line(),
                       location_.function_name(), runtime_error_.what());
  }

 private:
  std::runtime_error runtime_error_;  ///< The error message (nothrow copy constructible)
  ErrorSeverity severity_;            ///< Error severity level
  std::source_location location_;     ///< Source location of the exception
};

// Compile-time assertion to ensure BaseException is nothrow copy constructible
static_assert(std::is_nothrow_copy_constructible_v<BaseException>,
              "BaseException must be nothrow copy constructible (SEI CERT ERR60-CPP)");

/**
 * @brief Exception for validation and input errors
 *
 * Specialized exception for validation failures, with optional field name to identify which input
 * field caused the validation error.
 *
 * @code
 * // Field-specific validation error
 * throw ValidationException{"Must be a valid email address", "email"};
 *
 * // General validation error
 * throw ValidationException{"Input validation failed"};
 * @endcode
 */
class ValidationException : public BaseException {
 public:
  /**
   * @brief Constructs a ValidationException
   *
   * @param message Validation error message
   * @param field_name Optional name of the field that failed validation
   * @param location Source location (auto-captured)
   */
  explicit ValidationException(std::string_view message,
                               std::optional<std::string_view> field_name = std::nullopt,
                               std::source_location location = std::source_location::current())
      : BaseException{message, ErrorSeverity::kError, location}, field_name_{field_name} {}

  /**
   * @brief Gets the name of the field that failed validation
   * @return Optional string view of the field name (std::nullopt if not specified)
   */
  [[nodiscard]] auto GetFieldName() const noexcept -> std::optional<std::string_view> {
    return field_name_;
  }

 private:
  std::optional<std::string_view> field_name_;  ///< Name of the field that failed validation
};

// Compile-time assertion to ensure ValidationException is nothrow copy constructible
static_assert(std::is_nothrow_copy_constructible_v<ValidationException>,
              "ValidationException must be nothrow copy constructible (SEI CERT ERR60-CPP)");

/**
 * @brief Exception for resource-related errors
 *
 * Used for errors related to external resources such as files, network connections, databases, or
 * any other system resources.
 *
 * @code
 * // File resource error
 * throw ResourceException{"File not found", "/path/to/config.txt"};
 *
 * // Network resource error
 * throw ResourceException{"Connection timeout", "api.example.com:443"};
 * @endcode
 */
class ResourceException : public BaseException {
 public:
  /**
   * @brief Constructs a ResourceException
   *
   * @param message Resource error message
   * @param resource_name Optional name / identifier of the resource
   * @param location Source location (auto-captured)
   */
  explicit ResourceException(std::string_view message,
                             std::optional<std::string_view> resource_name = std::nullopt,
                             std::source_location location = std::source_location::current())
      : BaseException{message, ErrorSeverity::kError, location}, resource_name_{resource_name} {}

  /**
   * @brief Gets the name of the resource that caused the error
   * @return Optional string view of the resource name (std::nullopt if not specified)
   */
  [[nodiscard]] auto GetResourceName() const noexcept -> std::optional<std::string_view> {
    return resource_name_;
  }

 private:
  std::optional<std::string_view> resource_name_;  ///< Name / identifier of the resource
};

// Compile-time assertion to ensure ResourceException is nothrow copy constructible
static_assert(std::is_nothrow_copy_constructible_v<ResourceException>,
              "ResourceException must be nothrow copy constructible (SEI CERT ERR60-CPP)");

/**
 * @brief Exception for mathematical and calculation errors
 *
 * Specialized exception for mathematical operations, calculations, and numerical processing errors.
 * Stores the input value that caused the error.
 *
 * @code
 * // Division by zero with context
 * throw CalculationException{"Division by zero", 0.0};
 *
 * // Invalid mathematical operation
 * throw CalculationException{"Square root of negative number", -4.0};
 * @endcode
 */
class CalculationException : public BaseException {
 public:
  /**
   * @brief Constructs a CalculationException
   *
   * @param message Calculation error message
   * @param input_value The input value that caused the calculation error
   * @param location Source location (auto-captured)
   */
  explicit CalculationException(std::string_view message, double input_value = 0.0,
                                std::source_location location = std::source_location::current())
      : BaseException{message, ErrorSeverity::kError, location}, input_value_{input_value} {}

  /**
   * @brief Gets the input value that caused the calculation error
   * @return The problematic input value
   */
  [[nodiscard]] auto GetInputValue() const noexcept -> double { return input_value_; }

 private:
  double input_value_;  ///< Input value that caused the calculation error
};

// Compile-time assertion to ensure CalculationException is nothrow copy constructible
static_assert(std::is_nothrow_copy_constructible_v<CalculationException>,
              "CalculationException must be nothrow copy constructible (SEI CERT ERR60-CPP)");

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
  template <typename Func>
  static auto SafeExecute(Func &&func) noexcept -> bool {
    try {
      func();
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
  template <typename Func, typename DefaultValue>
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

/**
 * @brief Result type for operations that may fail
 *
 * A Result type that can hold either a successful value or an exception using std::variant.
 * Provides a functional programming approach to error handling as an alternative to exception
 * throwing. This implementation uses std::variant for type-safe storage and automatic
 * memory management.
 *
 * @tparam T Type of the successful result value
 *
 * @note While BaseException is nothrow copy constructible, std::variant<T, BaseException>
 * may not be considered nothrow copy constructible by some compiler implementations due to
 * conservative trait checking. This doesn't affect functionality but may impact some template
 * metaprogramming scenarios.
 *
 * @code
 * auto divide = [](double a, double b) -> Result<double> {
 *   if (b == 0.0) {
 *     return Result<double>{CalculationException{"Division by zero", b}};
 *   }
 *   return Result<double>{a / b};
 * };
 *
 * auto result = divide(10.0, 2.0);
 * if (result.HasValue()) {
 *   std::println("Result: {}", result.GetValue());
 * } else {
 *   std::println("Error: {}", result.GetException().what());
 * }
 * @endcode
 */
template <typename T>
class Result {
 public:
  /**
   * @brief Constructs a successful Result with a value
   * @param value The successful result value
   */
  explicit Result(T value) : data_{std::move(value)} {}

  /**
   * @brief Constructs a failed Result with an exception
   * @param exception The exception representing the failure
   */
  explicit Result(BaseException exception) : data_{std::move(exception)} {}

  /**
   * @brief Default copy constructor (provided by std::variant)
   */
  Result(const Result &) = default;

  /**
   * @brief Default move constructor (provided by std::variant)
   */
  Result(Result &&) = default;

  /**
   * @brief Default copy assignment operator (provided by std::variant)
   */
  auto operator=(const Result &) -> Result & = default;

  /**
   * @brief Default move assignment operator (provided by std::variant)
   */
  auto operator=(Result &&) -> Result & = default;

  /**
   * @brief Default destructor (provided by std::variant)
   */
  ~Result() = default;

  /**
   * @brief Checks if this Result contains a successful value
   * @return true if successful, false if contains an exception
   */
  [[nodiscard]] auto HasValue() const noexcept -> bool { return std::holds_alternative<T>(data_); }

  /**
   * @brief Gets the successful value (throws if Result contains exception)
   * @return Reference to the successful value
   * @throws BaseException if Result contains an exception instead of value
   */
  [[nodiscard]] auto GetValue() const -> const T & {
    if (const auto *value = std::get_if<T>(&data_)) {
      return *value;
    }
    throw std::get<BaseException>(data_);
  }

  /**
   * @brief Gets the successful value (throws if Result contains exception)
   * @return Reference to the successful value
   * @throws BaseException if Result contains an exception instead of value
   */
  [[nodiscard]] auto GetValue() -> T & {
    if (auto *value = std::get_if<T>(&data_)) {
      return *value;
    }
    throw std::get<BaseException>(data_);
  }

  /**
   * @brief Gets the exception (throws if Result contains successful value)
   * @return Reference to the exception
   * @throws BaseException if Result contains a value instead of exception
   */
  [[nodiscard]] auto GetException() const -> const BaseException & {
    if (const auto *exception = std::get_if<BaseException>(&data_)) {
      return *exception;
    }
    throw BaseException{"No exception in successful result"};
  }

  /**
   * @brief Visits the Result with a callable that handles both success and error cases
   * @tparam Visitor Callable type that can handle both T and BaseException
   * @param visitor The visitor function/lambda
   * @return The result of calling visitor with the contained value
   */
  template <typename Visitor>
  [[nodiscard]] auto Visit(Visitor &&visitor) const -> decltype(auto) {
    return std::visit(std::forward<Visitor>(visitor), data_);
  }

  /**
   * @brief Visits the Result with a callable that handles both success and error cases
   * @tparam Visitor Callable type that can handle both T and BaseException
   * @param visitor The visitor function/lambda
   * @return The result of calling visitor with the contained value
   */
  template <typename Visitor>
  [[nodiscard]] auto Visit(Visitor &&visitor) -> decltype(auto) {
    return std::visit(std::forward<Visitor>(visitor), data_);
  }

  /**
   * @brief Maps the successful value to a new type, preserving exceptions
   * @tparam Func Function type that transforms T to U
   * @param func Function to apply to the successful value
   * @return Result<U> with transformed value or original exception
   */
  template <typename Func>
  [[nodiscard]] auto Map(Func &&func) const -> Result<std::invoke_result_t<Func, const T &>> {
    using U = std::invoke_result_t<Func, const T &>;
    return Visit([&func](const auto &value) -> Result<U> {
      if constexpr (std::is_same_v<std::decay_t<decltype(value)>, T>) {
        return Result<U>{func(value)};
      } else {
        return Result<U>{value};
      }
    });
  }

  /**
   * @brief Chains Results together, applying a function that returns a Result
   * @tparam Func Function type that transforms T to Result<U>
   * @param func Function to apply to the successful value
   * @return Result<U> from the function or original exception
   */
  template <typename Func>
  [[nodiscard]] auto Then(Func &&func) const -> std::invoke_result_t<Func, const T &> {
    using ResultType = std::invoke_result_t<Func, const T &>;
    return Visit([&func](const auto &value) -> ResultType {
      if constexpr (std::is_same_v<std::decay_t<decltype(value)>, T>) {
        return func(value);
      } else {
        return ResultType{value};
      }
    });
  }

 private:
  std::variant<T, BaseException> data_;  ///< Variant holding either success value or exception
};

}  // namespace cpp_features::exceptions
