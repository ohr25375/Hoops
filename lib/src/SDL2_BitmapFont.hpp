#include "SDL2_Font.hpp"

namespace FONT {
    class BitmapFONT : public FONT {
    public:
        int setFont(const std::string& path, const VECTOR2i& size, const INCLUDES charMap);
        int setFont(const std::string& path, const VECTOR2i& size, const std::vector<char> charSet);
        virtual void drawText(VECTOR2d pos, std::string text, SDL_Color color, SDL_Color backgroundColor = {.a= 0}) override;
        virtual int init(SDL_Renderer* renderer) override;
        virtual std::pair<int,int> getDimension(const std::string string) override;

        virtual ~BitmapFONT() {};
        virtual void close() override;
        SDL_Texture* font = nullptr;
        std::map<char, VECTOR2i> charMap;
    private:
        std::string path = "../../Fonts/8BitFont.bmp";
        VECTOR2i size = VECTOR2i(16);
        std::vector<char> charSet;
    };

    std::unique_ptr<BitmapFONT> createBitmapFont(const VECTOR2i& fontSize, const std::string& path, const INCLUDES& charMap);
}