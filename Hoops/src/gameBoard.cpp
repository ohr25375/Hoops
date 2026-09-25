#include "gameBoard.hpp"

#include "gameSprites.hpp"

const int HOOP_SIZE = 3;
#include <queue>

GAME_BOARD::GAME_BOARD(const int x, const int y) : board(std::vector<char>(x * y)), pitch(x), height(y) {}
GAME_BOARD::GAME_BOARD() : GAME_BOARD(0, 0) {}

void GAME_BOARD::dropColumn(const int x, const int y) {
    std::vector<char> states(height);
    for (auto hy = 0; hy < height; hy++) {
        states[hy] = getCell(x, hy);
    }
    for (auto i = height - 2; i >= 0; i--) {
        if (i > y) continue;
        auto& here = states[i];
        if (here == 0) continue;
        for (auto j = i + 1; j < height; j++) {
            if ((states[j] & here) == 0) {
                if (j < height - 1) continue;
                j++;
            }
            states[j - 1] |= here;
            here = 0;
            break;
        }
    }
    for (auto hy = 0; hy < height; hy++) {
        setCell(x, hy, states[hy]);
    }
}

FALLING_COLUMN GAME_BOARD::getDropHeights(const int x, const int heightThreshold) const {
    FALLING_COLUMN dropHeights;
    std::vector<char> states(height);
    for (auto hy = 0; hy < height; hy++) {
        states[hy] = getCell(x, hy);
    }
    for (auto i = height - 2; i >= 0; i--) {
        if (i > heightThreshold) continue;
        auto& here = states[i];
        if (here == 0) continue;
        for (auto j = i + 1; j < height; j++) {
            if ((states[j] & here) == 0) {
                if (j < height - 1) continue;
                j++;
            }
            dropHeights.fallingRings.push_back(FALLING_RING{ .data = here, .currentHeight = (float)i, .targetHeight = (float)(j - 1) });
            states[j - 1] |= here;
            here = 0;
            break;
        }
    }
    return dropHeights;
}

void GAME_BOARD::render(SDL_Renderer*& renderer, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const VECTOR2i& offset) const {
    const VECTOR2i DIR[4] = { VECTOR2i(1, 0), VECTOR2i(0, 1), VECTOR2i(-1, 0), VECTOR2i(0, -1) };
    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < pitch; x++) {
            const auto cell = getCell(x, y);
            if ((cell & 0b111) == 0) continue;
            auto spriteOffset = VECTOR2i(x, y) * 8 + offset;
            for (auto s = 0; s < 3; s++) {
                if ((cell & (1 << s)) == 0) continue;
                const auto sprite = getGameSprite((GAME_SPRITES)((int)GAME_SPRITES::HOOP_001 + s));
                drawGameSprite(renderer, sprite, spriteOffset, palette);
            }
            if ((cell & 0b111) != 0b111) continue;
            for (auto i = 0; i < 4; i++) {
                const VECTOR2i neighbor = VECTOR2i(x, y) + DIR[i];
                if (neighbor.x < 0 || neighbor.x >= pitch || neighbor.y < 0 || neighbor.y >= height) continue;
                const auto neighborCell = getCell(neighbor.x, neighbor.y);
                if ((neighborCell & 0b111) != 0b111) continue;
                const auto sprite = getGameSprite((GAME_SPRITES)((int)GAME_SPRITES::CONNECTOR_RIGHT + i));
                drawGameSprite(renderer, sprite, spriteOffset, palette);
            }
        }
    }
}

bool GAME_BOARD::isColliding(const int x, const int y, const HOOPS& hoops) const {
    for (auto hy = 0; hy < HOOP_SIZE; hy++) {
        for (auto hx = 0; hx < HOOP_SIZE; hx++) {
            for (auto l = 0; l < 3; l++) {
                if (!hoops.isHoopExist(hx, hy, l)) continue;
                const auto bx = x + hx;
                const auto by = y + hy;
                if (by < 0) continue;
                if (by >= height) return true;
                if (bx < 0 || bx >= pitch) return true;
                const auto cell = getCell(bx, by);
                if ((cell & (1 << l)) == 0) continue;
                return true;
            }
        }
    }
    return false;
}

bool GAME_BOARD::addHoops(const int x, const int y, const HOOPS& hoops) {
    for (auto hy = 0; hy < HOOP_SIZE; hy++) {
        for (auto hx = 0; hx < HOOP_SIZE; hx++) {
            for (auto l = 0; l < 3; l++) {
                if (!hoops.isHoopExist(hx, hy, l)) continue;
                const auto bx = x + hx;
                const auto by = y + hy;
                if (by < 0) return false;
                const auto index = getIndex(bx, by);
                board[index] |= 1 << l;
            }
        }
    }
    return true;
}

std::vector<std::vector<int>> GAME_BOARD::getIslands(const int threshold) const {
    const std::vector<VECTOR2i> SEARCH_DIR = { VECTOR2i(1, 0), VECTOR2i(0, 1), VECTOR2i(-1, 0), VECTOR2i(0, -1) };
    std::vector<std::vector<int>> islands;
    std::vector<bool> visited(board.size(), false);
    for (auto i = 0; i < (int)board.size(); i++) {
        if (visited[i]) continue;
        visited[i] = true;
        if (board[i] != 0b111) continue;

        auto island = std::vector<int>();
        std::queue<int> queue;
        queue.push(i);
        while (!(queue.empty())) {
            const auto here = queue.front();
            queue.pop();
            const auto position = VECTOR2i(here % pitch, here / pitch);
            island.push_back(here);
            for (auto dir : SEARCH_DIR) {
                const auto nextPosition = position + dir;
                if (nextPosition.x < 0 || nextPosition.x >= pitch || nextPosition.y < 0 || nextPosition.y >= height) continue;
                const auto next = getIndex(nextPosition.x, nextPosition.y);
                if (visited[next]) continue;
                visited[next] = true;
                if (getCell(nextPosition.x, nextPosition.y) != 0b111) continue;
                queue.push(next);
            }
        }
        if (threshold > (int)island.size()) continue;

        islands.push_back(island);
    }
    return islands;
}