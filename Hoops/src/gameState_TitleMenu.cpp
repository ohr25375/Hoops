#include "gameState_TitleMenu.hpp"

#include "gameStateInGame.hpp"

void GAME_STATE_FUNCTIONS_TITLE::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;

    if (keys[SDLK_UP].down) {
        this->selectedMenuItem = (this->selectedMenuItem + this->MENU_POSITIONS.size() - 1) % this->MENU_POSITIONS.size();
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        this->selectedMenuItem = (this->selectedMenuItem + 1) % this->MENU_POSITIONS.size();
        systemVariables.playBlip();
    }
    if (keys[SDLK_RETURN].down) {
        switch (this->selectedMenuItem) {
            case 0: {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_INGAME>());
                systemVariables.playSelect();
                break;
            }
            case 1: gameVariables.isRunning = false; break;
        }
    }
}

void GAME_STATE_FUNCTIONS_TITLE::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    gameVariables.bitmapFont.drawText(VECTOR2i(5,13) * 8, "START", gameVariables.palette[1], {.a = 0});
    gameVariables.bitmapFont.drawText(VECTOR2i(5,15) * 8, "QUIT", gameVariables.palette[1], {.a = 0});
    gameVariables.bitmapFont.drawText(this->MENU_POSITIONS[this->selectedMenuItem] * 8, std::string(1, 0x10), gameVariables.palette[1], {.a = 0});
}

void GAME_STATE_FUNCTIONS_TITLE::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
}
