#pragma once

#include <cmath>

#include "hittable.hh"
#include "ray.hh"
#include "vec3.hh"

class Sphere : public Hittable {
 public:
  Sphere(const Point3& center, const double radius)
      : center_(center), radius_(std::fmax(0, radius)) {}

  auto Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const -> bool override {
    const Vec3 oc = center_ - r.Origin();
    const double a = r.Direction().LengthSquared();
    const double h = Dot(r.Direction(), oc);
    const double c = oc.LengthSquared() - (radius_ * radius_);
    const double discriminant = (h * h) - (a * c);
    if (discriminant < 0) {
      return false;
    }

    const double sqrt_d = std::sqrt(discriminant);
    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrt_d) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
      root = (h + sqrt_d) / a;
      if (root <= ray_tmin || ray_tmax <= root) {
        return false;
      }
    }
    rec.SetT(root);
    rec.SetP(r.At(rec.T()));
    const Vec3 outward_normal = (rec.P() - center_) / radius_;
    rec.SetFaceNormal(r, outward_normal);
    return true;
  }

 private:
  Point3 center_;
  double radius_;
};
