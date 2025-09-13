/**
 * @file callable_concepts.hpp
 * @brief Callable-related concepts for C++ template constraints
 *
 * This file contains concept definitions for callable types, providing compile-time constraints
 * for template parameters that need to work with functions, lambdas, functors, and other
 * callable objects.
 */

#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace cpp_features::concepts {

/**
 * @brief Concept for types that can be called without arguments
 *
 * @tparam Func The callable type to check
 *
 * This concept ensures that a type can be invoked without any arguments.
 * It accepts functions, lambdas, functors, and any other callable objects that can be called with
 * no parameters. The return type is not constrained, allowing for flexible usage in various
 * contexts.
 *
 * Types that satisfy this concept include:
 *
 * - Function pointers: void (*)()
 * - Lambda expressions: []() { ... }
 * - Function objects with operator()
 * - std::function<void()> and similar
 * - Member function pointers (when properly bound)
 *
 * @code
 * template <NullaryCallable Func>
 * void ExecuteCallback(Func &&callback) {
 *   callback();
 * }
 *
 * ExecuteCallback([]() { std::println("Hello, World!"); });
 * ExecuteCallback(std::function<void()>{[]() { return 42; }});
 * @endcode
 */
template <typename Func>
concept NullaryCallable = std::invocable<Func>;

/**
 * @brief Concept for types that can be called without arguments and return void
 *
 * @tparam Func The callable type to check
 *
 * This concept is more restrictive than NullaryCallable, requiring that the callable returns void.
 * This is useful for contexts where side effects are expected but return values are not needed or
 * should be discarded.
 *
 * @code
 * template <VoidNullaryCallable Func>
 * void ExecuteAction(Func &&action) {
 *   action();  // Guaranteed to return void
 * }
 *
 * ExecuteAction([]() { std::println("Action executed"); });
 * @endcode
 */
template <typename Func>
concept VoidNullaryCallable =
    NullaryCallable<Func> && std::same_as<std::invoke_result_t<Func>, void>;

/**
 * @brief Concept for timer callback functions
 *
 * @tparam Func The callable type to check for timer callback suitability
 *
 * This concept ensures that a type can be used as a timer callback function, which should accept a
 * single std::int64_t parameter (elapsed time in nanoseconds) and return void. This is specifically
 * designed for use with ScopedTimer callbacks.
 *
 * @code
 * template <TimerCallback Func>
 * void RegisterTimerCallback(Func &&callback) {
 *   // Store callback to be called with elapsed time
 *   stored_callback_ = std::forward<Func>(callback);
 * }
 *
 * RegisterTimerCallback([](std::int64_t ns) {
 *   std::println("Operation took {}ns", ns);
 * });
 * @endcode
 */
template <typename Func>
concept TimerCallback = std::invocable<Func, std::int64_t> &&
                        std::same_as<std::invoke_result_t<Func, std::int64_t>, void>;

/**
 * @brief Concept for transformation functions that can be used with container transformations
 *
 * @tparam Func The type of the transformation function
 * @tparam Input The type of the input elements
 * @tparam Output The type of the output elements
 *
 * This concept ensures that a type can be used as a transformation function for container
 * operations. The function must be invocable with an Input type and return an Output type.
 *
 * @code
 * template <TransformFunction<int, int> Func>
 * auto transform_elements(const std::vector<int> &vec, Func func) {
 *   return vec | std::views::transform(func);
 * }
 *
 * auto square = [](int n) { return n * n; };
 * auto squared_numbers = transform_elements(numbers, square);
 * @endcode
 */
template <typename Func, typename Input, typename Output = Input>
concept TransformFunction =
    std::invocable<Func, Input> && std::convertible_to<std::invoke_result_t<Func, Input>, Output>;

/**
 * @brief Concept for predicate functions that can be used with container filtering
 *
 * @tparam Predicate The predicate type to check
 * @tparam T The type of elements the predicate will be called with
 *
 * This concept ensures that a type can be used as a predicate for filtering operations.
 * The predicate must be invocable with a const reference to T and return a type that
 * is convertible to bool.
 *
 * @code
 * template <typename T, PredicateFor<T> Predicate>
 * auto filter_elements(const std::vector<T> &vec, Predicate predicate) {
 *   return vec | std::views::filter(predicate);
 * }
 *
 * auto is_even = [](int n) { return n % 2 == 0; };
 * auto filtered_numbers = filter_elements(numbers, is_even);
 * @endcode
 */
template <typename Predicate, typename T>
concept PredicateFor = TransformFunction<Predicate, const T &, bool>;

}  // namespace cpp_features::concepts
