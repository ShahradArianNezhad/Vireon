#include <glm/fwd.hpp>
#include <optional>
#include <string>
#include <vector>

class ObjectData {

private:
  std::vector<float> vertexData;
  std::optional<std::string> texturePath;

public:
  ObjectData(std::vector<float> data) : vertexData(data) {};
  ObjectData(std::vector<float> data, std::string texPath)
      : vertexData(data), texturePath(texPath) {};
  ObjectData(const ObjectData &other)
      : vertexData(other.vertexData), texturePath(other.texturePath) {}
  glm::vec3 position();
  std::vector<float> &getVertexData() { return vertexData; }
  int getDataCount() { return vertexData.size(); }
};
