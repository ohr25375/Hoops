#include "gameState_Config.hpp"

int convertPaletteValueToInternal(const uint8_t& value) {
    return (value >> 4) & 0xf;
}

std::string getText(const char initial, const uint8_t& value) {
    std::stringstream text;
    text << initial << (char)0x11 << std::setw(2) << std::setfill('0') << std::to_string(convertPaletteValueToInternal(value)) << (char)0x10;
    return text.str();
}

void drawConfigColor(FONT::BitmapFONT& bitmapFont, const VECTOR2i& position, const SDL_Color& textColor, const SDL_Color& color) {
    char initials[] = { 'R', 'G', 'B' };
    uint8_t rgb[]   = { color.r, color.g, color.b };
    for (int i = 0; i < 3; i++) {
        bitmapFont.drawText((position + VECTOR2i(2, i)) * 8, getText(initials[i], rgb[i]), textColor);
    }
}

void doChangeColor(bool isLeft, std::array<SDL2Addon::SDL_COLOR, 2>& palette, const int& selectedItem) {
    auto& color = palette[-(selectedItem / 3) + 1];
    auto rgb    = std::array<uint8_t*, 3>{ &color.r, &color.g, &color.b };
    auto index  = selectedItem % 3;
    auto value  = convertPaletteValueToInternal(*rgb[index]);
    value       = isLeft ? decrementBound(value, 16) : incrementBound(value, 16);
    *rgb[index] = (value << 4) | value;
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuColor(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_UP].down) {
        selectedSubMenuSubItem = decrementBound(selectedSubMenuSubItem, 6);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedSubMenuSubItem = incrementBound(selectedSubMenuSubItem, 6);
        systemVariables.playBlip();
    }
    if (isSubMenuItemSelected) {
        if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down) {
            isSubMenuItemSelected = false;
            systemVariables.playSelect();
        }
        if (keys[SDLK_LEFT].down || keys[SDLK_RIGHT].down) {
            doChangeColor(keys[SDLK_LEFT].down, gameVariables.palette, selectedSubMenuSubItem);
            systemVariables.playBlip();
        }
    } else {
        if (keys[SDLK_RETURN].down || keys[SDLK_RIGHT].down) {
            isSubMenuItemSelected = true;
            systemVariables.playSelect();
        }
        if (keys[SDLK_ESCAPE].down || keys[SDLK_LEFT].down) {
            selectedSubMenuItem    = SUBMENU_ID::NONE;
            selectedSubMenuSubItem = 0;
            systemVariables.playSelect();
        }
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuColorRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette    = gameVariables.palette;
    VECTOR2i positions[] = {
        VECTOR2i(11, 2),
        VECTOR2i(11, 6),
    };
    bitmapFont.drawText(positions[0] * 8, "A", palette[1]);
    drawConfigColor(bitmapFont, positions[0], palette[1], palette[1]);
    bitmapFont.drawText(positions[1] * 8, "B", palette[1]);
    drawConfigColor(bitmapFont, positions[1], palette[1], palette[0]);

    if (selectedSubMenuItem != SUBMENU_ID::COLOR) return;
    if (renderTime & 0b1000) {
        if (isSubMenuItemSelected) {
            auto overlay = std::string(4, 0xdb);
            if (selectedSubMenuSubItem < 3) {
                bitmapFont.drawText((positions[0] + VECTOR2i(3, selectedSubMenuSubItem)) * 8, overlay, palette[0]);
            } else {
                bitmapFont.drawText((positions[1] + VECTOR2i(3, selectedSubMenuSubItem - 3)) * 8, overlay, palette[0]);
            }
        } else {
            auto overlay = std::string(1, 0xdb);
            if (selectedSubMenuSubItem < 3) {
                bitmapFont.drawText((positions[0] + VECTOR2i(2, selectedSubMenuSubItem)) * 8, overlay, palette[0]);
            } else {
                bitmapFont.drawText((positions[1] + VECTOR2i(2, selectedSubMenuSubItem - 3)) * 8, overlay, palette[0]);
            }
        }
    }
}
