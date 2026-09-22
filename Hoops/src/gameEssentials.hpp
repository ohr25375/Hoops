#pragma once

#include <map>
#include <vector>
#include <array>
#include <memory>
#include "src/vector2.hpp"
#include "src/SDL2_Addon.hpp"
#include "systemEssentials.hpp"
#include "bgmAssets.hpp"

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
    bool isPaused = false;
    bool isDisplayDebug = false;
    bool isLargeWindow = true;

    AUDIO_ASSET_ID gameBGM = AUDIO_ASSET_ID::RINGS;
    
    SDL2Addon::SDL2A_Rect screenArea;

    FONT::BitmapFONT bitmapFont{};
    
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

struct MENU_ITEM {
    const VECTOR2i position;
    const std::string text;
    constexpr MENU_ITEM(const VECTOR2i& position, const std::string& text) : position(position), text(text) {}
};