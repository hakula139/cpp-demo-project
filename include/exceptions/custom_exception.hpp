#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <source_location>
#include <format>

namespace cpp_features::exceptions {

enum class ErrorSeverity {
  kInfo,
  kWarning,
  kError,
  kCritical
};

class BaseException : public std::exception {
 public:
  explicit BaseException(
    std::string_view message,
    ErrorSeverity severity = ErrorSeverity::kError,
    std::source_location location = std::source_location::current()
  ) : message_(message), severity_(severity), location_(location) {}

  [[nodiscard]] const char* what() const noexcept override {
    return message_.c_str();
  }

  [[nodiscard]] auto GetSeverity() const noexcept -> ErrorSeverity {
    return severity_;
  }

  [[nodiscard]] auto GetLocation() const noexcept -> const std::source_location& {
    return location_;
  }

  [[nodiscard]] auto GetFormattedMessage() const -> std::string {
    return std::format("{}:{} in {}: {}",
                      location_.file_name(),
                      location_.line(),
                      location_.function_name(),
                      message_);
  }

 private:
  std::string message_;
  ErrorSeverity severity_;
  std::source_location location_;
};

class ValidationException : public BaseException {
 public:
  explicit ValidationException(
    std::string_view message,
    std::string_view field_name = "",
    std::source_location location = std::source_location::current()
  ) : BaseException(message, ErrorSeverity::kError, location),
      field_name_(field_name) {}

  [[nodiscard]] auto GetFieldName() const noexcept -> std::string_view {
    return field_name_;
  }

 private:
  std::string field_name_;
};

class ResourceException : public BaseException {
 public:
  explicit ResourceException(
    std::string_view message,
    std::string_view resource_name = "",
    std::source_location location = std::source_location::current()
  ) : BaseException(message, ErrorSeverity::kError, location),
      resource_name_(resource_name) {}

  [[nodiscard]] auto GetResourceName() const noexcept -> std::string_view {
    return resource_name_;
  }

 private:
  std::string resource_name_;
};

class CalculationException : public BaseException {
 public:
  explicit CalculationException(
    std::string_view message,
    double input_value = 0.0,
    std::source_location location = std::source_location::current()
  ) : BaseException(message, ErrorSeverity::kError, location),
      input_value_(input_value) {}

  [[nodiscard]] auto GetInputValue() const noexcept -> double {
    return input_value_;
  }

 private:
  double input_value_;
};

class ExceptionHandler {
 public:
  template<typename Func>
  static auto SafeExecute(Func&& func) noexcept -> bool {
    try {
      func();
      return true;
    } catch (const BaseException& e) {
      LogException(e);
      return false;
    } catch (const std::exception& e) {
      LogStandardException(e);
      return false;
    } catch (...) {
      LogUnknownException();
      return false;
    }
  }

  template<typename Func, typename DefaultValue>
  static auto SafeExecuteWithDefault(Func&& func, DefaultValue&& default_value)
    -> std::decay_t<DefaultValue> {
    try {
      return func();
    } catch (...) {
      return std::forward<DefaultValue>(default_value);
    }
  }

  static void LogException(const BaseException& exception);
  static void LogStandardException(const std::exception& exception);
  static void LogUnknownException();

 private:
  static auto SeverityToString(ErrorSeverity severity) -> std::string_view;
};

void DemonstrateCustomExceptions();

void DemonstrateExceptionHandling();

void DemonstrateSourceLocation();

template<typename T>
class Result {
 public:
  Result(T value) : value_(std::move(value)), has_value_(true) {}
  Result(BaseException exception) : exception_(std::move(exception)), has_value_(false) {}

  [[nodiscard]] auto HasValue() const noexcept -> bool {
    return has_value_;
  }

  [[nodiscard]] auto GetValue() const -> const T& {
    if (!has_value_) {
      throw exception_;
    }
    return value_;
  }

  [[nodiscard]] auto GetException() const -> const BaseException& {
    if (has_value_) {
      throw BaseException("No exception in successful result");
    }
    return exception_;
  }

 private:
  union {
    T value_;
    BaseException exception_;
  };
  bool has_value_;
};

}  // namespace cpp_features::exceptions