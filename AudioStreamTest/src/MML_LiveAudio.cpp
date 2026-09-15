#include "MML_LiveAudio.hpp"

#include <cmath>
#include <iostream>

namespace MML_LiveAudio_Internal {
    void ChannelRenderer::insertMML(const std::string& mml) {
        channelAttributes.mml          = mml;
        channelAttributes.currentIndex = channelAttributes.mml.begin();
        channelAttributes.repeats.clear();
        channelAttributes.repeatStarts.clear();
    }
    std::vector<int16_t> ChannelRenderer::getSamples(const int sampleCount) {
        std::vector<int16_t> out(sampleCount);
        for (int i = 0; i < sampleCount; i++) {
            out[i] = getSingleSample();
        }
        return out;
    }
    std::vector<int16_t> ChannelRenderer::readAll(bool& clear) {
        std::vector<int16_t> out(500000);
        auto& inst = channelAttributes.instrumentAttributes;
        int sampleSize = 0;
        while (true) {
            if (clear) break;
            if (inst.currentSample >= inst.expectedSampleCount) {
                channelAttributes.instrumentAttributes.currentSample = 0;
                if (readNext()) break;
            }
            int16_t sample = MML_Internal::generateSample(channelAttributes);
            out[sampleSize] = sample;
            sampleSize++;
            if (sampleSize >= out.size()) break;
        }
        return std::vector<int16_t>(out.begin(), out.begin() + sampleSize);
    }
    int16_t ChannelRenderer::getSingleSample() {
        auto& inst = channelAttributes.instrumentAttributes;
        if (inst.currentSample >= inst.expectedSampleCount) {
            channelAttributes.instrumentAttributes.currentSample = 0;
            readNext();
        }
        return MML_Internal::generateSample(channelAttributes);
    }
    MML_Internal::ChannelAttributes ChannelRenderer::getCurrentInfo() {
        return channelAttributes;
    }
    void ChannelRenderer::clearChannelAttributes() {
        channelAttributes = {};
    }
    bool ChannelRenderer::readNext() {
        bool isReading = true;
        bool isEnd = false;
        auto& index    = channelAttributes.currentIndex;
        while (isReading) {
            if (index == channelAttributes.mml.end()) {
                isEnd = true;
                index = channelAttributes.mml.begin();
            }
            char c = std::tolower(*index++);
            switch (c) {
                case MML::MML_TEMPO_SYMBOL: {
                    uint32_t out;
                    MML_Internal::getNumber(index, MML::MML_TEMPO_MAXVALUE, out);
                    channelAttributes.tempo = out;
                    break;
                }
                case MML::MML_OCTAVE_SYMBOL: {
                    uint32_t out;
                    MML_Internal::getNumber(index, MML::MML_OCTAVE_MAXVALUE, out);
                    channelAttributes.octave = out;
                    break;
                }
                case MML::MML_OCTAVE_ADD_SYMBOL: {
                    channelAttributes.octave = std::min((uint32_t)(++channelAttributes.octave), MML::MML_OCTAVE_MAXVALUE);
                    break;
                }
                case MML::MML_OCTAVE_SUB_SYMBOL: {
                    int32_t out              = channelAttributes.octave;
                    channelAttributes.octave = std::max(out - 1, 0);
                    break;
                }
                case MML::MML_LENGTH_SYMBOL: {
                    uint32_t out;
                    MML_Internal::getNumber(index, MML::MML_LENGTH_MAXVALUE, out);
                    channelAttributes.length = out;
                    break;
                }
                case MML::MML_VOLUME_SYMBOL: {
                    uint32_t volume;
                    MML_Internal::getNumber(index, channelAttributes.volumeRange, volume);
                    channelAttributes.volume = volume;
                    break;
                }
                case MML::MML_REPEAT_START_SYMBOL: {
                    channelAttributes.repeatStarts.push_front(index);
                    break;
                }
                case MML::MML_INSTRUMENT_SYMBOL: {
                    uint32_t val;
                    MML_Internal::getNumber(index, MML::MML_INSTRUMENT_MAXVALUE, val);
                    switch (val) {
                        case 0: channelAttributes.instrumentGenerator = MML_Internal::generateSquare; break;
                        case 1: channelAttributes.instrumentGenerator = MML_Internal::generateTriangle; break;
                        case 2: channelAttributes.instrumentGenerator = MML_Internal::generateNoise; break;
                    }
                    break;
                }
                case MML::MML_INSTRUMENTMOD_SYMBOL: {
                    uint32_t val;
                    MML_Internal::getNumber(index, MML::MML_INSTRUMENTMOD_MAXVALUE, val);
                    channelAttributes.instMod = val;
                    break;
                }
                case MML::MML_FINEPITCH_SYMBOL: {
                    uint32_t val;
                    MML_Internal::getNumber(index, MML::MML_FINEPITCH_MAXVALUE, val);
                    channelAttributes.fineTune = val;
                    break;
                }
                case MML::MML_REPEAT_END_SYMBOL: {
                    uint32_t count;
                    MML_Internal::getNumber(index, MML::MML_REPEAT_MAXVALUE, count, false, 2);
                    int32_t num   = count;
                    auto& repeats = channelAttributes.repeats;
                    auto ind      = index;
                    if (!repeats.contains(index)) {
                        repeats.emplace(index, num - 1);
                    }
                    if (repeats[index] == 0) {
                        repeats.erase(index);
                        break;
                    }
                    --repeats[index];
                    index = channelAttributes.repeatStarts.front();
                    if (repeats[ind] <= 0) {
                        channelAttributes.repeatStarts.pop_front();
                    }
                    break;
                }
                default: {
                    if (!MML::MML_VALID_CHARACTERS.contains(c)) break;
                    channelAttributes.note        = c;
                    channelAttributes.noteFreqMod = 0;
                    bool loop                     = true;
                    int32_t noteOffset            = 0;
                    double realVolume             = 0;

                    if (c != 'r') {
                        noteOffset = MML::MML_NOTE_OFFSET[(c - 'a')];
                        realVolume = channelAttributes.volume / (float)channelAttributes.volumeRange;
                    }
                    channelAttributes.instrumentAttributes.baseVolume = realVolume;
                    channelAttributes.instrumentAttributes.volume     = realVolume;
                    // get +- symbols
                    while (loop) {
                        c = std::tolower(*index++);
                        switch (c) {
                            case MML::MML_NOTE_ADD_SYMBOL: {
                                channelAttributes.noteFreqMod++;
                                noteOffset++;
                                break;
                            }
                            case MML::MML_NOTE_SUB_SYMBOL: {
                                channelAttributes.noteFreqMod--;
                                noteOffset--;
                                break;
                            }
                            default: {
                                index--;
                                loop = false;
                                break;
                            }
                        }
                    }
                    // get length symbol
                    channelAttributes.noteLength = channelAttributes.length;
                    if (std::isdigit(c)) {
                        uint32_t out;
                        MML_Internal::getNumber(index, MML::MML_LENGTH_MAXVALUE, out);
                        channelAttributes.noteLength = out;
                    }
                    float samplesInMinute      = (channelAttributes.sampleRate * 60);
                    float wholeTempo           = (channelAttributes.tempo / 4.0);
                    float realNoteLength       = (channelAttributes.noteLength == 0 ? 0.5 : channelAttributes.noteLength);
                    double expectedSampleCount = samplesInMinute / (wholeTempo * realNoteLength);
                    while (*index == MML::MML_NOTE_DOT_SYMBOL) {
                        index++;
                        expectedSampleCount *= 1.5;
                    }
                    double frequency = MML::MML_A0 * pow(2, channelAttributes.octave) * pow(2, (noteOffset + channelAttributes.transpose + ((channelAttributes.fineTune - 0x80) / 100.0)) / 12.0f);

                    channelAttributes.instrumentAttributes.baseFrequency       = frequency;
                    channelAttributes.instrumentAttributes.frequency           = frequency;
                    channelAttributes.instrumentAttributes.expectedSampleCount = expectedSampleCount;
                    isReading                                                  = false;
                    break;
                }
            }
        }
        return isEnd;
    }
}  // namespace MML_LiveAudio_Internal

namespace MML_LiveAudio {
    void LiveAudioHandler::clearMML() {
        channelRenderers = std::vector<MML_LiveAudio_Internal::ChannelRenderer>(0);
    }

    void LiveAudioHandler::loadNewMML(const std::vector<std::string>& mml) {
        int channelCount = mml.size();
        channelRenderers = std::vector<MML_LiveAudio_Internal::ChannelRenderer>(channelCount);
        for (int i = 0; i < channelCount; i++) {
            channelRenderers[i].insertMML(mml[i]);
        }
    }
    void LiveAudioHandler::loadNewMML(const std::string& mml) {
        channelRenderers = std::vector<MML_LiveAudio_Internal::ChannelRenderer>(1, MML_LiveAudio_Internal::ChannelRenderer{});
        channelRenderers[0].insertMML(mml);
    }
    std::vector<int16_t> LiveAudioHandler::getSamples(const int sampleCount) {
        int channelCount = channelRenderers.size();
        std::vector<int16_t> out(sampleCount);
        for (int s = 0; s < sampleCount; s++) {
            if (this->queueClear) {
                out[s] = 0;
                continue;
            }
            float val = 0;
            for (int c = 0; c < channelCount; c++) {
                val += channelRenderers[c].getSingleSample() / (float)channelCount;
            }
            out[s] = (int16_t)val * masterVolume;
        }
        this->queueClear = false;
        return out;
    }
    std::vector<int16_t> LiveAudioHandler::readAll() {
        int channelCount = channelRenderers.size();
        std::vector<std::vector<int16_t>> samples(channelCount);
        size_t sampleSize = 0;
        std::vector<int16_t> out(sampleSize);
        if (this->queueClear) {
            this->queueClear = false;
            return out;
        }
        for (auto c = 0; c < channelCount; c++) {
            samples[c] = channelRenderers[c].readAll(this->queueClear);
            if (sampleSize < samples[c].size()) {
                sampleSize = samples[c].size();
            }
        }
        out.resize(sampleSize);
        for (auto s = 0; s < sampleSize; s++) {
            float val = 0;
            for (int c = 0; c < channelCount; c++) {
                val += samples[c][s] / (float)channelCount;
            }
            out[s] = (int16_t)val * masterVolume;
        }
        return out;
    }
    std::vector<MML_Internal::ChannelAttributes> LiveAudioHandler::getCurrentInfos() {
        std::vector<MML_Internal::ChannelAttributes> out(channelRenderers.size());
        for (int i = 0; i < channelRenderers.size(); i++) {
            out[i] = channelRenderers[i].getCurrentInfo();
        }
        return out;
    }
}  // namespace MML_LiveAudio