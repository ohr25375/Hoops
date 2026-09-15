#pragma once

#include "gameEssentials.hpp"

class GAME_STATE_FUNCTIONS_TITLE : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
private:
    int selectedMenuItem = 0;
    const std::vector<VECTOR2i> MENU_POSITIONS = {
    VECTOR2i(4, 13),
    VECTOR2i(4, 15)
};
};
