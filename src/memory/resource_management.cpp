#include "memory/resource_management.hpp"

#include <exception>
#include <print>
#include <ranges>

namespace cpp_features::memory {

void ResourceManager::ExecuteCleanup() {
  std::println("Executing {} cleanup functions", cleanup_functions_.size());
  for (auto &cleanup_function : std::ranges::reverse_view(cleanup_functions_)) {
    cleanup_function();  // May throw - caller handles exceptions
  }
  cleanup_functions_.clear();
}

void ResourceManager::ExecuteCleanupNoexcept() noexcept {
  std::println("Executing {} cleanup functions (noexcept)", cleanup_functions_.size());
  for (auto &cleanup_function : std::ranges::reverse_view(cleanup_functions_)) {
    try {
      cleanup_function();
    } catch (const std::exception &e) {
      // Log the exception but continue with other cleanup functions
      std::println("Warning: Cleanup function threw exception: {}", e.what());
    } catch (...) {
      // Catch any other exceptions
      std::println("Warning: Cleanup function threw unknown exception");
    }
  }
  cleanup_functions_.clear();
}

}  // namespace cpp_features::memory
