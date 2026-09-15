#pragma once

#include "src/MML_LiveAudio.hpp"
#include <queue>
#include <SDL.h>

class AUDIO_HANDLER {
public:
    MML_LiveAudio::LiveAudioHandler liveAudioHandler;
    std::vector<int16_t> getSamples(const int sampleCount);
    void loadNewMML(const std::vector<std::string>& mml);
    void clearMML();
    void registerOneShot(const std::vector<std::string>& mml);
    void clearQueue();
    void setAudioID(const SDL_AudioDeviceID id);
    void pauseAudioDevice(bool state) const;
    void close();
private:
    SDL_AudioDeviceID audioID;
    bool queueReset = false;
    std::queue<int16_t> oneShotQueue;
};