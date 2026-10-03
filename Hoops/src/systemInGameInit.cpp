#include "systemInGameInit.hpp"

#include <filesystem>
#include "gameState_TitleMenu.hpp"
#include "src/fileManager.hpp"
#include "src/SDL2_BitmapFont.hpp"

void loadSave(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    std::vector<std::byte> data;
    auto& save = gameVariables.save;
    auto path = systemVariables.execPath / "saves.data";
    if (!readBinary(path, data)) {
        SDL_Log("File not found");
        SDL_Log("Creating new save file");
        data = SAVE_FILE_V0001::writeSave(save);
        writeBinary(path, data);
    } else {
        SAVE_FILE_V0001::loadSaveFromData(data, save);
    }

    systemVariables.audioHandler->setBGMVolume(0.25 * ((save.config.volume >> 4) & 0xf));
    systemVariables.audioHandler->setSFXVolume(0.25 * (save.config.volume & 0xf));
    gameVariables.gameBGM = (AUDIO_ASSET_ID)save.config.track;
    gameVariables.palette[1] = SDL2Util::createColorFromRGB444(save.config.colorA);
    gameVariables.palette[0] = SDL2Util::createColorFromRGB444(save.config.colorB);
    gameVariables.palettePreset = save.config.preset;
    systemVariables.screenSizeMultiplier = save.config.screenSize;
}

void initGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    loadSave(systemVariables, gameVariables);
    systemVariables.toggleWindowSize(gameVariables.isLargeWindow);

    VECTOR2i textureSize       = systemVariables.essentials.screen.size;
    gameVariables.renderTarget = SDL_CreateTexture(systemVariables.essentials.screen.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureSize.x, textureSize.y);
    SDL_SetTextureScaleMode(gameVariables.renderTarget, SDL_ScaleModeNearest);

    gameVariables.screenArea = SDL2Addon::SDL2A_Rect(systemVariables.essentials.screen.size);

    gameVariables.bitmapFont = FONT::createBitmapFont(VECTOR2i(8),"8bitFont.bmp", FONT::INCLUDES::Codepage437);
    gameVariables.bitmapFont->init(systemVariables.essentials.screen.renderer);

    gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());

    SDL_Log("systemInGameInit: done");
}