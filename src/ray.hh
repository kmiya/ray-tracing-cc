#pragma once

#include "vec3.hh"

class Ray {
 public:
  Ray() = default;

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Ray(const Point3& origin, const Vec3& direction, double time)
      : orig_(origin), dir_(direction), time_(time) {}

  Ray(const Point3& origin, const Vec3& direction) : Ray(origin, direction, 0) {}

  [[nodiscard]] auto Origin() const -> const Point3& { return orig_; }
  [[nodiscard]] auto Direction() const -> const Vec3& { return dir_; }

  [[nodiscard]] auto Time() const -> double { return time_; }

  [[nodiscard]] auto At(double t) const -> Point3 { return orig_ + t * dir_; }

 private:
  Point3 orig_;
  Vec3 dir_;
  double time_{};
};
