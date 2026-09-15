#include "gameEssentials.hpp"

void GAME_VARIABLES::doState(SYSTEM_VARIABLES& systemVariables) {
    this->gameStateFunctions->doState(systemVariables, *this);
}

void GAME_VARIABLES::doRender(SYSTEM_VARIABLES& systemVariables) {
    this->gameStateFunctions->doRender(systemVariables, *this);
}

void GAME_VARIABLES::setState(SYSTEM_VARIABLES& systemVariables, std::unique_ptr<GAME_STATE_FUNCTIONS> newState) {
    this->gameStateFunctions.reset();
    this->gameStateFunctions = std::move(newState);
    this->gameStateFunctions->doInit(systemVariables, *this);
}
