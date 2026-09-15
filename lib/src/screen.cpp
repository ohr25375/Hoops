#include "screen.hpp"

#include <exception>
#include <stdexcept>
#include <iostream>

namespace SDL_SCREEN {
    void SCREEN::setSize(VECTOR2i windowSize, double fieldScale) {
        size  = windowSize * fieldScale;
        scale = fieldScale;
    }

    void SCREEN::setSize(int WIDTH, int HEIGHT, double FIELD_SCALE) {
        size.x = WIDTH * FIELD_SCALE;
        size.y = HEIGHT * FIELD_SCALE;
        scale  = FIELD_SCALE;
    }

    void SCREEN::setSize(int WINDOW_SIZE) {
        size  = VECTOR2i(WINDOW_SIZE);
        scale = 1;
    }

    SCREEN::~SCREEN() {
        if (font != nullptr) {
            font->close();
            // delete font;
            // font = nullptr;
        }
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
        }
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        }
    }

    int32_t SCREEN::init() {
        if (InitScreen() < 0) {
            return -1;
        }
        if (font == nullptr) {
            font = std::make_unique<FONT::BitmapFONT>();
        }
        if (font->init(renderer) < 0) {
            return -1;
        }
        return 0;
    }

    void SCREEN::drawText(VECTOR2d pos, const std::string text, SDL_Color color, SDL_Color backgroundColor) const {
        font->drawText(pos, text, color, backgroundColor);
    }

    void SCREEN::insertDebugText(const std::string &str, const SDL_Color& color, const bool newLine) {
        debugTexts.emplace_back(str, color, newLine);
    }

    int SCREEN::drawDebugText(const SDL_Color& backgroundColor) {
        try {
            float yPos = 0;
            float xPos = 0;
            for (int i = 0; i < std::size(debugTexts); i++) {
                auto [text, color, newLine] = debugTexts[i];
                drawText(VECTOR2i(xPos, yPos), text, color, backgroundColor);
                auto [w, h] = font->getDimension(text.data());
                yPos += newLine ? h : 0;
                xPos = newLine ? 0 : xPos + w;
            }
            debugTexts.clear();
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
            return -1;
        }
        return 0;
    }

    void SCREEN::drawSurface(SDL_Surface* surface, const VECTOR2i pos, const VECTOR2f scale) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        drawTexture(tex, SDL2Addon::SDL2A_Rect(), pos, scale);
        SDL_DestroyTexture(tex);
    }

    void SCREEN::drawTexture(SDL_Texture* texture, const SDL2Addon::SDL2A_Rect source, const VECTOR2i pos, const VECTOR2f scale) {
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        drawTexture(texture, source, SDL2Addon::SDL2A_Rect(VECTOR2i(w, h).scale(scale), pos));
    }

    void SCREEN::drawTexture(SDL_Texture* texture, const SDL2Addon::SDL2A_Rect source, const SDL2Addon::SDL2A_Rect dest) {
        SDL_Rect srect = source;
        SDL_Rect drect = dest;
        SDL_RenderCopy(renderer, texture, &srect, &drect);
    }

    void SCREEN::setInitValues(VECTOR2i windowSize, double fieldScale) {
        size  = windowSize * fieldScale;
        scale = fieldScale;
    }

    int SCREEN::clear() {
        return SDL_RenderClear(this->renderer);
    }

    void SCREEN::present() {
        return SDL_RenderPresent(this->renderer);
    }

    int32_t SCREEN::InitScreen() {
        rect.pos  = VECTOR2f(0);
        rect.size = size;
        if (SDL_CreateWindowAndRenderer(size.x, size.y, 0, &window, &renderer) < 0) {
            std::cout << "SCREEN >> Failed to create window";
            return -1;
        };
        SDL_Log("Created window and renderer with size [%i x %i]", size.x, size.y);
        if (SDL_RenderSetScale(renderer, scale, scale) < 0) {
            std::cout << "SCREEN >> Failed to set render scale";
            return -1;
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        return 0;
    }
} // namespace SDL_SCREEN