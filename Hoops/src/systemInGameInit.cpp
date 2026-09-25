#include "systemInGameInit.hpp"

#include <filesystem>
#include "gameState_TitleMenu.hpp"
#include "src/fileManager.hpp"

void initGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    std::vector<std::byte> data;
    auto& save = gameVariables.save;
    auto path = systemVariables.execPath / "saves.data";
    if (!readBinary(path, data)) {
        SDL_Log("File not found");
        SDL_Log("Creating new save file");
        data = SAVE_FILE_V0001::writeSave(save);
        writeBinary(path, data);
    } else {
        SAVE_FILE_V0001::loadSave(data, save);
    }

    systemVariables.audioHandler->setBGMVolume(0.25 * (save.config.volume >> 4 & 0xf));
    systemVariables.audioHandler->setSFXVolume(0.25 * (save.config.volume & 0xf));
    gameVariables.gameBGM = (AUDIO_ASSET_ID)save.config.track;
    gameVariables.palette[1] = SDL2Addon::SDL_COLOR(
        ((save.config.colorA >> 8 & 0xf) << 4) | (save.config.colorA >> 8 & 0xf),
        ((save.config.colorA >> 4 & 0xf) << 4) | (save.config.colorA >> 4 & 0xf),
        ((save.config.colorA >> 0 & 0xf) << 4) | (save.config.colorA >> 0 & 0xf)
    );
    gameVariables.palette[0] = SDL2Addon::SDL_COLOR(
        ((save.config.colorB >> 8 & 0xf) << 4) | (save.config.colorB >> 8 & 0xf),
        ((save.config.colorB >> 4 & 0xf) << 4) | (save.config.colorB >> 4 & 0xf),
        ((save.config.colorB >> 0 & 0xf) << 4) | (save.config.colorB >> 0 & 0xf)
    );
    gameVariables.palettePreset = save.config.preset;
    systemVariables.screenSizeMultiplier = save.config.screenSize;
    

    systemVariables.toggleWindowSize(gameVariables.isLargeWindow);

    VECTOR2i textureSize       = systemVariables.essentials.screen.size;
    gameVariables.renderTarget = SDL_CreateTexture(systemVariables.essentials.screen.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureSize.x, textureSize.y);
    SDL_SetTextureScaleMode(gameVariables.renderTarget, SDL_ScaleModeNearest);

    gameVariables.screenArea = SDL2Addon::SDL2A_Rect(systemVariables.essentials.screen.size);

    gameVariables.bitmapFont = FONT::BitmapFONT();
    gameVariables.bitmapFont.setFont("8bitFont.bmp", VECTOR2i(8), FONT::INCLUDES::Codepage437);
    gameVariables.bitmapFont.init(systemVariables.essentials.screen.renderer);

    gameVariables.setState(systemVariables, std::make_unique<GAME_STATE_FUNCTIONS_TITLE>());

    SDL_Log("systemInGameInit: done");
}