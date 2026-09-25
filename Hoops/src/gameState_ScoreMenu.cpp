#include "gameState_ScoreMenu.hpp"

#include "gameState_TitleMenu.hpp"
#include "gameSprites.hpp"

const std::vector<char> ALLOWED_CHARACTERS = {
    ' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '0','1','2','3','4','5','6','7','8','9',
    '.','-','\'',
    '!','?',(char)11,(char)12,(char)13,'^',':',(char)1,(char)2,(char)3
};

char getNextChar(const char c) {
    switch (c) {
        case ' ': return 'A';
        case 'Z': return '0';
        case '9': return '.';
        case '.': return '-';
        case '-': return '\'';
        case '\'': return '!';
        case '!': return '?';
        case '?': return 11;
        case 13: return '^';
        case '^': return ':';
        case ':': return 1;
        case 3: return ' ';
        default: return c + 1;
    }
}

char getPreviousChar(const char c) {
    switch (c) {
        case 'A': return ' ';
        case '0': return 'Z';
        case '.': return '9';
        case '-': return '.';
        case '\'': return '-';
        case '!': return '\'';
        case '?': return '!';
        case 11: return '?';
        case '^': return 13;
        case ':': return '^';
        case 1: return ':';
        case ' ': return 3;
        default: return c - 1;
    }
}

void GAME_STATE_FUNCTIONS_SCORE::doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;

    if (!isScoreUpdated) {
        if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down || keys[SDLK_z].down || keys[SDLK_x].down) {
            gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
            systemVariables.playSelect();
            return;
        }
    } else {
        auto& scores = gameVariables.save.scores;
        char* name = nullptr;
        switch (scoreRankNumber) {
            case 0: name = scores.score1Name; break;
            case 1: name = scores.score2Name; break;
            case 2: name = scores.score3Name; break;
        }
        if (keys[SDLK_UP].down) {
            name[selectedNameCharacter] = getPreviousChar(name[selectedNameCharacter]);
            systemVariables.playBlip();
        }
        if (keys[SDLK_DOWN].down) {
            name[selectedNameCharacter] = getNextChar(name[selectedNameCharacter]);
            systemVariables.playBlip();
        }
        if (keys[SDLK_LEFT].down) {
            if (selectedNameCharacter == 0) return;
            if (!isNameConfirmed) {
                selectedNameCharacter--;
            }
            systemVariables.playBlip();
            isNameConfirmed = false;
        }
        if (keys[SDLK_RIGHT].down) {
            if (selectedNameCharacter == 6 - 1) {
                if (isNameConfirmed) return;
                isNameConfirmed = true;
                systemVariables.playSelect();
                return;
            }
            char* names[] = {scores.score1Name, scores.score2Name, scores.score3Name};
            const char c = names[scoreRankNumber][selectedNameCharacter];
            selectedNameCharacter++;
            names[scoreRankNumber][selectedNameCharacter] = c;
            systemVariables.playBlip();
        }
        if (keys[SDLK_z].down || keys[SDLK_RETURN].down) {
            if (!isNameConfirmed) {
                isNameConfirmed = true;
            } else {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
            }
            systemVariables.playSelect();
            return;
        }
        if (keys[SDLK_x].down) {
            if (isNameConfirmed) {
                isNameConfirmed = false;
                systemVariables.playBlip();
            }
        }
        if (keys[SDLK_ESCAPE].down) {
            if (isNameConfirmed) {
                gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());
                systemVariables.playSelect();
                return;
            }
        }
    }
}

void drawDivider(FONT::BitmapFONT& bitmapFont, const std::array<SDL2Addon::SDL_COLOR, 2>& palette) {
    bitmapFont.drawText(VECTOR2i(0,9) * 8, std::string(20, 0xdf), palette[1]);
    bitmapFont.drawText(VECTOR2i(0,10) * 8, std::string(20, 0xb2), palette[1]);
    bitmapFont.drawText(VECTOR2i(0,11) * 8, std::string(20, 0xdc), palette[1]);
}

void drawWinnersPost(FONT::BitmapFONT& bitmapFont, const std::array<SDL2Addon::SDL_COLOR, 2>& palette) {
    bitmapFont.drawText(VECTOR2i(0x0c, 8) * 8, std::string(3, 0xdb), palette[1]);
    bitmapFont.drawText(VECTOR2i(0x0d, 7) * 8, std::string({(char)0xdb, (char)0xdc}), palette[1]);
}

void drawMen(SDL_Renderer* renderer, const std::array<SDL2Addon::SDL_COLOR, 2>& palette) {
    VECTOR2f positions[] = {VECTOR2i(0x0d, 6), VECTOR2f(0x0e, 6.5), VECTOR2i(0x0c, 7)};
    for (auto position : positions) {
        drawGameSprite(renderer, getGameSprite(GAME_SPRITES::MAN), position * 8, palette);
    }
}

void drawJumpingMen(SDL_Renderer* renderer, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const int scoreRankNumber, const int renderTime) {
    VECTOR2f positions[] = {VECTOR2i(0x0d, 6), VECTOR2f(0x0e, 6.5), VECTOR2i(0x0c, 7)};
    drawGameSprite(renderer, getGameSprite(GAME_SPRITES::BLANK), positions[scoreRankNumber] * 8, palette);
    auto sprite = renderTime & 0b100000 ? getGameSprite(GAME_SPRITES::MAN) : getGameSprite(GAME_SPRITES::MAN_HAPPY);
    drawGameSprite(renderer, sprite, positions[scoreRankNumber] * 8, palette);
}

std::string getScoreText(const std::string& header, const int score, const char* name) {
    std::stringstream text;
    text << header << " " << std::string(name, name + 6) << "  " << std::setw(6) << score;
    return text.str();
}

void drawScores(FONT::BitmapFONT& bitmapFont, const GAME_VARIABLES& gameVariables) {
    auto& palette = gameVariables.palette;
    const auto& scores = gameVariables.save.scores;
    bitmapFont.drawText(VECTOR2i(2, 0xd) * 8, getScoreText("1", scores.score1, scores.score1Name), palette[1]);
    bitmapFont.drawText(VECTOR2i(2, 0xe) * 8, getScoreText("2", scores.score2, scores.score2Name), palette[1]);
    bitmapFont.drawText(VECTOR2i(2, 0xf) * 8, getScoreText("3", scores.score3, scores.score3Name), palette[1]);
}

void GAME_STATE_FUNCTIONS_SCORE::doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    renderTime++;
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette = gameVariables.palette;
    auto& renderer = systemVariables.essentials.screen.renderer;
    drawDivider(bitmapFont, palette);
    drawWinnersPost(bitmapFont, palette);
    drawMen(renderer, palette);
    drawJumpingMen(renderer, palette, scoreRankNumber, renderTime);
    drawScores(bitmapFont, gameVariables);
    if (isScoreUpdated) {
        if (selectedNameCharacter < 6 && !isNameConfirmed) {
            const auto& scores = gameVariables.save.scores;
            auto position = VECTOR2i(selectedNameCharacter, scoreRankNumber) + VECTOR2i(0x4, 0xd);
            const char* names[] = {scores.score1Name, scores.score2Name, scores.score3Name};
            auto foreground = palette[((renderTime & 0b1000) ? 1 : 0)];
            auto background = palette[((renderTime & 0b1000) ? 0 : 1)];
            bitmapFont.drawText(position * 8, std::string(1, names[scoreRankNumber][selectedNameCharacter]), foreground, background);
        }
    }
    bitmapFont.drawText(VECTOR2i(0), "HIGH SCORES", palette[0], palette[1]);
}

void GAME_STATE_FUNCTIONS_SCORE::doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    systemVariables.audioHandler->clearMML();
    renderTime = 0;
    gameVariables.isPaused = false;
}

GAME_STATE_FUNCTIONS_SCORE::GAME_STATE_FUNCTIONS_SCORE(const bool isScoreUpdated, const int scoreRankNumber)
    : isScoreUpdated(isScoreUpdated), scoreRankNumber(scoreRankNumber) {}
