#include "keypress.hpp"

KEYS::KEYS() {
    keys = std::map<long, KEYPRESS>();
}

void KEYS::update() {
    for (auto& [_, k] : keys) {
        k.update();
    }
}

KEYPRESS& KEYS::operator[](const long n) {
    return keys[n];
}

KEYPRESS KEYS::operator[](const long n) const {
    return keys.at(n);
}

KEYPRESS::KEYPRESS() : down(false), up(false), isHold(false), lastState(false), state(false) {}

void KEYPRESS::update() {
    if (state != lastState) {
        down      = state;
        up        = !state;
        isHold    = false;
        lastState = state;
        return;
    }
    if (state) {
        if (isHold) return;
        if (!down) return;

        down   = false;
        isHold = true;
        return;
    } else {
        if (!up) return;

        up = false;
    }
}

void MOUSE::update() {
    Uint32 state = SDL_GetMouseState(&x, &y);
    SDL_GetRelativeMouseState(&xd, &yd);
    xdir         = xd * mouseSensitivity;
    ydir         = yd * mouseSensitivity;
    left.state   = (state & SDL_BUTTON(1)) != 0;
    middle.state = (state & SDL_BUTTON(2)) != 0;
    right.state  = (state & SDL_BUTTON(3)) != 0;
    left.update();
    middle.update();
    right.update();
}

void updateKeys(std::map<int, KEYPRESS>& keys) {
    for (auto& [_, k] : keys) {
        k.update();
    }
}

bool isAnyKeyDown(std::map<int, KEYPRESS>& keys) {
    for (auto [_, key] : keys) {
        if (key.down) {
            return true;
        }
    }
    return false;
}
