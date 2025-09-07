#pragma once

#include <array>
#include <cmath>
#include <iostream>

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

  [[nodiscard]] auto Length() const -> double {
    return std::sqrt(LengthSquared());
  }

  [[nodiscard]] auto LengthSquared() const -> double {
    return (e_[0] * e_[0]) + (e_[1] * e_[1]) + (e_[2] * e_[2]);
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

inline auto operator*(double t, const Vec3& v) -> Vec3 {
  return {t * v[0], t * v[1], t * v[2]};
}

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
