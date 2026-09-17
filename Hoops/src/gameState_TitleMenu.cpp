#include "gameState_TitleMenu.hpp"

#include "gameState_Config.hpp"
#include "gameStateInGame.hpp"

void GAME_STATE_FUNCTIONS_TITLE::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    auto item = (int)this->selectedMenuItem;
    auto itemMax = (int)MENU_TITLE::MENU_TITLE_MAX;

    if (keys[SDLK_UP].down) {
        this->selectedMenuItem = (MENU_TITLE)decrementBound(item, itemMax);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        this->selectedMenuItem = (MENU_TITLE)incrementBound(item, itemMax);
        systemVariables.playBlip();
    }
    if (keys[SDLK_RETURN].down) {
        switch (this->selectedMenuItem) {
            case MENU_TITLE::MENU_TITLE_START: {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_INGAME>());
                systemVariables.playSelect();
                break;
            }
            case MENU_TITLE::MENU_TITLE_CONFIG: {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_CONFIG>());
                systemVariables.playSelect();
                break;
            }
            case MENU_TITLE::MENU_TITLE_QUIT: gameVariables.isRunning = false; break;
            default: break;
        }
    }
    if (keys[SDLK_ESCAPE].down) {
        gameVariables.isRunning = false;
    }
}

void GAME_STATE_FUNCTIONS_TITLE::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    gameVariables.bitmapFont.drawText(MENU_POSITIONS[(int)MENU_TITLE::MENU_TITLE_START] * 8, "START", gameVariables.palette[1]);
    gameVariables.bitmapFont.drawText(MENU_POSITIONS[(int)MENU_TITLE::MENU_TITLE_CONFIG] * 8, "CONFIG", gameVariables.palette[1]);
    gameVariables.bitmapFont.drawText(MENU_POSITIONS[(int)MENU_TITLE::MENU_TITLE_QUIT] * 8, "QUIT", gameVariables.palette[1]);
    gameVariables.bitmapFont.drawText((this->MENU_POSITIONS[(int)this->selectedMenuItem] - VECTOR2i(1, 0)) * 8, std::string(1, 0x10), gameVariables.palette[1]);
}

void GAME_STATE_FUNCTIONS_TITLE::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
}
