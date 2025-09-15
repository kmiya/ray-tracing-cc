#pragma once

#include <limits>
#include <numbers>
#include <random>

constexpr double kInfinity = std::numeric_limits<double>::infinity();
constexpr double kPi = std::numbers::pi;

constexpr auto DegreeToRadians(double degrees) -> double { return degrees * kPi / 180.0; };

inline auto RandomDouble() -> double {
  static std::mt19937 generator{12345};  // NOLINT(cert-msc32-c,cert-msc51-cpp)
  static std::uniform_real_distribution<double> distribution{0.0, 1.0};
  return distribution(generator);
}
