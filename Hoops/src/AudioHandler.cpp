#include "AudioHandler.hpp"

#include <iostream>

std::vector<int16_t> AUDIO_HANDLER::getSamples(const int sampleCount) {
    auto samples = this->liveAudioHandler.getSamples(sampleCount);
    for (auto i = 0; i < sampleCount; i++) {
        if (this->oneShotQueue.empty()) break;
        samples[i] += this->oneShotQueue.front();
        this->oneShotQueue.pop();
    }
    return samples;
}

void AUDIO_HANDLER::loadNewMML(const std::vector<std::string>& mml) {
    this->liveAudioHandler.loadNewMML(mml);
}

void AUDIO_HANDLER::clearMML() {
    this->liveAudioHandler.clearMML();
}

void AUDIO_HANDLER::registerOneShot(const std::vector<std::string>& mml) {
    MML_LiveAudio::LiveAudioHandler tempLah;
    tempLah.loadNewMML(mml);
    auto sample = tempLah.readAll();
    for (auto s : sample) {
        this->oneShotQueue.push(s);
    }
}

void AUDIO_HANDLER::clearQueue() {
    std::queue<int16_t> empty;
    this->oneShotQueue.swap(empty);
}
