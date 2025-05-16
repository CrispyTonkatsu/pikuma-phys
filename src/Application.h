#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Particle.h"

class Application {
private:

  Particle* particle{nullptr};

  int time_prev_frame{0};

  bool running = false;

public:

  Application() = default;
  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;
  ~Application() = default;

  bool IsRunning();
  void Setup();
  void Input();
  void Update();
  void Render();
  void Destroy();
};

#endif
