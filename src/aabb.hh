#pragma once

#include "interval.hh"
#include "ray.hh"
#include "vec3.hh"

class AABB {
 public:
  AABB() = default;

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  AABB(const Interval& x, const Interval& y, const Interval& z) : x_(x), y_(y), z_(z) {}

  AABB(const Point3& a, const Point3& b) {
    // Treat the two points a and b as extrema for the bounding box, so we don't require a
    // particular minimum/maximum coordinate order.
    x_ = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
    y_ = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
    z_ = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
  }

  AABB(const AABB& box0, const AABB& box1) {
    x_ = Interval(box0.X(), box1.X());
    y_ = Interval(box0.Y(), box1.Y());
    z_ = Interval(box0.Z(), box1.Z());
  }

  [[nodiscard]] auto AxisInterval(int n) const -> const Interval& {
    if (n == 1) {
      return y_;
    }
    if (n == 2) {
      return z_;
    }
    return x_;
  }

  [[nodiscard]] auto Hit(const Ray& r, Interval ray_t) const -> bool {
    const Point3& ray_orig = r.Origin();
    const Vec3& ray_dir = r.Direction();

    for (int axis = 0; axis < 3; axis++) {
      const Interval& ax = AxisInterval(axis);
      const double ad_inv = 1.0 / ray_dir[axis];

      const auto t0 = (ax.Min() - ray_orig[axis]) * ad_inv;
      const auto t1 = (ax.Max() - ray_orig[axis]) * ad_inv;

      if (t0 < t1) {
        if (t0 > ray_t.Min()) {
          ray_t.SetMin(t0);
        };
        if (t1 < ray_t.Max()) {
          ray_t.SetMax(t1);
        };
      } else {
        if (t1 > ray_t.Min()) {
          ray_t.SetMin(t1);
        };
        if (t0 < ray_t.Max()) {
          ray_t.SetMax(t0);
        };
      }

      if (ray_t.Max() <= ray_t.Min()) {
        return false;
      }
    }
    return true;
  }

  [[nodiscard]] auto X() const -> Interval { return x_; }
  [[nodiscard]] auto Y() const -> Interval { return y_; }
  [[nodiscard]] auto Z() const -> Interval { return z_; }

 private:
  Interval x_, y_, z_;
};
