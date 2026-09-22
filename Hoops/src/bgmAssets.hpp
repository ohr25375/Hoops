#pragma once

#include <vector>
#include <string>

enum struct AUDIO_ASSET_ID {
    NONE,
    RINGS,
    ALLOY,
    MAX,
};

std::vector<std::string> getMML(const AUDIO_ASSET_ID& assetID);
std::string getAudioAssetName(const AUDIO_ASSET_ID& assetID);