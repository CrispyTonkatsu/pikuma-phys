#include "Application.h"
#include <algorithm>
#include <execution>
#include "Graphics.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include "Physics/Particle.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"

bool IsInRect(Particle& particle, SDL_Rect& rect) {
  // NOLINTBEGIN
  return (particle.position.x >= rect.x
          && particle.position.x <= (rect.x + rect.w))
      && (particle.position.y >= rect.y
          && particle.position.y <= (rect.y + rect.h));
  // NOLINTEND
}

void DrawSDLRect(SDL_Rect& rect, Uint32 color) {
  Graphics::DrawFillRect(
    rect.x + (rect.w / 2),
    rect.y + (rect.h / 2),
    rect.w,
    rect.h,
    color
  );
}

bool Application::IsRunning() const { return running; }

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
  running = Graphics::OpenWindow();

  const float spacing = 100.f;

  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      particles.emplace_back(
        std::make_unique<Particle>(
          Vec2(
            Graphics::Width<float>() * 0.4f + i * spacing,
            Graphics::Height<float>() * 0.1f + j * spacing
          ),
          1.f,
          6.f
        )
      );
    }
  }

  liquid.x = 0;
  liquid.y = 0;

  liquid.w = Graphics::Width();
  liquid.h = Graphics::Height() * 0.1f;
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        {
          running = false;
        }
        break;
      case SDL_KEYDOWN:
        {
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
          }

          auto& particle = *particles.front();

          const float strength = 10000.f;

          if (event.key.keysym.sym == SDLK_UP) {
            particle.AddForce({0.f, -strength});
          }
          if (event.key.keysym.sym == SDLK_DOWN) {
            particle.AddForce({0.f, strength});
          }
          if (event.key.keysym.sym == SDLK_LEFT) {
            particle.AddForce({-strength, 0.f});
          }
          if (event.key.keysym.sym == SDLK_RIGHT) {
            particle.AddForce({strength, 0.f});
          }
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          particles.emplace_back(
            std::make_unique<Particle>(
              Vec2(static_cast<float>(x), static_cast<float>(y)),
              1.f,
              4.f
            )
          );
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

  for (auto& particle: particles) {
    particle->AddForce(force::GenerateWeight(*particle));
    particle->AddForce(force::GenerateDragSimple(*particle, 0.01));

    for (auto& anchor_particle: particles) {
      if (particle == anchor_particle) {
        continue;
      }

      particle->AddForce(
        force::GenerateSpring(*particle, anchor_particle->position, 200.f, 1500.f)
      );
    }

    // if (IsInRect(*particle, liquid)) {
    //   particle->AddForce(force::GenerateDragSimple(*particle, 0.04));
    // }
  }

  for (auto& particle: particles) {
    particle->Integrate(delta_time);
  }

  for (auto& particle: particles) {
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

      particle->velocity.y *= -0.4f;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
  Graphics::ClearScreen(0xFF056263);

  DrawSDLRect(liquid, 0xFF6E3713);

  for (std::unique_ptr<Particle>& particle: particles) {
    Graphics::DrawFillCircle(
      static_cast<int>(particle->position.x),
      static_cast<int>(particle->position.y),
      static_cast<int>(particle->radius),
      0xFFFFFFFF
    );

    for (auto& anchor_particle: particles) {
      if (particle == anchor_particle) {
        continue;
      }

      Graphics::DrawLine(
        particle->position.x,
        particle->position.y,
        anchor_particle->position.x,
        anchor_particle->position.y,
        0xFF6E3713
      );
    }
  }

  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() { Graphics::CloseWindow(); } // NOLINT
