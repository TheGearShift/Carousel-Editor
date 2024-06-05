#pragma once

#include <Core/Scene.hpp>
#include <Core/Window.hpp>
#include <string>

class Application {
public:
    static Application& Get();

    Application();
    ~Application();
    void start(const std::string title, int width = 800, int height = 600);
    void pushScene(Scene* scene);
    void run();
    Window& getWindow();

private:
    static Application* instance;

    Window* window;
    Scene* currentScene;
    uint32_t width, height;
};