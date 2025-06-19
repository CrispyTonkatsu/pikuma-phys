#include "Application.h"
#include <algorithm>
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

  // bodies.emplace_back(
  //   std::make_unique<Body>(
  //     std::make_unique<BoxShape>(Graphics::Width(), 100.f),
  //     Vec2(Graphics::Width<float>() * 0.5f, Graphics::Height<float>() *
  //     0.9f), 0.f
  //   )
  // );
  //
  // bodies.emplace_back(
  //   std::make_unique<Body>(
  //     std::make_unique<BoxShape>(
  //       Graphics::Width() * 0.1f,
  //       Graphics::Height() * 0.7f
  //     ),
  //     Vec2(Graphics::Width<float>() * 0.01f, Graphics::Height<float>() *
  //     0.5f), 0.f
  //   )
  // );
  //
  // bodies.emplace_back(
  //   std::make_unique<Body>(
  //     std::make_unique<BoxShape>(
  //       Graphics::Width() * 0.1f,
  //       Graphics::Height() * 0.7f
  //     ),
  //     Vec2(
  //       Graphics::Width<float>() * (1 - 0.01f),
  //       Graphics::Height<float>() * 0.5f
  //     ),
  //     0.f
  //   )
  // );
  //
  // bodies
  //   .emplace_back(
  //     std::make_unique<Body>(
  //       std::make_unique<BoxShape>(200.f, 100.f),
  //       Vec2(Graphics::Width<float>() * 0.5f, Graphics::Height<float>() * 0.5f),
  //       0.f
  //     )
  //   )
  //   ->rotation = 15.f * (std::numbers::pi_v<float> / 180.f);

  bodies.emplace_back(
    std::make_unique<Body>(
      std::make_unique<BoxShape>(100.f, 100.f),
      Vec2(Graphics::Width<float>() * 0.5f, Graphics::Height<float>() * 0.5f),
      0.f
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
          bool left = (event.button.button == SDL_BUTTON_LEFT);
          bool right(event.button.button == SDL_BUTTON_RIGHT);

          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);

          if (left) {
            bodies.emplace_back(
              std::make_unique<Body>(
                std::make_unique<BoxShape>(50.f, 50.f),
                Vec2(static_cast<float>(x), static_cast<float>(y)),
                1.f,
                0.f,
                1.f
              )
            );
          }

          if (right) {
            bodies.emplace_back(
              std::make_unique<Body>(
                std::make_unique<CircleShape>(25.f),
                Vec2(static_cast<float>(x), static_cast<float>(y)),
                1.f,
                0.f,
                1.f
              )
            );
          }
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

  // Clamping the delta time so that debugging is or
  delta_time = std::clamp(delta_time, 0.f, MAX_DELTA_TIME);

  time_prev_frame = static_cast<int>(SDL_GetTicks());

  contacts.clear();

  for (auto& body: bodies) {
    // body->AddForce(force::GenerateWeight(*body));
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
        contacts.push_back(contact_opt.value());
      }
    }
  }

  for (auto& contact: contacts) {
    contact.ResolveCollision();
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

    body->shape->DebugRender(body->position, body->rotation, 0xFFFFFFFF);
  }

  for (auto& contact: contacts) {
    // NOLINTBEGIN
    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
    Graphics::DrawLine(
      contact.start.x,
      contact.start.y,
      contact.end.x,
      contact.end.y,
      0xFFFF00FF
    );
    Graphics::DrawLine(
      contact.end.x,
      contact.end.y,
      contact.end.x + contact.normal.x * contact.depth,
      contact.end.y + contact.normal.y * contact.depth,
      0xFF00FFFF
    );
    // NOLINTEND
  }

  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() { Graphics::CloseWindow(); } // NOLINT
