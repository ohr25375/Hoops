#include "inGameState_GameOver.hpp"

#include "gameState_ScoreMenu.hpp"

void INGAME_STATE_GameOver::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down || keys[SDLK_z].down || keys[SDLK_x].down) {
        auto& scores = gameVariables.save.scores;
        const uint32_t currentScores[] = {scores.score1, scores.score2, scores.score3};
        int scoreRank = -1;
        bool isUpdateScore = false;
        for (auto i = 0; i < 3; i++) {
            if (inGameVariables.score >= currentScores[i]) {
                scoreRank = i;
                isUpdateScore = true;
                break;
            }
        }
        if (isUpdateScore) {
            uint32_t* updateScores[] = {&scores.score1, &scores.score2, &scores.score3};
            char* updateNames[] = {scores.score1Name, scores.score2Name, scores.score3Name};
            for (auto i = 2; i > scoreRank; i--) {
                *updateScores[i] = *updateScores[i - 1];
                for (auto j = 0; j < 6; j++) {
                    updateNames[i][j] = updateNames[i - 1][j];
                }
            }
            *updateScores[scoreRank] = inGameVariables.score;
            for (auto i = 0; i < 6; i++) {
                updateNames[scoreRank][i] = ' ';
            }
        }
        gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_SCORE>(isUpdateScore, scoreRank));
    }
}

void INGAME_STATE_GameOver::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    gameVariables.bitmapFont.drawText(VECTOR2i(0), "GAME OVER", gameVariables.palette[0], gameVariables.palette[1]);
    if (inGameVariables.renderTime == 1) {
        systemVariables.audioHandler->clearQueue();
        systemVariables.audioHandler->registerOneShot({"T180 O2 v15 l32cr l64[bfr32]7", "T180 !1 O2 v15 l32br l64[b-er32]7", "T180 !2 O2 v15 l32 b-r l64 [ae-r32]7"});
        systemVariables.audioHandler->clearMML();
    }
}

void INGAME_STATE_GameOver::doInit(INGAME_VARIABLES& inGameVariables) {
}
