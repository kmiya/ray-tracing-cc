#pragma once
#include <limits>

class Interval {
 public:
  Interval() = default;
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Interval(double min, double max) : min_(min), max_(max) {}

  [[nodiscard]] auto Min() const -> double { return min_; }
  [[nodiscard]] auto Max() const -> double { return max_; }
  [[nodiscard]] auto Size() const -> double { return max_ - min_; }
  [[nodiscard]] auto Contains(double x) const -> bool { return min_ <= x && x <= max_; }
  [[nodiscard]] auto Surrounds(double x) const -> bool { return min_ < x && x < max_; }
  [[nodiscard]] auto Clamp(double x) const -> double { return std::clamp(x, min_, max_); }

  static auto Empty() -> const Interval& {
    static const Interval kEmptyInterval(+kInfinity, -kInfinity);
    return kEmptyInterval;
  }

  static auto Universe() -> const Interval& {
    static const Interval kUniverseInterval(-kInfinity, +kInfinity);
    return kUniverseInterval;
  }

 private:
  static constexpr double kInfinity = std::numeric_limits<double>::infinity();
  double min_ = +kInfinity;
  double max_ = -kInfinity;
};
