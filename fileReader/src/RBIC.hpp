#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <memory>

namespace RBIC {
    struct CHUNK;
    struct FILETYPE;
    struct DATACHUNK;

    /// @brief Raw Binary Image Container : RBIC
    /// @param header const = "RBIC"
    /// @param dataSize file data size - 8, calculate with recalculateDataSize()
    struct RBIC {
        const std::array<uint8_t,4> header = {'R', 'B', 'I', 'C'};
        uint32_t dataSize;
        std::shared_ptr<FILETYPE> fileType;
        std::vector<std::shared_ptr<CHUNK>> file;
        std::vector<uint8_t> unreadData;
    };

    struct FILETYPE {
        const std::array<uint8_t,4> header;
        uint32_t dataSize;

        FILETYPE(std::array<uint8_t,4> header);
        virtual ~FILETYPE() = default;
    };
    
    /// @brief Image File type
    /// @param header const = "IMAG"
    /// @param dataSize header data size = 4
    /// @param version RBIC version
    struct IMAG : public FILETYPE {
        uint32_t version;
        
        IMAG();
    };
    
    /// @brief Animated image file
    /// @param header const = "ANIM"
    /// @param dataSize header data size = 12
    /// @param frameCount frame count
    /// @param stepCount step count
    struct ANIM : public FILETYPE {
        uint32_t version;
        uint32_t frameCount;
        uint32_t stepCount;
        
        ANIM();
    };

    /// @brief Unknown file type
    /// @param header const = "NTYP"
    /// @param dataSize unknown data size
    /// @param unknownData unknown data
    struct UNKNOWN_FILETYPE : public FILETYPE {
        std::vector<uint8_t> unknownData;

        UNKNOWN_FILETYPE(std::array<uint8_t, 4> header);
    };

    struct CHUNK {
        const std::array<uint8_t,4> header;
        uint32_t dataSize;

        CHUNK(std::array<uint8_t,4> header);
        virtual ~CHUNK() = default;
    };
    
    
    struct DATACHUNK {
        const std::array<uint8_t,4> header;
        uint32_t dataSize;

        DATACHUNK(std::array<uint8_t,4> header);
        virtual ~DATACHUNK() = default;
    };

    struct DATA : public CHUNK {
        std::vector<std::shared_ptr<DATACHUNK>> data;
        std::vector<uint8_t> unreadData;

        DATA();
    };

    /// @brief Sequence
    /// @param header const = "SEQ " notice space
    /// @param dataSize Step count x 4
    struct SEQ_ : public DATACHUNK {
        std::vector<uint32_t> sequence;

        SEQ_();
    };

    /// @brief Sequence
    /// @param header const = "SEQ " notice space
    /// @param dataSize Step count x 4
    struct RATE : public DATACHUNK {
        std::vector<uint32_t> rate;

        RATE();
    };

    /// @brief Bitmap image
    /// @details color is RGBA32
    /// @details left to right, top to bottom
    /// @param header const = "BIMG"
    /// @param dataSize width x height x 4 + 8
    /// @param width image width
    /// @param height image height
    /// @param colorData image data
    struct BIMG : public DATACHUNK {
        uint32_t width;
        uint32_t height;
        std::vector<uint32_t> colorData;

        BIMG();
    };

    void recalculateDataSize(RBIC& rbic);
    void recalculateDataSize(std::shared_ptr<FILETYPE> filetype);
    void recalculateDataSize(std::shared_ptr<BIMG> bimg);

    uint32_t headerToID(std::array<uint8_t, 4> header);
    std::array<uint8_t, 4> idToHeader(uint32_t id);
}