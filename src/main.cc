#include <iostream>

// #include "vec3.hh"
#include "color.hh"

auto main() -> int {
  // Image
  const int image_width = 256;   // NOLINT(readability-magic-numbers)
  const int image_height = 256;  // NOLINT(readability-magic-numbers)

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
              << std::flush;
    for (int i = 0; i < image_width; i++) {
      auto pixel_color =
          Color(static_cast<double>(i) / (image_width - 1),
                static_cast<double>(j) / (image_height - 1), 0.0);

      WriteColor(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                 \n";
}
