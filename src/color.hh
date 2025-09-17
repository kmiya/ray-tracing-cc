#pragma once

#include <iostream>

#include "interval.hh"
#include "vec3.hh"

using Color = Vec3;

inline auto LinerToGamma(double linear_component) -> double {
  if (linear_component > 0) {
    return std::sqrt(linear_component);
  }
  return 0.0;
}

constexpr void WriteColor(std::ostream& out, const Color& pixel_color) {
  double r = pixel_color.X();
  double g = pixel_color.Y();
  double b = pixel_color.Z();

  // Apply a linear to gamma transform for gamma 2
  r = LinerToGamma(r);
  g = LinerToGamma(g);
  b = LinerToGamma(b);

  // Translate the [0,1] component values to the byte range [0,255].
  static const Interval kIntensity(0.000, 0.999);
  const int r_byte = static_cast<int>(256 * kIntensity.Clamp(r));
  const int g_byte = static_cast<int>(256 * kIntensity.Clamp(g));
  const int b_byte = static_cast<int>(256 * kIntensity.Clamp(b));

  // Write out the pixel color components.
  out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}
