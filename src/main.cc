#include <algorithm>
#include <cmath>
#include <iostream>

#include "color.hh"
#include "ray.hh"
#include "vec3.hh"

namespace {
auto HitSphere(const Point3& center, const double radius, const Ray& r) -> double {
  const Vec3 oc = center - r.Origin();
  const double a = r.Direction().LengthSquared();
  const double h = Dot(r.Direction(), oc);
  const double c = oc.LengthSquared() - (radius * radius);
  const double discriminant = (h * h) - (a * c);

  if (discriminant < 0) {
    return -1.0;
  }
  return (h - std::sqrt(discriminant)) / a;
}

constexpr auto RayColor(const Ray& r) -> Color {
  const double t = HitSphere(Point3{0, 0, -1}, 0.5, r);
  if (t > 0.0) {
    const Vec3 n = UnitVector(r.At(t) - Vec3{0, 0, -1});
    return 0.5 * Color{n.X() + 1, n.Y() + 1, n.Z() + 1};
  }
  const Vec3 unit_direction = UnitVector(r.Direction());
  const double a = 0.5 * (unit_direction.Y() + 1.0);
  return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

constexpr const double kAspectRatio = 16.0 / 9.0;
}  // namespace

auto main() -> int {
  // Image
  constexpr const int kImageWidth = 400;
  // Calculate the image height, and ensure that it's at least 1.
  constexpr const int kImageHight = std::max(1, static_cast<int>(kImageWidth / kAspectRatio));

  // Camera
  constexpr const double kFocalLength = 1.0;
  constexpr const double kViewportHeight = 2.0;
  constexpr const double kViewportWidth =
      kViewportHeight * static_cast<double>(kImageWidth) / kImageHight;
  const Point3 camera_center{0, 0, 0};

  // Calculate the vectors across the horizontal and down the vertical viewport
  // edges.
  const Vec3 viewport_u{kViewportWidth, 0, 0};
  const Vec3 viewport_v{0, -kViewportHeight, 0};

  // Calculate the horizontal and vertical delta vectors from pixel to pixel.
  const Vec3 pixel_delta_u = viewport_u / kImageWidth;
  const Vec3 pixel_delta_v = viewport_v / kImageHight;

  // Calculate the location of the upper left pixel.
  const Vec3 viewport_upper_left =
      camera_center - Vec3(0, 0, kFocalLength) - viewport_u / 2 - viewport_v / 2;
  const Vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // Render
  std::cout << "P3\n" << kImageWidth << ' ' << kImageHight << "\n255\n";

  for (int j = 0; j < kImageHight; j++) {
    std::clog << "\rScanlines remaining: " << (kImageHight - j) << ' ' << std::flush;
    for (int i = 0; i < kImageWidth; i++) {
      const Vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
      const Vec3 ray_direction = pixel_center - camera_center;
      const Ray r(camera_center, ray_direction);
      const Color pixel_color = RayColor(r);
      WriteColor(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                 \n";
}
