#pragma once

#include "src/MML_LiveAudio.hpp"
#include <queue>
#include <SDL.h>

class AUDIO_HANDLER {
public:
    MML_LiveAudio::LiveAudioHandler liveAudioHandler;

    AUDIO_HANDLER();

    std::vector<int16_t> getSamples(const int sampleCount);
    void loadNewMML(const std::vector<std::string>& mml);
    void clearMML();
    void registerOneShot(const std::vector<std::string>& mml);
    void registerOneShot(const std::vector<std::string>& mml, const float volume);
    void clearQueue();
    void setAudioID(const SDL_AudioDeviceID id);
    void pauseAudioDevice(bool state) const;
    void close();

    void setBGMVolume(const float value);
    void setSFXVolume(const float value);
    float getBGMVolume() const;
    float getSFXVolume() const;

    void pauseAudio();
    void unpauseAudio();
    
private:
    bool isAudioPaused = false;
    float bgmVolume = 0.5f;
    float sfxVolume = 0.5f;
    const float GLOBAL_VOLUME = 0.5f;
    SDL_AudioDeviceID audioID;
    bool queueReset = false;
    std::queue<int16_t> oneShotQueue;
};

inline void AUDIO_HANDLER::registerOneShot(const std::vector<std::string>& mml) {
    registerOneShot(mml, sfxVolume);
}

inline void AUDIO_HANDLER::setAudioID(const SDL_AudioDeviceID id) {
    audioID = id;
}