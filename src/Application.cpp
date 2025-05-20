#include "Application.h"
#include <algorithm>
#include <execution>
#include "Graphics.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include "Physics/Body.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"

bool IsInRect(Body& body, SDL_Rect& rect) {
  // NOLINTBEGIN
  return (body.position.x >= rect.x
          && body.position.x <= (rect.x + rect.w))
      && (body.position.y >= rect.y
          && body.position.y <= (rect.y + rect.h));
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
      bodies.emplace_back(
        std::make_unique<Body>(
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

          auto& body = *bodies.front();

          const float strength = 10000.f;

          if (event.key.keysym.sym == SDLK_UP) {
            body.AddForce({0.f, -strength});
          }
          if (event.key.keysym.sym == SDLK_DOWN) {
            body.AddForce({0.f, strength});
          }
          if (event.key.keysym.sym == SDLK_LEFT) {
            body.AddForce({-strength, 0.f});
          }
          if (event.key.keysym.sym == SDLK_RIGHT) {
            body.AddForce({strength, 0.f});
          }
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          bodies.emplace_back(
            std::make_unique<Body>(
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

  for (auto& body: bodies) {
    body->AddForce(force::GenerateWeight(*body));
    body->AddForce(force::GenerateDragSimple(*body, 0.01));

    for (auto& anchor_body: bodies) {
      if (body == anchor_body) {
        continue;
      }

      body->AddForce(
        force::GenerateSpring(*body, anchor_body->position, 200.f, 1500.f)
      );
    }

    // if (IsInRect(*body, liquid)) {
    //   body->AddForce(force::GenerateDragSimple(*body, 0.04));
    // }
  }

  for (auto& body: bodies) {
    body->Integrate(delta_time);
  }

  for (auto& body: bodies) {
    // Keep the body in the screen (The entire circle)
    const Vec2 screen_start = Vec2(body->radius, body->radius);

    const Vec2 screen_end = Vec2(
      static_cast<float>(Graphics::Width()) - body->radius,
      static_cast<float>(Graphics::Height()) - body->radius
    );

    if (body->position.x > screen_end.x
        || body->position.x < screen_start.x) {
      body->position.x =
        std::clamp(body->position.x, screen_start.x, screen_end.x);

      body->velocity.x *= -1.f;
    }

    if (body->position.y > screen_end.y
        || body->position.y < screen_start.y) {
      body->position.y =
        std::clamp(body->position.y, screen_start.y, screen_end.y);

      body->velocity.y *= -0.4f;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
  Graphics::ClearScreen(0xFF056263);

  DrawSDLRect(liquid, 0xFF6E3713);

  for (std::unique_ptr<Body>& body: bodies) {
    Graphics::DrawFillCircle(
      static_cast<int>(body->position.x),
      static_cast<int>(body->position.y),
      static_cast<int>(body->radius),
      0xFFFFFFFF
    );

    for (auto& anchor_body: bodies) {
      if (body == anchor_body) {
        continue;
      }

      Graphics::DrawLine(
        body->position.x,
        body->position.y,
        anchor_body->position.x,
        anchor_body->position.y,
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
