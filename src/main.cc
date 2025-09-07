#include <iostream>

auto main() -> int {
  // Image
  const int image_width = 256;
  const int image_height = 256;

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
              << std::flush;
    for (int i = 0; i < image_width; i++) {
      auto r = static_cast<double>(i) / (image_width - 1);
      auto g = static_cast<double>(j) / (image_height - 1);
      auto b = 0.0;

      const int ir = static_cast<int>(255.999 * r);
      const int ig = static_cast<int>(255.999 * g);
      const int ib = static_cast<int>(255.999 * b);

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
  std::clog << "\rDone.                 \n";
}
