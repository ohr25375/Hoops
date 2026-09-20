#include "gameState_TitleMenu.hpp"

#include "gameState_Config.hpp"
#include "gameStateInGame.hpp"
#include "gameSprites.hpp"

void GAME_STATE_FUNCTIONS_TITLE::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    auto item = (int)selectedMenuItem;
    auto itemMax = (int)MENU_TITLE::MAX;

    if (keys[SDLK_UP].down) {
        selectedMenuItem = (MENU_TITLE)decrementBound(item, itemMax);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedMenuItem = (MENU_TITLE)incrementBound(item, itemMax);
        systemVariables.playBlip();
    }
    if (keys[SDLK_RETURN].down || keys[SDLK_z].down) {
        switch (selectedMenuItem) {
            case MENU_TITLE::START: {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_INGAME>());
                systemVariables.playSelect();
                break;
            }
            case MENU_TITLE::CONFIG: {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_CONFIG>());
                systemVariables.playSelect();
                break;
            }
            case MENU_TITLE::QUIT: gameVariables.isRunning = false; break;
            default: break;
        }
    }
    if (keys[SDLK_ESCAPE].down || keys[SDLK_x].down) {
        if (selectedMenuItem != MENU_TITLE::QUIT) {
            selectedMenuItem = MENU_TITLE::QUIT;
            systemVariables.playBlip();
            return;
        } else {
            gameVariables.isRunning = false;
            systemVariables.playSelect();
        }
    }
}

const std::vector<std::string> TITLE_GRAPH = {
    std::string({(char)0xe5, (char)0xdf, (char)0xe1}),
    std::string({(char)0xdd, ' ', ' ', (char)0xdd}),
    std::string({(char)0xe8, ' ', (char)0xe5, (char)0xe0, (char)0xe6, (char)0xdd}),
    std::string({(char)0xdd, (char)0xdf, (char)0xe1, ' ', (char)0xdf, (char)0xe0, (char)0xe4, (char)0xe4, ' ', (char)0xdc, (char)0xe4, (char)0xe5, (char)0xdf, (char)0xe0}),
    std::string({(char)0xdd, ' ', ' ', (char)0xdd, (char)0xde, ' ', (char)0xe6, (char)0xde, (char)0xde, ' ', (char)0xde, ' ', (char)0xdf, (char)0xe4}),
    std::string({(char)0xe0, ' ', ' ', (char)0xe0, (char)0xe2, ' ', (char)0xe0, (char)0xe2, (char)0xe2, (char)0xdc, (char)0xe9, (char)0xe2, (char)0xdf}),
    std::string({' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', (char)0xde}),
    std::string({' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', (char)0xe2, (char)0xdc, (char)0xe3}),
};

void drawTitleLogo(FONT::BitmapFONT& bitmapFont, const SDL_Color& textColor, const VECTOR2f& offset) {
    for (auto i = 0; i < (int)TITLE_GRAPH.size(); i++) {
        bitmapFont.drawText((offset + VECTOR2i(0,i)) * 8, TITLE_GRAPH[i], textColor);
    }
}

void drawTitleScreenTextElements(FONT::BitmapFONT& bitmapFont, const SDL_Color& textColor) {
    auto line = std::string(5, (char)0xdb) + std::string(10,(char)0xdc) + std::string(5, (char)0xdb);
    bitmapFont.drawText(VECTOR2i(0,10) * 8, line, textColor);
    for (auto i = 0; i < 7; i++){
        bitmapFont.drawText(VECTOR2i(9, 11 + i) * 8, std::string(1, (char)0xde), textColor);
    }
}

void drawTitleScreenSpriteElements(SDL_Renderer*& renderer, const std::array<SDL2Addon::SDL_COLOR, 2>& palette) {
    for (auto i = 0; i < 10; i++) {
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::WALL), VECTOR2i(0, i) * 8, palette);
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::WALL), VECTOR2i(19, i) * 8, palette);
    }
    for (auto i = 1; i < 5; i++) {
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::HOOP_100), VECTOR2i(i, 9) * 8, palette);
    }
    for (auto i = 1; i < 4; i++) {
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::HOOP_100), VECTOR2i(19 - i, 9) * 8, palette);
    }
}

void GAME_STATE_FUNCTIONS_TITLE::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    renderTime++;
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette = gameVariables.palette;
    const std::vector<VECTOR2i> MENU_POSITIONS = {
        VECTOR2i(2, 12),
        VECTOR2i(2, 14),
        VECTOR2i(2, 16)
    };
    const std::vector<std::string> HEADERS = {
        "START",
        "CONFIG",
        "QUIT"
    };
    auto& renderer = systemVariables.essentials.screen.renderer;
    drawTitleLogo(bitmapFont, palette[1], VECTOR2f(3, 1.75 + (std::cos(renderTime * 0.03f)) * 0.25f));
    drawTitleScreenTextElements(bitmapFont, palette[1]);
    drawTitleScreenSpriteElements(renderer, palette);

    for (int i = 0; i < (int)MENU_TITLE::MAX; i++) {
        bitmapFont.drawText(MENU_POSITIONS[i] * 8, HEADERS[i], palette[1]);
    }
    bitmapFont.drawText((MENU_POSITIONS[(int)selectedMenuItem] - VECTOR2i(1, 0)) * 8, std::string(1, 0x10), palette[1]);
}

void GAME_STATE_FUNCTIONS_TITLE::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
    renderTime = 0;
    gameVariables.isPaused = false;
}
