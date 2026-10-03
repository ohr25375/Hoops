#include "SDL2_Addon.hpp"

namespace SDL2Addon {
    SDL2A_FRect::SDL2A_FRect(const SDL_FRect& rect) {
        pos  = VECTOR2f(rect.x, rect.y);
        size = VECTOR2f(rect.w, rect.h);
    }

    SDL2A_FRect::operator SDL_FRect() const {
        SDL_FRect res;
        res.x = pos.x;
        res.y = pos.y;
        res.w = size.x;
        res.h = size.y;
        return res;
    }

    SDL2A_Rect::SDL2A_Rect(const SDL_Rect& rect) {
        pos  = VECTOR2i(rect.x, rect.y);
        size = VECTOR2i(rect.w, rect.h);
    }

    SDL2A_Rect::operator SDL_Rect() const {
        SDL_Rect res;
        res.x = pos.x;
        res.y = pos.y;
        res.w = size.x;
        res.h = size.y;
        return res;
    }

    bool IsFunctionSuccessful(int32_t functionResult) {
        if (functionResult < 0) {
            SDL_Log(SDL_GetError());
            return false;
        }
        return true;
    }

    int32_t SetRenderDrawColor(SDL_Renderer*& renderer, const SDL_Color color) {
        return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
    int32_t doDelay(const double FPS_INC) {
        // get current time
        auto currentTick = SDL_GetTicks64();
        // initialize nextTick to next expected time;
        static Uint64 nextTick = currentTick + FPS_INC;
        // initialize extra tick;
        static float extraTick = 0;
        // initialize previous tick to current tick;
        static auto previous = currentTick;

        // initialize delay to 0
        int32_t delay = 0;
        // get cycle duration
        auto ticksPassed = currentTick - previous;
        // if current time is less than the next expected tick
        if (currentTick < nextTick) {
            // delay until next expected time
            delay = nextTick - currentTick;
        }
        // increment next expected frame with decimal carry
        nextTick += (int)FPS_INC + (int)extraTick;
        // increment extra tick by decimal of FPS_INC and keep decimal only
        extraTick += (FPS_INC - (int)FPS_INC) - (int)extraTick;
        // delay the frame
        SDL_Delay(delay);
        // set previous to current time append delay
        previous = currentTick + delay;
        return delay;
    }

    void DrawFilledRectF(SDL_Renderer*& renderer, const SDL_FRect& rect, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRectF(renderer, &rect);
    }

    void DrawFilledRect(SDL_Renderer*& renderer, const SDL_Rect& rect, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void DrawRect(SDL_Renderer*& renderer, const SDL_Rect& rect, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &rect);
    }

    void DrawFilledRectsF(SDL_Renderer*& renderer, const std::vector<SDL_FRect> rects, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRectsF(renderer, rects.data(), rects.size());
    }

    void DrawFilledRects(SDL_Renderer*& renderer, const std::vector<SDL_Rect> rects, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRects(renderer, rects.data(), rects.size());
    }

    void DrawRects(SDL_Renderer*& renderer, const std::vector<SDL_Rect> rects, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRects(renderer, rects.data(), rects.size());
    }

    void DrawPoint(SDL_Renderer*& renderer, const SDL_Point point, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(renderer, point.x, point.y);
    }

    void DrawPointF(SDL_Renderer*& renderer, const SDL_FPoint point, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPointF(renderer, point.x, point.y);
    }

    void DrawPoints(SDL_Renderer*& renderer, const std::vector<SDL_Point> points, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoints(renderer, points.data(), points.size());
    }

    void DrawPointsF(SDL_Renderer*& renderer, const std::vector<SDL_FPoint> points, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPointsF(renderer, points.data(), points.size());
    }
    void DrawLineF(SDL_Renderer*& renderer, const SDL_FPoint from, const SDL_FPoint to, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
    }
    void DrawCircle(SDL_Renderer*& renderer, const VECTOR2f pos, double radius, int indices, const SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        VECTOR2f center = pos;
        auto v1         = VECTOR2f(radius, 0);
        SDL_Vertex vert = {
            .position = center,
            .color    = color,
        };
        std::vector<SDL_Vertex> v(3, vert);
        for (int i = 0; i < indices; i++) {
            auto v2       = v1.rotate2D((M_PI * 2) / indices);
            v[1].position = v1 + pos;
            v[2].position = v2 + pos;
            SDL_RenderGeometry(renderer, nullptr, v.data(), 3, nullptr, 3);
            v1 = v2;
        }
    }
}  // namespace SDL2Addon