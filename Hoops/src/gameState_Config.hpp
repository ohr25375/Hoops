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
        BACK,
        MAX,
    };
    enum struct SUBMENU_ID {
        NONE = -1,
        SOUND,
        COLOR,
        BACK,
        MAX,
    };

    int renderTime = 0;

    int selectedMenuItem;
    int selectedSubMenuSubItem;
    bool isSubMenuItemSelected = false;
    SUBMENU_ID selectedSubMenuItem;
    const std::vector<VECTOR2i> MENU_POSITIONS = {
        VECTOR2i(2, 2),
        VECTOR2i(2, 4),
        VECTOR2i(2, 15)
    };
    const std::vector<std::string> MENU_TEXTS = {
        "SOUND",
        "COLOR",
        "BACK"
    };

    void doSubMenuNone(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuColor(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuColorRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuSound(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doSubMenuSoundRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
};
