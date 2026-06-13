#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <format>
#include <glm/glm.hpp>
#include "./game/game.hpp"

class myGame : public Game{
public:
  Text fpsText = engine.makeText("fps=0", {0,0,0}, "./assets/Arial.ttf", 64);
  void init() override {
  }
  void update(double fps) override {
    engine.changeText(fpsText, std::format("fps:{}",(int)(1/fps)));
  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
