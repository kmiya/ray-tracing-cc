#pragma once
#include <limits>

class Interval {
 public:
  Interval() = default;
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Interval(double min, double max) : min_(min), max_(max) {}

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Interval(const Interval& a, const Interval& b) {
    // Create the interval tightly enclosing the two input intervals.
    min_ = a.Min() <= b.Min() ? a.Min() : b.Min();
    max_ = a.Max() >= b.Max() ? a.Max() : b.Max();
  }

  [[nodiscard]] auto Min() const -> double { return min_; }
  [[nodiscard]] auto Max() const -> double { return max_; }
  [[nodiscard]] auto Size() const -> double { return max_ - min_; }
  [[nodiscard]] auto Contains(double x) const -> bool { return min_ <= x && x <= max_; }
  [[nodiscard]] auto Surrounds(double x) const -> bool { return min_ < x && x < max_; }
  [[nodiscard]] auto Clamp(double x) const -> double { return std::clamp(x, min_, max_); }
  [[nodiscard]] auto Expand(double delta) const -> Interval {
    const double padding = delta / 2;
    return {min_ - padding, max_ + padding};
  }
  [[nodiscard]] auto SetMin(double min) -> double { min_ = min; }
  [[nodiscard]] auto SetMax(double max) -> double { max_ = max; }

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
