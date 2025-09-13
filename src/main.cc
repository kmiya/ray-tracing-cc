#include <algorithm>
#include <iostream>
#include <memory>

#include "color.hh"
#include "common.hh"
#include "hittable.hh"
#include "hittable_list.hh"
#include "ray.hh"
#include "sphere.hh"
#include "vec3.hh"

namespace {
auto RayColor(const Ray& r, const Hittable& world) -> Color {
  HitRecord rec;
  if (world.Hit(r, Interval(0, kInfinity), rec)) {
    return 0.5 * (rec.Normal() + Color(1, 1, 1));
  }
  const Vec3 unit_direction = UnitVector(r.Direction());
  const double a = 0.5 * (unit_direction.Y() + 1.0);
  return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

constexpr const double kAspectRatio = 16.0 / 9.0;
}  // namespace

// TODO: Remove NOLINT
// NOLINTNEXTLINE(bugprone-exception-escape)
auto main() -> int {
  //// Image
  constexpr const int kImageWidth = 400;
  // Calculate the image height, and ensure that it's at least 1.
  constexpr const int kImageHight = std::max(1, static_cast<int>(kImageWidth / kAspectRatio));

  //// World
  HittableList world;
  world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  //// Camera
  constexpr const double kFocalLength = 1.0;
  constexpr const double kViewportHeight = 2.0;
  constexpr const double kViewportWidth =
      kViewportHeight * static_cast<double>(kImageWidth) / kImageHight;
  const Point3 camera_center{0, 0, 0};

  // Calculate the vectors across the horizontal and down the vertical viewport edges.
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

      const Color pixel_color = RayColor(r, world);
      WriteColor(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                 \n";
}
