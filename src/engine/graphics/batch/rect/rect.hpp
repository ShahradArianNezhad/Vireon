#include "engine/graphics/renderable/renderable.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Rect : public Renderable {
public:
  glm::vec3 pos;
  int width, height;
};
