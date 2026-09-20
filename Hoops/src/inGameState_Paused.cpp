#include "inGameVariables.hpp"

#include "gameSprites.hpp"
#include "gameState_TitleMenu.hpp"

void INGAME_VARIABLES::pauseGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    gameVariables.isPaused = true;
    systemVariables.audioHandler->pauseAudio();
    systemVariables.playBlip();
}

void INGAME_VARIABLES::doPausedState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;

    if (keys[SDLK_UP].down) {
        selectedMenuItem = decrementBound(selectedMenuItem, 2);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedMenuItem = incrementBound(selectedMenuItem, 2);
        systemVariables.playBlip();
    }
    if (keys[SDLK_ESCAPE].down || keys[SDLK_x].down) {
        if (selectedMenuItem != 1) {
            selectedMenuItem = 1;
            systemVariables.playBlip();
        } else {
            gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
            systemVariables.audioHandler->unpauseAudio();
            systemVariables.playSelect();         
        }
    }
    if (keys[SDLK_RETURN].down || keys[SDLK_z].down) {
        if (selectedMenuItem == 0) {
            gameVariables.isPaused = false;
            systemVariables.audioHandler->unpauseAudio();
        } else {
            gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
            systemVariables.audioHandler->unpauseAudio();
        }
        systemVariables.playSelect();
    }
}



void INGAME_VARIABLES::doPausedRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& renderer = systemVariables.essentials.screen.renderer;
    auto& palette = gameVariables.palette;
    for (auto y = 0; y < BOARD_HEIGHT; y++) {
        for (auto x = 0; x < BOARD_WIDTH; x++) {
            auto position = VECTOR2i(x,y) + VECTOR2i(2, 0);
            drawGameSprite(renderer, getGameSprite(GAME_SPRITES::BLANK), position * 8, palette);
        }
    }
    bitmapFont.drawText(VECTOR2i(4,5) * 8, "PAUSED", palette[1], palette[0]);
    bitmapFont.drawText(VECTOR2i(4,7) * 8, "RESUME", palette[1], palette[0]);
    bitmapFont.drawText(VECTOR2i(4,9) * 8, "TITLE", palette[1], palette[0]);
    bitmapFont.drawText(VECTOR2i(3,7 + selectedMenuItem * 2) * 8, std::string(1, 0x10), palette[1]);
}
