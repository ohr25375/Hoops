#pragma once

#include "inGameVariables.hpp"
#include "systemEssentials.hpp"
#include "gameEssentials.hpp"

class INGAME_STATE_DropControlPiece : public INGAME_STATES {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& ingameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& ingameVariables);
    virtual void doInit(INGAME_VARIABLES& ingameVariables);
private:
    int holdTime = 0;
};
