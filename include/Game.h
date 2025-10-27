#pragma once
#include <raylib.h>
#include <entt/entt.hpp>

class Game {
public:
  Game(const char* title, int width, int height);
  ~Game();

  void setup();
  void frame_start();
  void handle_events();
  void update();
  void render();
  void frame_end();
  void clean();
  bool running();

private:
  int  screen_width;
  int  screen_height;
  bool isRunning{true};

  entt::registry r;
  entt::entity   paddle{entt::null};
  entt::entity   ball{entt::null};

  bool intersects(const Rectangle& a, const Rectangle& b) const;
};
