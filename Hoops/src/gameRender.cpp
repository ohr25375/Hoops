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

std::string getBooleanSymbol(const bool& state) {
    return std::string(1,(state ? (char)0x7 : (char)0x9));
}

void drawDebugTexts(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& frameRate = systemVariables.essentials.frameRate;
    auto& screen = systemVariables.essentials.screen;
    auto& textColor = gameVariables.palette[0];
    auto& background = gameVariables.palette[1];
    renderOverlay(screen.renderer, gameVariables.renderTargetSize);
    std::stringstream fps;
    fps << "FPS: " << std::fixed << std::setprecision(2) << frameRate.getCurrentFramesPerSecond() << " / " << frameRate.FPS;
    screen.insertDebugText(fps.str(), textColor);
    screen.insertDebugText("UPF : " + std::to_string(frameRate.getUpdatesPerFrame()), textColor);
    screen.insertDebugText("TPF : " + std::to_string(frameRate.getTicksPerFrame()), textColor);

    auto& keys = systemVariables.essentials.controls.keys;
    screen.insertDebugText("Inputs:", textColor);
    screen.insertDebugText("A:" + getBooleanSymbol(keys[SDLK_z].state), textColor, false);
    screen.insertDebugText("|B:" + getBooleanSymbol(keys[SDLK_x].state), textColor, false);
    screen.insertDebugText("|C:" + getBooleanSymbol(keys[SDLK_c].state), textColor);
    screen.insertDebugText("LEFT :" + getBooleanSymbol(keys[SDLK_LEFT].state), textColor, false);
    screen.insertDebugText("|RIGHT :" + getBooleanSymbol(keys[SDLK_RIGHT].state), textColor);
    screen.insertDebugText("UP   :" + getBooleanSymbol(keys[SDLK_UP].state), textColor, false);
    screen.insertDebugText("|DOWN  :" + getBooleanSymbol(keys[SDLK_DOWN].state), textColor);
    screen.insertDebugText("START:" + getBooleanSymbol(keys[SDLK_RETURN].state), textColor, false);
    screen.insertDebugText("|SELECT:" + getBooleanSymbol(keys[SDLK_ESCAPE].state), textColor);


    screen.drawDebugText(background);
}

bool updateGameRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& ess = systemVariables.essentials;
    auto& screen = ess.screen;
    SDL_SetRenderTarget(screen.renderer, gameVariables.renderTarget);
    SDL2Addon::SetRenderDrawColor(screen.renderer, gameVariables.palette[0]);
    SDL_RenderClear(screen.renderer);

    gameVariables.doRender(systemVariables);

    if (gameVariables.isDisplayDebug) {
        drawDebugTexts(systemVariables, gameVariables);
    }

    SDL_SetRenderTarget(screen.renderer, NULL);
    return true;
}