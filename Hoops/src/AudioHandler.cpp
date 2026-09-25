#include "AudioHandler.hpp"

#include <iostream>

AUDIO_HANDLER::AUDIO_HANDLER() {
    setBGMVolume(bgmVolume);
}

std::vector<int16_t> AUDIO_HANDLER::getSamples(const int sampleCount) {
    SDL_LockAudioDevice(audioID);
    auto samples = std::vector<int16_t>(sampleCount);
    if (!isAudioPaused) {
        samples = liveAudioHandler.getSamples(sampleCount);
    }
    for (auto i = 0; i < sampleCount; i++) {
        if (queueReset) break;
        if (oneShotQueue.empty()) break;
        samples[i] += oneShotQueue.front();
        oneShotQueue.pop();
    }
    if (queueReset) {
        samples = std::vector<int16_t>(sampleCount);
        queueReset = false;
    }
    SDL_UnlockAudioDevice(audioID);
    return samples;
}

void AUDIO_HANDLER::loadNewMML(const std::vector<std::string>& mml) {
    SDL_LockAudioDevice(audioID);
    liveAudioHandler.loadNewMML(mml);
    queueReset = true;
    liveAudioHandler.queueClear = true;
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::clearMML() {
    SDL_LockAudioDevice(audioID);
    liveAudioHandler.clearMML();
    queueReset = true;
    liveAudioHandler.queueClear = true;
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::registerOneShot(const std::vector<std::string>& mml, const float volume) {
    SDL_LockAudioDevice(audioID);
    MML_LiveAudio::LiveAudioHandler tempLah;
    tempLah.masterVolume = volume * GLOBAL_VOLUME;
    tempLah.loadNewMML(mml);
    auto sample = tempLah.readAll();
    for (auto s : sample) {
        oneShotQueue.push(s);
    }
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::clearQueue() {
    SDL_LockAudioDevice(audioID);
    queueReset = true;
    liveAudioHandler.queueClear = true;
    std::queue<int16_t> empty;
    oneShotQueue.swap(empty);
    SDL_UnlockAudioDevice(audioID);
}

void AUDIO_HANDLER::pauseAudioDevice(bool state) const {
    SDL_PauseAudioDevice(audioID, state ? 1 : 0);
}

void AUDIO_HANDLER::close() {
    pauseAudioDevice(true);
    SDL_CloseAudioDevice(audioID);
    SDL_Log("Audio closed successfully");
}

void AUDIO_HANDLER::setBGMVolume(const float value) {
    bgmVolume = value;
    liveAudioHandler.masterVolume = bgmVolume * GLOBAL_VOLUME;
}

void AUDIO_HANDLER::setSFXVolume(const float value) {
    sfxVolume = value;
}

float AUDIO_HANDLER::getBGMVolume() const {
    return bgmVolume;
}
float AUDIO_HANDLER::getSFXVolume() const {
    return sfxVolume;
}

void AUDIO_HANDLER::pauseAudio() {
    isAudioPaused = true;
}

void AUDIO_HANDLER::unpauseAudio() {
    isAudioPaused = false;
}
