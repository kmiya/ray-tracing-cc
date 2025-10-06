#pragma once

#include <cmath>

#include "hittable.hh"
#include "ray.hh"
#include "vec3.hh"

class Sphere : public Hittable {
 public:
  // Stationary Sphere
  Sphere(const Point3& static_center, const double radius, std::shared_ptr<Material> mat)
      : center_(static_center, Vec3(0, 0, 0)),
        radius_(std::fmax(0, radius)),
        mat_(std::move(mat)) {}

  // Moving Sphere
  Sphere(const Point3& center1, const Point3& center2, const double radius,
         std::shared_ptr<Material> mat)
      : center_(center1, center2 - center1), radius_(std::fmax(0, radius)), mat_(std::move(mat)) {}

  auto Hit(const Ray& r, const Interval& ray_t, HitRecord& rec) const -> bool override {
    const Point3 current_center = center_.At(r.Time());
    const Vec3 oc = current_center - r.Origin();
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
    if (!ray_t.Surrounds(root)) {
      root = (h + sqrt_d) / a;
      if (!ray_t.Surrounds(root)) {
        return false;
      }
    }
    rec.SetT(root);
    rec.SetP(r.At(rec.T()));
    const Vec3 outward_normal = (rec.P() - current_center) / radius_;
    rec.SetFaceNormal(r, outward_normal);
    rec.SetMaterial(mat_);
    return true;
  }

 private:
  Ray center_;
  double radius_;
  std::shared_ptr<Material> mat_;
};
