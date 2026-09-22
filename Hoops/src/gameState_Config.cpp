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

void drawCenterLine(FONT::BitmapFONT& bitmapFont, const SDL_Color& color) {
    for (auto i = 0; i < 18; i++) {
        bitmapFont.drawText(VECTOR2i(8, i) * 8, std::string(1, 0xba), color);
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    const MENU_ITEM MENU_ITEMS[(int)CONFIG_ID::MAX] = {
        MENU_ITEM(VECTOR2i(2,  2), "SOUND"),
        MENU_ITEM(VECTOR2i(2,  4), "COLOR"),
        MENU_ITEM(VECTOR2i(2,  6), "SCREEN"),
        MENU_ITEM(VECTOR2i(2,  15), "BACK"),
    };
    renderTime++;
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette = gameVariables.palette;
    drawCenterLine(bitmapFont, palette[1]);
    for (auto i = 0; i < (int)CONFIG_ID::MAX; i++) {
        auto foreground = palette[1];
        auto background = palette[0];
        if (i == (int)selectedSubMenuItem) {
            foreground = palette[0];
            background = palette[1];
        }
        bitmapFont.drawText(MENU_ITEMS[i].position * 8, MENU_ITEMS[i].text, foreground, background);
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
    bitmapFont.drawText((MENU_ITEMS[(int)selectedMenuItem].position - VECTOR2i(1, 0)) * 8, std::string(1, 0x10), palette[1]);
}

void GAME_STATE_FUNCTIONS_CONFIG::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
    selectedMenuItem = (int)CONFIG_ID::SOUND;
    selectedSubMenuItem = SUBMENU_ID::NONE;
}