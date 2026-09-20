#include "gameStateInGame.hpp"

#include "gameBoard.hpp"
#include "gameSprites.hpp"
#include "hoops.hpp"
#include "inGameState_DropControlPiece.hpp"
#include "bgmAssets.hpp"

void GAME_STATE_FUNCTIONS_INGAME::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    if (gameVariables.isPaused) {
        inGameVariables.doPausedState(systemVariables, gameVariables);
    } else {
        this->inGameVariables.doState(systemVariables, gameVariables);
    }
}

std::string getNumberText(const int& value) {
    std::stringstream ss;
    ss << std::setw(5) << std::setfill(' ') << std::min(value, 99999);
    return ss.str();
}

void GAME_STATE_FUNCTIONS_INGAME::renderBackground(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& renderer = systemVariables.essentials.screen.renderer;
    auto& palette = gameVariables.palette;
    auto& bitmapFont = gameVariables.bitmapFont;
    for (auto y = 0; y < this->inGameVariables.BOARD_HEIGHT; y++) {
        auto offset = VECTOR2i(1, y);
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::WALL), (offset) * 8, palette);
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::WALL), (offset + VECTOR2i(11,0)) * 8, palette);
    }
    struct BACKGROUND_TEXTS {
        VECTOR2i position;
        std::string header;
        std::string number;
    };
    BACKGROUND_TEXTS backgroundTexts[] = {
        {.position = VECTOR2i(14,2), .header = "SCORE", .number = getNumberText(inGameVariables.score)},
        {.position = VECTOR2i(14,5), .header = "CHAIN", .number = getNumberText(inGameVariables.chainCount)},
        {.position = VECTOR2i(14,8), .header = "BLOCK", .number = getNumberText(inGameVariables.blocks)},
    };
    for (auto [position, header, number] : backgroundTexts) {
        bitmapFont.drawText(position * 8, header, palette[1]);
        bitmapFont.drawText((position + VECTOR2i(0,1)) * 8, number, palette[1]);
    }
    bitmapFont.drawText(VECTOR2i(15, 13) * 8, "NEXT", palette[1], {.a = 0});
    this->inGameVariables.nextPiece.render(renderer, VECTOR2i(15, 14) * 8, palette);
}

void GAME_STATE_FUNCTIONS_INGAME::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto offset = VECTOR2i(2, 0) * 8;
    auto& renderer = systemVariables.essentials.screen.renderer;
    auto& renderTime = this->inGameVariables.renderTime;
    renderBackground(systemVariables, gameVariables);
    this->inGameVariables.gameBoard.render(renderer, gameVariables.palette, offset);
    this->inGameVariables.doRender(systemVariables, gameVariables);
    if (gameVariables.isPaused) {
        inGameVariables.doPausedRender(systemVariables, gameVariables);
    } else {
        renderTime++;
    }
}

void GAME_STATE_FUNCTIONS_INGAME::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    inGameVariables.initialize();
    systemVariables.audioHandler->loadNewMML(getMML(gameVariables.gameBGM));
    inGameVariables.setState(std::make_unique<INGAME_STATE_DropControlPiece>());
}
