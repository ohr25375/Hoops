#pragma once

#include <map>
#include <vector>
#include <array>
#include <memory>
#include "src/vector2.hpp"
#include "src/SDL2_Addon.hpp"
#include "systemEssentials.hpp"

enum GAME_STATE {
    GAME_STATE_FULLSCREEN,
    GAME_STATE_CHOOSE_ZOOM,
    GAME_STATE_ZOOMED4,
    GAME_STATE_ZOOMED8
};

struct GAME_VARIABLES;

class GAME_STATE_FUNCTIONS {
public:
    virtual void doState(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) = 0;
    virtual void doRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) = 0;
    virtual void doInit(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) = 0;
    virtual ~GAME_STATE_FUNCTIONS() = default;
};

class GAME_VARIABLES {
public:
    bool isRunning = true;
    bool isDisplayDebug = false;
    bool isLargeWindow = true;

    GAME_STATE gameState = GAME_STATE::GAME_STATE_FULLSCREEN;
    
    SDL2Addon::SDL2A_Rect screenArea;

    FONT::BitmapFONT bitmapFont{};

    
    VECTOR2i mousePosition{};
    VECTOR2i mouseDelta{};
    SDL2Addon::SDL2A_Rect mouseCaptureArea;
    
    SDL_Texture* renderTarget;
    const VECTOR2i renderTargetSize = VECTOR2i(160, 144);
    std::array<SDL2Addon::SDL_COLOR, 2> palette = {
        0x000000,
        0xffffff,
    };
    void doState(SYSTEM_VARIABLES& systemVariables);
    void doRender(SYSTEM_VARIABLES& systemVariables);
    void setState(SYSTEM_VARIABLES& systemVariables, std::unique_ptr<GAME_STATE_FUNCTIONS> newState);
private:
    std::unique_ptr<GAME_STATE_FUNCTIONS> gameStateFunctions;
};