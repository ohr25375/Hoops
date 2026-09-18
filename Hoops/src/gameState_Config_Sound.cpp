#include "gameState_Config.hpp"

void drawVolumeSlider(FONT::BitmapFONT& bitmapFont, const VECTOR2i& position, const SDL_Color& textColor, const float& volumeValue) {
    std::stringstream ss;
    int volume = volumeValue * 4;
    auto slider = std::string(5, (char)0xc4);
    slider[volume] = (char)0xdb;
    ss << (char)0x11 << slider << (char)0x10;
    bitmapFont.drawText((position + VECTOR2i(0,1)) * 8, ss.str(), textColor);
}
void drawTrackName(FONT::BitmapFONT& bitmapFont, const VECTOR2i& position, const SDL_Color& textColor, const std::string& trackName) {
    auto spaces = std::string(7 - trackName.size(), ' ');
    std::stringstream ss;
    ss << (char)0x11 << trackName << spaces << (char)0x10;
    bitmapFont.drawText((position + VECTOR2i(-1,1)) * 8, ss.str(), textColor);
}

void doChangeVolume(const bool& isLeft, AUDIO_HANDLER& audioHandler, const int selectedItem) {
    if (selectedItem == 0) {
        int value = audioHandler.getBGMVolume() * 4;
        value = isLeft ? std::max(value - 1, 0) : std::min(value + 1, 4);
        audioHandler.setBGMVolume(value * 0.25);
    } else {
        int value = audioHandler.getSFXVolume() * 4;
        value = isLeft ? std::max(value - 1, 0) : std::min(value + 1, 4);
        audioHandler.setSFXVolume(value * 0.25);
    }
}

void doChangeTrack(const bool& isLeft, AUDIO_ASSET_ID& trackID) {
    auto bound = (int)AUDIO_ASSET_ID::AUDIO_ASSET_BGM_MAX;
    trackID = (AUDIO_ASSET_ID)(isLeft ? decrementBound((int)trackID, bound)  : incrementBound((int)trackID, bound));
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuSound(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_UP].down) {
        selectedSubMenuSubItem = decrementBound(selectedSubMenuSubItem, 3);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedSubMenuSubItem = incrementBound(selectedSubMenuSubItem, 3);
        systemVariables.playBlip();
    }
    if (isSubMenuItemSelected) {
        if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down || keys[SDLK_z].down || keys[SDLK_x].down) {
            isSubMenuItemSelected = false;
            systemVariables.playSelect();
        }
        if (keys[SDLK_LEFT].down || keys[SDLK_RIGHT].down) {
            if (selectedSubMenuSubItem < 2) {
                doChangeVolume(keys[SDLK_LEFT].down, *systemVariables.audioHandler, selectedSubMenuSubItem);
                auto volumes = std::array<float,2>{
                    systemVariables.audioHandler->getBGMVolume(),
                    systemVariables.audioHandler->getSFXVolume()
                };
                systemVariables.playBlip(volumes[selectedSubMenuSubItem]);
            } else {
                doChangeTrack(keys[SDLK_LEFT].down, gameVariables.gameBGM);
                systemVariables.playBlip();
            }
        }
    } else {
        if (keys[SDLK_RETURN].down || keys[SDLK_RIGHT].down || keys[SDLK_z].down) {
            isSubMenuItemSelected = true;
            systemVariables.playSelect();
        }
        if (keys[SDLK_ESCAPE].down || keys[SDLK_LEFT].down || keys[SDLK_x].down) {
            selectedSubMenuItem    = SUBMENU_ID::NONE;
            selectedSubMenuSubItem = 0;
            systemVariables.playSelect();
        }
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuSoundRender(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& bitmapFont = gameVariables.bitmapFont;
    auto& palette    = gameVariables.palette;
    VECTOR2i positions[] = {
        VECTOR2i(11,2),
        VECTOR2i(11,5),
        VECTOR2i(11,8),
    };
    bitmapFont.drawText(positions[0] * 8, "BGM", palette[1]);
    drawVolumeSlider(bitmapFont, positions[0], palette[1], systemVariables.audioHandler->getBGMVolume());
    bitmapFont.drawText(positions[1] * 8, "SFX", palette[1]);
    drawVolumeSlider(bitmapFont, positions[1], palette[1], systemVariables.audioHandler->getSFXVolume());
    bitmapFont.drawText(positions[2] * 8, "TRACK", palette[1]);
    drawTrackName(bitmapFont, positions[2], palette[1], getAudioAssetName(gameVariables.gameBGM));

    if (selectedSubMenuItem != SUBMENU_ID::SOUND) return;
    if (renderTime & 0b1000) {
        auto overlay = std::string(5, 0xdb);
        auto position = positions[selectedSubMenuSubItem];
        if (isSubMenuItemSelected) {
            overlay = std::string(7, 0xdb);
            position += VECTOR2i(0, 1);
        }
        bitmapFont.drawText(position * 8, overlay, palette[0]);
    }
}
