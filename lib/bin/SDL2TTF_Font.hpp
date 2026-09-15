#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDL2_Font.hpp"
#include "vector2.hpp"


namespace FONT {
    class TTF_FONT : public FONT::FONT {
    public:
        int size;
        int setFont(const char* path, const int& size = 12);
        virtual void drawText(VECTOR2d pos, std::string text, SDL_Color color) override;
        virtual int init(SDL_Renderer* renderer) override;
        virtual std::pair<int,int> getDimension(const std::string string) override;
        virtual void close() override;
        ~TTF_FONT();
    
    private:
        TTF_Font* font;
    };
}