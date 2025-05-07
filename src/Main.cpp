#include "Application.h"

int main(int, char*[]) {
  Application app;

  app.Setup();

  while (app.IsRunning()) {
    app.Input();
    app.Update();
    app.Render();
  }

  app.Destroy();

  return 0;
}
