#include "inGameState_FallingPieces.hpp"

#include "gameSprites.hpp"
#include "inGameState_Chain.hpp"
#include "inGameState_DropControlPiece.hpp"

void INGAME_STATE_FallingPieces::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
}

void dropColumns(GAME_BOARD& gameBoard, const std::vector<int>& droppingColumns) {
    for (int x = 0; x < (int)droppingColumns.size(); x++) {
        if (droppingColumns[x] == -1) continue;
        const auto y = droppingColumns[x];
        gameBoard.dropColumn(x,y);
    }
}

void drawMask(SDL_Renderer*& renderer, const VECTOR2i& offset, const std::array<SDL2Addon::SDL_COLOR,2>& palette, std::vector<VECTOR2i>& mask) {
    for (auto position : mask) {
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::BLANK), offset + position * 8, palette, false);
    }
}

void drawFallingPieces(SDL_Renderer*& renderer, const VECTOR2i& offset, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const FALLING_RINGS& fallingRings) {
    for (auto x = 0; x < (int)fallingRings.fallingColumns.size(); x++) {
        for (const auto& ring : fallingRings.fallingColumns[x].fallingRings) {
            for (auto i = 0; i < 3; i++) {
                if ((ring.data & (1 << i)) == 0) continue;
                drawGameSprite(renderer, getGameSprite(GAME_SPRITES((int)GAME_SPRITES::HOOP_001 + i)), offset + (VECTOR2f(x, (int)ring.currentHeight) * 8), palette, false);
            }
        }
    }
}

void INGAME_STATE_FallingPieces::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables, INGAME_VARIABLES& inGameVariables) {
    inGameVariables.renderTime++;
    auto& fallingRings = inGameVariables.fallingRings;
    bool hasUpdatedValues = false;
    bool queueSFX = false;
    for (auto& column : fallingRings.fallingColumns) {
        for (auto& ring : column.fallingRings) {
            if (ring.currentHeight >= ring.targetHeight) continue;
            hasUpdatedValues = true;
            ring.currentHeight = std::min(ring.currentHeight + this->velocity, ring.targetHeight);
            if (ring.currentHeight == ring.targetHeight) {
                queueSFX = true;
            }
        }
    }
    if (queueSFX) {
        systemVariables.playPlace();
    }
    this->velocity = std::min(this->velocity + 0.02, 1.0);
    
    auto& renderer = systemVariables.essentials.screen.renderer;
    const auto offset = VECTOR2i(2, 0) * 8;
    drawMask(renderer, offset, gameVariables.palette, mask);
    if (inGameVariables.renderTime & 0b10) {
        drawFallingPieces(renderer, offset, gameVariables.palette, fallingRings);
    }
    if (hasUpdatedValues) return;

    SDL_Log("Finished dropping pieces");

    auto& islands = inGameVariables.islands;
    auto& gameBoard = inGameVariables.gameBoard;
    dropColumns(gameBoard, inGameVariables.getDropColumns());

    islands = gameBoard.getIslands(3);
    if (islands.empty()) {
        inGameVariables.setState(std::make_unique<INGAME_STATE_DropControlPiece>());
        return;
    }
    inGameVariables.setState(std::make_unique<INGAME_STATE_Chain>());
}

void INGAME_STATE_FallingPieces::doInit(INGAME_VARIABLES& ingameVariables) {
    this->velocity = 0;
    const auto& columns = ingameVariables.fallingRings.fallingColumns;
    for (auto x = 0; x < (int)columns.size(); x++) {
        const auto& column = columns[x];
        for (const auto& rings : column.fallingRings) {
            mask.push_back(VECTOR2i(x, rings.currentHeight));
        }
    }
}
