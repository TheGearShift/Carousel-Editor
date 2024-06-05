#include "Input.hpp"

bool* Input::keyState;
bool Input::leftMouseDown;
bool Input::rightMouseDown;
bool Input::leftMouseClicked;
bool Input::rightMouseClicked;
MousePosition Input::mousePosition;

void Input::Init() {
    keyState = new bool[KeyCode::NUM_KEYS];
    leftMouseDown = 0;
    rightMouseDown = 0;
    leftMouseClicked = 0;
    rightMouseClicked = 0;
    mousePosition = { 0, 0 };

    for (int i = 0; i < KeyCode::NUM_KEYS; i++) {
        keyState[i] = 0;
    }
}

void Input::HandleEvent(const SDL_Event& event) {
    if (IsValidKeyCode(event.key.keysym.scancode)) {
        if (event.type == SDL_KEYDOWN) {
            keyState[event.key.keysym.scancode] = 1;
        } else if (event.type == SDL_KEYUP) {
            keyState[event.key.keysym.scancode] = 0;
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (!leftMouseClicked && event.button.button == SDL_BUTTON_LEFT) {
            leftMouseDown = true;
            leftMouseClicked = true;
        }
        if (!rightMouseClicked && event.button.button == SDL_BUTTON_RIGHT) {
            rightMouseDown = true;
            rightMouseClicked = true;
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            leftMouseDown = false;
            leftMouseClicked = false;
        }
        if (event.button.button == SDL_BUTTON_RIGHT) {
            rightMouseDown = false;
            rightMouseClicked = false;
        }
    }
}

void Input::UpdateMousePosition() {
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
}

bool Input::IsKeyDown(Key key) { return keyState[key]; }

bool Input::IsLeftMouseDown() { return leftMouseDown; }

bool Input::IsRightMouseDown() { return rightMouseDown; }

bool Input::IsLeftMouseClicked() {
    if (leftMouseDown) {
        leftMouseDown = false;
        return true;
    }
    return false;
}

bool Input::IsRightMouseClicked() {
    if (rightMouseDown) {
        rightMouseDown = false;
        return true;
    }
    return false;
}

MousePosition Input::GetMousePosition() { return mousePosition; }

bool Input::IsValidKeyCode(SDL_Keycode keyCode) {
    return (keyCode < KeyCode::NUM_KEYS);
}