#pragma once

#include "gameBoard.hpp"
#include "hoops.hpp"
#include "systemEssentials.hpp"
#include "gameEssentials.hpp"

class INGAME_VARIABLES;

class INGAME_STATES {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& ingameVariables) = 0;
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& ingameVariables) = 0;
    virtual void doInit(INGAME_VARIABLES& ingameVariables) = 0;
    virtual ~INGAME_STATES() = default;
};

class INGAME_VARIABLES {
public:
    const int BOARD_WIDTH = 10;
    const int BOARD_HEIGHT = 18;
    int score = 0;
    int chainCount = 0;
    int blocks = 0;
    int chains = 0;
    GAME_BOARD gameBoard = GAME_BOARD(BOARD_WIDTH, BOARD_HEIGHT);
    std::vector<std::vector<int>> islands{};
    FALLING_RINGS fallingRings{};
    int renderTime = 0;
    int lockTime = 0;
    int delayTime = 60;
    VECTOR2i piecePosition;
    VECTOR2i ghostPosition;
    HOOPS controlPiece;
    HOOPS nextPiece;

    void initialize();

    void clearBoard();
    void createNewPiece();

    void addIslandScores();

    void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void setState(std::unique_ptr<INGAME_STATES> newState);

    void pauseGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doPausedState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);
    void doPausedRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables);

    std::vector<int> getDropColumns();
private:
    std::unique_ptr<INGAME_STATES> inGameStates;
    int selectedMenuItem = 0;
};