#include <memory>

#include <brabbit/camera.hpp>
#include <brabbit/mesh.hpp>
#include <brabbit/model.hpp>
#include <brabbit/scene.hpp>
#include <brabbit/shader.hpp>
#include <brabbit/window.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

auto main(int argc, char** argv) -> int {
  auto window = std::make_unique<brabbit::Window>("BRabbit's OpenGL Demo"sv, 1920, 1080);
  if (!window || !window->isVaild()) {
    return -1;
  }

  auto* scene = window->setScene<brabbit::Scene>(50, 50, 50);
  if (!scene) {
    return -1;
  }

  auto* camera = scene->getCamera();
  auto* light  = scene->getLight();
  if (!camera || !light) {
    return -1;
  }

  camera->setPosition({ 0.0f, 0.0f, 3.0f });
  light->setLampVisible(true);

  auto  mesh  = std::make_unique<brabbit::Mesh>("cube.stl"sv);
  auto* model = scene->emplaceObject<brabbit::Model>(mesh);
  if (!model) {
    return -1;
  }

  return window->executeLoop();
}
