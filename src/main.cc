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

  const auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = RandomDouble();
      const Point3 center(a + (0.9 * RandomDouble()), 0.2, b + (0.9 * RandomDouble()));

      if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
        std::shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::Random() * Color::Random();
          sphere_material = std::make_shared<Lambertian>(albedo);
          auto center2 = center + Vec3(0, RandomDouble(0, .5), 0);
          world.Add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::Random(0.5, 1);
          auto fuzz = RandomDouble(0, 0.5);
          sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = std::make_shared<Dielectric>(1.5);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  const auto material1{std::make_shared<Dielectric>(1.5)};
  world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  const auto material2{std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1))};
  world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  const auto material3{std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0)};
  world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  Camera cam;
  cam.SetAspectRatio(16.0 / 9.0);
  cam.SetImageWidth(400);
  cam.SetSamplePerPixel(20);
  cam.SetMaxDepth(50);

  cam.SetVFov(20);
  cam.SetLookFrom(Point3{13, 2, 3});
  cam.SetLookAt(Point3{0, 0, 0});
  cam.SetVUp(Vec3{0, 1, 0});

  cam.SetDefocusAngle(0.6);
  cam.SetFocusDist(10.0);

  cam.RenderParallel(world);
}
