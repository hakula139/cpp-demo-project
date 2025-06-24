#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <concepts>

#include "concepts/arithmetic_concepts.hpp"

namespace cpp_features::memory {

template<typename T>
concept Destructible = std::destructible<T>;

template<typename T>
concept CopyConstructible = std::copy_constructible<T>;

class ResourceManager {
 public:
  ResourceManager() = default;
  ~ResourceManager() = default;

  ResourceManager(const ResourceManager&) = delete;
  auto operator=(const ResourceManager&) -> ResourceManager& = delete;
  ResourceManager(ResourceManager&&) noexcept = default;
  auto operator=(ResourceManager&&) noexcept -> ResourceManager& = default;

  template<Destructible T, typename... Args>
  [[nodiscard]] auto CreateUnique(Args&&... args) -> std::unique_ptr<T> {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template<Destructible T, typename... Args>
  [[nodiscard]] auto CreateShared(Args&&... args) -> std::shared_ptr<T> {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template<typename T>
  void RegisterCleanup(std::function<void()> cleanup_func) {
    cleanup_functions_.push_back(std::move(cleanup_func));
  }

  void ExecuteCleanup();

 private:
  std::vector<std::function<void()>> cleanup_functions_;
};

template<typename T>
class UniqueResource {
 public:
  explicit UniqueResource(std::unique_ptr<T> resource)
    : resource_(std::move(resource)) {}

  [[nodiscard]] auto Get() const noexcept -> T* {
    return resource_.get();
  }

  [[nodiscard]] auto Release() noexcept -> std::unique_ptr<T> {
    return std::move(resource_);
  }

  auto operator->() const noexcept -> T* {
    return resource_.get();
  }

  auto operator*() const noexcept -> T& {
    return *resource_;
  }

 private:
  std::unique_ptr<T> resource_;
};

void DemonstrateUniquePtr();

void DemonstrateSharedPtr();

void DemonstrateWeakPtr();

void DemonstrateCustomDeleter();

void DemonstrateResourceManagement();

template<typename T, typename Deleter = std::default_delete<T>>
[[nodiscard]] auto MakeUniqueWithDeleter(T* ptr, Deleter deleter = Deleter{})
  -> std::unique_ptr<T, Deleter> {
  return std::unique_ptr<T, Deleter>(ptr, deleter);
}

}  // namespace cpp_features::memory