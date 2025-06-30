/**
 * @file resource_management.hpp
 * @brief Modern C++ resource management utilities and RAII patterns
 *
 * This module provides resource management utilities, RAII patterns, and memory
 * safety demonstrations. It showcases resource lifecycle management, automatic cleanup,
 * and modern C++ memory management best practices.
 *
 * The module includes:
 *
 * - Resource management utilities with automatic cleanup
 * - Enhanced smart pointer wrappers with additional functionality
 * - Custom deleter support for specialized resource handling
 * - Type-safe resource creation with concepts
 */

#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "concepts/concepts.hpp"

namespace cpp_features::memory {

using cpp_features::concepts::CopyableType;
using cpp_features::concepts::DestructibleType;

/**
 * @brief Resource manager class for automatic resource lifecycle management
 *
 * The ResourceManager class provides centralized resource creation and cleanup management.
 * It allows creating smart pointers of various types and registering cleanup functions that will be
 * executed when explicitly requested or when the manager is destroyed.
 *
 * This class follows RAII principles and provides exception-safe resource management.
 * It is move-only to prevent accidental copying of resource ownership.
 *
 * @code
 * ResourceManager manager;
 *
 * // Create resources through the manager
 * auto unique_resource = manager.CreateUnique<MyClass>(constructor_args...);
 * auto shared_resource = manager.CreateShared<MyClass>(constructor_args...);
 *
 * // Register cleanup functions
 * manager.RegisterCleanup<int>([]() { cleanup_database_connections(); });
 * manager.RegisterCleanup<int>([]() { flush_log_buffers(); });
 *
 * // Scope ends - automatic cleanup happens here
 * @endcode
 */
class ResourceManager {
 public:
  /**
   * @brief Default constructor
   *
   * Creates an empty resource manager with no registered cleanup functions.
   */
  ResourceManager() = default;

  /**
   * @brief Destructor with automatic cleanup
   *
   * Automatically executes all registered cleanup functions in reverse order.
   * If any cleanup function throws an exception, it is caught and logged, but does not prevent
   * other cleanup functions from executing.
   *
   * To disable automatic cleanup, call DisableAutomaticCleanup() before destruction.
   */
  ~ResourceManager() noexcept {
    if (enable_automatic_cleanup_) {
      ExecuteCleanupNoexcept();
    }
  }

  /**
   * @brief Deleted copy constructor
   *
   * ResourceManager is move-only to prevent accidental copying of resource ownership.
   */
  ResourceManager(const ResourceManager &) = delete;

  /**
   * @brief Deleted copy assignment operator
   *
   * ResourceManager is move-only to prevent accidental copying of resource ownership.
   */
  auto operator=(const ResourceManager &) -> ResourceManager & = delete;

  /**
   * @brief Default move constructor
   *
   * Allows moving ResourceManager instances, transferring ownership of cleanup functions.
   */
  ResourceManager(ResourceManager &&) noexcept = default;

  /**
   * @brief Default move assignment operator
   *
   * Allows moving ResourceManager instances, transferring ownership of cleanup functions.
   */
  auto operator=(ResourceManager &&) noexcept -> ResourceManager & = default;

  /**
   * @brief Create a unique_ptr for the specified type
   *
   * @tparam T The type to create. Must satisfy the DestructibleType concept.
   * @tparam Args Types of constructor arguments
   * @param args Arguments to forward to the constructor of T
   * @return std::unique_ptr<T> pointing to the newly created object
   *
   * Creates a new object of type T using std::make_unique and forwards all constructor arguments.
   * The type T must be destructible to ensure proper cleanup.
   *
   * @code
   * auto circle = manager.CreateUnique<shapes::Circle>(5.0);
   * auto rectangle = manager.CreateUnique<shapes::Rectangle>(3.0, 4.0);
   * @endcode
   */
  template <DestructibleType T, typename... Args>
  [[nodiscard]] auto CreateUnique(Args &&...args) -> std::unique_ptr<T> {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  /**
   * @brief Create a shared_ptr for the specified type
   *
   * @tparam T The type to create. Must satisfy the DestructibleType concept.
   * @tparam Args Types of constructor arguments
   * @param args Arguments to forward to the constructor of T
   * @return std::shared_ptr<T> pointing to the newly created object
   *
   * Creates a new object of type T using std::make_shared and forwards all constructor arguments.
   * The type T must be destructible to ensure proper cleanup.
   *
   * @code
   * auto shared_circle = manager.CreateShared<shapes::Circle>(5.0);
   * auto shared_rect = manager.CreateShared<shapes::Rectangle>(3.0, 4.0);
   * @endcode
   */
  template <DestructibleType T, typename... Args>
  [[nodiscard]] auto CreateShared(Args &&...args) -> std::shared_ptr<T> {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  /**
   * @brief Register a cleanup function to be executed later
   *
   * @tparam T Type parameter for template specialization (can be any type)
   * @param cleanup_func Function to be called during cleanup
   *
   * Registers a cleanup function that will be executed when ExecuteCleanup() is called.
   * Cleanup functions are executed in the reverse order they were registered.
   *
   * @code
   * manager.RegisterCleanup<int>([]() { std::println("Cleaning up resources"); });
   * manager.RegisterCleanup<int>([]() { close_file_handles(); });
   * @endcode
   */
  template <typename T>
  void RegisterCleanup(std::function<void()> cleanup_func) {
    cleanup_functions_.push_back(std::move(cleanup_func));
  }

  /**
   * @brief Execute all registered cleanup functions
   *
   * Executes all cleanup functions in the reverse order they were registered, then clears the
   * cleanup function list. This method can throw exceptions if cleanup functions throw.
   */
  void ExecuteCleanup();

  /**
   * @brief Execute cleanup functions without throwing exceptions
   *
   * Same as ExecuteCleanup() but catches and logs any exceptions thrown by cleanup functions.
   * This method is noexcept and safe to call from destructors.
   */
  void ExecuteCleanupNoexcept() noexcept;

  /**
   * @brief Disable automatic cleanup in destructor
   *
   * Call this method if you want to manually control when cleanup happens and prevent the
   * destructor from automatically executing cleanup functions.
   */
  void DisableAutomaticCleanup() noexcept { enable_automatic_cleanup_ = false; }

 private:
  std::vector<std::function<void()>> cleanup_functions_;  ///< Registered cleanup functions
  bool enable_automatic_cleanup_{true};  ///< Whether to automatically cleanup in destructor
};

/**
 * @brief RAII wrapper for unique_ptr with additional functionality
 *
 * @tparam T The type of object managed by the unique_ptr
 *
 * UniqueResource provides a wrapper around std::unique_ptr with additional convenience methods and
 * RAII guarantees. It maintains exclusive ownership of the wrapped resource and provides safe
 * access methods.
 *
 * The class provides pointer-like semantics through operator-> and operator*, making it a drop-in
 * replacement for raw unique_ptr usage in many cases.
 *
 * @code
 * auto unique_ptr = std::make_unique<shapes::Circle>(5.0);
 * UniqueResource<shapes::Circle> wrapped_circle{std::move(unique_ptr)};
 *
 * // Use like a regular pointer
 * auto radius = wrapped_circle->GetRadius();
 * auto area = (*wrapped_circle).GetArea();
 *
 * // Release ownership when needed
 * auto released_ptr = wrapped_circle.Release();
 * @endcode
 */
template <typename T>
class UniqueResource {
 public:
  /**
   * @brief Construct UniqueResource from a unique_ptr
   *
   * @param resource The unique_ptr to wrap and manage
   *
   * Takes ownership of the provided unique_ptr.
   * The original unique_ptr will be moved and become invalid.
   */
  explicit UniqueResource(std::unique_ptr<T> resource) : resource_{std::move(resource)} {}

  /**
   * @brief Get raw pointer to the managed object
   *
   * @return T* Raw pointer to the managed object, or nullptr if no object is managed
   *
   * Returns a raw pointer to the managed object without transferring ownership.
   * The returned pointer remains valid only as long as the UniqueResource exists.
   */
  [[nodiscard]] auto Get() const noexcept -> T * { return resource_.get(); }

  /**
   * @brief Release ownership of the managed object
   *
   * @return std::unique_ptr<T> The released unique_ptr
   *
   * Releases ownership of the managed object and returns the underlying unique_ptr.
   * After this call, the UniqueResource no longer manages any object.
   */
  [[nodiscard]] auto Release() noexcept -> std::unique_ptr<T> { return std::move(resource_); }

  /**
   * @brief Access the managed object via pointer semantics
   *
   * @return T* Pointer to the managed object
   *
   * Provides pointer-like access to the managed object.
   * Behavior is undefined if no object is currently managed.
   */
  auto operator->() const noexcept -> T * { return resource_.get(); }

  /**
   * @brief Access the managed object via reference semantics
   *
   * @return T& Reference to the managed object
   *
   * Provides reference-like access to the managed object.
   * Behavior is undefined if no object is currently managed.
   */
  auto operator*() const noexcept -> T & { return *resource_; }

 private:
  std::unique_ptr<T> resource_;  ///< The managed unique_ptr resource
};

/**
 * @brief Create a unique_ptr with a custom deleter
 *
 * @tparam T The type of object to manage
 * @tparam Deleter The type of custom deleter function
 * @param ptr Raw pointer to the object to manage
 * @param deleter Custom deleter function (defaults to std::default_delete<T>)
 * @return std::unique_ptr<T, Deleter> A unique_ptr with the specified custom deleter
 *
 * Creates a unique_ptr that uses a custom deleter function instead of the default delete operator.
 * This is useful for managing resources that require special cleanup procedures.
 *
 * @code
 * auto custom_deleter = [](MyClass *ptr) {
 *   std::println("Custom cleanup for {}", ptr->GetName());
 *   delete ptr;
 * };
 *
 * auto ptr = MakeUniqueWithDeleter(new MyClass{"test"}, custom_deleter);
 * @endcode
 */
template <typename T, typename Deleter = std::default_delete<T>>
[[nodiscard]] auto MakeUniqueWithDeleter(T *ptr, Deleter deleter = Deleter{})
    -> std::unique_ptr<T, Deleter> {
  return std::unique_ptr<T, Deleter>(ptr, deleter);
}

}  // namespace cpp_features::memory
