#include "SDL2TTF_Font.hpp"

namespace FONT {
    int TTF_FONT::setFont(const char* path, const int& size) {
        this->size = size;
        font = TTF_OpenFont(path, size);
        if (!font) return -1;
        return 0;
    }

    void TTF_FONT::drawText(VECTOR2d pos, std::string text, SDL_Color color) {
        try {
            if (!font) {
                throw std::runtime_error("Missing font");
            }
            SDL_Surface* textSurfaceVel = TTF_RenderText_Solid(font, text.c_str(), color);
            SDL_Texture* textTextureVel = SDL_CreateTextureFromSurface(renderer, textSurfaceVel);
            SDL_Rect textRectVel        = {(int)pos.x, (int)pos.y, textSurfaceVel->w, textSurfaceVel->h};
            SDL_RenderCopy(renderer, textTextureVel, NULL, &textRectVel);
            SDL_DestroyTexture(textTextureVel);
            SDL_FreeSurface(textSurfaceVel);
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
            return;
        }
    }

    int TTF_FONT::init(SDL_Renderer* renderer) {
        FONT::init(renderer);
        return TTF_Init();
    }

    std::pair<int, int> TTF_FONT::getDimension(const std::string string) {
        int w, h;
        TTF_SizeText(font, string.data(), &w, &h);
        return std::pair<int,int>(w,h);
    }

    void TTF_FONT::close() {
        TTF_CloseFont(font);
    }
    
    TTF_FONT::~TTF_FONT() {
        TTF_Quit();
    }
}