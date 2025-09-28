#include <memory>

#include "camera.hh"
#include "hittable_list.hh"
#include "material.hh"
#include "sphere.hh"
#include "vec3.hh"

// TODO: Remove NOLINT
// NOLINTNEXTLINE(bugprone-exception-escape)
auto main() -> int {
  HittableList world;

  auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto material_left = std::make_shared<Dielectric>(1.50);
  auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
  auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
  world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

  Camera cam;
  cam.SetAspectRatio(16.0 / 9.0);
  cam.SetImageWidth(400);
  cam.SetSamplePerPixel(100);
  cam.SetMaxDepth(50);

  cam.SetVFov(20);
  cam.SetLookFrom(Point3{-2, 2, 1});
  cam.SetLookAt(Point3{0, 0, -1});
  cam.SetVUp(Vec3{0, 1, 0});

  cam.Render(world);
}
