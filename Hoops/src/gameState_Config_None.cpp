#include "gameState_Config.hpp"
#include "gameState_TitleMenu.hpp"

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuNone(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    auto itemMax = (int)CONFIG_ID::MAX;

    if (keys[SDLK_UP].down) {
        selectedMenuItem = decrementBound(selectedMenuItem, itemMax);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedMenuItem = incrementBound(selectedMenuItem, itemMax);
        systemVariables.playBlip();
    }
    if (keys[SDLK_RETURN].down || keys[SDLK_RIGHT].down || keys[SDLK_z].down) {
        switch ((CONFIG_ID)selectedMenuItem) {
            case CONFIG_ID::BACK: {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
                systemVariables.playSelect();
                break;
            }
            case CONFIG_ID::MAX: break;
            default: {
                selectedSubMenuItem = (SUBMENU_ID)selectedMenuItem;
                systemVariables.playSelect();
                break;
            }
        }
    }
    if (keys[SDLK_ESCAPE].down || keys[SDLK_x].down) {
        if ((CONFIG_ID)selectedMenuItem != CONFIG_ID::BACK) {
            selectedMenuItem = (int)CONFIG_ID::BACK;
            systemVariables.playBlip();
            return;
        } else {
            gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
            systemVariables.playSelect();
        }
    }
}