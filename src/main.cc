#include <cmath>
#include <memory>

#include "camera.hh"
#include "common.hh"
#include "hittable_list.hh"
#include "material.hh"
#include "sphere.hh"
#include "vec3.hh"

// TODO: Remove NOLINT
// NOLINTNEXTLINE(bugprone-exception-escape)
auto main() -> int {
  HittableList world;

  const double r = std::cos(kPi / 4);
  auto material_left = std::make_shared<Lambertian>(Color(0, 0, 1));
  auto material_right = std::make_shared<Lambertian>(Color(1, 0, 0));

  world.Add(std::make_shared<Sphere>(Point3(-r, 0.0, -1.0), r, material_left));
  world.Add(std::make_shared<Sphere>(Point3(r, 0.0, -1.0), r, material_right));

  Camera cam;
  cam.SetAspectRatio(16.0 / 9.0);
  cam.SetImageWidth(400);
  cam.SetSamplePerPixel(100);
  cam.SetMaxDepth(50);
  cam.SetVFov(90);
  cam.Render(world);
}
