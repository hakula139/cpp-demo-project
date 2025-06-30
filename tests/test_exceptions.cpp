#include <cmath>
#include <cstddef>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "exceptions/custom_exception.hpp"

namespace {

using namespace cpp_features::exceptions;

// Helper functions for testing exception scenarios
namespace test_helpers {

/**
 * @brief Validates user age and throws ValidationException if invalid
 * @param age The age to validate
 * @param min_age Minimum required age
 * @throws ValidationException if age is below minimum
 */
auto ValidateAge(int age, int min_age = 18) -> void {
  if (age < min_age) {
    throw ValidationException{std::format("Age must be at least {} years old", min_age), "age"};
  }
}

/**
 * @brief Validates email format (simple check)
 * @param email The email to validate
 * @throws ValidationException if email format is invalid
 */
auto ValidateEmail(std::string_view email) -> void {
  auto at_pos = email.find('@');
  if (at_pos == 0 || at_pos == std::string_view::npos) {
    throw ValidationException{"Invalid email format", "email"};
  }
  auto dot_pos = email.find('.', at_pos + 2);  // Skip the first character after @
  if (dot_pos == std::string_view::npos) {
    throw ValidationException{"Invalid email format", "email"};
  }
}

/**
 * @brief Validates array bounds
 * @param index The index to validate
 * @param size The size of the array
 * @throws ValidationException if index is out of bounds
 */
auto ValidateArrayBounds(std::size_t index, std::size_t size) -> void {
  if (index >= size) {
    throw ValidationException{
        std::format("Array index out of bounds: {} >= {}", index, size),
        "index",
    };
  }
}

/**
 * @brief Simulates file access that may fail
 * @param filename The filename to access
 * @throws ResourceException if file cannot be accessed
 */
auto AccessFile(std::string_view filename) -> std::string {
  if (filename.empty() || filename == "non_existent.txt") {
    throw ResourceException{"File not found", filename};
  }
  if (filename == "no_permission.txt") {
    throw ResourceException{"Permission denied", filename};
  }
  return std::format("File content: {}", filename);
}

/**
 * @brief Simulates database connection
 * @param connection_string The database connection string
 * @throws ResourceException if connection fails
 */
auto ConnectToDatabase(std::string_view connection_string) -> void {
  if (connection_string.empty()) {
    throw ResourceException{"Empty connection string", "database"};
  }
  if (connection_string == "invalid_server") {
    throw ResourceException{"Cannot connect to database server", connection_string};
  }
}

/**
 * @brief Performs division with validation
 * @param dividend The number to divide
 * @param divisor The number to divide by
 * @return The result of division
 * @throws CalculationException if divisor is zero
 */
auto SafeDivide(double dividend, double divisor) -> double {
  if (divisor == 0.0) {
    throw CalculationException{"Division by zero", divisor};
  }
  return dividend / divisor;
}

/**
 * @brief Wraps SafeDivide in a Result type
 * @param dividend The number to divide
 * @param divisor The number to divide by
 * @return The result of division
 * @throws CalculationException if divisor is zero
 */
auto SafeDivideWrapper(double dividend, double divisor) -> Result<double> {
  try {
    return Result<double>{SafeDivide(dividend, divisor)};
  } catch (const CalculationException &e) {
    return Result<double>{e};
  }
}

/**
 * @brief Calculates square root with validation
 * @param value The value to calculate square root for
 * @return The square root
 * @throws CalculationException if value is negative
 */
auto SafeSquareRoot(double value) -> double {
  if (value < 0.0) {
    throw CalculationException{"Cannot calculate square root of negative number", value};
  }
  return std::sqrt(value);
}

/**
 * @brief Wraps SafeSquareRoot in a Result type
 * @param value The value to calculate square root for
 * @return The square root
 * @throws CalculationException if value is negative
 */
auto SafeSquareRootWrapper(double value) -> Result<double> {
  try {
    return Result<double>{SafeSquareRoot(value)};
  } catch (const CalculationException &e) {
    return Result<double>{e};
  }
}

}  // namespace test_helpers

TEST_CASE("BaseException creation and properties", "[exceptions][base]") {
  SECTION("Create BaseException with default severity") {
    std::string_view message{"Test error message"};
    BaseException ex{message};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetSeverity() == ErrorSeverity::kError);
    REQUIRE_FALSE(std::string_view{ex.GetLocation().file_name()}.empty());
    REQUIRE(ex.GetLocation().line() > 0);
  }

  SECTION("Create BaseException with custom severity") {
    std::string_view message{"Fatal error"};
    auto severity{ErrorSeverity::kFatal};
    BaseException ex{message, severity};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetSeverity() == severity);
  }

  SECTION("GetFormattedMessage contains source location") {
    std::string_view message{"Test message"};
    BaseException ex{message};
    auto formatted = ex.GetFormattedMessage();

    REQUIRE(formatted.find("test_exceptions.cpp") != std::string::npos);
    REQUIRE(formatted.find(message) != std::string::npos);
    REQUIRE(formatted.find(':') != std::string::npos);
  }

  SECTION("BaseException is nothrow copy constructible") {
    std::string_view message{"Original message"};
    BaseException original{message};

    REQUIRE_NOTHROW([&original, &message]() {
      BaseException copy{original};  // NOLINT(performance-unnecessary-copy-initialization)
      REQUIRE(std::string_view{copy.what()} == message);
    }());
  }
}

TEST_CASE("Exception message and severity combinations", "[exceptions][severity]") {
  SECTION("Test all severity levels") {
    std::vector severities = {
        ErrorSeverity::kTrace,   ErrorSeverity::kDebug, ErrorSeverity::kInfo,
        ErrorSeverity::kWarning, ErrorSeverity::kError, ErrorSeverity::kFatal,
    };

    for (auto severity : severities) {
      BaseException ex{"Test message", severity};
      REQUIRE(ex.GetSeverity() == severity);
      REQUIRE_FALSE(SeverityToString(severity).empty());
    }
  }

  SECTION("Exception hierarchy polymorphism") {
    std::vector<std::unique_ptr<BaseException>> exceptions;
    exceptions.emplace_back(std::make_unique<BaseException>("Base error"));
    exceptions.emplace_back(std::make_unique<ValidationException>("Validation error", "field"));
    exceptions.emplace_back(std::make_unique<ResourceException>("Resource error", "resource"));
    exceptions.emplace_back(std::make_unique<CalculationException>("Calculation error", 42.0));

    for (const auto &ex : exceptions) {
      REQUIRE_FALSE(std::string_view{ex->what()}.empty());
      REQUIRE(ex->GetSeverity() == ErrorSeverity::kError);
      REQUIRE_FALSE(ex->GetFormattedMessage().empty());
    }
  }
}

TEST_CASE("ValidationException functionality", "[exceptions][validation]") {
  SECTION("Create ValidationException without field name") {
    std::string_view message{"Invalid input"};
    ValidationException ex{message};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetSeverity() == ErrorSeverity::kError);
    REQUIRE_FALSE(ex.GetFieldName().has_value());
  }

  SECTION("Create ValidationException with field name") {
    std::string_view message{"Must be a valid email"};
    std::string_view field_name{"email"};
    ValidationException ex{message, field_name};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetFieldName().has_value());
    REQUIRE(ex.GetFieldName().value() == field_name);
  }

  SECTION("ValidationException can be caught as BaseException") {
    auto throw_func = []() { throw ValidationException{"Test validation error"}; };
    REQUIRE_THROWS_AS(throw_func(), BaseException);
  }

  SECTION("ValidationException preserves field name through inheritance") {
    std::string_view field_name{"phone"};
    try {
      throw ValidationException{"Invalid phone number", field_name};
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().value() == field_name);
    } catch (...) {
      FAIL("Should have caught ValidationException");
    }
  }
}

TEST_CASE("ResourceException functionality", "[exceptions][resource]") {
  SECTION("Create ResourceException without resource name") {
    std::string_view message{"Resource unavailable"};
    ResourceException ex{message};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE_FALSE(ex.GetResourceName().has_value());
  }

  SECTION("Create ResourceException with resource name") {
    std::string_view message{"File not found"};
    std::string_view resource_name{"/path/to/config.txt"};
    ResourceException ex{message, resource_name};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetResourceName().has_value());
    REQUIRE(ex.GetResourceName().value() == resource_name);
  }

  SECTION("ResourceException can be caught as BaseException") {
    auto throw_func = []() { throw ResourceException{"Database connection failed", "db_server"}; };

    REQUIRE_THROWS_AS(throw_func(), BaseException);
  }
}

TEST_CASE("CalculationException functionality", "[exceptions][calculation]") {
  SECTION("Create CalculationException with default input value") {
    std::string_view message{"Mathematical error"};
    CalculationException ex{message};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetInputValue() == 0.0);
  }

  SECTION("Create CalculationException with input value") {
    std::string_view message{"Division by zero"};
    auto input_value = 0.0;
    CalculationException ex{message, input_value};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetInputValue() == input_value);
  }

  SECTION("CalculationException with negative input") {
    std::string_view message{"Cannot calculate square root of negative number"};
    auto input_value = -4.0;
    CalculationException ex{message, input_value};

    REQUIRE(std::string_view{ex.what()} == message);
    REQUIRE(ex.GetInputValue() == input_value);
  }

  SECTION("CalculationException can be caught as BaseException") {
    auto throw_func = []() { throw CalculationException{"Overflow error", 1e100}; };

    REQUIRE_THROWS_AS(throw_func(), BaseException);
  }
}

TEST_CASE("ExceptionHandler SafeExecute operations", "[exceptions][handler]") {
  SECTION("SafeExecute with successful operation") {
    auto success_op = []() { return 42; };

    REQUIRE(ExceptionHandler::SafeExecute(success_op) == true);
  }

  SECTION("SafeExecute with throwing operation") {
    auto throwing_op = []() { throw ValidationException{"Test error"}; };

    REQUIRE(ExceptionHandler::SafeExecute(throwing_op) == false);
  }

  SECTION("SafeExecute with standard exception") {
    auto std_throwing_op = []() { throw std::runtime_error{"Standard error"}; };

    REQUIRE(ExceptionHandler::SafeExecute(std_throwing_op) == false);
  }

  SECTION("SafeExecuteWithDefault successful operation") {
    auto value = 42;
    auto success_op = [value]() { return value; };

    auto default_value = -1;
    auto result = ExceptionHandler::SafeExecuteWithDefault(success_op, default_value);
    REQUIRE(result == value);
  }

  SECTION("SafeExecuteWithDefault with throwing operation") {
    auto throwing_op = []() -> int { throw ValidationException{"Test error"}; };

    auto default_value = -1;
    auto result = ExceptionHandler::SafeExecuteWithDefault(throwing_op, default_value);
    REQUIRE(result == default_value);
  }

  SECTION("SafeExecuteWithDefault with different types") {
    auto throwing_string_op = []() -> std::string { throw ResourceException{"Test error"}; };

    std::string default_value{"default"};
    auto result = ExceptionHandler::SafeExecuteWithDefault(throwing_string_op, default_value);
    REQUIRE(result == default_value);
  }
}

TEST_CASE("Custom function exception scenarios", "[exceptions][functions]") {
  SECTION("Age validation throws ValidationException") {
    REQUIRE_NOTHROW(test_helpers::ValidateAge(25));
    REQUIRE_NOTHROW(test_helpers::ValidateAge(18));
    REQUIRE_THROWS_AS(test_helpers::ValidateAge(17), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateAge(16, 21), ValidationException);
    REQUIRE_NOTHROW(test_helpers::ValidateAge(16, 14));

    try {
      test_helpers::ValidateAge(15);
      FAIL("Should have thrown ValidationException");
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().value() == "age");
      REQUIRE(std::string_view{e.what()} == "Age must be at least 18 years old");
    }
  }

  SECTION("Email validation throws ValidationException") {
    REQUIRE_NOTHROW(test_helpers::ValidateEmail("user@example.com"));
    REQUIRE_NOTHROW(test_helpers::ValidateEmail("test.email@domain.org"));
    REQUIRE_THROWS_AS(test_helpers::ValidateEmail("invalid-email"), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateEmail("missing-at-sign.com"), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateEmail("test.email@missing-dot"), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateEmail("test@.invalid-domain"), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateEmail("@example.com"), ValidationException);

    try {
      test_helpers::ValidateEmail("no-domain@");
      FAIL("Should have thrown ValidationException");
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().value() == "email");
    }
  }

  SECTION("Array bounds validation throws ValidationException") {
    REQUIRE_NOTHROW(test_helpers::ValidateArrayBounds(0, 5));
    REQUIRE_NOTHROW(test_helpers::ValidateArrayBounds(4, 5));
    REQUIRE_THROWS_AS(test_helpers::ValidateArrayBounds(5, 5), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateArrayBounds(10, 3), ValidationException);
    REQUIRE_THROWS_AS(test_helpers::ValidateArrayBounds(0, 0), ValidationException);

    try {
      test_helpers::ValidateArrayBounds(7, 5);
      FAIL("Should have thrown ValidationException");
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().value() == "index");
      REQUIRE(std::string_view{e.what()} == "Array index out of bounds: 7 >= 5");
    }
  }

  SECTION("File access throws ResourceException") {
    REQUIRE_NOTHROW(test_helpers::AccessFile("valid_file.txt"));
    REQUIRE_THROWS_AS(test_helpers::AccessFile(""), ResourceException);
    REQUIRE_THROWS_AS(test_helpers::AccessFile("non_existent.txt"), ResourceException);
    REQUIRE_THROWS_AS(test_helpers::AccessFile("no_permission.txt"), ResourceException);

    std::string_view filename{"non_existent.txt"};
    try {
      test_helpers::AccessFile(filename);
      FAIL("Should have thrown ResourceException");
    } catch (const ResourceException &e) {
      REQUIRE(e.GetResourceName().value() == filename);
      REQUIRE(std::string_view{e.what()} == "File not found");
    }
  }

  SECTION("Database connection throws ResourceException") {
    REQUIRE_NOTHROW(test_helpers::ConnectToDatabase("valid_connection_string"));
    REQUIRE_THROWS_AS(test_helpers::ConnectToDatabase(""), ResourceException);
    REQUIRE_THROWS_AS(test_helpers::ConnectToDatabase("invalid_server"), ResourceException);

    std::string_view connection_string{"invalid_server"};
    try {
      test_helpers::ConnectToDatabase(connection_string);
      FAIL("Should have thrown ResourceException");
    } catch (const ResourceException &e) {
      REQUIRE(e.GetResourceName().value() == connection_string);
      REQUIRE(std::string_view{e.what()} == "Cannot connect to database server");
    }
  }

  SECTION("Safe division throws CalculationException") {
    REQUIRE_NOTHROW(test_helpers::SafeDivide(10.0, 2.0));
    REQUIRE(test_helpers::SafeDivide(15.0, 3.0) == 5.0);
    REQUIRE_THROWS_AS(test_helpers::SafeDivide(10.0, 0.0), CalculationException);

    try {
      test_helpers::SafeDivide(5.0, 0.0);
      FAIL("Should have thrown CalculationException");
    } catch (const CalculationException &e) {
      REQUIRE(e.GetInputValue() == 0.0);
      REQUIRE(std::string_view{e.what()} == "Division by zero");
    }
  }

  SECTION("Safe square root throws CalculationException") {
    REQUIRE_NOTHROW(test_helpers::SafeSquareRoot(16.0));
    REQUIRE(test_helpers::SafeSquareRoot(9.0) == 3.0);
    REQUIRE_THROWS_AS(test_helpers::SafeSquareRoot(-1.0), CalculationException);

    try {
      test_helpers::SafeSquareRoot(-4.0);
      FAIL("Should have thrown CalculationException");
    } catch (const CalculationException &e) {
      REQUIRE(e.GetInputValue() == -4.0);
      REQUIRE(std::string_view{e.what()} == "Cannot calculate square root of negative number");
    }
  }
}

TEST_CASE("Result type success cases", "[exceptions][result]") {
  SECTION("Create successful Result") {
    auto value = 42;
    Result<int> result{value};

    REQUIRE(result.HasValue() == true);
    REQUIRE(result.GetValue() == value);
  }

  SECTION("Create successful Result with string") {
    std::string value{"success"};
    Result<std::string> result{value};

    REQUIRE(result.HasValue() == true);
    REQUIRE(result.GetValue() == value);
  }

  SECTION("Result Map operation") {
    Result<int> result{10};

    auto mapped = result.Map([](int x) { return x * 2; });
    REQUIRE(mapped.HasValue() == true);
    REQUIRE(mapped.GetValue() == 20);
  }

  SECTION("Result Map chaining") {
    Result<int> result{5};

    auto chained = result.Map([](int x) { return x * 2; }).Map([](int x) { return x + 3; });
    REQUIRE(chained.HasValue() == true);
    REQUIRE(chained.GetValue() == 13);  // (5 * 2) + 3
  }
}

TEST_CASE("Result type error cases", "[exceptions][result]") {
  SECTION("Create failed Result") {
    std::string_view message{"Test error"};
    Result<int> result{ValidationException{message}};

    REQUIRE(result.HasValue() == false);
    REQUIRE_THROWS_AS(result.GetValue(), BaseException);
    REQUIRE(std::string_view{result.GetException().what()} == message);
  }

  SECTION("Result with error Map does not execute") {
    Result<int> result{CalculationException{"Error", 0.0}};

    auto mapped = result.Map([](int x) { return x * 2; });
    REQUIRE(mapped.HasValue() == false);
  }

  SECTION("Result Then operation with success") {
    Result<int> result{10};

    auto then_result = result.Then([](int x) { return Result<double>{x * 1.5}; });
    REQUIRE(then_result.HasValue() == true);
    REQUIRE(then_result.GetValue() == 15.0);
  }

  SECTION("Result Then operation propagates error") {
    Result<int> result{ValidationException{"Initial error"}};

    auto then_result = result.Then([](int x) { return Result<double>{x * 1.5}; });
    REQUIRE(then_result.HasValue() == false);
  }
}

TEST_CASE("Result Visit pattern", "[exceptions][result][visit]") {
  SECTION("Visit successful Result") {
    auto value = 42;
    Result<int> result{value};

    auto visited_value = false;
    result.Visit([&visited_value, value](const auto &current_value) {
      if constexpr (std::is_same_v<std::decay_t<decltype(current_value)>, int>) {
        visited_value = true;
        REQUIRE(current_value == value);
      }
    });
    REQUIRE(visited_value == true);
  }

  SECTION("Visit failed Result") {
    std::string_view message{"Test error"};
    Result<int> result{ValidationException{message}};

    auto visited_exception = false;
    result.Visit([&visited_exception, &message](const auto &current_value) {
      if constexpr (std::is_same_v<std::decay_t<decltype(current_value)>, BaseException>) {
        visited_exception = true;
        REQUIRE(std::string_view{current_value.what()} == message);
      }
    });
    REQUIRE(visited_exception == true);
  }
}

TEST_CASE("Complex Result operation chains", "[exceptions][result][integration]") {
  SECTION("Successful operation chain") {
    auto result = test_helpers::SafeDivideWrapper(16.0, 4.0)
                      .Then([](double x) { return test_helpers::SafeSquareRootWrapper(x); })
                      .Map([](double x) { return x * 3.0; });

    REQUIRE(result.HasValue() == true);
    REQUIRE(result.GetValue() == 6.0);  // sqrt(16 / 4) * 3
  }

  SECTION("Error propagation in operation chain") {
    // Chain that starts with error should propagate error
    auto result = test_helpers::SafeDivideWrapper(10.0, 0.0)
                      .Map([](double x) { return x * 2.0; })
                      .Map([](double x) { return x + 1.0; });

    REQUIRE(result.HasValue() == false);
    REQUIRE(std::string_view{result.GetException().what()} == "Division by zero");
  }
}

}  // namespace
