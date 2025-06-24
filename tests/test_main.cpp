#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Test framework is working", "[basic]") {
    REQUIRE(true);
    CHECK(1 + 1 == 2);

    SECTION("Testing basic arithmetic") {
        int a = 5;
        int b = 3;
        REQUIRE(a + b == 8);
        REQUIRE(a - b == 2);
        REQUIRE(a * b == 15);
    }
}