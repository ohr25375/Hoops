#pragma once

#include "gameEssentials.hpp"

enum struct SPLASHSCREEN_STATE {
    FADE_IN_4,
    FADE_IN_3,
    FADE_IN_2,
    FADE_IN_1,
    FADE_IN_0,
    HOLD_SPLASH,
    FADE_OUT_0,
    FADE_OUT_1,
    FADE_OUT_2,
    FADE_OUT_3,
    FADE_OUT_4,
    MOVE_TO_NEXT_SCENE
};

class GAME_STATE_FUNCTIONS_SPLASHSCREEN : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
private:
    int renderTime = 0;
    SPLASHSCREEN_STATE state = SPLASHSCREEN_STATE::FADE_IN_4;
};
