#include "Window.hpp"

Window::Window(const WindowProps& props) : width(props.width), height(props.height) {
    nativeWindow = SDL_CreateWindow(
        props.title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        props.width,
        props.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (!nativeWindow) {
        printf("Failed to open %d x %d window: %s\n", props.width, props.height, SDL_GetError());
        SDL_Quit();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    glContext = SDL_GL_CreateContext(nativeWindow);
    if (!glContext) {
        printf("[ERROR] Failed to create OpenGL context:\n - %s\n", SDL_GetError());
        SDL_DestroyWindow(nativeWindow);
        SDL_Quit();
    }
}

Window::~Window() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(nativeWindow);
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(nativeWindow);
}

void Window::setWindowSize() {
    SDL_GetWindowSize(nativeWindow, &width, &height);
}

SDL_Window* Window::getNativeWindow() { return nativeWindow; }

SDL_GLContext& Window::getContext() { return glContext; }

int Window::getWidth() { return width; }

int Window::getHeight() { return height; }