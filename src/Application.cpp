#include "Application.h"
#include <algorithm>
#include <memory>
#include "Graphics.h"
#include "Physics/Constants.h"
#include "Physics/Body.h"
#include "Physics/Constraint.h"
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

  Vec2 screen_center(Graphics::Width() * 0.5f, Graphics::Height() * 0.5f);

  Body& center = world.AddBody(
    std::make_unique<Body>(
      std::make_unique<CircleShape>(100.f),
      screen_center,
      0.f
    )
  );

  world.AddBody(
    std::make_unique<Body>(
      std::make_unique<CircleShape>(50.f),
      screen_center - Vec2{200.f, 200.f},
      0.f
    )
  );

  world.constraints.push_back(
    std::make_unique<JointConstraint>(
      world.bodies[0].get(),
      world.bodies[1].get(),
      screen_center
    )
  );
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
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {
          bool left = (event.button.button == SDL_BUTTON_LEFT);
          bool right(event.button.button == SDL_BUTTON_RIGHT);

          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);

          if (left) {
            world
              .AddBody(
                std::make_unique<Body>(
                  std::make_unique<BoxShape>(50.f, 50.f),
                  Vec2(static_cast<float>(x), static_cast<float>(y)),
                  1.f,
                  0.8f,
                  0.9f
                )
              )
              .SetTexture("./assets/crate.png");
          }

          if (right) {
            world
              .AddBody(
                std::make_unique<Body>(
                  std::make_unique<CircleShape>(25.f),
                  Vec2(static_cast<float>(x), static_cast<float>(y)),
                  1.f,
                  0.8f,
                  0.9f
                )
              )
              .SetTexture("./assets/basketball.png");
          }
        }
        break;
      case SDL_MOUSEMOTION:
        {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          // bodies.back()->position = Vec2(x, y);
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

  world.Update(delta_time);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
  Graphics::ClearScreen(0xFF056263);

  auto& bodies = world.GetBodies();

  // This is just for nicer reading rendering, the course does not do this
  // because the rendering should be done by the user of the physics library
  for (std::unique_ptr<Body>& body: bodies) {
    if (body->shape == nullptr) {
      continue;
    }

    if (body->texture != nullptr) {
      int width{0};
      int height{0};

      if (body->shape->GetType() == ShapeType::CIRCLE) {
        width = body->shape->as<CircleShape>()->radius * 2;
        height = body->shape->as<CircleShape>()->radius * 2;
      } else if (body->shape->GetType() == ShapeType::BOX) {
        width = body->shape->as<BoxShape>()->width;
        height = body->shape->as<BoxShape>()->height;
      }

      Graphics::DrawTexture(
        body->position.x,
        body->position.y,
        width,
        height,
        body->rotation,
        body->texture
      );
    } else {
      body->shape->DebugRender(body->position, body->rotation, 0xFFFFFFFF);
    }
  }

  auto contacts = world.GetContacts();

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
