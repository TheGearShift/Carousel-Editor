#include "Core/Application.hpp"
#include <SDL2/SDL.h>

Application* Application::instance = NULL;

Application& Application::Get() { return *instance; }

Application::Application() : currentScene(NULL) {
    SDL_assert(!instance);
    instance = this;
}

Application::~Application() {
    if (currentScene) {
        delete(currentScene);
    }
    delete(window);

    SDL_Quit();
}

void Application::start(const std::string title, int width, int height) {
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
        printf("[ERROR] Could not initialize SDL subsystems:\n - %s\n", SDL_GetError());
        SDL_Quit();
    }

    window = new Window(WindowProps(title, width, height));
}

void Application::pushScene(Scene* scene) {
    currentScene = scene;
    currentScene->start();
}

void Application::run() {
    float elapsedTime = 0.0f;
    float previousTick = (float) SDL_GetTicks64();

    while (currentScene->shouldRun()) {
        float currentTick = (float) SDL_GetTicks64();
        elapsedTime += currentTick - previousTick;
        float deltaTime = (currentTick - previousTick) / 1000.0f;
        previousTick = currentTick;

        currentScene->handleEvents();
        currentScene->update(deltaTime);
        currentScene->render();
    }
}

Window& Application::getWindow() { return *window; }