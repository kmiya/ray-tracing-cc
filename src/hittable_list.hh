#pragma once

#include <memory>
#include <vector>

#include "aabb.hh"
#include "hittable.hh"
#include "ray.hh"

class HittableList : public Hittable {
 public:
  HittableList() = default;
  explicit HittableList(const std::shared_ptr<Hittable>& object) { Add(object); }

  auto Objects() -> std::vector<std::shared_ptr<Hittable>> { return objects_; }

  auto Clear() -> void { objects_.clear(); }

  auto Add(const std::shared_ptr<Hittable>& object) -> void {
    objects_.push_back(object);
    bbox_ = AABB(bbox_, object->BoundingBox());
  }

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  auto Hit(const Ray& r, const Interval& ray_t, HitRecord& rec) const -> bool override {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.Max();

    for (const auto& object : objects_) {
      if (object->Hit(r, Interval(ray_t.Min(), closest_so_far), temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.T();
        rec = temp_rec;
      }
    }
    return hit_anything;
  }

  [[nodiscard]] auto BoundingBox() const -> AABB override { return bbox_; }

 private:
  std::vector<std::shared_ptr<Hittable>> objects_;
  AABB bbox_;
};
