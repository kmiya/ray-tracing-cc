#pragma once

#include "interval.hh"
#include "ray.hh"
#include "vec3.hh"

class Material;

class HitRecord {
 public:
  HitRecord() = default;

  // getter
  [[nodiscard]] auto P() const -> const Point3& { return p_; }
  [[nodiscard]] auto Normal() const -> const Vec3& { return normal_; }
  [[nodiscard]] auto T() const -> double { return t_; }
  [[nodiscard]] auto FrontFace() const -> bool { return front_face_; }
  [[nodiscard]] auto Mat() const -> std::shared_ptr<Material> { return mat_; }

  // setter
  auto SetP(const Point3& p) -> void { p_ = p; }
  auto SetT(double t) -> void { t_ = t; }

  auto SetFaceNormal(const Ray& r, const Vec3& outward_normal) -> void {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    front_face_ = Dot(r.Direction(), outward_normal) < 0;
    normal_ = front_face_ ? outward_normal : -outward_normal;
  }
  auto SetMaterial(std::shared_ptr<Material> mat) -> void { mat_ = std::move(mat); }

 private:
  Point3 p_;
  Vec3 normal_;
  double t_{};
  bool front_face_{};
  std::shared_ptr<Material> mat_;
};

class Hittable {
 public:
  Hittable() = default;
  Hittable(const Hittable&) = delete;
  auto operator=(const Hittable&) -> Hittable& = delete;
  Hittable(Hittable&&) = delete;
  auto operator=(const Hittable&&) -> Hittable& = delete;

  virtual ~Hittable() = default;
  virtual auto Hit(const Ray& r, const Interval& ray_t, HitRecord& rec) const -> bool = 0;
};
