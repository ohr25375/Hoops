#include "inGameState_DropControlPiece.hpp"
#include "inGameState_GameOver.hpp"
#include "inGameState_Chain.hpp"
#include "bgmAssets.hpp"

const std::vector<int> GRAVITY = {
    60,50,45,40,35,
    30,27,25,20,15,
    10, 9, 8, 7, 6,
    6, 5, 5, 4, 3, 2
};

const std::vector<std::vector<VECTOR2i>> KICK_OFFSET_L_CLOCKWISE = {
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i( 0, -1),
        VECTOR2i(-1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(+1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i( 0, +1),
        VECTOR2i(+1, +1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(-1, +1),
    }
};
const std::vector<std::vector<VECTOR2i>> KICK_OFFSET_L_ANTICLOCKWISE = {
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(-1, 0),
        VECTOR2i(-1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i( 0, -1),
        VECTOR2i(+1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(+1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i( 0, +1),
        VECTOR2i(-1, +1),
    }
};

const std::vector<std::vector<VECTOR2i>> KICK_OFFSET_I_CLOCKWISE = {
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(+1, +1),
        VECTOR2i(+1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(-1, +1),
        VECTOR2i(-1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(+1, +1),
        VECTOR2i(+1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(-1, +1),
        VECTOR2i(-1, -1),
    }
};

const std::vector<std::vector<VECTOR2i>> KICK_OFFSET_I_ANTICLOCKWISE = {
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(-1, +1),
        VECTOR2i(-1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(+1, +1),
        VECTOR2i(+1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(-1, +1),
        VECTOR2i(-1, -1),
    },
    std::vector<VECTOR2i>{
        VECTOR2i( 0,  0),
        VECTOR2i(+1,  0),
        VECTOR2i(-1,  0),
        VECTOR2i(+1, +1),
        VECTOR2i(+1, -1),
    }
};

void playSpin(SYSTEM_VARIABLES& systemVariables) {
    systemVariables.audioHandler->registerOneShot({"T240 O5 l32 V6 P160 g b-> d"});
}

void playMove(SYSTEM_VARIABLES& systemVariables) {
    systemVariables.audioHandler->registerOneShot({"T180 O6 l32 V6 P160 f"});
}

void playHardDrop(SYSTEM_VARIABLES& systemVariables) {
    systemVariables.audioHandler->registerOneShot({"T180 O7 l64 P160 V11 b- V8 f < V5 b"});
}


void lowerPiece(int& renderTime, VECTOR2i& piecePosition) {
    piecePosition += VECTOR2i(0,1);
    renderTime = 0;
}

void calculateGhost(INGAME_VARIABLES& inGameVariables) {
    const auto& controlPiece = inGameVariables.controlPiece;
    auto& gameBoard = inGameVariables.gameBoard;
    auto& ghostPosition = inGameVariables.ghostPosition;
    ghostPosition = inGameVariables.piecePosition;
    if (gameBoard.isColliding(ghostPosition.x, ghostPosition.y, controlPiece)) return;
    while (!gameBoard.isColliding(ghostPosition.x, ghostPosition.y + 1, controlPiece)) {
        ghostPosition.y++;
    }
    inGameVariables.lockTime = 0;
}

bool rotatePieceLeft(INGAME_VARIABLES& inGameVariables) {
    auto& controlPiece = inGameVariables.controlPiece;
    auto& piecePosition = inGameVariables.piecePosition;
    auto& gameBoard = inGameVariables.gameBoard;

    auto newPiece = controlPiece;
    auto orientation = controlPiece.getOrientation();
    newPiece.rotateLeft();
    for (auto dir : controlPiece.getIsSymmetric() ? KICK_OFFSET_I_ANTICLOCKWISE[orientation] : KICK_OFFSET_L_ANTICLOCKWISE[orientation]) {
        auto position = piecePosition + dir;
        if (gameBoard.isColliding(position.x, position.y, newPiece)) continue;
        controlPiece = newPiece;
        piecePosition = position;
        return true;
    }
    return false;
}

bool rotatePieceRight(INGAME_VARIABLES& inGameVariables) {
    auto& controlPiece = inGameVariables.controlPiece;
    auto& piecePosition = inGameVariables.piecePosition;
    auto& gameBoard = inGameVariables.gameBoard;

    auto newPiece = controlPiece;
    auto orientation = controlPiece.getOrientation();
    newPiece.rotateRight();
    for (auto dir : controlPiece.getIsSymmetric() ? KICK_OFFSET_I_CLOCKWISE[orientation] : KICK_OFFSET_L_CLOCKWISE[orientation]) {
        auto position = piecePosition + dir;
        if (gameBoard.isColliding(position.x, position.y, newPiece)) continue;
        piecePosition = position;
        controlPiece = newPiece;
        return true;
    }
    return false;
}

bool rotatePieceDepth(INGAME_VARIABLES& inGameVariables) {
    auto& controlPiece = inGameVariables.controlPiece;
    auto& piecePosition = inGameVariables.piecePosition;
    auto& gameBoard = inGameVariables.gameBoard;

    auto newPiece = controlPiece;
    newPiece.rotateDepth();
    if (gameBoard.isColliding(piecePosition.x, piecePosition.y, newPiece)) return false;

    controlPiece = newPiece;
    return true;
}

bool movePieceHorizontal(const int offset, INGAME_VARIABLES& inGameVariables) {
    auto& controlPiece = inGameVariables.controlPiece;
    auto& piecePosition = inGameVariables.piecePosition;
    auto& gameBoard = inGameVariables.gameBoard;
    if (gameBoard.isColliding(piecePosition.x + offset, piecePosition.y, controlPiece)) return false;

    piecePosition += (VECTOR2i(offset, 0));
    return true;
}

int movePieceDown(INGAME_VARIABLES& inGameVariables) {
    const auto& controlPiece = inGameVariables.controlPiece;
    auto& gameBoard = inGameVariables.gameBoard;
    auto& piecePosition = inGameVariables.piecePosition;
    if (gameBoard.isColliding(piecePosition.x, piecePosition.y, controlPiece)) {
        inGameVariables.setState(std::make_unique<INGAME_STATE_GameOver>());
        return -1;
    }
    if (gameBoard.isColliding(piecePosition.x, piecePosition.y + 1, controlPiece)) {
        if (!gameBoard.addHoops(piecePosition.x, piecePosition.y, controlPiece)) {
            inGameVariables.setState(std::make_unique<INGAME_STATE_GameOver>());
            return -1;
        }

        auto& islands = inGameVariables.islands;
        islands = gameBoard.getIslands(3);
        inGameVariables.score += 1;
        if (!(islands.empty())) {
            inGameVariables.setState(std::make_unique<INGAME_STATE_Chain>());
            return 2;
        }
        inGameVariables.createNewPiece();
        calculateGhost(inGameVariables);
        return 1;
    }

    auto& renderTime = inGameVariables.renderTime;
    lowerPiece(renderTime, piecePosition);
    return 0;
}

void hardDropPiece(INGAME_VARIABLES& inGameVariables, SYSTEM_VARIABLES& systemVariables) {
    if (!(inGameVariables.piecePosition.equality(inGameVariables.ghostPosition))) {
        inGameVariables.piecePosition = inGameVariables.ghostPosition;
        inGameVariables.renderTime = 0;
        playHardDrop(systemVariables);
    } else {
        movePieceDown(inGameVariables);
        systemVariables.playPlace();
    }
}

void INGAME_STATE_DropControlPiece::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    auto& mouse = systemVariables.essentials.controls.mouse;
    const auto& renderTime = inGameVariables.renderTime;

    if (keys[SDLK_ESCAPE].down) {
        inGameVariables.pauseGame(systemVariables, gameVariables);
    }

    if (mouse.left.down || keys[SDLK_c].down) {
        if (rotatePieceRight(inGameVariables)) {
            playSpin(systemVariables);
            calculateGhost(inGameVariables);
            return;
        }
    }
    if (mouse.right.down || keys[SDLK_x].down) {
        if(rotatePieceDepth(inGameVariables)) {
            playSpin(systemVariables);
            calculateGhost(inGameVariables);
            return;
        }
    }
    if (keys[SDLK_z].down) {
        if(rotatePieceLeft(inGameVariables)) {
            playSpin(systemVariables);
            calculateGhost(inGameVariables);
            return;
        }
    }

    if (keys[SDLK_LEFT].down) {
        if(movePieceHorizontal(-1, inGameVariables)) {
            playMove(systemVariables);
            calculateGhost(inGameVariables);
            return;
        }
    }

    if (keys[SDLK_RIGHT].down) {
        if(movePieceHorizontal(1, inGameVariables)) {
            playMove(systemVariables);
            calculateGhost(inGameVariables);
            return;
        }
    }

    if (keys[SDLK_UP].down) {
        hardDropPiece(inGameVariables, systemVariables);
    }

    if (renderTime >= inGameVariables.delayTime) { 
        auto moveState = movePieceDown(inGameVariables);
        if (moveState == 1) {
            systemVariables.playPlace();
        }
        return;
    }
    if (keys[SDLK_DOWN].down) {
        int moveState = movePieceDown(inGameVariables);
        if (moveState == 0) {
            playMove(systemVariables);
        }
        if (moveState == 1) {
            systemVariables.playPlace();
        }
    }
    if ((this->holdTime - 30) > 2 && keys[SDLK_DOWN].isHold) {
        int moveState = movePieceDown(inGameVariables);
        if (moveState == 0) {
            playMove(systemVariables);
        }
        if (moveState == 1) {
            systemVariables.playPlace();
        }
        this->holdTime = 30;
        return;
    }
    if (inGameVariables.lockTime > 30) {
        movePieceDown(inGameVariables);
        systemVariables.playPlace();
        return;
    }
}

void INGAME_STATE_DropControlPiece::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    auto& renderTime = inGameVariables.renderTime;
    const auto offset = VECTOR2i(2, 0) * 8;
    auto& renderer = systemVariables.essentials.screen.renderer;
    auto& keys = systemVariables.essentials.controls.keys;

    if (keys[SDLK_DOWN].isHold) {
        this->holdTime++;
    } else {
        this->holdTime = 0;
    }
    if (inGameVariables.piecePosition.equality(inGameVariables.ghostPosition)) {
        inGameVariables.lockTime++;
        renderTime &= 0b1;
    }

    inGameVariables.controlPiece.render(renderer, (inGameVariables.ghostPosition * 8) + offset, gameVariables.palette, true);
    if (renderTime & 0b1) {
        inGameVariables.controlPiece.render(renderer, (inGameVariables.piecePosition * 8) + offset, gameVariables.palette);
    }
}

void INGAME_STATE_DropControlPiece::doInit(INGAME_VARIABLES& inGameVariables) {
    inGameVariables.createNewPiece();
    calculateGhost(inGameVariables);
    inGameVariables.delayTime = GRAVITY[std::min(inGameVariables.chainCount / 10, 20)];
    inGameVariables.chains = 0;
}
