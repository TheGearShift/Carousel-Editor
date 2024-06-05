#pragma once

#include <SDL2/SDL.h>
#include "KeyCodes.hpp"

struct MousePosition {
    int x;
    int y;
};

class Input {
public:
    static void Init();
    static void HandleEvent(const SDL_Event& event);
    static void UpdateMousePosition();
    static bool IsKeyDown(Key key);
    static bool IsLeftMouseDown();
    static bool IsRightMouseDown();
    static bool IsLeftMouseClicked();
    static bool IsRightMouseClicked();
    static MousePosition GetMousePosition();

private:
    static bool* keyState;
    static bool leftMouseDown;
    static bool rightMouseDown;
    static bool leftMouseClicked;
    static bool rightMouseClicked;
    static MousePosition mousePosition;

    static bool IsValidKeyCode(SDL_Keycode keyCode);
};