#pragma once

#include "color.hh"
#include "hittable.hh"
#include "ray.hh"
#include "vec3.hh"

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

class Lambertian : public Material {
 public:
  explicit Lambertian(const Color& albedo) : albedo_(albedo) {}

  auto Scatter([[maybe_unused]] const Ray& r_in, const HitRecord& rec, Color& attenuation,
               Ray& scattered) const -> bool override {
    auto scatter_direction = rec.Normal() + RandomUnitVector();

    // Catch degenerate scatter direction
    if (scatter_direction.NearZero()) {
      scatter_direction = rec.Normal();
    }

    scattered = Ray(rec.P(), scatter_direction);
    attenuation = albedo_;
    return true;
  }

 private:
  Color albedo_;
};

class Metal : public Material {
 public:
  explicit Metal(const Color& albedo, double fuzz)
      : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1.0) {}

  auto Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
      -> bool override {
    Vec3 reflected = Reflect(r_in.Direction(), rec.Normal());
    reflected = UnitVector(reflected) + (fuzz_ * RandomUnitVector());
    scattered = Ray(rec.P(), reflected);
    attenuation = albedo_;
    return Dot(scattered.Direction(), rec.Normal()) > 0;
  }

 private:
  Color albedo_;
  double fuzz_;
};
