#pragma once
#include "engine/engine.hpp"

class Game{

public:
  Engine engine{};
  virtual void init()=0;
  virtual void update(double dt)=0;

  void run();
};
