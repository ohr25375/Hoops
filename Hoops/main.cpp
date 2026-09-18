#include "main.hpp"

#include "src/systemPreInit.hpp"
#include "src/systemInGame.hpp"

#include "src/SDL2_Font.hpp"
#include "src/AudioHandler.hpp"
#include <SDL_mixer.h>

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
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_ERROR,"Error at preInit");
        return -1;
    }
    SDL_Log("PreInit Successful");
    if (!systemInit(sysVar)) {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_ERROR,"Error at Init");
        return -1;
    }
    SDL_Log("Init Successful");
    if (!doGame(sysVar)) {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_ERROR,"Error at doGame");
        return -1;
    }
    SDL_Log("Game closed successfully");
    sysVar.audioHandler->close();
    SDL_Quit();
    return 0;
}