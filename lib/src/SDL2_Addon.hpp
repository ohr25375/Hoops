#pragma once
#ifndef SDL2_addon_hpp
#define SDL2_addon_hpp
#include <SDL.h>
#include <vector>
#include "vector2.hpp"

namespace SDL2Addon {
    template<typename T>
    requires std::integral<T> || std::floating_point<T>
    struct SDL2A_TRect {
        VECTOR2<T> pos;
        VECTOR2<T> size;
        SDL2A_TRect(VECTOR2<T> wh = VECTOR2<T>(0), VECTOR2<T> xy = VECTOR2<T>(0)) : pos(xy), size(wh) {}

        VECTOR2<T> getTopLeft() const { return pos; };
        VECTOR2<T> getBottomRight() const { return pos + size; };
        bool operator==(const SDL2A_TRect<T>& rhs) const { return (pos.equality(rhs.pos) && size.equality(rhs.size)); }
        bool operator!=(const SDL2A_TRect<T>& rhs) const { return !(*this == rhs); }
    };

    struct SDL2A_FRect : public SDL2A_TRect<float> {
        using SDL2A_TRect<float>::SDL2A_TRect;
        SDL2A_FRect(const SDL_FRect& rect);
        operator SDL_FRect() const;
    };

    struct SDL2A_Rect : public SDL2A_TRect<int> {
        using SDL2A_TRect<int>::SDL2A_TRect;
        SDL2A_Rect(const SDL_Rect& rect);
        operator SDL_Rect() const;
    };

    template<typename T>
    bool rectContainsPoint(const SDL2A_TRect<T>& rect, const SDL_FPoint& point) {
        auto topLeft     = rect.getTopLeft();
        auto bottomRight = rect.getBottomRight();
        if (
            point.x < topLeft.x || point.y < topLeft.y ||
            point.x >= bottomRight.x || point.y >= bottomRight.y
        ) {
            return false;
        }
        return true;
    }

    template<typename T>
    bool isRectOverlapping(const SDL2A_TRect<T>& rect1, const SDL2A_FRect& rect2) {
        const auto& r1AA = rect1.getTopLeft();
        const auto& r1BB = rect1.getBottomRight();
        const auto& r2AA = rect2.getTopLeft();
        const auto& r2BB = rect2.getBottomRight();
        if (r1BB.x < r2AA.x || r2BB.x < r1AA.x)
            return false;
        if (r1BB.y < r2AA.y || r2BB.y < r1AA.y)
            return false;
        return true;
    }

    bool IsFunctionSuccessful(int32_t functionResult);

    int32_t SetRenderDrawColor(SDL_Renderer*& renderer, const SDL_Color color);
    int32_t doDelay(const double FPS_INC);

    void DrawFilledRectF(SDL_Renderer*& renderer, const SDL_FRect& rect, const SDL_Color color);
    void DrawFilledRect(SDL_Renderer*& renderer, const SDL_Rect& rect, const SDL_Color color);
    void DrawRect(SDL_Renderer*& renderer, const SDL_Rect& rect, const SDL_Color color);
    void DrawFilledRectsF(SDL_Renderer*& renderer, const std::vector<SDL_FRect> rects, const SDL_Color color);
    void DrawFilledRects(SDL_Renderer*& renderer, const std::vector<SDL_Rect> rects, const SDL_Color color);
    void DrawRects(SDL_Renderer*& renderer, const std::vector<SDL_Rect> rects, const SDL_Color color);
    void DrawPoint(SDL_Renderer*& renderer, const SDL_Point point, const SDL_Color color);
    void DrawPointF(SDL_Renderer*& renderer, const SDL_FPoint point, const SDL_Color color = SDL_Color{.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff});
    void DrawPoints(SDL_Renderer*& renderer, const std::vector<SDL_Point> points, const SDL_Color color);
    void DrawPointsF(SDL_Renderer*& renderer, const std::vector<SDL_FPoint> points, const SDL_Color color);

    void DrawLineF(SDL_Renderer*& renderer, const SDL_FPoint from, const SDL_FPoint to, const SDL_Color& color = SDL_Color{.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff});

    void DrawCircle(SDL_Renderer*& renderer, const VECTOR2f pos, double radius, int indices, const SDL_Color color);
}  // namespace SDL2Addon

#endif