#include "systemInGameInit.hpp"

#include <filesystem>
#include "gameState_TitleMenu.hpp"

void initGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.toggleWindowSize(gameVariables.isLargeWindow);

    VECTOR2i textureSize       = systemVariables.essentials.screen.size;
    gameVariables.renderTarget = SDL_CreateTexture(systemVariables.essentials.screen.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureSize.x, textureSize.y);
    SDL_SetTextureScaleMode(gameVariables.renderTarget, SDL_ScaleModeNearest);

    gameVariables.screenArea = SDL2Addon::SDL2A_Rect(systemVariables.essentials.screen.size);

    gameVariables.bitmapFont = FONT::BitmapFONT();
    gameVariables.bitmapFont.setFont("8bitFont.bmp", VECTOR2i(8), FONT::INCLUDES::Codepage437);
    gameVariables.bitmapFont.init(systemVariables.essentials.screen.renderer);

    gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());

    SDL_Log("systemInGameInit: done");
}