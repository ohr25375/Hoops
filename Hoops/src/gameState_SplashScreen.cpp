#include "gameState_SplashScreen.hpp"
#include "gameSprites.hpp"
#include "gameState_TitleMenu.hpp"

void GAME_STATE_FUNCTIONS_SPLASHSCREEN::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {

}

const char FADE_CHARACTERS[] = {
    (char)0x20,
    (char)0xb0,
    (char)0xb1,
    (char)0xb2,
    (char)0xdb,
};

void screenOverlay(std::unique_ptr<FONT::FONT>& bitmapFont, const int fade, const SDL_Color& color) {
    for (auto i = 0; i < 18; i++) {
        bitmapFont->drawText(VECTOR2i(0,i) * 8, std::string(20, FADE_CHARACTERS[fade]), color);
    }
}

void incrementState(int& renderTime, SPLASHSCREEN_STATE& state) {
    renderTime = 0;
    state = (SPLASHSCREEN_STATE)((int)state + 1);
}

void doFadeIn(SPLASHSCREEN_STATE& state, int& renderTime, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto backgroundColor = gameVariables.palette[0];
    auto fade = (int)state - (int)SPLASHSCREEN_STATE::FADE_IN_4;
    screenOverlay(bitmapFont, 4 - fade, backgroundColor);
    if (renderTime < 10) return;
    incrementState(renderTime, state);
}

void doFadeOut(SPLASHSCREEN_STATE& state, int& renderTime, GAME_VARIABLES& gameVariables, SYSTEM_VARIABLES& systemVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto backgroundColor = gameVariables.palette[0];
    auto fade = (int)state - (int)SPLASHSCREEN_STATE::FADE_OUT_0;
    screenOverlay(bitmapFont, fade, backgroundColor);
    if (renderTime < 10) return;
    incrementState(renderTime, state);
}

void GAME_STATE_FUNCTIONS_SPLASHSCREEN::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    renderTime++;
    auto& bitmapFont = gameVariables.bitmapFont;
    bitmapFont->drawText(VECTOR2i(6,9) * 8, "GAME BY", gameVariables.palette[1]);
    bitmapFont->drawText(VECTOR2i(8,10) * 8, "SO-", gameVariables.palette[1]);

    switch (state) {
        case SPLASHSCREEN_STATE::FADE_IN_4:
        case SPLASHSCREEN_STATE::FADE_IN_3:
        case SPLASHSCREEN_STATE::FADE_IN_2:
        case SPLASHSCREEN_STATE::FADE_IN_1:
        case SPLASHSCREEN_STATE::FADE_IN_0: {
            doFadeIn(state, renderTime, gameVariables);
            break;
        }
        case SPLASHSCREEN_STATE::HOLD_SPLASH: {
            if (renderTime < 120) break;
            incrementState(renderTime, state);
            break;
        }
        case SPLASHSCREEN_STATE::FADE_OUT_0:
        case SPLASHSCREEN_STATE::FADE_OUT_1:
        case SPLASHSCREEN_STATE::FADE_OUT_2:
        case SPLASHSCREEN_STATE::FADE_OUT_3:
        case SPLASHSCREEN_STATE::FADE_OUT_4: {
            doFadeOut(state, renderTime, gameVariables, systemVariables);
            break;
        }
        case SPLASHSCREEN_STATE::MOVE_TO_NEXT_SCENE: {
            screenOverlay(bitmapFont, 4, gameVariables.palette[0]);
            if (renderTime < 30) break;
            setStateToTitle(systemVariables, gameVariables, true);
            break;
        }
    }
}

void GAME_STATE_FUNCTIONS_SPLASHSCREEN::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
    renderTime = 0;
    gameVariables.isPaused = false;
}
