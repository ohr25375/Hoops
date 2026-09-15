#pragma once

#include <map>
#include "src/MML.hpp"
#include <stack>

namespace MML_LiveAudio_Internal {

    class ChannelRenderer {
    public:
        void insertMML(const std::string& mml);
        std::vector<int16_t> getSamples(const int sampleCount);
        std::vector<int16_t> readAll();
        int16_t getSingleSample();
        MML_Internal::ChannelAttributes getCurrentInfo();
        void clearChannelAttributes();
    private:
        MML_Internal::ChannelAttributes channelAttributes;
        bool readNext();
    };
}  // namespace MML_LiveAudio_Internal

namespace MML_LiveAudio {
    class LiveAudioHandler {
    public:
        float masterVolume = 0.1;
        void clearMML();
        void loadNewMML(const std::vector<std::string>& mml);
        void loadNewMML(const std::string& mml);
        std::vector<int16_t> getSamples(const int sampleCount);
        std::vector<MML_Internal::ChannelAttributes> getCurrentInfos();
        std::vector<int16_t> readAll();
    private:
        std::vector<MML_LiveAudio_Internal::ChannelRenderer> channelRenderers;
    };
}  // namespace MML_LiveAudio