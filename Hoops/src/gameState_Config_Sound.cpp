#include "gameState_Config.hpp"

void drawVolumeSlider(FONT::BitmapFONT& bitmapFont, const VECTOR2i& position, const SDL_Color& textColor, const float& volumeValue) {
    std::stringstream ss;
    int volume = volumeValue * 4;
    ss << (char)0x11;
    for (int i = 0; i < 5; i++) {
        ss << (i == volume ? (char)0xdb : (char)(0xc4));
    }
    ss << (char)0x10;
    bitmapFont.drawText((position + VECTOR2i(0,1)) * 8, ss.str(), textColor);
}

void doChangeVolume(const bool& isLeft, AUDIO_HANDLER& audioHandler, const int selectedItem) {
    if (selectedItem == 0) {
        int value = audioHandler.getBGMVolume() * 4;
        if (isLeft) {
            value = std::max(value - 1, 0);
        } else {
            value = std::min(value + 1, 4);
        }
        audioHandler.setBGMVolume(value * 0.25);
    } else {
        int value = audioHandler.getSFXVolume() * 4;
        if (isLeft) {
            value = std::max(value - 1, 0);
        } else {
            value = std::min(value + 1, 4);
        }
        audioHandler.setSFXVolume(value * 0.25);
    }
}

void GAME_STATE_FUNCTIONS_CONFIG::doSubMenuSound(SYSTEM_VARIABLES& systemVariables, GAME_VARIABLES& gameVariables) {
    auto& keys = systemVariables.essentials.controls.keys;
    if (keys[SDLK_UP].down) {
        selectedSubMenuSubItem = decrementBound(selectedSubMenuSubItem, 2);
        systemVariables.playBlip();
    }
    if (keys[SDLK_DOWN].down) {
        selectedSubMenuSubItem = incrementBound(selectedSubMenuSubItem, 2);
        systemVariables.playBlip();
    }
    if (isSubMenuItemSelected) {
        if (keys[SDLK_ESCAPE].down || keys[SDLK_RETURN].down) {
            isSubMenuItemSelected = false;
            systemVariables.playSelect();
        }
        if (keys[SDLK_LEFT].down || keys[SDLK_RIGHT].down) {
            doChangeVolume(keys[SDLK_LEFT].down, *systemVariables.audioHandler, selectedSubMenuSubItem);
            auto volumes = std::array<float,2>{
                systemVariables.audioHandler->getBGMVolume(),
                systemVariables.audioHandler->getSFXVolume()
            };
            systemVariables.playBlip(volumes[selectedSubMenuSubItem]);
        }
    } else {
        if (keys[SDLK_RETURN].down || keys[SDLK_RIGHT].down) {
            isSubMenuItemSelected = true;
            systemVariables.playSelect();
        }
        if (keys[SDLK_ESCAPE].down || keys[SDLK_LEFT].down) {
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
    };
    bitmapFont.drawText(positions[0] * 8, "BGM", palette[1]);
    drawVolumeSlider(bitmapFont, positions[0], palette[1], systemVariables.audioHandler->getBGMVolume());
    bitmapFont.drawText(positions[1] * 8, "SFX", palette[1]);
    drawVolumeSlider(bitmapFont, positions[1], palette[1], systemVariables.audioHandler->getSFXVolume());

    if (selectedSubMenuItem != SUBMENU_ID::SOUND) return;
    if (renderTime & 0b1000) {
        if (isSubMenuItemSelected) {
            auto overlay = std::string(7, 0xdb);
            bitmapFont.drawText((positions[selectedSubMenuSubItem] + VECTOR2i(0, 1)) * 8, overlay, palette[0]);
        } else {
            auto overlay = std::string(3, 0xdb);
            bitmapFont.drawText(positions[selectedSubMenuSubItem] * 8, overlay, palette[0]);
        }
    }
}
