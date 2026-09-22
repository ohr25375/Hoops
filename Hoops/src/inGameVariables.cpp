#include "inGameVariables.hpp"

const std::vector<HOOPS> DEFAULT_HOOPS = {
    // L
    HOOPS(std::array<uint16_t,3>{
        0b010000000,
        0b000010000,
        0b000001000 
    }),
    // L (reverse order)
    HOOPS(std::array<uint16_t,3>{
        0b000001000,
        0b000010000,
        0b010000000 
    }),
    // -
    HOOPS(std::array<uint16_t,3>{
        0b000100000,
        0b000010000,
        0b000001000 
    }, true),
};

void INGAME_VARIABLES::initialize() {
    clearBoard();
    delayTime = 60;
    chainCount = 0;
    score = 0;
    blocks = 0;
    createNewPiece();
    createNewPiece();
}

void INGAME_VARIABLES::clearBoard() {
    gameBoard.clear();
}

void INGAME_VARIABLES::createNewPiece() {
    static int next = 0;
    this->piecePosition = VECTOR2i(3,-1);
    this->renderTime = 0;

    // 112 = 223 = 331 = 133 = 211 = 322
    // 122 = 233 = 311 = 113 = 221 = 332
    // 131 = 212 = 323
    // 121 = 232 = 313
    
    // 111 = 222 = 333
    // 123 = 231 = 312 = 132 = 213 = 321
    
    this->controlPiece = this->nextPiece;
    next = (next + 1) % DEFAULT_HOOPS.size();
    this->nextPiece = DEFAULT_HOOPS[next];
    this->nextPiece.setRandomRotaionDepth();
}

void INGAME_VARIABLES::addIslandScores() {
    for (auto island : islands) {
        // score of block(1) = 10 * chains
        // score of block(n + 1) = score of block(n) + 5
        const int blockCount = island.size();
        int countBonus = ((blockCount - 1) * blockCount) / 2;
        int addScore = (10 * chains) * blockCount + (countBonus * 5);
        score += addScore;
        blocks += blockCount;
        chainCount++;
    }
}

void INGAME_VARIABLES::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    this->inGameStates->doState(systemVariables, gameVariables, *this);
}

void INGAME_VARIABLES::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    this->inGameStates->doRender(systemVariables, gameVariables, *this);
}

void INGAME_VARIABLES::setState(std::unique_ptr<INGAME_STATES> newState) {
    this->inGameStates.reset();
    this->inGameStates = std::move(newState);
    renderTime = 0;
    this->inGameStates->doInit(*this);
}

std::vector<int> INGAME_VARIABLES::getDropColumns() {
    std::vector<int> droppingColumns(BOARD_WIDTH, -1);
    for (auto island : islands) {
        for (auto cell : island) {
            const auto x = cell % BOARD_WIDTH;
            const auto y = cell / BOARD_WIDTH;
            gameBoard.setCell(x, y, 0);
            if (droppingColumns[x] > y) continue;
            droppingColumns[x] = y;
        }
    }
    return droppingColumns;
}
