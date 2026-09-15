#include "systemEssentials.hpp"
#include "bgmAssets.hpp"

void toggleWindowSize(bool state, SDL_SCREEN::SCREEN& screen) {
    VECTOR2i size = state ? screen.size * 4 : screen.size;
    SDL_SetWindowSize(screen.window, size.x, size.y);
    SDL_SetWindowPosition(screen.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void SYSTEM_VARIABLES::playPlace() {
    this->audioHandler->registerOneShot(getMML(AUDIO_ASSET_ID::AUDIO_ASSET_SFX_PLACE));
}

void SYSTEM_VARIABLES::playBlip() {
    this->audioHandler->registerOneShot({"T180 O5 P160 l32 V15 b"});
}

void SYSTEM_VARIABLES::playSelect() {
    this->audioHandler->registerOneShot({"T180 O5 P160 l32 V8 f+ V11 f+ V7 f+ V3 f+"});
}
