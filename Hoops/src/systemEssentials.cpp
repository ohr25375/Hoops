#include "systemEssentials.hpp"
#include "bgmAssets.hpp"
#include "sfxAssets.hpp"

void SYSTEM_VARIABLES::toggleWindowSize(bool state) {
    auto& screen = essentials.screen;
    const VECTOR2i size = state ? screen.size * screenSizeMultiplier : screen.size;
    SDL_SetWindowSize(screen.window, size.x, size.y);
    SDL_SetWindowPosition(screen.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void SYSTEM_VARIABLES::playPlace() {
    audioHandler->registerOneShot(getMML(SFX_ASSET_ID::PLACE));
}

void SYSTEM_VARIABLES::playBlip() {
    playBlip(audioHandler->getSFXVolume());
}
void SYSTEM_VARIABLES::playBlip(const float& volume) {
    audioHandler->registerOneShot(getMML(SFX_ASSET_ID::BLIP), volume);
}

void SYSTEM_VARIABLES::playSelect() {
    audioHandler->registerOneShot(getMML(SFX_ASSET_ID::SELECT));
}

int incrementBound(const int& value, const int& max, const int& min) {
    auto res = (value + 1) % max;
    if (res < min) {
        res = min;
    }
    return res;
}

int decrementBound(const int& value, const int& max, const int& min) {
    auto res = (value + max - 1) % max;
    if (res < min) {
        res = max - 1;
    }
    return res;
}