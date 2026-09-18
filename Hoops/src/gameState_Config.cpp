#include "gameState_Config.hpp"

void GAME_STATE_FUNCTIONS_CONFIG::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    switch (selectedSubMenuItem) {
        case SUBMENU_ID::NONE: {
            this->doSubMenuNone(systemVariables, gameVariables);
            break;
        }
        case SUBMENU_ID::SOUND: {
            this->doSubMenuSound(systemVariables, gameVariables);
            break;
        }
        case SUBMENU_ID::COLOR: {
            this->doSubMenuColor(systemVariables, gameVariables);
            break;
        }
        case SUBMENU_ID::SCREEN: {
            this->doSubMenuScreen(systemVariables, gameVariables);
            break;
        }
        default: break;
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    renderTime++;
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette = gameVariables.palette;
    for (auto i = 0; i < 18; i++) {
        bitmapFont.drawText(VECTOR2i(8, i) * 8, std::string(1, 0xba), palette[1]);
    }
    for (auto i = 0; i < (int)CONFIG_ID::MAX; i++) {
        auto foreground = palette[1];
        auto background = SDL_Color{.a = 0};
        if (i == (int)selectedSubMenuItem) {
            foreground = palette[0];
            background = palette[1];
        }
        bitmapFont.drawText(MENU_POSITIONS[i] * 8, MENU_TEXTS[i], foreground, background);
    }
    switch ((CONFIG_ID)selectedMenuItem) {
        case CONFIG_ID::SOUND: {
            doSubMenuSoundRender(systemVariables, gameVariables);
            break;
        }
        case CONFIG_ID::COLOR: {
            doSubMenuColorRender(systemVariables, gameVariables);
            break;
        }
        case CONFIG_ID::SCREEN: {
            doSubMenuScreenRender(systemVariables, gameVariables);
            break;
        }
        default: break;
    }
    bitmapFont.drawText((MENU_POSITIONS[(int)selectedMenuItem] - VECTOR2i(1, 0)) * 8, std::string(1, 0x10), palette[1]);
}

void GAME_STATE_FUNCTIONS_CONFIG::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
    selectedMenuItem = (int)CONFIG_ID::SOUND;
    selectedSubMenuItem = SUBMENU_ID::NONE;
}