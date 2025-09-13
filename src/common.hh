#pragma once

#include <limits>
#include <numbers>

constexpr double kInfinity = std::numeric_limits<double>::infinity();
constexpr double kPi = std::numbers::pi;

constexpr auto DegreeToRadians(double degrees) -> double { return degrees * kPi / 180.0; };
