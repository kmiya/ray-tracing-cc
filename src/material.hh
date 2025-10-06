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

    scattered = Ray(rec.P(), scatter_direction, r_in.Time());
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
    scattered = Ray(rec.P(), reflected, r_in.Time());
    attenuation = albedo_;
    return Dot(scattered.Direction(), rec.Normal()) > 0;
  }

 private:
  Color albedo_;
  double fuzz_;
};

class Dielectric : public Material {
 public:
  explicit Dielectric(double refraction_index) : refraction_index_(refraction_index) {}

  auto Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
      -> bool override {
    attenuation = Color(1.0, 1.0, 1.0);
    const double ri = rec.FrontFace() ? (1.0 / refraction_index_) : refraction_index_;

    const Vec3 unit_direction = UnitVector(r_in.Direction());
    const double cos_theta = std::fmin(Dot(-unit_direction, rec.Normal()), 1.0);
    const double sin_theta = std::sqrt(1.0 - (cos_theta * cos_theta));

    const bool cannot_refract = ri * sin_theta > 1.0;
    Vec3 direction;

    if (cannot_refract || Reflectance(cos_theta, ri) > RandomDouble()) {
      direction = Reflect(unit_direction, rec.Normal());
    } else {
      direction = Refract(unit_direction, rec.Normal(), ri);
    }

    scattered = Ray(rec.P(), direction, r_in.Time());
    return true;
  }

 private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive index over
  // the refractive index of the enclosing media
  double refraction_index_;

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  static auto Reflectance(double cosine, double refraction_index) -> double {
    // Use Schlick's approximation
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + ((1 - r0) * std::pow((1 - cosine), 5));
  }
};
