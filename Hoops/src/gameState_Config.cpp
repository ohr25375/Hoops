#include "gameState_Config.hpp"

void GAME_STATE_FUNCTIONS_CONFIG::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    switch (selectedSubMenuItem) {
        case SUBMENU_ID::NONE: {
            this->doSubMenuNone(systemVariables, gameVariables);
            break;
        }
        case SUBMENU_ID::COLOR: {
            this->doSubMenuColor(systemVariables, gameVariables);
            break;
        }
        default: break;
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    renderTime++;
    for (auto i = 0; i < (int)CONFIG_ID::MAX; i++) {
        auto foreground = gameVariables.palette[1];
        auto background = SDL_Color{.a = 0};
        if (i == (int)selectedSubMenuItem) {
            foreground = gameVariables.palette[0];
            background = gameVariables.palette[1];
        }
        gameVariables.bitmapFont.drawText(MENU_POSITIONS[i] * 8, MENU_TEXTS[i], foreground, background);
    }
    switch ((CONFIG_ID)selectedMenuItem) {
        case CONFIG_ID::COLOR: {
            doSubMenuColorRender(systemVariables, gameVariables);
            break;
        }
        default: break;
    }
    gameVariables.bitmapFont.drawText((MENU_POSITIONS[(int)selectedMenuItem] - VECTOR2i(1, 0)) * 8, std::string(1, 0x10), gameVariables.palette[1]);
}

void GAME_STATE_FUNCTIONS_CONFIG::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
    selectedMenuItem = (int)CONFIG_ID::COLOR;
    selectedSubMenuItem = SUBMENU_ID::NONE;
}