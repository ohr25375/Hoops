#pragma once

#include "src/SDL2_2DSuite.hpp"
#include "AudioHandler.hpp"

struct SYSTEM_VARIABLES {
    SDL2_2DSuite::Essentials essentials;
    AUDIO_HANDLER* audioHandler;
    SDL_AudioCallback callback;
    int audioID;
    int SAMPLE_RATE = 48000;
    void playPlace();
    void playBlip();
    void playSelect();
};

void toggleWindowSize(bool state, SDL_SCREEN::SCREEN& screen);