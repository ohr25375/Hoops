#pragma once

#include <vector>
#include <string>

enum AUDIO_ASSET_ID {
    AUDIO_ASSET_NONE,
    AUDIO_ASSET_RINGS,
    AUDIO_ASSET_ALLOY,
    AUDIO_ASSET_SFX_PLACE,
};

std::vector<std::string> getMML(const AUDIO_ASSET_ID& assetID);