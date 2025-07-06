/**
 * @file unique_resource.hpp
 * @brief RAII wrapper for unique_ptr with additional functionality
 *
 * This module provides a RAII wrapper for unique_ptr with additional functionality.
 */

#pragma once

#include <memory>
#include <utility>

namespace cpp_features::memory {

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
