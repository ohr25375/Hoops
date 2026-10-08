#include "systemInGameClose.hpp"
#include "src/fileManager.hpp"
#include <filesystem>

void saveGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& save = gameVariables.save;
    int bgmVolume = systemVariables.audioHandler->getBGMVolume() * 4;
    int sfxVolume = systemVariables.audioHandler->getSFXVolume() * 4;
    save.config.volume = (bgmVolume << 4) | sfxVolume;
    save.config.track = (int)gameVariables.gameBGM;
    save.config.colorA = gameVariables.palette[1].getRGB444();
    save.config.colorB = gameVariables.palette[0].getRGB444();
    save.config.preset = gameVariables.palettePreset;
    save.config.screenSize = systemVariables.screenSizeMultiplier;

    auto data = SAVE_FILE_V0001::writeSave(save);
    auto path = systemVariables.execPath / "saves.data";
    writeBinary(path.string(), data);
}

void closeGame(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    saveGame(systemVariables, gameVariables);

    SDL_DestroyTexture(gameVariables.renderTarget);
}