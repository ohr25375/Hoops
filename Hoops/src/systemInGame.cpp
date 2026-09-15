#include "systemInGame.hpp"

#include <algorithm>

#include "gameEssentials.hpp"
#include "gameRender.hpp"

#include "systemInGameInit.hpp"

#include <SDL_image.h>
#include <ctime>
#include <fstream>
#include <filesystem>

bool saveScreenshot(SDL_Renderer*& renderer, SDL_Texture* renderTarget) {
    std::cout << "Taking Screenshot... ";
    std::filesystem::path folder = "screenshots";
    SDL_SetRenderTarget(renderer, renderTarget);
    Uint32 format;
    int w, h;
    SDL_QueryTexture(renderTarget, &format, NULL, &w, &h);
    auto surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, format);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    char buff[100];
    std::time_t t = std::time(nullptr);

    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directory(folder);
    }

    std::strftime(buff, sizeof(buff), "%Y-%m-%d at %H.%M.%S.png", std::localtime(&t));
    std::filesystem::path fileName = folder/std::string(buff);
    if (IMG_SavePNG(surface, fileName.string().data())) {
        std::cout << "failed\n";
    } else {
        std::cout << "done\n";
    }
    SDL_SetRenderTarget(renderer, NULL);
    SDL_FreeSurface(surface);
    return false;
}

VECTOR2i getScreenMousePosition(VECTOR2i mousePosition, const bool isLargeWindow) {
    if (isLargeWindow) {
        mousePosition /= 2;
    }
    return mousePosition;
}

VECTOR2i correctMousePosition(VECTOR2i mousePosition, const SDL2Addon::SDL2A_Rect& mouseCaptureArea) {
    mousePosition.x = std::min(std::max(mousePosition.x, mouseCaptureArea.pos.x), mouseCaptureArea.pos.x + mouseCaptureArea.size.x - 1);
    mousePosition.y = std::min(std::max(mousePosition.y, mouseCaptureArea.pos.y), mouseCaptureArea.pos.y + mouseCaptureArea.size.y - 1);
    return mousePosition;
}

void updateMousePosition(const MOUSE& mouse, GAME_VARIABLES& gameVariables) {
    auto previousMousePos = gameVariables.mousePosition;
    auto screenMousePos = getScreenMousePosition(VECTOR2i(mouse.x, mouse.y), gameVariables.isLargeWindow);
    auto previousScreenMousePos = getScreenMousePosition(VECTOR2i(mouse.x, mouse.y) - VECTOR2i(mouse.xdir, mouse.ydir), gameVariables.isLargeWindow);

    auto delta = screenMousePos - previousScreenMousePos;
    gameVariables.mousePosition += delta;
    gameVariables.mousePosition = correctMousePosition(gameVariables.mousePosition, gameVariables.mouseCaptureArea);
    gameVariables.mouseDelta = gameVariables.mousePosition - previousMousePos;
}

bool doGame(SYSTEM_VARIABLES& systemVariables) {
    GAME_VARIABLES gameVariables;

    initGame(systemVariables, gameVariables);

    while (gameVariables.isRunning) {
        updateInternals(systemVariables, gameVariables);
        auto& ess  = systemVariables.essentials;
        auto& keys = ess.controls.keys;
        auto& mouse = ess.controls.mouse;

        updateMousePosition(mouse, gameVariables);
        
        if (keys[SDLK_F2].down) {
            gameVariables.isLargeWindow = !gameVariables.isLargeWindow;
            toggleWindowSize(gameVariables.isLargeWindow, ess.screen);
        }

        if (keys[SDLK_F4].down) {
            gameVariables.isDisplayDebug = !gameVariables.isDisplayDebug;
            SDL_SetWindowBordered(ess.screen.window, gameVariables.isDisplayDebug ? SDL_TRUE : SDL_FALSE);
        }

        if (keys[SDLK_F11].down) {
            saveScreenshot(ess.screen.renderer, gameVariables.renderTarget);
        }

        gameVariables.doState(systemVariables);

        if (ess.isRenderStep()) {
            doRender(systemVariables, gameVariables);
        }
    }
    SDL_DestroyTexture(gameVariables.renderTarget);
    return true;
}

bool updateInternals(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& ess = systemVariables.essentials;

    ess.pollEvents(gameVariables.isRunning);
    ess.controls.update();

    return true;
}