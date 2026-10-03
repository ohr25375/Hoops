#pragma once

#include <vector>
#include <string>

enum struct SFX_ASSET_ID {
    PLACE,
    BLIP,
    SELECT,
    CHAIN,
    SPIN,
    MOVE,
    HARD_DROP,
    GAME_OVER
};

std::vector<std::string> getMML(const SFX_ASSET_ID& assetID);