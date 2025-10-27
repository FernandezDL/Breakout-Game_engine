#pragma once
#include <raylib.h>
#include <entt/entt.hpp>
#include "Scene/Scene.h"

class Game {
public:
  Game(const char* title, int width, int height);
  ~Game();

  void setup();
  void frameStart();
  void handleEvents();
  void update();
  void render();
  void frameEnd();
  void clean();
  bool running();
  void setScene(Scene* newScene);
  Scene* getCurrentScene() const;

protected:
  int counter;
  int screen_width;
  int screen_height;
  bool isRunning;
  
  float dT = 0.0f;
  float FPS = 0.0f;
  int frameCount = 0;

  Scene* currentScene;
};
