#pragma once

#include "src/MML_LiveAudio.hpp"
#include <queue>

class AUDIO_HANDLER {
public:
    MML_LiveAudio::LiveAudioHandler liveAudioHandler;
    std::vector<int16_t> getSamples(const int sampleCount);
    void loadNewMML(const std::vector<std::string>& mml);
    void clearMML();
    void registerOneShot(const std::vector<std::string>& mml);
    void clearQueue();
private:
    std::queue<int16_t> oneShotQueue;
};