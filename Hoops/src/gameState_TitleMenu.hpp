#pragma once

#include "gameEssentials.hpp"

enum struct MENU_TITLE {
    MENU_TITLE_START,
    MENU_TITLE_CONFIG,
    MENU_TITLE_QUIT,
    MENU_TITLE_MAX
};

class GAME_STATE_FUNCTIONS_TITLE : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
private:
    MENU_TITLE selectedMenuItem = MENU_TITLE::MENU_TITLE_START;
    const std::vector<VECTOR2i> MENU_POSITIONS = {
        VECTOR2i(4, 11),
        VECTOR2i(4, 13),
        VECTOR2i(4, 15)
    };
};
