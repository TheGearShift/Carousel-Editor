#pragma once

#include <SDL2/SDL.h>
#include <string>

struct WindowProps {
    std::string title;
    int width;
    int height;

    WindowProps(const std::string title = "Application", int width = 800, int height = 600)
        : title(title), width(width), height(height) {
    }
};

class Window {
public:
    Window(const WindowProps& props);
    ~Window();
    void swapBuffers();
    void setWindowSize();
    SDL_Window* getNativeWindow();
    SDL_GLContext& getContext();
    int getWidth();
    int getHeight();

private:
    SDL_Window* nativeWindow;
    SDL_GLContext glContext;
    int width;
    int height;
};