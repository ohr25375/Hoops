#include "systemInit.hpp"

bool systemInit(SYSTEM_VARIABLES& systemVariables) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

    SDL_AudioSpec spec, aspec;
    SDL_zero(spec);
    spec.freq     = systemVariables.SAMPLE_RATE;
    spec.format   = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples  = 512;
    spec.callback = systemVariables.callback;
    spec.userdata = NULL;
    systemVariables.audioHandler->setAudioID(SDL_OpenAudioDevice(nullptr, 0, &spec, &aspec, SDL_AUDIO_ALLOW_ANY_CHANGE));
    systemVariables.audioHandler->pauseAudioDevice(false);

    systemVariables.essentials.init();

    auto& screen = systemVariables.essentials.screen;
    SDL_SetWindowBordered(screen.window, SDL_FALSE);
    SDL_SetRenderDrawBlendMode(screen.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_SetRelativeMouseMode(SDL_TRUE);

    return true;
}