#pragma once

#include <algorithm>
#include <iostream>

#include "color.hh"
#include "common.hh"
#include "hittable.hh"
#include "material.hh"
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
          pixel_color += RayColor(r, max_depth_, world);
        }
        WriteColor(std::cout, pixel_samples_scale_ * pixel_color);
      }
    }
    std::clog << "\rDone.                 \n";
  }

  constexpr auto SetAspectRatio(double ratio) -> void { aspect_ratio_ = ratio; }

  constexpr auto SetImageWidth(int width) -> void { image_width_ = width; }

  constexpr auto SetSamplePerPixel(int sample) -> void { samples_per_pixel_ = sample; }
  constexpr auto SetMaxDepth(int depth) -> void { max_depth_ = depth; }
  constexpr auto SetVFov(double degree) -> void { v_fov_ = degree; }
  constexpr auto SetLookFrom(Point3 point) -> void { look_from_ = point; }
  constexpr auto SetLookAt(Point3 point) -> void { look_at_ = point; }
  constexpr auto SetVUp(Vec3 vec) -> void { v_up_ = vec; }

 private:
  auto Initialize() -> void {
    image_height_ = std::max(1, static_cast<int>(image_width_ / aspect_ratio_));
    pixel_samples_scale_ = 1.0 / samples_per_pixel_;

    center_ = look_from_;

    // Determine viewport dimensions.
    const double focal_length = (look_from_ - look_at_).Length();
    const double theta = DegreeToRadians(v_fov_);
    const double h = std::tan(theta / 2);
    const double viewport_height = 2 * h * focal_length;
    const double viewport_width =
        viewport_height * static_cast<double>(image_width_) / image_height_;

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w_ = UnitVector(look_from_ - look_at_);
    u_ = UnitVector(Cross(v_up_, w_));
    v_ = Cross(w_, u_);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const Vec3 viewport_u{viewport_width * u_};
    const Vec3 viewport_v{viewport_height * -v_};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u_ = viewport_u / image_width_;
    pixel_delta_v_ = viewport_v / image_height_;

    // Calculate the location of the upper left pixel.
    const Vec3 viewport_upper_left =
        center_ - (focal_length * w_) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  static auto RayColor(const Ray& r, int depth, const Hittable& world) -> Color {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
      return {0, 0, 0};
    }

    HitRecord rec;
    if (world.Hit(r, Interval(0.001, kInfinity), rec)) {
      Ray scattered;
      Color attenuation;
      if (rec.Mat()->Scatter(r, rec, attenuation, scattered)) {
        return attenuation * RayColor(scattered, depth - 1, world);
      }
      return {0, 0, 0};
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

  double aspect_ratio_{1.0};
  int samples_per_pixel_{10};
  int max_depth_{10};
  double pixel_samples_scale_{};  // Color scale factor for a sum of pixel samples
  int image_width_{100};
  int image_height_{};      // Rendered image height
  Point3 center_{0, 0, 0};  // Camera center
  Point3 pixel00_loc_;      // Location of pixel (0, 0)
  Vec3 pixel_delta_u_;      // Offset to pixel to the right
  Vec3 pixel_delta_v_;      // Offset to pixel below
  Vec3 u_, v_, w_;          // Camera frame basis vectors

  double v_fov_{90};                   // Vertical view angle (field of view)
  Point3 look_from_{Point3(0, 0, 0)};  // Point camera is looking from
  Point3 look_at_{Point3(0, 0, -1)};   // Point camera is looking at
  Vec3 v_up_{Vec3(0, 1, 0)};           // Camera-relative "up" direction
};
