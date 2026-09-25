#pragma once

#include <vector>
#include <string>

enum struct SFX_ASSET_ID {
    PLACE,
    BLIP,
    SELECT,
};

std::vector<std::string> getMML(const SFX_ASSET_ID& assetID);