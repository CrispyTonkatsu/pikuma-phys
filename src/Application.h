#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"

class Application {
    private:
        bool running = false;

    public:
        Application() = default;
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;
        ~Application() = default;

        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
