#pragma once

#include "src/SDL2_2DSuite.hpp"
#include "AudioHandler.hpp"
#include <filesystem>

struct SYSTEM_VARIABLES {
    SDL2_2DSuite::Essentials essentials;
    AUDIO_HANDLER* audioHandler;
    SDL_AudioCallback callback;
    std::filesystem::path execPath;
    int SAMPLE_RATE = 48000;
    int screenSizeMultiplier = 4;
    void playPlace();
    void playBlip();
    void playBlip(const float& volume);
    void playSelect();
    void toggleWindowSize(bool state);
};
int incrementBound(const int& value, const int& max, const int& min = 0);

int decrementBound(const int& value, const int& max, const int& min = 0);