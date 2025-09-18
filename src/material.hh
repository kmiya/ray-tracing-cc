#pragma once

#include "color.hh"
#include "hittable.hh"
#include "ray.hh"

class Material {
 public:
  Material() = default;
  Material(const Material&) = delete;
  Material(const Material&&) = delete;
  auto operator=(const Material&) -> Material& = delete;
  auto operator=(const Material&&) -> Material&& = delete;

  virtual ~Material() = default;
  virtual auto Scatter([[maybe_unused]] const Ray& r_in, [[maybe_unused]] const HitRecord& rec,
                       [[maybe_unused]] Color& attenuation, [[maybe_unused]] Ray& scattered) const
      -> bool {
    return false;
  }
};
