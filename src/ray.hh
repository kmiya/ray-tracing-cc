#pragma once

#include "vec3.hh"

class Ray {
 public:
  Ray() = default;

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Ray(const Point3& origin, const Vec3& direction) : orig_(origin), dir_(direction) {}

  [[nodiscard]] auto Origin() const -> const Point3& { return orig_; }
  [[nodiscard]] auto Direction() const -> const Vec3& { return dir_; }

  [[nodiscard]] auto At(double t) const -> Point3 { return orig_ + t * dir_; }

 private:
  Point3 orig_;
  Vec3 dir_;
};
