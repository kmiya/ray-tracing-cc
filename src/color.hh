#pragma once

#include <iostream>

#include "interval.hh"
#include "vec3.hh"

using Color = Vec3;

constexpr void WriteColor(std::ostream& out, const Color& pixel_color) {
  // Translate the [0,1] component values to the byte range [0,255].
  static const Interval kIntensity(0.000, 0.999);
  const int r_byte = static_cast<int>(256 * kIntensity.Clamp(pixel_color.X()));
  const int g_byte = static_cast<int>(256 * kIntensity.Clamp(pixel_color.Y()));
  const int b_byte = static_cast<int>(256 * kIntensity.Clamp(pixel_color.Z()));

  // Write out the pixel color components.
  out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}
