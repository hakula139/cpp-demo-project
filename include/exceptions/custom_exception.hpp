/**
 * @file custom_exception.hpp
 * @brief Custom exception classes with enhanced error handling capabilities
 *
 * This module provides a comprehensive exception hierarchy with additional context information such
 * as source location, error severity levels, and specialized exception types for common error
 * scenarios.
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

}  // namespace cpp_features::exceptions
