#pragma once

#include <vector>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include "RBIC.hpp"
#include "image.hpp"

namespace RBICReader {
    int readRBIC(std::filesystem::path path, RBIC::RBIC& out);

    int readFILETYPE(std::ifstream& dataStream, RBIC::RBIC& out);
    int readANIM(std::ifstream& dataStream, std::shared_ptr<RBIC::ANIM> anim);
    int readNTYP(std::ifstream& dataStream, std::shared_ptr<RBIC::UNKNOWN_FILETYPE> ntyp);
    
    int readDATA(std::ifstream& dataStream, std::shared_ptr<RBIC::DATA> data, std::shared_ptr<RBIC::ANIM> anim);
    int readSEQ(std::ifstream& dataStream, std::shared_ptr<RBIC::SEQ_> seq, std::shared_ptr<RBIC::ANIM> anim);
    int readRATE(std::ifstream& dataStream, std::shared_ptr<RBIC::RATE> rate, std::shared_ptr<RBIC::ANIM> anim);
    int readBIMG(std::ifstream& dataStream, std::shared_ptr<RBIC::BIMG> bimg);

    image::animatedImage getUniversalAnimatedImage(const RBIC::RBIC& rbic);

    void printRBIC(const RBIC::RBIC& rbic);
    void printANIM(const std::shared_ptr<RBIC::ANIM> anim);
    void printNTYP(const std::shared_ptr<RBIC::UNKNOWN_FILETYPE> ntyp);
    void printDATA(const std::shared_ptr<RBIC::DATA> data);
    void printSEQ(const std::shared_ptr<RBIC::SEQ_> seq);
    void printBIMG(const std::shared_ptr<RBIC::BIMG> bimg);
}