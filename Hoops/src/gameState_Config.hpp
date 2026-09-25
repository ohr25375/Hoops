#pragma once

#include "gameEssentials.hpp"

class GAME_STATE_FUNCTIONS_CONFIG : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
private:
    enum struct CONFIG_ID {
        SOUND,
        COLOR,
        SCREEN,
        BACK,
        MAX,
    };
    enum struct SUBMENU_ID {
        NONE = -1,
        SOUND,
        COLOR,
        SCREEN,
        BACK,
        MAX,
    };

    int renderTime = 0;

    int selectedMenuItem;
    int selectedSubMenuSubItem;
    bool isSubMenuItemSelected = false;
    SUBMENU_ID selectedSubMenuItem;

    void doSubMenuNone(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuColor(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuColorRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuSound(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuSoundRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuScreen(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuScreenRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
};
