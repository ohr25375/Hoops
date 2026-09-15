#include "systemPreInit.hpp"

#include "src/SDL2_Font.hpp"

bool doPreInit(SYSTEM_VARIABLES& systemVariables) {
    systemVariables.essentials.screen.font = FONT::createBitmapFont(VECTOR2i(8), "8bitFont.bmp", FONT::INCLUDES::Codepage437);
    systemVariables.essentials.screen.setInitValues(VECTOR2i(160, 144));
    return true;
}