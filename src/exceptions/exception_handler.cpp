/**
 * @file exception_handler.cpp
 * @brief Implementation of exception handler utility class
 */

#include "exceptions/exception_handler.hpp"

#include <exception>
#include <print>

#include "exceptions/custom_exception.hpp"

namespace cpp_features::exceptions {

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
