/**
 * @file result.hpp
 * @brief Result type for operations that may fail
 *
 * This module provides a Result type that can hold either a successful value or an exception using
 * std::variant.
 */

#pragma once

#include <type_traits>
#include <utility>
#include <variant>

#include "custom_exception.hpp"

namespace cpp_features::exceptions {

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
    throw BaseException{std::get<BaseException>(data_)};
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
