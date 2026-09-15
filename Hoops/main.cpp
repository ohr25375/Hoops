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
    try {
        SYSTEM_VARIABLES sysVar;
        sysVar.audioHandler = &audioHandler;
        sysVar.callback = callback;
        if (!doPreInit(sysVar)) {
            throw "Error at preInit";
        }
        SDL_Log("PreInit Successful");
        if (!systemInit(sysVar)) {
            throw "Error at init";
        }
        SDL_Log("Init Successful");
        if (!doGame(sysVar)) {
            throw "Error at doGame";
        }
        SDL_Log("Game closed successfully");
        sysVar.audioHandler->close();
        SDL_Quit();
    } catch (const char* msg) {
        std::cout << msg << '\n';
    }
    return 0;
}