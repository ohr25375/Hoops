#include "inGameState_GameOver.hpp"

#include "gameState_TitleMenu.hpp"

void INGAME_STATE_GameOver::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down || keys[SDLK_z].down || keys[SDLK_x].down) {
        gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
    }
}

void INGAME_STATE_GameOver::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    gameVariables.bitmapFont.drawText(VECTOR2i(0), "GAME OVER", gameVariables.palette[0], gameVariables.palette[1]);
    if (inGameVariables.renderTime == 1) {
        systemVariables.audioHandler->clearQueue();
        systemVariables.audioHandler->registerOneShot({"T180 O2 v15 l32cr l64[bfr32]7", "T180 !1 O2 v15 l32br l64[b-er32]7", "T180 !2 O2 v15 l32 b-r l64 [ae-r32]7"});
        systemVariables.audioHandler->clearMML();
    }
}

void INGAME_STATE_GameOver::doInit(INGAME_VARIABLES& inGameVariables) {
}
