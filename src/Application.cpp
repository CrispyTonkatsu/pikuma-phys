#include "Application.h"
#include <algorithm>
#include <execution>
#include <memory>
#include "Graphics.h"
#include "Physics/Constants.h"
#include "Physics/Particle.h"
#include "SDL_timer.h"

bool Application::IsRunning() { return running; }

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
  running = Graphics::OpenWindow();

  particles.emplace_back(
    std::make_unique<Particle>(//
      Vec2(50.f, 50.f), 1.f, 4.f)
  );

  particles.emplace_back(
    std::make_unique<Particle>(//
      Vec2(50.f, 50.f), 3.f, 12.f)
  );
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: running = false; break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false;
        }
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {

  // Check if we are too fast, if so, wait until the desired time per frame
  // is reached
  int time_to_wait =
    MILISECONDS_PER_FRAME - static_cast<int>(SDL_GetTicks() - time_prev_frame);

  if (time_to_wait > 0) {
    SDL_Delay(time_to_wait);
  }

  float delta_time =
    static_cast<float>(SDL_GetTicks() - time_prev_frame) / 1000;

  // Clamping the delta time so that debugging is ok
  delta_time = std::clamp(delta_time, 0.f, MAX_DELTA_TIME);

  time_prev_frame = static_cast<int>(SDL_GetTicks());

  for (std::unique_ptr<Particle>& particle: particles) {
    particle->AddForce(Vec2(10.f, 0.f));
    particle->Integrate(delta_time);

    // Keep the particle in the screen (The entire circle)
    const Vec2 screen_start = Vec2(particle->radius, particle->radius);

    const Vec2 screen_end = Vec2(
      static_cast<float>(Graphics::Width()) - particle->radius,
      static_cast<float>(Graphics::Height()) - particle->radius
    );

    if (particle->position.x > screen_end.x
        || particle->position.x < screen_start.x) {
      particle->position.x =
        std::clamp(particle->position.x, screen_start.x, screen_end.x);

      particle->velocity.x *= -1.f;
    }

    if (particle->position.y > screen_end.y
        || particle->position.y < screen_start.y) {
      particle->position.y =
        std::clamp(particle->position.y, screen_start.y, screen_end.y);

      particle->velocity.y *= -1.f;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
  Graphics::ClearScreen(0xFF056263);

  for (std::unique_ptr<Particle>& particle: particles) {
    Graphics::DrawFillCircle(
      particle->position.x,
      particle->position.y,
      particle->radius,
      0xFFFFFFFF
    );
  }

  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() { Graphics::CloseWindow(); }
