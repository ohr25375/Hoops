#include "inGameState_Chain.hpp"

#include "gameSprites.hpp"
#include "inGameState_DropControlPiece.hpp"
#include "inGameState_FallingPieces.hpp"

FALLING_RINGS getFallingRings(GAME_BOARD& gameBoard, const std::vector<int>& droppingColumns) {
    FALLING_RINGS fallingRings;
    fallingRings.fallingColumns.resize(droppingColumns.size());
    for (int x = 0; x < (int)droppingColumns.size(); x++) {
        if (droppingColumns[x] == -1) continue;
        const auto y = droppingColumns[x];
        fallingRings.fallingColumns[x] = gameBoard.getDropHeights(x,y);
    }
    return fallingRings;
}

void INGAME_STATE_Chain::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    auto& renderTime = inGameVariables.renderTime;
    auto& keys = systemVariables.essentials.controls.keys;

    if (keys[SDLK_ESCAPE].down) {
        inGameVariables.pauseGame(systemVariables, gameVariables);
        return;
    }

    if (renderTime > 0b100000) {
        auto& gameBoard = inGameVariables.gameBoard;
        const std::vector<int> droppingColumns = inGameVariables.getDropColumns();
        inGameVariables.addIslandScores();
        inGameVariables.fallingRings = getFallingRings(gameBoard, droppingColumns);
        inGameVariables.setState(std::make_unique<INGAME_STATE_FallingPieces>());
    }
}

void drawBlankIslands(SDL_Renderer*& renderer, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, INGAME_VARIABLES& inGameVariables) {
    const auto offset = VECTOR2i(2, 0) * 8;
    const auto& islands = inGameVariables.islands;
    const auto pitch = inGameVariables.BOARD_WIDTH;
    for (auto island : islands) {
        for (auto cell : island) {
            auto position = VECTOR2i(cell % pitch, cell / pitch) * 8;
            drawGameSprite(renderer, getGameSprite(GAME_SPRITES::BLANK), offset + position, palette);
        }
    }
}

void INGAME_STATE_Chain::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    const auto& renderTime = inGameVariables.renderTime;
    if (renderTime & 0b100) {
        drawBlankIslands(systemVariables.essentials.screen.renderer, gameVariables.palette, inGameVariables);
    }
    if (renderTime == 1) {
        systemVariables.audioHandler->registerOneShot({"T180 O6 V8 l32 [b- > f<]5"});
    }
}

void INGAME_STATE_Chain::doInit(INGAME_VARIABLES& inGameVariables) {
    inGameVariables.chains++;
}
