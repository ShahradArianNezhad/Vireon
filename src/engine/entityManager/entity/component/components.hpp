
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/glm.hpp>
struct RenderComponent {
  MeshID mesh;
  MaterialID material;
  bool visible=true;
};

struct TransformComponent {
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
};

struct CameraComponent {
  glm::vec3 eye;
  glm::vec3 center;
  glm::vec3 up;
};
