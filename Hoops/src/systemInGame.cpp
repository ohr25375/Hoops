#include "systemInGame.hpp"

#include <algorithm>

#include "gameEssentials.hpp"
#include "gameRender.hpp"

#include "systemInGameInit.hpp"
#include "systemInGameClose.hpp"

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

bool doGame(SYSTEM_VARIABLES& systemVariables) {
    GAME_VARIABLES gameVariables;

    initGame(systemVariables, gameVariables);

    while (gameVariables.isRunning) {
        updateInternals(systemVariables, gameVariables);
        auto& ess  = systemVariables.essentials;
        auto& keys = ess.controls.keys;
        
        if (keys[SDLK_F2].down) {
            gameVariables.isLargeWindow = !gameVariables.isLargeWindow;
            systemVariables.toggleWindowSize(gameVariables.isLargeWindow);
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

    closeGame(systemVariables, gameVariables);
    return true;
}

bool updateInternals(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& ess = systemVariables.essentials;

    ess.pollEvents(gameVariables.isRunning);
    ess.controls.update();

    return true;
}