#pragma once

#include "color.hh"
#include "vec3.hh"

class Texture {
 public:
  Texture() = default;
  Texture(const Texture&) = default;
  auto operator=(const Texture&) -> Texture& = default;
  Texture(Texture&&) = default;
  auto operator=(const Texture&&) -> Texture& = default;
  virtual ~Texture() = default;
  [[nodiscard]] virtual auto Value(double u, double v, const Point3& p) const -> Color = 0;
};

class SolidColor : public Texture {
 public:
  explicit SolidColor(const Color& albedo) : albedo_(albedo) {}
  SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  [[nodiscard]] auto Value([[maybe_unused]] double u, [[maybe_unused]] double v,
                           [[maybe_unused]] const Point3& p) const -> Color override {
    return albedo_;
  }

 private:
  Color albedo_;
};
