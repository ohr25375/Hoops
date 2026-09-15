#pragma once
#ifndef screen_hpp
#define screen_hpp
#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "SDL2_Addon.hpp"
#include "vector2.hpp"
#include "SDL2_Font.hpp"
#include <memory>

namespace SDL_SCREEN {
    class SCREEN {
    public:
        SDL_Window* window     = nullptr;
        SDL_Renderer* renderer = nullptr;

        VECTOR2i size = VECTOR2i(128);
        double scale  = 1;
        std::unique_ptr<FONT::FONT> font = nullptr;

        SDL2Addon::SDL2A_FRect rect{};

        void setSize(VECTOR2i windowSize, double fieldScale = 1);
        void setSize(int WIDTH, int HEIGHT, double FIELD_SCALE = 1);
        void setSize(int WINDOW_SIZE);
        ~SCREEN();

        int32_t init();
        VECTOR2f getCenter() const { return (size / scale) / 2; }

        void drawText(VECTOR2d pos, const std::string text, SDL_Color color, SDL_Color backgroundColor = {.a = 0}) const;

        void insertDebugText(const std::string& str, const SDL_Color& color = {0xff, 0xff, 0xff, 0xff}, const bool newLine = true);
        int drawDebugText(const SDL_Color& backgroundColor = {.a = 0});

        void drawSurface(SDL_Surface* surface, const VECTOR2i pos = VECTOR2i(0), const VECTOR2f scale = VECTOR2f(1));
        void drawTexture(SDL_Texture* texture, const SDL2Addon::SDL2A_Rect source, const VECTOR2i pos = VECTOR2i(0), const VECTOR2f scale = VECTOR2f(1));
        void drawTexture(SDL_Texture* texture, const SDL2Addon::SDL2A_Rect source, const SDL2Addon::SDL2A_Rect dest);

        void setInitValues(VECTOR2i windowSize, double fieldScale = 1);

        int clear();
        void present();
    private:
        int32_t InitScreen();
        std::vector<std::tuple<std::string, SDL_Color, bool>> debugTexts;
    };
}  // namespace SDL_SCREEN

#endif