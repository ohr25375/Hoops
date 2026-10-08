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
}  // namespace FONT