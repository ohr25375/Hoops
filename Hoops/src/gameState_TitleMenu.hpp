#pragma once

#include "gameEssentials.hpp"

enum struct MENU_TITLE {
    START,
    CONFIG,
    QUIT,
    MAX
};

class GAME_STATE_FUNCTIONS_TITLE : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
private:
    MENU_TITLE selectedMenuItem = MENU_TITLE::START;
    int renderTime = 0;
};
