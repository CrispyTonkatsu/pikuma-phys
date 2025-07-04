#ifndef APPLICATION_H
#define APPLICATION_H

#include "Physics/Constants.h"
#include "Physics/World.h"

class Application {
private:

  World world{GRAVITY};

  int time_prev_frame{0};

  bool running = false;

public:

  Application() = default;
  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;
  ~Application() = default;

  [[nodiscard]] bool IsRunning() const;
  void Setup();
  void Input();
  void Update();
  void Render();
  void Destroy();
};

#endif
