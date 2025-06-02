#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <vector>
#include "Physics/Body.h"
#include "Physics/Contact.h"
#include "SDL_rect.h"

class Application {
private:

  std::vector<Contact> contacts{};

  std::vector<std::unique_ptr<Body>> bodies{};

  int time_prev_frame{0};

  bool running = false;

  SDL_Rect liquid{};

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
