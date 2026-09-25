#pragma once

#include "gameEssentials.hpp"

class GAME_STATE_FUNCTIONS_SCORE : public GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    GAME_STATE_FUNCTIONS_SCORE(const bool isScoreUpdated, const int scoreRankNumber = -1);
private:
    int renderTime = 0;
    const bool isScoreUpdated;
    const int scoreRankNumber;
    int selectedNameCharacter = 0;
    bool isNameConfirmed = false;
};
