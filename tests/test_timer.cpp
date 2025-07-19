#include <chrono>
#include <cstdint>
#include <ratio>
#include <string>
#include <thread>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "timing/benchmark.hpp"
#include "timing/timer.hpp"

namespace {

using namespace cpp_features::timing;

TEST_CASE("Timer basic functionality", "[timing][timer]") {
  SECTION("Timer construction and immediate timing") {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    auto elapsed_ms = timer.GetElapsed<std::chrono::milliseconds>();
    REQUIRE(elapsed_ms >= 10);
    REQUIRE(elapsed_ms < 20);  // Should be reasonable upper bound
  }

  SECTION("Timer start and stop") {
    Timer timer;
    timer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    timer.Stop();

    auto elapsed_ms = timer.GetElapsed<std::chrono::milliseconds>();
    REQUIRE(elapsed_ms >= 5);
    REQUIRE(elapsed_ms < 15);
  }

  SECTION("Timer reset functionality") {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.Reset();

    auto elapsed_ms = timer.GetElapsed<std::chrono::milliseconds>();
    REQUIRE(elapsed_ms < 5);  // Should be very small after reset
  }

  SECTION("Timer continues after stop") {
    Timer timer;
    timer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    timer.Stop();

    auto first_elapsed = timer.GetElapsed<std::chrono::milliseconds>();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    auto second_elapsed = timer.GetElapsed<std::chrono::milliseconds>();

    REQUIRE(first_elapsed == second_elapsed);  // Should be same after stop
  }
}

TEST_CASE("Timer duration type conversions", "[timing][timer][duration]") {
  SECTION("Different duration types") {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.Stop();

    auto ns = timer.GetElapsed<std::chrono::nanoseconds>();
    auto us = timer.GetElapsed<std::chrono::microseconds>();
    auto ms = timer.GetElapsed<std::chrono::milliseconds>();
    auto s = timer.GetElapsed<std::chrono::seconds>();

    REQUIRE(ns >= 10'000'000);  // At least 10 million nanoseconds
    REQUIRE(ns < 20'000'000);   // Less than 20 million nanoseconds
    REQUIRE(us >= 10'000);      // At least 10 thousand microseconds
    REQUIRE(us < 20'000);       // Less than 20 thousand microseconds
    REQUIRE(ms >= 10);          // At least 10 milliseconds
    REQUIRE(ms < 20);           // Less than 20 milliseconds
    REQUIRE(s == 0);            // Should be 0 seconds for short duration
  }

  SECTION("Duration object return") {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.Stop();

    auto duration = timer.GetElapsedDuration<std::chrono::microseconds>();

    REQUIRE(duration.count() >= 10000);
    REQUIRE(duration.count() < 20000);
    REQUIRE(duration >= std::chrono::milliseconds(10));
    REQUIRE(duration < std::chrono::milliseconds(20));
  }
}

TEST_CASE("Timer string formatting", "[timing][timer][format]") {
  SECTION("String representation contains time unit") {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.Stop();

    auto time_str = timer.GetElapsedString();

    REQUIRE_FALSE(time_str.empty());
    REQUIRE(time_str.find('s') != std::string::npos);
  }
}

TEST_CASE("ScopedTimer basic functionality", "[timing][scoped_timer]") {
  SECTION("ScopedTimer automatic timing") {
    auto callback_called = false;
    std::int64_t callback_time = 0;

    {
      ScopedTimer timer{
          "Test operation",
          [&](std::int64_t ns) {
            callback_called = true;
            callback_time = ns;
          },
      };

      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    REQUIRE(callback_called);
    REQUIRE(callback_time >= 5'000'000);  // At least 5 million nanoseconds
    REQUIRE(callback_time < 15'000'000);  // Less than 15 million nanoseconds
  }

  SECTION("ScopedTimer without callback") {
    {
      ScopedTimer timer{"Test operation"};
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Should not crash
    REQUIRE(true);
  }
}

TEST_CASE("BenchmarkRunner functionality", "[timing][benchmark]") {
  SECTION("Basic benchmarking") {
    auto simple_task = []() {
      volatile int sum = 0;
      for (int i = 0; i < 100; ++i) {
        sum += i;
      }
    };

    auto result = BenchmarkRunner::Benchmark("Simple task", simple_task, 10);

    REQUIRE(result.name == "Simple task");
    REQUIRE(result.iterations == 10);
    REQUIRE(result.total_ns > 0);
    REQUIRE(result.avg_ns > 0);
    REQUIRE(result.min_ns > 0);
    REQUIRE(result.max_ns > 0);
    REQUIRE(result.min_ns <= result.avg_ns);
    REQUIRE(result.max_ns >= result.avg_ns);
  }

  SECTION("Benchmark with different iteration counts") {
    auto task = []() {
      volatile int x = 42;
      x = x + 1;
    };

    auto result1 = BenchmarkRunner::Benchmark("Task 1", task, 5);
    auto result2 = BenchmarkRunner::Benchmark("Task 2", task, 20);

    REQUIRE(result1.iterations == 5);
    REQUIRE(result2.iterations == 20);
    REQUIRE(result1.total_ns < result2.total_ns);  // More iterations should take longer
  }
}

TEST_CASE("Utility functions", "[timing][utilities]") {
  SECTION("TimeFunction utility") {
    auto elapsed =
        TimeFunction([]() { std::this_thread::sleep_for(std::chrono::milliseconds(5)); });

    REQUIRE(elapsed >= 5'000'000);
    REQUIRE(elapsed < 15'000'000);
  }

  SECTION("ProfileFunction single execution") {
    ProfileFunction("Test profile", []() {
      volatile int sum = 0;
      for (int i = 0; i < 1000; ++i) {
        sum += i;
      }
    });

    // Should not crash
    REQUIRE(true);
  }

  SECTION("ProfileFunction with benchmarking") {
    ProfileFunction(
        "Test benchmark",
        []() {
          volatile int sum = 0;
          for (int i = 0; i < 100; ++i) {
            sum += i;
          }
        },
        5);

    // Should not crash
    REQUIRE(true);
  }
}

TEST_CASE("Timer edge cases", "[timing][timer][edge_cases]") {
  SECTION("Multiple start calls") {
    Timer timer;
    timer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    timer.Start();  // Should reset the start time
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    timer.Stop();

    auto elapsed = timer.GetElapsed<std::chrono::milliseconds>();
    REQUIRE(elapsed >= 5);
    REQUIRE(elapsed < 15);  // Should be closer to 5ms due to restart
  }

  SECTION("Multiple stop calls") {
    Timer timer;
    timer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    timer.Stop();
    auto first_elapsed = timer.GetElapsed<std::chrono::milliseconds>();
    timer.Stop();  // Second stop should have no effect
    auto second_elapsed = timer.GetElapsed<std::chrono::milliseconds>();

    REQUIRE(first_elapsed == second_elapsed);
  }

  SECTION("Reset after stop") {
    Timer timer;
    timer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    timer.Stop();
    timer.Reset();

    auto elapsed = timer.GetElapsed<std::chrono::milliseconds>();
    REQUIRE(elapsed < 5);  // Should be very small after reset
  }
}

TEST_CASE("BenchmarkResult structure", "[timing][benchmark][result]") {
  SECTION("BenchmarkResult initialization") {
    BenchmarkRunner::BenchmarkResult result{
        .name = "Test",
        .iterations = 100,
        .total_ns = 1'000'000,
        .avg_ns = 10'000,
        .min_ns = 8'000,
        .max_ns = 15'000,
    };

    REQUIRE(result.name == "Test");
    REQUIRE(result.iterations == 100);
    REQUIRE(result.total_ns == 1'000'000);
    REQUIRE(result.avg_ns == 10'000);
    REQUIRE(result.min_ns == 8'000);
    REQUIRE(result.max_ns == 15'000);
  }

  SECTION("PrintResult function does not crash") {
    BenchmarkRunner::BenchmarkResult result{
        .name = "Test output",
        .iterations = 50,
        .total_ns = 5'000'000,
        .avg_ns = 100'000,
        .min_ns = 80'000,
        .max_ns = 150'000,
    };
    BenchmarkRunner::PrintResult(result);

    // Should not crash
    REQUIRE(true);
  }
}

TEST_CASE("Type aliases and using declarations", "[timing][types]") {
  SECTION("Type alias functionality") {
    Clock::time_point now = Clock::now();
    TimePoint also_now = Clock::now();

    REQUIRE(now <= also_now);  // Time should progress

    Duration diff = also_now - now;
    REQUIRE(diff >= std::chrono::nanoseconds(0));

    TimeInterval<double, std::milli> interval{10.5};
    REQUIRE(interval.count() == Catch::Approx(10.5));
  }
}

}  // namespace
