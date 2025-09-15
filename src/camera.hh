#pragma once

#include <algorithm>
#include <iostream>

#include "color.hh"
#include "common.hh"
#include "hittable.hh"
#include "ray.hh"
#include "vec3.hh"

class Camera {
 public:
  auto Render(const Hittable& world) -> void {
    Initialize();

    std::cout << "P3\n" << image_width_ << ' ' << image_height_ << "\n255\n";

    for (int j = 0; j < image_height_; j++) {
      std::clog << "\rScanlines remaining: " << (image_height_ - j) << ' ' << std::flush;
      for (int i = 0; i < image_width_; i++) {
        Color pixel_color(0, 0, 0);
        for (int sample = 0; sample < samples_per_pixel_; sample++) {
          const Ray r = GetRay(i, j);
          pixel_color += RayColor(r, world);
        }
        WriteColor(std::cout, pixel_samples_scale_ * pixel_color);
      }
    }
    std::clog << "\rDone.                 \n";
  }

  constexpr auto AspectRatio(double ratio) -> void { aspect_ratio_ = ratio; }

  constexpr auto ImageWidth(int width) -> void { image_width_ = width; }

  constexpr auto SamplePerPixel(int sample) -> void { samples_per_pixel_ = sample; }

 private:
  auto Initialize() -> void {
    image_height_ = std::max(1, static_cast<int>(image_width_ / aspect_ratio_));
    pixel_samples_scale_ = 1.0 / samples_per_pixel_;

    // Determine viewport dimensions.
    constexpr const double kFocalLength = 1.0;
    constexpr const double kViewportHeight = 2.0;
    const double viewport_width =
        kViewportHeight * static_cast<double>(image_width_) / image_height_;

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const Vec3 viewport_u{viewport_width, 0, 0};
    const Vec3 viewport_v{0, -kViewportHeight, 0};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u_ = viewport_u / image_width_;
    pixel_delta_v_ = viewport_v / image_height_;

    // Calculate the location of the upper left pixel.
    const Vec3 viewport_upper_left =
        center_ - Vec3(0, 0, kFocalLength) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
  }

  static auto RayColor(const Ray& r, const Hittable& world) -> Color {
    HitRecord rec;
    if (world.Hit(r, Interval(0, kInfinity), rec)) {
      return 0.5 * (rec.Normal() + Color(1, 1, 1));
    }
    const Vec3 unit_direction = UnitVector(r.Direction());
    const double a = 0.5 * (unit_direction.Y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
  }

  auto GetRay(int i, int j) -> Ray {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location (i, j).
    const Vec3 offset = SampleSquare();
    const Vec3 pixel_sample =
        pixel00_loc_ + ((i + offset.X()) * pixel_delta_u_) + ((j + offset.Y()) * pixel_delta_v_);

    const Vec3 ray_origin = center_;
    const Vec3 ray_direction = pixel_sample - ray_origin;
    return {ray_origin, ray_direction};
  }

  [[nodiscard]] static auto SampleSquare() -> Vec3 {
    // Return the vector to a aa random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return {RandomDouble() - 0.5, RandomDouble() - 0.5, 0};
  }

  double aspect_ratio_{};
  int samples_per_pixel_{};
  double pixel_samples_scale_{};  // Color scale factor for a sum of pixel samples
  int image_width_{};
  int image_height_{};      // Rendered image height
  Point3 center_{0, 0, 0};  // Camera center
  Point3 pixel00_loc_;      // Location of pixel (0, 0)
  Vec3 pixel_delta_u_;      // Offset to pixel to the right
  Vec3 pixel_delta_v_;      // Offset to pixel below
};
