#include "MML.hpp"

#include <iostream>
#include <format>
#include <cmath>
#include <stack>
#include <map>

#include <src/hexManipulator.hpp>

#include <random>

std::random_device rd;
std::mt19937 mt = std::mt19937(rd());

namespace MML_Internal {
    int32_t getNumber(std::string::const_iterator &it, uint32_t maxValue, uint32_t& value, bool required, uint32_t defaultValue) {
        value = 0;
        if (std::isalpha(*it) && required) {
            return 1;
        }

        if (!std::isdigit(*it)) {
            value = defaultValue;
            return 0;
        }

        uint32_t offset = 0;
        while (std::isdigit(*(it + offset))) {
            uint32_t digit = *(it + offset) - '0';
            value = value * 10 + digit;
            if (value > maxValue) {
                return offset;
            }
            offset++;
        }

        it += offset;
        return 0;
    }
    int16_t generateSample(ChannelAttributes& channelAttributes) {
        auto& inst = channelAttributes.instrumentAttributes;
        double inc = (M_PI * 2) / ((double)channelAttributes.sampleRate / inst.frequency);
        auto& angle = inst.angle;
        angle = fmod(angle + inc, M_PI * 2);
        inst.currentSample++;
        return channelAttributes.instrumentGenerator(channelAttributes);
    }
    int16_t generateSine(ChannelAttributes& channelAttributes) {
        auto& angle = channelAttributes.instrumentAttributes.angle;
        double sampleNorm = sin(angle);
        return sampleNorm * (INT16_MAX) * channelAttributes.instrumentAttributes.volume; // sine
    }
    int16_t generateSquare(ChannelAttributes& channelAttributes) {
        auto& angle = channelAttributes.instrumentAttributes.angle;
        double duty = 0.125;
        if (channelAttributes.instMod % 4 > 0) {
            duty = 0.25 * channelAttributes.instMod;
        }
        return (angle > ((M_PI * 2) * duty) ? INT16_MAX : INT16_MIN) * channelAttributes.instrumentAttributes.volume;
    }
    int16_t generateTriangle(ChannelAttributes& channelAttributes) {
        // channelAttributes.instrumentAttributes.volume = channelAttributes.volumeRange;
        auto& angle = channelAttributes.instrumentAttributes.angle;
        double sampleNorm = (angle / (M_PI * 2)) * 2 - 1; // 0 - 1 -> 0 - 2 -> -1 - 1
        sampleNorm = (sampleNorm < 0 ? sampleNorm * 2 + 1 : -(sampleNorm * 2 - 1));
        int clamp = sampleNorm * 15;
        return (clamp / 15.0) * (INT16_MAX);
    }
    int16_t generateNoise(ChannelAttributes& channelAttributes) {
        return mt();
    }
}  // namespace MML_Internal

namespace MML {
    struct REPEATS {
        std::stack<std::string::const_iterator> repeatPositions;
        std::map<uint32_t, int32_t> repeatCounts;
        std::stack<std::string::const_iterator> repeatEndPos;
    };

    std::vector<std::vector<uint8_t>> MML_Renderer::render() const {
        std::vector<std::vector<std::vector<int32_t>>> insts(mml.size(), std::vector<std::vector<int32_t>>(isStereo ? 2 : 1));
        for (uint32_t inst = 0; inst < mml.size(); inst++) {
            insts[inst] = renderInstrument(inst);
        }
        
        auto track = compactInstruments(insts);

        scaleVolume(track);

        std::vector<std::vector<uint8_t>> stream(isStereo ? 2 : 1, std::vector<uint8_t>(0));
        pack(track, stream);

        return stream;
    }

    void MML_Renderer::setBitRate(int32_t bitRate) {
        maxBitValue = (int32_t)(pow(2, bitRate) / 2);
        this->bitRate = bitRate;
    }

    void MML_Renderer::pack(std::vector<std::vector<int32_t>> &track, std::vector<std::vector<uint8_t>> &stream) const {
        switch (bitRate) {
            case 16: {
                for (uint32_t s = 0; s < track[0].size(); s++) {
                    for (uint32_t c = 0; c < track.size(); c++) {
                        hexManipulator::packBytes((uint16_t)track[c][s], stream[c]);
                    }
                }
                break;
            }
            case 8: {
                for (uint32_t s = 0; s < track[0].size(); s++) {
                    for (uint32_t c = 0; c < track.size(); c++) {
                        hexManipulator::packBytes((uint8_t)(track[c][s] + 128), stream[c]);
                    }
                }
                break;
            }
        }
    }

    void MML_Renderer::scaleVolume(std::vector<std::vector<int32_t>> &track) const {
        for (auto &c : track) {
            for (auto &s : c) {
                s = std::clamp((int32_t)(s * renderSettings.volume), -(maxBitValue), (maxBitValue - 1));
            }
        }
        std::cout << "MML > scaled volume\n";
    }

    std::vector<std::vector<int32_t>> MML_Renderer::compactInstruments(std::vector<std::vector<std::vector<int32_t>>> &insts) const {
        uint32_t largestIndex = 0;
        for (uint32_t i = 0; i < insts.size(); i++) {
            if (i == largestIndex) continue;
            if (insts[i][0].size() > insts[largestIndex][0].size()) {
                largestIndex = i;
            }
        }
        auto& base = insts[largestIndex];
        for (uint32_t i = 0; i < insts.size(); i++) {
            if (i == largestIndex) continue;
            auto &sub = insts[i];
            for (uint32_t s = 0; s < sub[0].size(); s++) {
                for (uint32_t c = 0; c < base.size(); c++) {
                    base[c][s] = std::clamp(base[c][s] + sub[c][s], -(maxBitValue - 1), (maxBitValue - 1));
                }
            }
        }

        std::cout << "MML >> compacted instruments\n";
        return base;
    }

    std::vector<std::vector<int32_t>> MML_Renderer::renderInstrument(uint32_t inst) const {
        std::vector<std::vector<int32_t>> stream(isStereo ? 2 : 1, std::vector<int32_t>(0));
        REPEATS repeats;
        MML_Attributes attributes;
        attributes.tempo = renderSettings.globalTempo;
        auto begin = mml[inst].begin();
        auto end = mml[inst].end();
        auto it = mml[inst].begin();
        std::cout << "MML >> starting inst " << inst << " rendering\n";
        try {
            while (it != end) {
                int32_t error = 0;
                char c = std::tolower(*it);
                it++;
                switch (c) {
                    case MML_TEMPO_SYMBOL: {
                        error = MML_Internal::getNumber(it, MML_TEMPO_MAXVALUE, attributes.tempo);
                        break;
                    }
                    case MML_OCTAVE_SYMBOL: {
                        error = MML_Internal::getNumber(it, MML_OCTAVE_MAXVALUE, attributes.octave);
                        break;
                    }
                    case MML_OCTAVE_ADD_SYMBOL: {
                        attributes.octave = std::min(attributes.octave + 1, MML_OCTAVE_MAXVALUE);
                        break;
                    }
                    case MML_OCTAVE_SUB_SYMBOL: {
                        int32_t oct = attributes.octave;
                        attributes.octave = std::max(oct - 1, 0);
                        break;
                    }
                    case MML_LENGTH_SYMBOL: {
                        error = MML_Internal::getNumber(it, MML_LENGTH_MAXVALUE, attributes.length);
                        break;
                    }
                    case MML_VOLUME_SYMBOL: {
                        uint32_t volume;
                        error = MML_Internal::getNumber(it, maxVolume, volume);
                        attributes.volume = volume / (float)maxVolume;
                        break;
                    }
                    case MML_REPEAT_START_SYMBOL: {
                        repeats.repeatPositions.push(it);
                        break;
                    }
                    case MML_REPEAT_END_SYMBOL: {
                        DoRepeatEndSymbol(begin, it, repeats, end, error);
                        break;
                    }
                    default: {
                        if (!MML_VALID_CHARACTERS.contains(c)) break;
                        SampleNotes(c, attributes, it, error, stream);
                        break;
                    }
                }
                if (error != 0) {
                    throw std::runtime_error(std::format("MML >> invalid character at {}\n", (std::distance(begin, it) + error)));
                }
            }
        }
        catch (std::exception &e) {
            std::cout << e.what() << '\n';
        }
        return stream;
    }

    void MML_Renderer::SampleNotes(char c, MML::MML_Attributes &attributes, std::string::const_iterator &it, int32_t &error, std::vector<std::vector<int32_t>> &stream) const {
        bool isNote = (c >= 'a' && c <= 'g');
        bool isRest = c == MML_NOTE_REST_SYMBOL;
        if (isNote || isRest) {
            std::vector<std::vector<int32_t>> soundData;
            if (isNote) {
                soundData = readNote(c, attributes, it, error);
            } else {
                soundData = readRest(attributes, it, error);
            }
            if (isStereo) {
                stream[0].insert(std::end(stream[0]), std::begin(soundData[0]), std::end(soundData[0]));
                stream[1].insert(std::end(stream[1]), std::begin(soundData[1]), std::end(soundData[1]));
            } else {
                stream[0].insert(std::end(stream[0]), std::begin(soundData[0]), std::end(soundData[0]));
            }
        }
    }

    void MML_Renderer::DoRepeatEndSymbol(const std::string::const_iterator &begin, std::string::const_iterator &it, REPEATS &repeats, std::string::const_iterator &end, int32_t &error) const {
        uint32_t pos = std::distance(begin, it);
        if (!repeats.repeatCounts.contains(pos)) {
            auto ite = it;
            uint32_t repeatCount;
            if (it != end) {
                error = MML_Internal::getNumber(it, MML_REPEAT_MAXVALUE, repeatCount, false);
            } else {
                repeatCount = 2;
            }
            repeats.repeatCounts.emplace(pos, repeatCount);
            repeats.repeatEndPos.push(it);
            it = ite;
        } else if (repeats.repeatCounts[pos] == 0) {
            auto ite = it;
            uint32_t repeatCount;
            if (it != end) {
                error = MML_Internal::getNumber(it, MML_REPEAT_MAXVALUE, repeatCount, false);
            } else {
                repeatCount = 2;
            }
            repeats.repeatCounts[pos] = repeatCount;
            repeats.repeatEndPos.push(it);
            it = ite;
        }
        if (repeats.repeatCounts[pos] > 1) {
            it = repeats.repeatPositions.top();
            if (repeats.repeatCounts[pos] == 2) {
                repeats.repeatPositions.pop();
            }
            repeats.repeatCounts[pos]--;
            return;
        }
        if (repeats.repeatCounts[pos] == 1) {
            repeats.repeatCounts[pos]--;
            it = repeats.repeatEndPos.top();
            repeats.repeatEndPos.pop();
            return;
        }
    }

    std::vector<std::vector<int32_t>> MML_Renderer::readNote(char c, MML_Attributes& attributes, std::string::const_iterator &it, int32_t &error) const {
        int32_t noteOffset = MML_NOTE_OFFSET[(c - 'a')];
        double wholeTempo = attributes.tempo / 4.0;
        bool loop = true;
        while (loop) {
            char c = *it;
            switch (c) {
                case MML_NOTE_ADD_SYMBOL: {
                    it++;
                    noteOffset++;
                    break;
                }
                case MML_NOTE_SUB_SYMBOL: {
                    it++;
                    noteOffset--;
                    break;
                }
                default: {
                    loop = false;
                    break;
                }
            }
        }
        uint32_t length = attributes.length;
        if (std::isdigit(*it)) {
            error = MML_Internal::getNumber(it, MML_LENGTH_MAXVALUE, length);
        }
        double dur = (sampleRate * 60) / (wholeTempo * (length == 0 ? 0.5 : length));
        while (*it == MML_NOTE_DOT_SYMBOL) {
            it++;
            dur *= 1.5;
        }
        double frequency = MML_A0 * pow(2, attributes.octave) * pow(2, (noteOffset + renderSettings.transpose + (renderSettings.fineTune/100.0)) / 12.0f);
        std::vector<std::vector<int32_t>> soundData(isStereo ? 2 : 1);

        if (isStereo) {
            createSoundData(frequency, attributes, dur, soundData[0], -1);
            createSoundData(frequency, attributes, dur, soundData[1], 1);
        } else {
            createSoundData(frequency, attributes, dur, soundData[0]);
        }

        return soundData;
    }

    std::vector<std::vector<int32_t>> MML_Renderer::readRest(const MML_Attributes& attributes, std::string::const_iterator &it, int32_t &error) const {
        double wholeTempo = attributes.tempo / 4.0;
        uint32_t length = attributes.length;
        if (std::isdigit(*it)) {
            error = MML_Internal::getNumber(it, MML_LENGTH_MAXVALUE, length);
        }
        double dur = (sampleRate * 60) / (wholeTempo * (length == 0 ? 0.5 : length));
        while (*it == MML_NOTE_DOT_SYMBOL) {
            it++;
            dur *= 1.5;
        }
        std::vector<std::vector<int32_t>> soundData(isStereo ? 2 : 1, std::vector<int32_t>(((uint32_t)dur), 0));

        return soundData;
    }

    void MML_Renderer::createSoundData(double frequency, MML::MML_Attributes &attributes, double dur, std::vector<int32_t> &soundData, int32_t stereoPan) const {
        double inc = (M_PI * 2) / (sampleRate / frequency);
        for (uint32_t d = 0; d < dur; d++) {
            attributes.angle = fmod(attributes.angle + inc, M_PI * 2);
            double sampleNorm = sin(attributes.angle);

            int16_t sample = (sampleNorm >= 0 ? 1 : -1) * ((maxBitValue / 2) - 1) * (stereoPan == 0 ? attributes.volume : stereoPan < 1 ? attributes.volume * attributes.pan : attributes.volume * (1 - attributes.pan));
            // int16_t sample = (1.0 / (mt() % 16)) * ((maxBitValue / 2) - 1) * attributes.volume; // noise
            // sample *= std::max((float)(dur - d) / dur, 0.5);
            // int16_t sample = sampleNorm * ((maxBitValue / 2) - 1) * attributes.volume; // sine
            soundData.push_back(sample);
        }
    }
} // MML