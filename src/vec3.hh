#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include "common.hh"

class Vec3 {
 public:
  Vec3() : e_{0, 0, 0} {}
  Vec3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

  [[nodiscard]] auto X() const -> double { return e_[0]; }
  [[nodiscard]] auto Y() const -> double { return e_[1]; }
  [[nodiscard]] auto Z() const -> double { return e_[2]; }

  auto operator-() const -> Vec3 { return {-e_[0], -e_[1], -e_[2]}; }
  auto operator[](int i) const -> double { return e_.at(i); }
  auto operator[](int i) -> double& { return e_.at(i); }

  auto operator+=(const Vec3& v) -> Vec3& {
    e_[0] += v[0];
    e_[1] += v[1];
    e_[2] += v[2];
    return *this;
  }

  auto operator*=(double t) -> Vec3& {
    e_[0] *= t;
    e_[1] *= t;
    e_[2] *= t;
    return *this;
  }

  auto operator/=(double t) -> Vec3& { return *this *= 1 / t; }

  [[nodiscard]] auto Length() const -> double { return std::sqrt(LengthSquared()); }

  [[nodiscard]] auto LengthSquared() const -> double {
    return (e_[0] * e_[0]) + (e_[1] * e_[1]) + (e_[2] * e_[2]);
  }

  [[nodiscard]] auto NearZero() const -> bool {
    // Return true if the vector is close to zero in all dimensions.
    const double s = 1e-8;
    return (std::fabs(e_[0]) < s) && (std::fabs(e_[1]) < s) && (std::fabs(e_[2]) < s);
  }

  [[nodiscard]] static auto Random() -> Vec3 {
    return {RandomDouble(), RandomDouble(), RandomDouble()};
  }

  [[nodiscard]] static auto Random(double min, double max) -> Vec3 {
    return {RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max)};
  }

 private:
  std::array<double, 3> e_;
};

// point3 is just an alias for vec3, but useful for geometric clarity in the
// code.
using Point3 = Vec3;

// Vector Utility Functions

inline auto operator<<(std::ostream& out, const Vec3& v) -> std::ostream& {
  return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline auto operator+(const Vec3& u, const Vec3& v) -> Vec3 {
  return {u[0] + v[0], u[1] + v[1], u[2] + v[2]};
}

inline auto operator-(const Vec3& u, const Vec3& v) -> Vec3 {
  return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

inline auto operator*(const Vec3& u, const Vec3& v) -> Vec3 {
  return {u[0] * v[0], u[1] * v[1], u[2] * v[2]};
}

inline auto operator*(double t, const Vec3& v) -> Vec3 { return {t * v[0], t * v[1], t * v[2]}; }

inline auto operator*(const Vec3& v, double t) -> Vec3 { return t * v; }

inline auto operator/(const Vec3& v, double t) -> Vec3 { return (1 / t) * v; }

inline auto Dot(const Vec3& u, const Vec3& v) -> double {
  return (u[0] * v[0]) + (u[1] * v[1]) + (u[2] * v[2]);
}

inline auto Cross(const Vec3& u, const Vec3& v) -> Vec3 {
  return {(u[1] * v[2]) - (u[2] * v[1]), (u[2] * v[0]) - (u[0] * v[2]),
          (u[0] * v[1]) - (u[1] * v[0])};
}

inline auto UnitVector(const Vec3& v) -> Vec3 { return v / v.Length(); }

inline auto RandomUnitVector() -> Vec3 {
  while (true) {
    const Vec3 p = Vec3::Random(-1, 1);
    const double len_sq = p.LengthSquared();
    if (1e-160 < len_sq && len_sq <= 1) {
      return p / sqrt(len_sq);
    }
  }
}

inline auto RandomOnHemisphere(const Vec3& normal) -> Vec3 {
  Vec3 on_unit_hemisphere = RandomUnitVector();
  if (Dot(on_unit_hemisphere, normal) > 0.0) {  // In the same hemisphere as the normal
    return on_unit_hemisphere;
  }
  return -on_unit_hemisphere;
}

inline auto Reflect(const Vec3& v, const Vec3& n) -> Vec3 { return v - 2 * Dot(v, n) * n; }

inline auto Refract(const Vec3& uv, const Vec3& n, double eta_i_over_eta_t) -> Vec3 {
  const auto cos_theta = std::fmin(Dot(-uv, n), 1.0);
  const Vec3 r_out_perp = eta_i_over_eta_t * (uv + cos_theta * n);
  const Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.LengthSquared())) * n;
  return r_out_perp + r_out_parallel;
}
