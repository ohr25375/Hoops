#include "systemEssentials.hpp"
#include "bgmAssets.hpp"

void SYSTEM_VARIABLES::toggleWindowSize(bool state) {
    auto& screen = essentials.screen;
    VECTOR2i size = state ? screen.size * screenSizeMultiplier : screen.size;
    SDL_SetWindowSize(screen.window, size.x, size.y);
    SDL_SetWindowPosition(screen.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void SYSTEM_VARIABLES::playPlace() {
    this->audioHandler->registerOneShot(getMML(AUDIO_ASSET_ID::AUDIO_ASSET_SFX_PLACE));
}

void SYSTEM_VARIABLES::playBlip() {
    this->audioHandler->registerOneShot({"T180 O5 P160 l32 V15 b"}, this->audioHandler->getSFXVolume());
}
void SYSTEM_VARIABLES::playBlip(const float& volume) {
    this->audioHandler->registerOneShot({"T180 O5 P160 l32 V15 b"}, volume);
}

void SYSTEM_VARIABLES::playSelect() {
    this->audioHandler->registerOneShot({"T180 O5 P160 l32 V8 f+ V7 f+ V5 f+ V3 f+"});
}

int incrementBound(const int& value, const int& max) {
    return (value + 1) % max;
}

int decrementBound(const int& value, const int& max) {
    return (value + max - 1) % max;
}