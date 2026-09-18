#include "gameState_Config.hpp"

void drawConfigScreenSize(FONT::BitmapFONT& bitmapFont, const VECTOR2i& position, const int& screenSize, const SDL_Color& textColor) {
    std::stringstream ss;
    ss << (char)0x11 << std::to_string(screenSize) << (char)0x10;
    bitmapFont.drawText((position + VECTOR2i(0,1)) * 8, ss.str(), textColor);
}

void doChangeScreenSize(const bool isLeft, int& screenSize) {
    screenSize = (isLeft ? decrementBound(screenSize, 11, 1) : incrementBound(screenSize, 11, 1));
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuScreen(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_UP].down) {
        selectedSubMenuSubItem = decrementBound(selectedSubMenuSubItem, 1);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedSubMenuSubItem = incrementBound(selectedSubMenuSubItem, 1);
        systemVariables.playBlip();
    }
    if (isSubMenuItemSelected) {
        if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down || keys[SDLK_z].down || keys[SDLK_x].down) {
            isSubMenuItemSelected = false;
            systemVariables.playSelect();
        }
        if (keys[SDLK_LEFT].down || keys[SDLK_RIGHT].down) {
            doChangeScreenSize(keys[SDLK_LEFT].down, systemVariables.screenSizeMultiplier);
            systemVariables.toggleWindowSize(true);
            systemVariables.playBlip();
        }
    } else {
        if (keys[SDLK_RETURN].down || keys[SDLK_RIGHT].down || keys[SDLK_z].down) {
            isSubMenuItemSelected = true;
            systemVariables.playSelect();
        }
        if (keys[SDLK_ESCAPE].down || keys[SDLK_LEFT].down || keys[SDLK_x].down) {
            selectedSubMenuItem    = SUBMENU_ID::NONE;
            selectedSubMenuSubItem = 0;
            systemVariables.playSelect();
        }
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuScreenRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette    = gameVariables.palette;
    VECTOR2i positions[] = {
        VECTOR2i(11, 2),
    };
    bitmapFont.drawText(positions[0] * 8, "MAGNIFY", palette[1]);
    drawConfigScreenSize(bitmapFont, positions[1], systemVariables.screenSizeMultiplier, palette[1]);
    if (selectedSubMenuItem != SUBMENU_ID::SCREEN) return;
    if (renderTime & 0b1000) {
        auto overlay = std::string(7, 0xdb);
        auto position = positions[selectedSubMenuSubItem];
        if (isSubMenuItemSelected) {
            auto overlay = std::string(3, 0xdb);
            position += VECTOR2i(0,1);
        }
        bitmapFont.drawText(position * 8, overlay, palette[0]);
    }
}
