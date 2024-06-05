#pragma once

using Key = unsigned int;

namespace KeyCode {
    enum : Key {
        A = SDL_SCANCODE_A,
        D = SDL_SCANCODE_D,
        S = SDL_SCANCODE_S,
        W = SDL_SCANCODE_W,
        SPACE = SDL_SCANCODE_SPACE,
        L_SHIFT = SDL_SCANCODE_LSHIFT,
        ALT = SDL_SCANCODE_LALT,
        NUM_KEYS
    };
}