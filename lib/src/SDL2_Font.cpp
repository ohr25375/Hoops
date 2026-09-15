#include "SDL2_Font.hpp"

#include <numeric>
#include <string>

namespace FONT {
    int FONT::init(SDL_Renderer* renderer) {
        this->renderer = renderer;
        return 0;
    }

    VECTOR2i FONT::getSize() const {
        return VECTOR2i(w,h);
    }

    int BitmapFONT::init(SDL_Renderer* renderer) {
        FONT::init(renderer);
        SDL_Surface* s = SDL_LoadBMP(path.data());
        if (!s) {
            SDL_FreeSurface(s);
            SDL_Log("BitmapFONT: Failed to load BMP");
            return -1;
        }
        font = SDL_CreateTextureFromSurface(renderer, s);
        if (!font) {
            SDL_FreeSurface(s);
            SDL_DestroyTexture(font);
            SDL_Log("BitmapFONT: Failed to create Texture");
            return -1;
        }
        if (charSet.empty()) {
            charSet = std::vector<char>(256);
            for (int i = 0; i < 256; i++) {
                charSet[i] = i;
            }
            SDL_Log("BitmapFONT: charSet empty. setting defaults");
        }
        w = size.x;
        h = size.y;
        SDL_Log("BitmapFONT: setting size...");
        for (int i = 0; i < charSet.size(); i++) {
            int row = (s->w) / w;
            char c = charSet[i];
            charMap.emplace(c, VECTOR2i((i % row) * w, (i / row) * h));
        }
        SDL_Log("Loaded font[%d,%d] with size [%d,%d]", s->w / w, s->h / h, w, h);
        SDL_FreeSurface(s);
        return 0;
    }

    int BitmapFONT::setFont(const std::string& path, const VECTOR2i& size, const INCLUDES charMap) {
        std::vector<char> cs;
        if (charMap == Codepage437) {
            cs = std::vector<char>(256);
            for (int i = 0; i < 256; i++) {
                cs[i] = i;
            }
        }
        int er = setFont(path, size, cs);
        cs.clear();
        return er;
    }

    int BitmapFONT::setFont(const std::string& path, const VECTOR2i& size, const std::vector<char> charSet) {
        this->path = path;
        this->size = size;
        this->charSet = charSet;
        return 0;
    }

    void BitmapFONT::close() {
        SDL_Log("Freeing Bitmap texture");
        SDL_DestroyTexture(font);
        renderer = nullptr;
        charMap.clear();
        charSet.clear();
        charSet = std::vector<char>(0);
    }

    void BitmapFONT::drawText(VECTOR2d pos, std::string text, SDL_Color color, SDL_Color backgroundColor) {
        try {
            if (!renderer) {
                throw std::runtime_error("Missing renderer");
            }
            if (!font) {
                throw std::runtime_error("Missing font");
            }
            SDL_SetTextureColorMod(font, backgroundColor.r, backgroundColor.g, backgroundColor.b);
            SDL_SetTextureAlphaMod(font, backgroundColor.a);
            SDL_Rect textRectVel        = {(int)pos.x, (int)pos.y, w, h};
            SDL_Rect fontPos = {0, 0, w, h};
            for (int i = 0; i < text.size(); i++) {
                char c = 0xDB;
                if (!charMap.contains(c)) {
                    c = '\0';
                }
                auto [x, y] = charMap[c];
                fontPos.x = x;
                fontPos.y = y;
                SDL_RenderCopy(renderer, font, &fontPos, &textRectVel);
                textRectVel.x += w;
            }
            textRectVel.x = (int)pos.x;
            SDL_SetTextureColorMod(font, color.r, color.g, color.b);
            SDL_SetTextureAlphaMod(font, color.a);
            for (int i = 0; i < text.size(); i++) {
                char c = text[i];
                if (!charMap.contains(c)) {
                    c = '\0';
                }
                auto [x, y] = charMap[c];
                fontPos.x = x;
                fontPos.y = y;
                SDL_RenderCopy(renderer, font, &fontPos, &textRectVel);
                textRectVel.x += w;
            }
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
            return;
        }
    }

    std::pair<int, int> BitmapFONT::getDimension(const std::string string) {
        return std::pair<int,int>(w * string.size(),h);
    }
    std::unique_ptr<BitmapFONT> createBitmapFont(const VECTOR2i& fontSize, const std::string& path, const INCLUDES& charMap) {
        auto result = std::make_unique<BitmapFONT>();
        result->setFont(path, fontSize, charMap);
        return result;
    }
}  // namespace FONT