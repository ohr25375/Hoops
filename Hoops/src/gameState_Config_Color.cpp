#include "gameState_Config.hpp"

#include "colorPresets.hpp"

int paletteIndex = 0;

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
void drawColorPreset(FONT::BitmapFONT& bitmapFont, const VECTOR2i& position, const SDL_Color& textColor) {
    auto paletteName = getColorPresetName(paletteIndex);
    auto spaces = std::string(7 - paletteName.size(), ' ');
    std::stringstream text;
    text << (char)0x11 << paletteName << spaces << (char)0x10;
    bitmapFont.drawText((position + VECTOR2i(-1, 1)) * 8, text.str(), textColor);
}

void doChangeColor(bool isLeft, std::array<SDL2Addon::SDL_COLOR, 2>& palette, const int& selectedItem) {
    auto& color = palette[-(selectedItem / 3) + 1];
    auto rgb    = std::array<uint8_t*, 3>{ &color.r, &color.g, &color.b };
    auto index  = selectedItem % 3;
    auto value  = convertPaletteValueToInternal(*rgb[index]);
    value       = isLeft ? decrementBound(value, 16) : incrementBound(value, 16);
    *rgb[index] = (value << 4) | value;
}

void doChangePalette(bool isLeft, std::array<SDL2Addon::SDL_COLOR, 2>& palette) {
    auto bound = getPresetSize();
    paletteIndex = isLeft ? decrementBound(paletteIndex, bound) : incrementBound(paletteIndex, bound);
    palette = getColorPreset(paletteIndex);
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuColor(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_UP].down) {
        selectedSubMenuSubItem = decrementBound(selectedSubMenuSubItem, 7);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedSubMenuSubItem = incrementBound(selectedSubMenuSubItem, 7);
        systemVariables.playBlip();
    }
    if (isSubMenuItemSelected) {
        if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down || keys[SDLK_z].down || keys[SDLK_x].down) {
            isSubMenuItemSelected = false;
            systemVariables.playSelect();
        }
        if (keys[SDLK_LEFT].down || keys[SDLK_RIGHT].down) {
            if (selectedSubMenuSubItem != 6) {
                doChangeColor(keys[SDLK_LEFT].down, gameVariables.palette, selectedSubMenuSubItem);
            } else {
                doChangePalette(keys[SDLK_LEFT].down, gameVariables.palette);
            }
            systemVariables.playBlip();
        }
    } else {
        if (keys[SDLK_RETURN].down || keys[SDLK_RIGHT].down || keys[SDLK_z].down) {
            if (keys[SDLK_RIGHT].down && selectedSubMenuSubItem == 6) return;
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

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuColorRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette    = gameVariables.palette;
    VECTOR2i positions[] = {
        VECTOR2i(11, 2),
        VECTOR2i(11, 6),
        VECTOR2i(11, 11),
    };
    bitmapFont.drawText(positions[0] * 8, "A", palette[1]);
    drawConfigColor(bitmapFont, positions[0], palette[1], palette[1]);
    bitmapFont.drawText(positions[1] * 8, "B", palette[1]);
    drawConfigColor(bitmapFont, positions[1], palette[1], palette[0]);
    bitmapFont.drawText(positions[2] * 8, "PRESETS", palette[1]);
    drawColorPreset(bitmapFont, positions[2], palette[1]);

    if (selectedSubMenuItem != SUBMENU_ID::COLOR) return;
    if (renderTime & 0b1000) {
        auto positionIndex = selectedSubMenuSubItem / 3;
        auto menuHeightOffset = selectedSubMenuSubItem % 3;
        auto menuOffset = positions[positionIndex];
        std::string overlay = std::string(1, 0xdb);
        if (selectedSubMenuSubItem == 6) {
            overlay = std::string(7, 0xdb);
            if (isSubMenuItemSelected) {
                menuOffset += VECTOR2i(-1, 1);
                overlay = std::string(9, 0xdb);
            }
        } else {
            menuOffset += VECTOR2i(2, menuHeightOffset);
            if (isSubMenuItemSelected) {
                overlay = std::string(4, 0xdb);
                menuOffset += VECTOR2i(1, 0);
            }
        }
        bitmapFont.drawText(menuOffset * 8, overlay, palette[0]);
    }
}
