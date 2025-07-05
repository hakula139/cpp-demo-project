/**
 * @file test_shapes.cpp
 * @brief Comprehensive tests for the shapes module
 */

#include <memory>
#include <numbers>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "exceptions/custom_exception.hpp"
#include "shapes/circle.hpp"
#include "shapes/rectangle.hpp"
#include "shapes/shape.hpp"

namespace {

using namespace cpp_features::shapes;
using cpp_features::exceptions::ValidationException;

TEST_CASE("Circle construction and basic properties", "[shapes][circle]") {
  SECTION("Valid circle creation") {
    Circle circle(5.0);

    REQUIRE(circle.GetRadius() == 5.0);
    REQUIRE(circle.GetName() == "Circle");
  }

  SECTION("Circle with zero radius throws exception") {
    REQUIRE_THROWS_AS(Circle(0.0), ValidationException);
  }

  SECTION("Circle with negative radius throws exception") {
    REQUIRE_THROWS_AS(Circle(-3.0), ValidationException);
  }

  SECTION("ValidationException contains field information") {
    try {
      Circle circle(-1.0);
      FAIL("Expected ValidationException to be thrown");
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().has_value());
      REQUIRE(e.GetFieldName().value() == "radius");
    }
  }
}

TEST_CASE("Circle area calculations", "[shapes][circle][area]") {
  SECTION("Valid area calculation") {
    Circle circle(5.0);
    double area = circle.GetArea();

    REQUIRE(area == Catch::Approx(std::numbers::pi * 25.0));
  }

  SECTION("Area with unit radius") {
    Circle unit_circle(1.0);
    double area = unit_circle.GetArea();

    REQUIRE(area == Catch::Approx(std::numbers::pi));
  }
}

TEST_CASE("Circle perimeter calculations", "[shapes][circle][perimeter]") {
  SECTION("Valid perimeter calculation") {
    Circle circle(5.0);
    double perimeter = circle.GetPerimeter();

    REQUIRE(perimeter == Catch::Approx(2.0 * std::numbers::pi * 5.0));
  }

  SECTION("Perimeter with unit radius") {
    Circle unit_circle(1.0);
    double perimeter = unit_circle.GetPerimeter();

    REQUIRE(perimeter == Catch::Approx(2.0 * std::numbers::pi));
  }
}

TEST_CASE("Circle comparison operations", "[shapes][circle][comparison]") {
  SECTION("Equality comparison") {
    Circle circle1(5.0);
    Circle circle2(5.0);
    Circle circle3(3.0);

    REQUIRE(circle1 == circle2);
    REQUIRE_FALSE(circle1 == circle3);
  }

  SECTION("Three-way comparison") {
    Circle small_circle(3.0);
    Circle large_circle(5.0);

    REQUIRE(small_circle < large_circle);
    REQUIRE(large_circle > small_circle);
    REQUIRE_FALSE(small_circle > large_circle);
  }

  SECTION("Comparison with same radius") {
    Circle circle1(4.0);
    Circle circle2(4.0);

    REQUIRE(circle1 == circle2);
    REQUIRE_FALSE(circle1 < circle2);
    REQUIRE_FALSE(circle1 > circle2);
  }
}

TEST_CASE("Rectangle construction and basic properties", "[shapes][rectangle]") {
  SECTION("Valid rectangle creation with separate dimensions") {
    Rectangle rect(4.0, 6.0);

    REQUIRE(rect.GetWidth() == 4.0);
    REQUIRE(rect.GetHeight() == 6.0);
    REQUIRE(rect.GetName() == "Rectangle");
    REQUIRE_FALSE(rect.IsSquare());
  }

  SECTION("Rectangle creation with Dimensions struct") {
    Rectangle::Dimensions dims{3.0, 5.0};
    Rectangle rect(dims);

    REQUIRE(rect.GetWidth() == 3.0);
    REQUIRE(rect.GetHeight() == 5.0);
    REQUIRE_FALSE(rect.IsSquare());
  }

  SECTION("Square creation with single value") {
    Rectangle square(5.0);

    REQUIRE(square.GetWidth() == 5.0);
    REQUIRE(square.GetHeight() == 5.0);
    REQUIRE(square.IsSquare());
  }

  SECTION("Rectangle with zero width throws exception") {
    REQUIRE_THROWS_AS(Rectangle(0.0, 5.0), ValidationException);
  }

  SECTION("Rectangle with zero height throws exception") {
    REQUIRE_THROWS_AS(Rectangle(5.0, 0.0), ValidationException);
  }

  SECTION("Rectangle with negative width throws exception") {
    REQUIRE_THROWS_AS(Rectangle(-3.0, 5.0), ValidationException);
  }

  SECTION("Rectangle with negative height throws exception") {
    REQUIRE_THROWS_AS(Rectangle(5.0, -3.0), ValidationException);
  }

  SECTION("ValidationException contains field information for width") {
    try {
      Rectangle rect(-1.0, 5.0);
      FAIL("Expected ValidationException to be thrown");
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().has_value());
      REQUIRE(e.GetFieldName().value() == "width");
    }
  }

  SECTION("ValidationException contains field information for height") {
    try {
      Rectangle rect(5.0, -1.0);
      FAIL("Expected ValidationException to be thrown");
    } catch (const ValidationException &e) {
      REQUIRE(e.GetFieldName().has_value());
      REQUIRE(e.GetFieldName().value() == "height");
    }
  }
}

TEST_CASE("Rectangle area calculations", "[shapes][rectangle][area]") {
  SECTION("Valid area calculation") {
    Rectangle rect(4.0, 6.0);
    double area = rect.GetArea();

    REQUIRE(area == 24.0);
  }

  SECTION("Square area calculation") {
    Rectangle square(5.0, 5.0);
    double area = square.GetArea();

    REQUIRE(area == 25.0);
  }

  SECTION("Unit rectangle area") {
    Rectangle unit_rect(1.0, 1.0);
    double area = unit_rect.GetArea();

    REQUIRE(area == 1.0);
  }
}

TEST_CASE("Rectangle perimeter calculations", "[shapes][rectangle][perimeter]") {
  SECTION("Valid perimeter calculation") {
    Rectangle rect(4.0, 6.0);
    double perimeter = rect.GetPerimeter();

    REQUIRE(perimeter == 20.0);
  }

  SECTION("Square perimeter calculation") {
    Rectangle square(5.0, 5.0);
    double perimeter = square.GetPerimeter();

    REQUIRE(perimeter == 20.0);
  }
}

TEST_CASE("Rectangle square detection", "[shapes][rectangle][square]") {
  SECTION("Square detection positive") {
    Rectangle square1(5.0, 5.0);
    Rectangle square2(1.0, 1.0);
    Rectangle square3(10.0, 10.0);

    REQUIRE(square1.IsSquare());
    REQUIRE(square2.IsSquare());
    REQUIRE(square3.IsSquare());
  }

  SECTION("Square detection negative") {
    Rectangle rect1(4.0, 6.0);
    Rectangle rect2(1.0, 2.0);
    Rectangle rect3(10.0, 5.0);

    REQUIRE_FALSE(rect1.IsSquare());
    REQUIRE_FALSE(rect2.IsSquare());
    REQUIRE_FALSE(rect3.IsSquare());
  }
}

TEST_CASE("Rectangle comparison operations", "[shapes][rectangle][comparison]") {
  SECTION("Equality comparison") {
    Rectangle rect1(4.0, 3.0);
    Rectangle rect2(4.0, 3.0);
    Rectangle rect3(3.0, 4.0);

    REQUIRE(rect1 == rect2);
    REQUIRE_FALSE(rect1 == rect3);
  }

  SECTION("Three-way comparison by area") {
    Rectangle small_rect(2.0, 3.0);  // area = 6.0
    Rectangle large_rect(4.0, 5.0);  // area = 20.0

    REQUIRE(small_rect < large_rect);
    REQUIRE(large_rect > small_rect);
  }

  SECTION("Three-way comparison with same area") {
    Rectangle rect1(2.0, 6.0);  // area = 12.0
    Rectangle rect2(3.0, 4.0);  // area = 12.0

    // Should compare by width when areas are equal
    REQUIRE(rect1 < rect2);  // 2.0 < 3.0
  }

  SECTION("Three-way comparison with same area and width") {
    Rectangle rect1(3.0, 4.0);  // area = 12.0, width = 3.0
    Rectangle rect2(3.0, 5.0);  // area = 15.0, width = 3.0

    REQUIRE(rect1 < rect2);  // Compare by area first
  }
}

TEST_CASE("Shape factory functions", "[shapes][factory]") {
  SECTION("CreateCircle with double") {
    auto circle = CreateCircle(7.5);

    REQUIRE(circle != nullptr);
    REQUIRE(circle->GetRadius() == 7.5);
    REQUIRE(circle->GetName() == "Circle");
  }

  SECTION("CreateCircle with integer") {
    auto circle = CreateCircle(10);

    REQUIRE(circle != nullptr);
    REQUIRE(circle->GetRadius() == 10.0);
  }

  SECTION("CreateCircle with float") {
    auto circle = CreateCircle(3.14F);

    REQUIRE(circle != nullptr);
    REQUIRE(circle->GetRadius() == Catch::Approx(3.14));
  }

  SECTION("CreateCircle with invalid radius throws exception") {
    REQUIRE_THROWS_AS(CreateCircle(0.0), ValidationException);
    REQUIRE_THROWS_AS(CreateCircle(-5.0), ValidationException);
  }

  SECTION("CreateRectangle with doubles") {
    auto rect = CreateRectangle(3.0, 4.0);

    REQUIRE(rect != nullptr);
    REQUIRE(rect->GetWidth() == 3.0);
    REQUIRE(rect->GetHeight() == 4.0);
    REQUIRE(rect->GetName() == "Rectangle");
  }

  SECTION("CreateRectangle with mixed types") {
    auto rect = CreateRectangle(5, 7.5);

    REQUIRE(rect != nullptr);
    REQUIRE(rect->GetWidth() == 5.0);
    REQUIRE(rect->GetHeight() == 7.5);
  }

  SECTION("CreateRectangle with invalid dimensions throws exception") {
    REQUIRE_THROWS_AS(CreateRectangle(0.0, 5.0), ValidationException);
    REQUIRE_THROWS_AS(CreateRectangle(5.0, -3.0), ValidationException);
  }

  SECTION("CreateSquare function") {
    auto square = CreateSquare(6.0);

    REQUIRE(square != nullptr);
    REQUIRE(square->GetWidth() == 6.0);
    REQUIRE(square->GetHeight() == 6.0);
    REQUIRE(square->IsSquare());
  }

  SECTION("CreateSquare with integer") {
    auto square = CreateSquare(8);

    REQUIRE(square != nullptr);
    REQUIRE(square->GetWidth() == 8.0);
    REQUIRE(square->GetHeight() == 8.0);
    REQUIRE(square->IsSquare());
  }

  SECTION("CreateSquare with invalid side throws exception") {
    REQUIRE_THROWS_AS(CreateSquare(0.0), ValidationException);
    REQUIRE_THROWS_AS(CreateSquare(-2.0), ValidationException);
  }
}

TEST_CASE("Shape polymorphism", "[shapes][polymorphism]") {
  SECTION("Polymorphic behavior through base pointer") {
    auto circle = CreateCircle(3.0);
    auto rectangle = CreateRectangle(4.0, 5.0);
    auto square = CreateSquare(2.0);

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(circle));
    shapes.push_back(std::move(rectangle));
    shapes.push_back(std::move(square));

    REQUIRE(shapes.size() == 3);

    for (const auto &shape : shapes) {
      REQUIRE(shape != nullptr);
      REQUIRE_FALSE(shape->GetName().empty());

      double area = shape->GetArea();
      REQUIRE(area > 0.0);

      double perimeter = shape->GetPerimeter();
      REQUIRE(perimeter > 0.0);
    }
  }

  SECTION("Polymorphic error handling with exceptions") {
    // Test that invalid shapes cannot be created
    REQUIRE_THROWS_AS(CreateCircle(0.0), ValidationException);
    REQUIRE_THROWS_AS(CreateRectangle(-1.0, 5.0), ValidationException);
  }
}

TEST_CASE("Shape type traits and concepts", "[shapes][concepts]") {
  SECTION("Type traits verification") {
    static_assert(std::is_base_of_v<Shape, Circle>);
    static_assert(std::is_base_of_v<Shape, Rectangle>);
    static_assert(std::is_final_v<Circle>);
    static_assert(std::is_final_v<Rectangle>);
  }

  SECTION("Move semantics") {
    static_assert(std::is_move_constructible_v<Circle>);
    static_assert(std::is_move_assignable_v<Circle>);
    static_assert(std::is_move_constructible_v<Rectangle>);
    static_assert(std::is_move_assignable_v<Rectangle>);
  }

  SECTION("Copy semantics") {
    static_assert(std::is_copy_constructible_v<Circle>);
    static_assert(std::is_copy_assignable_v<Circle>);
    static_assert(std::is_copy_constructible_v<Rectangle>);
    static_assert(std::is_copy_assignable_v<Rectangle>);
  }
}

}  // namespace
