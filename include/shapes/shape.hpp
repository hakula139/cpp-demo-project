#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <expected>
#include <iostream>
#include <cstdint>

#include "concepts/arithmetic_concepts.hpp"

namespace cpp_features::shapes {

enum class ShapeError : std::uint8_t {  // NOLINT(performance-enum-size)
    kInvalidRadius,
    kInvalidDimensions,
    kNegativeValue
};

class Shape {
protected:
    std::string name_;  // NOLINT(misc-non-private-member-variables-in-classes)

public:
    explicit Shape(std::string_view name) : name_(name) {}
    virtual ~Shape() = default;

    [[nodiscard]] virtual auto GetArea() const -> std::expected<double, ShapeError> = 0;
    [[nodiscard]] virtual auto GetPerimeter() const -> std::expected<double, ShapeError> = 0;

    virtual void Draw() const {
        std::cout << "Drawing " << name_ << "\n";
    }

    [[nodiscard]] constexpr auto GetName() const noexcept -> std::string_view {
        return name_;
    }

    auto operator<=>(const Shape& other) const = default;

    Shape(Shape&&) noexcept = default;
    auto operator=(Shape&&) noexcept -> Shape& = default;

    Shape(const Shape&) = default;
    auto operator=(const Shape&) -> Shape& = default;
};

template<typename ShapeType, cpp_features::concepts::ArithmeticType... Args>
    requires std::derived_from<ShapeType, Shape>
[[nodiscard]] auto CreateShape(Args&&... args) -> std::unique_ptr<ShapeType> {
    return std::make_unique<ShapeType>(std::forward<Args>(args)...);
}

}  // namespace cpp_features::shapes