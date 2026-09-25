#include "systemInGameClose.hpp"
#include "src/fileManager.hpp"
#include <filesystem>

void closeGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& save = gameVariables.save;
    int bgmVolume = systemVariables.audioHandler->getBGMVolume() * 4;
    int sfxVolume = systemVariables.audioHandler->getSFXVolume() * 4;
    save.config.volume = (bgmVolume << 4) | sfxVolume;
    save.config.track = (int)gameVariables.gameBGM;
    auto& colorA = gameVariables.palette[1];
    save.config.colorA = 
    (((uint16_t)colorA.r & 0xf0) << 4) |
    (((uint16_t)colorA.g & 0xf0) << 0) |
    (((uint16_t)colorA.b & 0xf0) >> 4);
    auto& colorB = gameVariables.palette[0];
    save.config.colorB = 
    (((uint16_t)colorB.r & 0xf0) << 4) |
    (((uint16_t)colorB.g & 0xf0) << 0) |
    (((uint16_t)colorB.b & 0xf0) >> 4);
    save.config.preset = gameVariables.palettePreset;
    save.config.screenSize = systemVariables.screenSizeMultiplier;

    auto data = SAVE_FILE_V0001::writeSave(save);
    auto path = systemVariables.execPath / "saves.data";
    writeBinary(path.string(), data);

    SDL_DestroyTexture(gameVariables.renderTarget);
}