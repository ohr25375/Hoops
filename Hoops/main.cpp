#include "main.hpp"

#include "src/systemPreInit.hpp"
#include "src/systemInGame.hpp"

#include "src/SDL2_Font.hpp"
#include "src/AudioHandler.hpp"
#include <SDL2/SDL_mixer.h>

AUDIO_HANDLER audioHandler;

void callback(void* userdata, uint8_t* stream, int len) {
    short* snd  = reinterpret_cast<short*>(stream);
    len        /= sizeof(*snd);

    auto samp = audioHandler.getSamples(len);

    for (int i = 0; i < len; i++) {
        snd[i] = samp[i];
    }
}

int main(void) {
    SYSTEM_VARIABLES sysVar;
    sysVar.audioHandler = &audioHandler;
    sysVar.callback = callback;
    if (!doPreInit(sysVar)) {
        std::cout << "Error at doPreInit\n";
        return -1;
    }
    if (!systemInit(sysVar)) {
        std::cout << "Error at systemInit\n";
        return -1;
    }
    if (!doGame(sysVar)) {
        std::cout << "Error at doGame\n";
        return -1;
    }
    
    SDL_Log("closing audio device: %d", sysVar.audioID);
    SDL_CloseAudioDevice(sysVar.audioID);
    SDL_Quit();
}