#pragma once

#include "gameEssentials.hpp"
#include "inGameVariables.hpp"

class GAME_STATE_FUNCTIONS_INGAME : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
private:
    INGAME_VARIABLES inGameVariables;
    void renderBackground(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
};