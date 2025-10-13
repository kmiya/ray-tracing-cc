#pragma once

#include <memory>
#include <vector>

#include "aabb.hh"
#include "hittable.hh"
#include "hittable_list.hh"
#include "interval.hh"
#include "ray.hh"

class BVHNode : public Hittable {
 public:
  explicit BVHNode(HittableList list) : BVHNode(list.Objects(), 0, list.Objects().size()) {
    // There's a C++ subtlety here. This constructor (without span indices) creates an
    // implicit copy of the hittable list, which we will modify. The lifetime of the copied
    // list only extends until this constructor exists. That's OK, because we only need to
    // persist the resulting bounding volume hierarchy.
  }

  BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    // To be implemented later.
  }

  auto Hit(const Ray& r, Interval ray_t, HitRecord& rec) const -> bool {
    if (!bbox_.Hit(r, ray_t)) {
      return false;
    }
    const bool hit_left = left_->Hit(r, ray_t, rec);
    const bool hit_right =
        right_->Hit(r, Interval(ray_t.Min(), hit_left ? rec.T() : ray_t.Max()), rec);

    return hit_left || hit_right;
  }

  [[nodiscard]] auto BoundingBox() const -> AABB override { return bbox_; }

 private:
  std::shared_ptr<Hittable> left_;
  std::shared_ptr<Hittable> right_;
  AABB bbox_;
};
