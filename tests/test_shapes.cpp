#include <numbers>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "shapes/shape.hpp"

using namespace cpp_features::shapes;

TEST_CASE("Circle tests", "[shapes][circle]") {
  SECTION("Valid circle creation and calculations") {
    Circle circle(5.0);

    REQUIRE(circle.GetRadius() == 5.0);
    REQUIRE(circle.GetName() == "Circle");

    auto area_result = circle.GetArea();
    REQUIRE(area_result.has_value());
    REQUIRE(area_result.value() == Catch::Approx(std::numbers::pi * 25.0));

    auto perimeter_result = circle.GetPerimeter();
    REQUIRE(perimeter_result.has_value());
    REQUIRE(perimeter_result.value() == Catch::Approx(2.0 * std::numbers::pi * 5.0));
  }

  SECTION("Zero radius handling") {
    Circle zero_circle(0.0);

    auto area_result = zero_circle.GetArea();
    REQUIRE_FALSE(area_result.has_value());
    REQUIRE(area_result.error() == ShapeError::kInvalidRadius);

    auto perimeter_result = zero_circle.GetPerimeter();
    REQUIRE_FALSE(perimeter_result.has_value());
    REQUIRE(perimeter_result.error() == ShapeError::kInvalidRadius);
  }

  SECTION("Negative radius handling") {
    Circle negative_circle(-3.0);

    auto area_result = negative_circle.GetArea();
    REQUIRE_FALSE(area_result.has_value());
    REQUIRE(area_result.error() == ShapeError::kNegativeValue);

    auto perimeter_result = negative_circle.GetPerimeter();
    REQUIRE_FALSE(perimeter_result.has_value());
    REQUIRE(perimeter_result.error() == ShapeError::kNegativeValue);
  }

  SECTION("Circle comparison") {
    Circle circle1(5.0);
    Circle circle2(5.0);
    Circle circle3(3.0);

    REQUIRE(circle1 == circle2);
    REQUIRE_FALSE(circle1 == circle3);
    REQUIRE(circle1 > circle3);
  }
}

TEST_CASE("Rectangle tests", "[shapes][rectangle]") {
  SECTION("Valid rectangle creation and calculations") {
    Rectangle rect(4.0, 6.0);

    REQUIRE(rect.GetWidth() == 4.0);
    REQUIRE(rect.GetHeight() == 6.0);
    REQUIRE(rect.GetName() == "Rectangle");
    REQUIRE_FALSE(rect.IsSquare());

    auto area_result = rect.GetArea();
    REQUIRE(area_result.has_value());
    REQUIRE(area_result.value() == 24.0);

    auto perimeter_result = rect.GetPerimeter();
    REQUIRE(perimeter_result.has_value());
    REQUIRE(perimeter_result.value() == 20.0);
  }

  SECTION("Square detection") {
    Rectangle square(5.0, 5.0);
    REQUIRE(square.IsSquare());

    auto area_result = square.GetArea();
    REQUIRE(area_result.has_value());
    REQUIRE(area_result.value() == 25.0);
  }

  SECTION("Zero dimension handling") {
    Rectangle zero_width(0.0, 5.0);
    Rectangle zero_height(5.0, 0.0);

    auto area_result1 = zero_width.GetArea();
    REQUIRE_FALSE(area_result1.has_value());
    REQUIRE(area_result1.error() == ShapeError::kInvalidDimensions);

    auto area_result2 = zero_height.GetArea();
    REQUIRE_FALSE(area_result2.has_value());
    REQUIRE(area_result2.error() == ShapeError::kInvalidDimensions);
  }

  SECTION("Negative dimension handling") {
    Rectangle negative_rect(-3.0, 5.0);

    auto area_result = negative_rect.GetArea();
    REQUIRE_FALSE(area_result.has_value());
    REQUIRE(area_result.error() == ShapeError::kNegativeValue);
  }
}

TEST_CASE("Shape factory functions", "[shapes][factory]") {
  SECTION("CreateCircle function") {
    auto circle = CreateCircle(7.5);
    REQUIRE(circle != nullptr);
    REQUIRE(circle->GetRadius() == 7.5);

    auto circle_int = CreateCircle(10);
    REQUIRE(circle_int != nullptr);
    REQUIRE(circle_int->GetRadius() == 10.0);
  }

  SECTION("CreateRectangle function") {
    auto rect = CreateRectangle(3.0, 4.0);
    REQUIRE(rect != nullptr);
    REQUIRE(rect->GetWidth() == 3.0);
    REQUIRE(rect->GetHeight() == 4.0);

    auto rect_mixed = CreateRectangle(5, 7.5);
    REQUIRE(rect_mixed != nullptr);
    REQUIRE(rect_mixed->GetWidth() == 5.0);
    REQUIRE(rect_mixed->GetHeight() == 7.5);
  }

  SECTION("CreateSquare function") {
    auto square = CreateSquare(6.0);
    REQUIRE(square != nullptr);
    REQUIRE(square->GetWidth() == 6.0);
    REQUIRE(square->GetHeight() == 6.0);
    REQUIRE(square->IsSquare());
  }
}

TEST_CASE("Polymorphic behavior", "[shapes][polymorphism]") {
  SECTION("Shape polymorphism through base pointer") {
    auto circle = CreateCircle(3.0);
    auto rectangle = CreateRectangle(4.0, 5.0);

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(circle));
    shapes.push_back(std::move(rectangle));

    for (const auto &shape : shapes) {
      auto area = shape->GetArea();
      REQUIRE(area.has_value());
      REQUIRE(area.value() > 0.0);

      auto perimeter = shape->GetPerimeter();
      REQUIRE(perimeter.has_value());
      REQUIRE(perimeter.value() > 0.0);
    }
  }
}
