#include "Application.h"
#include "Physics/Particle.h"

bool Application::IsRunning() { return running; }

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
  running = Graphics::OpenWindow();

  // TODO: setup objects in the scene
  particle = new Particle(Vec2(50.f, 50.f), 10.f);
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

  // TODO:Check if we are too fast, if so, wait until the desired time per frame is reached

  particle->velocity = Vec2(2.f, 0.f);
  particle->position += particle->velocity;
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
  Graphics::ClearScreen(0xFF056263);
  Graphics::DrawFillCircle(
    particle->position.x,
    particle->position.y,
    40,
    0xFFFFFFFF
  );
  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
  // TODO: destroy all objects in the scene

  delete particle;

  Graphics::CloseWindow();
}
