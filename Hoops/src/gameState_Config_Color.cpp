#include "gameState_Config.hpp"

int convertPaletteValueToInternal(const uint8_t& value) {
    return (value >> 4) & 0xf;
}

std::string getText(const char initial, const uint8_t& value) {
    std::stringstream text;
    text << initial << (char)0x11 << std::setw(2) << std::setfill('0') << std::to_string(convertPaletteValueToInternal(value)) << (char)0x10 ;
    return text.str();
}

void drawConfigColor(
    FONT::BitmapFONT& bitmapFont, const VECTOR2<int>& position, const std::array<SDL_Color, 2>& colors,
    const SDL_Color& color
) {
    bitmapFont.drawText((position + VECTOR2i(2, 0)) * 8, getText('R', color.r), colors[1], colors[0]);
    bitmapFont.drawText((position + VECTOR2i(2, 1)) * 8, getText('G', color.g), colors[1], colors[0]);
    bitmapFont.drawText((position + VECTOR2i(2, 2)) * 8, getText('B', color.b), colors[1], colors[0]);
}

void doChangeColor(bool isLeft, std::array<SDL2Addon::SDL_COLOR, 2>& palette, int& selectedItem) {
    auto& color = palette[-(selectedItem / 3) + 1];
    auto rgb = std::array<uint8_t*, 3>{
        &color.r,
        &color.g,
        &color.b,
    };
    auto index = selectedItem % 3;
    auto value = convertPaletteValueToInternal(*rgb[index]);
    value = isLeft ? decrementBound(value, 16) : incrementBound(value, 16);
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
            selectedSubMenuItem = SUBMENU_ID::NONE;
            selectedSubMenuSubItem = 0;
            systemVariables.playSelect();
        }
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuColorRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    std::array<SDL_Color, 2> colors{
        {gameVariables.palette[0], gameVariables.palette[1]}
    };
    auto a_position = VECTOR2i(11, 2);
    auto b_position = VECTOR2i(11, 6);
    bitmapFont.drawText(a_position * 8, "A", colors[1], colors[0]);
    drawConfigColor(bitmapFont, a_position, colors,gameVariables.palette[1]);
    bitmapFont.drawText(b_position * 8, "B", colors[1], colors[0]);
    drawConfigColor(bitmapFont, b_position, colors, gameVariables.palette[0]);

    if (selectedSubMenuItem != SUBMENU_ID::COLOR) return;
    if (renderTime & 0b1000) {
        if (isSubMenuItemSelected) {
            auto overlay = std::string(4, 0xdb);
            if (selectedSubMenuSubItem < 3) {
                bitmapFont.drawText((a_position + VECTOR2i(3, selectedSubMenuSubItem)) * 8, overlay, colors[0]);
            } else {
                bitmapFont.drawText((b_position + VECTOR2i(3, selectedSubMenuSubItem - 3)) * 8, overlay, colors[0]);
            }
        } else {
            auto overlay = std::string(1, 0xdb);
            if (selectedSubMenuSubItem < 3) {
                bitmapFont.drawText((a_position + VECTOR2i(2, selectedSubMenuSubItem)) * 8, overlay, colors[0]);
            } else {
                bitmapFont.drawText((b_position + VECTOR2i(2, selectedSubMenuSubItem - 3)) * 8, overlay, colors[0]);
            }
        }
    }
}
