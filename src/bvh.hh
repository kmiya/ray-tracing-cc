#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "aabb.hh"
#include "common.hh"
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

  // NOLINTNEXTLINE(misc-no-recursion)
  BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end)
      : bbox_(AABB::Empty()) {
    // Build the bounding box of the span of source objects.
    for (size_t object_index = start; object_index < end; object_index++) {
      bbox_ = AABB(bbox_, objects[object_index]->BoundingBox());
    }
    const int axis = bbox_.LongestAxis();

    // NOLINTNEXTLINE(readability-avoid-nested-conditional-operator)
    const auto comparator = (axis == 0) ? BoxXCompare : (axis == 1) ? BoxYCompare : BoxZCompare;

    const size_t object_span = end - start;
    if (object_span == 1) {
      left_ = right_ = objects[start];
    } else if (object_span == 2) {
      left_ = objects[start];
      right_ = objects[start + 1];
    } else {
      std::sort(std::begin(objects) + static_cast<int>(start),
                std::begin(objects) + static_cast<int>(end), comparator);
      const auto mid = start + (object_span / 2);
      left_ = std::make_shared<BVHNode>(objects, start, mid);
      right_ = std::make_shared<BVHNode>(objects, mid, end);
    }
  }

  auto Hit(const Ray& r, const Interval& ray_t, HitRecord& rec) const -> bool override {
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
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  static auto BoxCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b,
                         int axis_index) -> bool {
    const auto a_axis_interval = a->BoundingBox().AxisInterval(axis_index);
    const auto b_axis_interval = b->BoundingBox().AxisInterval(axis_index);
    return a_axis_interval.Min() < b_axis_interval.Min();
  }

  static auto BoxXCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
      -> bool {
    return BoxCompare(a, b, 0);
  }
  static auto BoxYCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
      -> bool {
    return BoxCompare(a, b, 1);
  }
  static auto BoxZCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
      -> bool {
    return BoxCompare(a, b, 2);
  }

  std::shared_ptr<Hittable> left_;
  std::shared_ptr<Hittable> right_;
  AABB bbox_;
};
