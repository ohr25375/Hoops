#include "AudioHandler.hpp"

#include <iostream>

std::vector<int16_t> AUDIO_HANDLER::getSamples(const int sampleCount) {
    SDL_LockAudioDevice(audioID);
    auto samples = this->liveAudioHandler.getSamples(sampleCount);
    for (auto i = 0; i < sampleCount; i++) {
        if (this->queueReset) break;
        if (this->oneShotQueue.empty()) break;
        samples[i] += this->oneShotQueue.front();
        this->oneShotQueue.pop();
    }
    if (this->queueReset) {
        samples = std::vector<int16_t>(sampleCount);
        this->queueReset = false;
    }
    SDL_UnlockAudioDevice(audioID);
    return samples;
}

void AUDIO_HANDLER::loadNewMML(const std::vector<std::string>& mml) {
    SDL_LockAudioDevice(audioID);
    this->liveAudioHandler.loadNewMML(mml);
    this->queueReset = true;
    this->liveAudioHandler.queueClear = true;
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::clearMML() {
    SDL_LockAudioDevice(audioID);
    this->liveAudioHandler.clearMML();
    this->queueReset = true;
    this->liveAudioHandler.queueClear = true;
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::registerOneShot(const std::vector<std::string>& mml) {
    registerOneShot(mml, sfxVolume);
}

void AUDIO_HANDLER::registerOneShot(const std::vector<std::string>& mml, const float& volume) {
    SDL_LockAudioDevice(audioID);
    MML_LiveAudio::LiveAudioHandler tempLah;
    tempLah.masterVolume = volume * GLOBAL_VOLUME;
    tempLah.loadNewMML(mml);
    auto sample = tempLah.readAll();
    for (auto s : sample) {
        this->oneShotQueue.push(s);
    }
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::clearQueue() {
    SDL_LockAudioDevice(audioID);
    this->queueReset = true;
    this->liveAudioHandler.queueClear = true;
    std::queue<int16_t> empty;
    this->oneShotQueue.swap(empty);
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::setAudioID(const SDL_AudioDeviceID id) {
    this->audioID = id;
}

void AUDIO_HANDLER::pauseAudioDevice(bool state) const {
    SDL_PauseAudioDevice(this->audioID, state ? 1 : 0);
}

void AUDIO_HANDLER::close() {
    this->pauseAudioDevice(true);
    SDL_CloseAudioDevice(this->audioID);
    SDL_Log("Audio closed successfully");
}

void AUDIO_HANDLER::setBGMVolume(const float& value) {
    bgmVolume = value;
    liveAudioHandler.masterVolume = bgmVolume * GLOBAL_VOLUME;
}

void AUDIO_HANDLER::setSFXVolume(const float& value) {
    sfxVolume = value;
}

float AUDIO_HANDLER::getBGMVolume() const {
    return bgmVolume;
}
float AUDIO_HANDLER::getSFXVolume() const {
    return sfxVolume;
}
