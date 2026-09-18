#include "gameStateInGame.hpp"

#include "gameBoard.hpp"
#include "gameSprites.hpp"
#include "hoops.hpp"
#include "inGameState_DropControlPiece.hpp"
#include "bgmAssets.hpp"

void GAME_STATE_FUNCTIONS_INGAME::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    this->inGameVariables.doState(systemVariables, gameVariables);
}

void GAME_STATE_FUNCTIONS_INGAME::renderBackground(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& renderer = systemVariables.essentials.screen.renderer;
    for (auto y = 0; y < this->inGameVariables.BOARD_HEIGHT; y++) {
        auto offset = VECTOR2i(1, y);
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::GAME_SPRITE_LEFT_WALL), (offset) * 8, gameVariables.palette, false);
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::GAME_SPRITE_LEFT_WALL), (offset + VECTOR2i(11,0)) * 8, gameVariables.palette, false);
    }
    std::stringstream scoreStream;
    scoreStream << std::setw(5) << std::setfill(' ') << std::min(this->inGameVariables.score, 99999);
    gameVariables.bitmapFont.drawText(VECTOR2i(14, 2) * 8, "SCORE", gameVariables.palette[1], {.a = 0});
    gameVariables.bitmapFont.drawText(VECTOR2i(14, 3) * 8, scoreStream.str(), gameVariables.palette[1], {.a = 0});
    std::stringstream chainStream;
    chainStream << std::setw(5) << std::setfill(' ') << std::min(this->inGameVariables.chainCount, 99999);
    gameVariables.bitmapFont.drawText(VECTOR2i(14, 5) * 8, "CHAIN", gameVariables.palette[1], {.a = 0});
    gameVariables.bitmapFont.drawText(VECTOR2i(14, 6) * 8, chainStream.str(), gameVariables.palette[1], {.a = 0});
    std::stringstream blockStream;
    blockStream << std::setw(5) << std::setfill(' ') << std::min(this->inGameVariables.blocks, 99999);
    gameVariables.bitmapFont.drawText(VECTOR2i(14, 8) * 8, "BLOCK", gameVariables.palette[1], {.a = 0});
    gameVariables.bitmapFont.drawText(VECTOR2i(14, 9) * 8, blockStream.str(), gameVariables.palette[1], {.a = 0});
    gameVariables.bitmapFont.drawText(VECTOR2i(15, 13) * 8, "NEXT", gameVariables.palette[1], {.a = 0});
    this->inGameVariables.nextPiece.render(renderer, VECTOR2i(15, 14) * 8, gameVariables.palette);
}

void GAME_STATE_FUNCTIONS_INGAME::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto offset = VECTOR2i(2, 0) * 8;
    auto& renderer = systemVariables.essentials.screen.renderer;
    auto& renderTime = this->inGameVariables.renderTime;
    renderBackground(systemVariables, gameVariables);
    this->inGameVariables.gameBoard.render(renderer, gameVariables.palette, offset);

    renderTime += 1;

    this->inGameVariables.doRender(systemVariables, gameVariables);
}

void GAME_STATE_FUNCTIONS_INGAME::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    this->inGameVariables.clearBoard();
    this->inGameVariables.delayTime = 60;
    this->inGameVariables.chainCount = 0;
    this->inGameVariables.score = 0;
    this->inGameVariables.blocks = 0;
    this->inGameVariables.createNewPiece();
    this->inGameVariables.createNewPiece();
    systemVariables.audioHandler->loadNewMML(getMML(gameVariables.gameBGM));
    inGameVariables.setState(std::make_unique<INGAME_STATE_DropControlPiece>());
}
