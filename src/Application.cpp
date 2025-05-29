#include "Application.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include "Graphics.h"
#include "Physics/Collision.h"
#include "Physics/Constants.h"
#include "Physics/Body.h"
#include "Physics/Force.h"
#include "Physics/Shape.h"
#include "Physics/Vec2.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"

bool IsInRect(Body& body, SDL_Rect& rect) {
  // NOLINTBEGIN
  return (body.position.x >= rect.x && body.position.x <= (rect.x + rect.w))
      && (body.position.y >= rect.y && body.position.y <= (rect.y + rect.h));
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

  bodies.emplace_back(
    std::make_unique<Body>(
      std::make_unique<CircleShape>(100.f),
      Vec2(Graphics::Width<float>() * 0.1f, Graphics::Height<float>() * 0.1f),
      1.f
    )
  );

  bodies.emplace_back(
    std::make_unique<Body>(
      std::make_unique<CircleShape>(200.f),
      Vec2(Graphics::Width<float>() * 0.5f, Graphics::Height<float>() * 0.5f),
      1.f
    )
  );

  liquid.x = 0;
  liquid.y = 0;

  liquid.w = Graphics::Width();
  liquid.h = Graphics::Height() * 0.1f; // NOLINT
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
              std::make_unique<CircleShape>(6.f),
              Vec2(static_cast<float>(x), static_cast<float>(y)),
              1.f
            )
          );
        }
        break;
      case SDL_MOUSEMOTION:
        {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          bodies[0]->position = Vec2(x, y);
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
    // body->AddForce(force::GenerateWeight(*body));
    //
    // body->AddTorque(200.f);
    //
    // if (IsInRect(*body, liquid)) {
    //   body->AddForce(force::GenerateDragSimple(*body, 0.04));
    // }
  }

  for (auto& body: bodies) {
    body->Update(delta_time);
    body->isColliding = false;
  }

  for (size_t i = 0; i < bodies.size() - 1; i++) {
    for (size_t j = i + 1; j < bodies.size(); j++) {

      auto contact_opt =
        collision_detection::IsColliding(*bodies[i], *bodies[j]);

      if (contact_opt.has_value()) {
        bodies[i]->isColliding = true;
        bodies[j]->isColliding = true;
      }
    }
  }

  // TODO: refactor into something that can be used generically for all body
  // types
  for (auto& body: bodies) {
    auto* circle_opt = body->shape->as<CircleShape>();
    if (circle_opt == nullptr) {
      continue;
    }

    auto& circle = *circle_opt;

    const Vec2 screen_start = Vec2(circle.radius, circle.radius);

    const Vec2 screen_end = Vec2(
      static_cast<float>(Graphics::Width()) - circle.radius,
      static_cast<float>(Graphics::Height()) - circle.radius
    );

    if (body->position.x > screen_end.x || body->position.x < screen_start.x) {
      body->position.x =
        std::clamp(body->position.x, screen_start.x, screen_end.x);

      body->velocity.x *= -1.f;
    }

    if (body->position.y > screen_end.y || body->position.y < screen_start.y) {
      body->position.y =
        std::clamp(body->position.y, screen_start.y, screen_end.y);

      body->velocity.y *= -1.f;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
  Graphics::ClearScreen(0xFF056263);

  DrawSDLRect(liquid, 0xFF6E3713);

  // This is just for nicer reading rendering, the course does not do this
  // because the rendering should be done by the user of the physics library
  for (std::unique_ptr<Body>& body: bodies) {
    if (body->shape == nullptr) {
      continue;
    }

    body->shape->DebugRender(
      body->position,
      body->rotation,
      body->isColliding ? 0xFF0000FF : 0xFFFFFFFF
    );
  }

  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() { Graphics::CloseWindow(); } // NOLINT
