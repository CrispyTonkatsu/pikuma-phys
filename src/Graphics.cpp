#include "Graphics.h"
#include <iostream>
#include "SDL_video.h"

SDL_Window* Graphics::window = nullptr;
SDL_Renderer* Graphics::renderer = nullptr;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;

int Graphics::Width() { return windowWidth; }

int Graphics::Height() { return windowHeight; }

bool Graphics::OpenWindow() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL" << std::endl;
    return false;
  }
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  windowWidth = display_mode.w * 0.6f;
  windowHeight = display_mode.h * 0.6f;
  window = SDL_CreateWindow(
    nullptr,
    0,
    0,
    windowWidth,
    windowHeight,
    SDL_WINDOW_BORDERLESS
  );
  if (!window) {
    std::cerr << "Error creating SDL window" << std::endl;
    return false;
  }
  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (!renderer) {
    std::cerr << "Error creating SDL renderer" << std::endl;
    return false;
  }
  return true;
}

void Graphics::ClearScreen(Uint32 color) {
  SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
  SDL_RenderClear(renderer);
}

void Graphics::RenderFrame() { SDL_RenderPresent(renderer); }

void Graphics::DrawLine(int x0, int y0, int x1, int y1, Uint32 color) {
  lineColor(renderer, x0, y0, x1, y1, color); // NOLINT
}

void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color) {
  // NOLINTBEGIN
  circleColor(renderer, x, y, radius, color);
  lineColor(
    renderer,
    x,
    y,
    x + cos(angle) * radius,
    y + sin(angle) * radius,
    color
  );
  // NOLINTEND
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color) {
  filledCircleColor(renderer, x, y, radius, color); // NOLINT
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color) {
  lineColor(
    renderer,
    // NOLINTBEGIN
    x - width / 2.0,
    y - height / 2.0,
    x + width / 2.0,
    y - height / 2.0,
    color
    // NOLINTEND
  );
  lineColor(
    // NOLINTBEGIN
    renderer,
    x + width / 2.0,
    y - height / 2.0,
    x + width / 2.0,
    y + height / 2.0,
    color
    // NOLINTEND
  );
  lineColor(
    // NOLINTBEGIN
    renderer,
    x + width / 2.0,
    y + height / 2.0,
    x - width / 2.0,
    y + height / 2.0,
    color
    // NOLINTEND
  );
  lineColor(
    // NOLINTBEGIN
    renderer,
    x - width / 2.0,
    y + height / 2.0,
    x - width / 2.0,
    y - height / 2.0,
    color
    // NOLINTEND
  );
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color) {
  // NOLINTBEGIN
  boxColor(
    renderer,
    x - width / 2.0,
    y - height / 2.0,
    x + width / 2.0,
    y + height / 2.0,
    color
  );
  // NOLINTEND
}

void Graphics::DrawPolygon(
  int x,
  int y,
  const std::vector<Vec2>& vertices,
  Uint32 color
) {
  for (size_t i = 0; i < vertices.size(); i++) {
    // NOLINTBEGIN
    int currIndex = i;
    int nextIndex = (i + 1) % vertices.size();
    lineColor(
      renderer,
      vertices[currIndex].x,
      vertices[currIndex].y,
      vertices[nextIndex].x,
      vertices[nextIndex].y,
      color
    );
  }
  filledCircleColor(renderer, x, y, 1, color);
  // NOLINTEND
}

void Graphics::DrawFillPolygon(
  int x,
  int y,
  const std::vector<Vec2>& vertices,
  Uint32 color
) {
  std::vector<short> vx;
  std::vector<short> vy;

  // NOLINTBEGIN
  vx.reserve(vertices.size());
  for (const auto& vertice: vertices) {
    vx.push_back(static_cast<int>(vertice.x));
  }

  vy.reserve(vertices.size());
  for (const auto& vertice: vertices) {
    vy.push_back(static_cast<int>(vertice.y));
  }
  filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
  filledCircleColor(renderer, x, y, 1, 0xFF000000);

  // NOLINTEND
}

void Graphics::DrawTexture(
  int x,
  int y,
  int width,
  int height,
  float rotation,
  SDL_Texture* texture
) {
  SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
  float rotationDeg = rotation * 57.2958f;
  SDL_RenderCopyEx(
    renderer,
    texture,
    nullptr,
    &dstRect,
    rotationDeg,
    nullptr,
    SDL_FLIP_NONE
  );
}

void Graphics::CloseWindow() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
