#include <memory>

#include "camera.hh"
#include "hittable_list.hh"
#include "sphere.hh"
#include "vec3.hh"

// TODO: Remove NOLINT
// NOLINTNEXTLINE(bugprone-exception-escape)
auto main() -> int {
  HittableList world;
  world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera cam;
  cam.AspectRatio(16.0 / 9.0);
  cam.ImageWidth(400);
  cam.SamplePerPixel(100);
  cam.Render(world);
}
