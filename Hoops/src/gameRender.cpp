#include "gameRender.hpp"

#include <format>
#include <locale>
#include <codecvt>
#include <string>
#include "gameSprites.hpp"

void renderOverlay(SDL_Renderer*& renderer, const VECTOR2i& rendererSize) {
    auto rect = SDL2Addon::SDL2A_Rect(rendererSize);
    SDL2Addon::DrawFilledRect(renderer, rect, SDL2Addon::SDL_COLOR(0,128));
}

bool doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& ess = systemVariables.essentials;
    auto& screen = ess.screen;
    SDL_RenderClear(screen.renderer);

    updateGameRender(systemVariables, gameVariables);
    SDL_RenderCopy(screen.renderer, gameVariables.renderTarget, NULL, NULL);

    SDL_RenderPresent(screen.renderer);
    return true;
}

bool updateGameRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& ess = systemVariables.essentials;
    auto& screen = ess.screen;
    SDL_SetRenderTarget(screen.renderer, gameVariables.renderTarget);
    SDL2Addon::SetRenderDrawColor(screen.renderer, gameVariables.palette[0]);
    SDL_RenderClear(screen.renderer);

    gameVariables.doRender(systemVariables);

    if (gameVariables.isDisplayDebug) {
        renderOverlay(screen.renderer, gameVariables.renderTargetSize);
        screen.insertDebugText(ess.frameRate.getDebugText());
        auto& mouse = systemVariables.essentials.controls.mouse;
        screen.insertDebugText("raw mouse position: " + std::string(VECTOR2i(mouse.x, mouse.y)));
        screen.insertDebugText(std::to_string((int)gameVariables.gameState));
        screen.insertDebugText("mouseLeftState = " + std::string((ess.controls.mouse.left.state ? "true" : "false")));
        screen.drawDebugText();
    }
    
    drawGameSprite(screen.renderer, getGameSprite(GAME_SPRITES::GAME_SPRITE_CURSOR), gameVariables.mousePosition, gameVariables.palette, ess.controls.mouse.left.state);

    SDL_SetRenderTarget(screen.renderer, NULL);
    return true;
}