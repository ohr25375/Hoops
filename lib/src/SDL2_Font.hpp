#pragma once

#include <SDL.h>
#include <vector>
#include <map>
#include "vector2.hpp"
#include <memory>

namespace FONT {
    class FONT {
    public:
        int w, h;
        virtual void drawText(VECTOR2d pos, std::string text, SDL_Color color, SDL_Color backgroundColor = {.a= 0}) {};
        virtual int init(SDL_Renderer* renderer);
        virtual std::pair<int,int> getDimension(const std::string string) { return std::pair<int,int>(0,0); };
        virtual void close() { std::cout << "closing font\n"; };
        VECTOR2i getSize() const;
        virtual ~FONT() {};
    protected:
        SDL_Renderer* renderer = nullptr;
    };
    const std::string LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
    const std::string UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string NUMERIC = "0123456789";


    enum INCLUDES {
        nothing,
        lowercase,
        uppercase,
        Alphabet,
        numerics,
        lowerNumerics,
        UpperNumerics,
        AlphaNumerics,
        UpperASCII,
        Codepage437,
        JISX0208
    };
}