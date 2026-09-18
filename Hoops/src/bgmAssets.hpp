#pragma once

#include <vector>
#include <string>

enum struct AUDIO_ASSET_ID {
    AUDIO_ASSET_NONE,
    AUDIO_ASSET_RINGS,
    AUDIO_ASSET_ALLOY,
    AUDIO_ASSET_BGM_MAX,
    AUDIO_ASSET_SFX_PLACE,
};

std::vector<std::string> getMML(const AUDIO_ASSET_ID& assetID);
std::string getAudioAssetName(const AUDIO_ASSET_ID& assetID);