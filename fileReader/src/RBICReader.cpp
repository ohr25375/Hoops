#include "RBICReader.hpp"

#include <iostream>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string_view>

#include "binaryReader.hpp"
#include "hexManip.hpp"


#define endl '\n'
#define all(n) n.begin(),n.end()
#define allA(n) std::begin(n),std::end(n)

namespace RBICReader {
    const uint32_t ANIM = 0x4D494E41;
    const uint32_t NTYP = 0x5059544E;
    const uint32_t DATA = 0x41544144;
    const uint32_t SEQ_ = 0x20716573;
    const uint32_t RATE = 0x65746172;

    int readRBIC(std::filesystem::path path, RBIC::RBIC& out) {
        // Raw Bitmap Image Container
        // "RBIC" < must be at the start
        //  4byte dataWidth (whole data width in bytes - 4)
        //  ???byte data
        std::cout << "RBICReader >> opening file" << endl;

        std::ifstream data(path, std::ios::binary);

        std::array<uint8_t,4> fileType;
        data.read((char*)&(fileType[0]), sizeof(uint8_t) * 4);
        
        if (strncmp((const char*)fileType.data(),"RBIC", 4) != 0) {
            std::cout << "File not recognized" << endl;
            return 1;
        }
        std::cout << "RBICReader >> RBIC File found" << endl;
        data.read(reinterpret_cast<char*>(&(out.dataSize)), sizeof(out.dataSize));

        readFILETYPE(data, out);

        data.close();
        
        return 0;
    }

    int readFILETYPE(std::ifstream& dataStream, RBIC::RBIC &out)
    {
        uint32_t fileID;
        dataStream.read(reinterpret_cast<char*>(&fileID), sizeof(uint32_t));
        switch (fileID) {
            case ANIM: {
                std::cout << "RBICReader >> found ANIM\n";
                std::shared_ptr<RBIC::ANIM> anim = std::make_shared<RBIC::ANIM>();
                out.fileType = anim;
                readANIM(dataStream, anim);
                break;
            }
            default: {
                std::cout << "RBICReader >> found NTYP\n";
                std::shared_ptr<RBIC::UNKNOWN_FILETYPE> ntyp = std::make_shared<RBIC::UNKNOWN_FILETYPE>(RBIC::idToHeader(fileID));
                out.fileType = ntyp;
                readNTYP(dataStream, ntyp);
                break;
            }
        }

        while (dataStream.tellg() < out.dataSize + 4) {
            uint32_t fileID;
            dataStream.read(reinterpret_cast<char*>(&fileID), sizeof(uint32_t));
            switch(fileID) {
                case DATA: {
                    std::cout << "RBICReader >> found DATA\n";
                    std::shared_ptr<RBIC::DATA> dataHeader = std::make_shared<RBIC::DATA>();
                    out.file.push_back(dataHeader);
                    readDATA(dataStream, dataHeader, std::dynamic_pointer_cast<RBIC::ANIM>(out.fileType));
                    break;
                }
                default: {
                    std::cout << "RBICReader >> found UNREADABLE : " << hexManip::toHex(fileID, 8) << '\n';
                    out.unreadData = std::vector<uint8_t>(
                        (std::istreambuf_iterator<char>(dataStream)),
                        (std::istreambuf_iterator<char>())
                    );
                }
            }
        }

        return 0;
    }

    int readANIM(std::ifstream& dataStream, std::shared_ptr<RBIC::ANIM> anim) {
        dataStream.read(reinterpret_cast<char*>(&(anim->dataSize)), sizeof(anim->dataSize));
        dataStream.read(reinterpret_cast<char*>(&(anim->version)), sizeof(anim->version));
        dataStream.read(reinterpret_cast<char*>(&(anim->frameCount)), sizeof(anim->frameCount));
        dataStream.read(reinterpret_cast<char*>(&(anim->stepCount)), sizeof(anim->stepCount));
        return 0;
    }

    int readNTYP(std::ifstream &dataStream, std::shared_ptr<RBIC::UNKNOWN_FILETYPE> ntyp)
    {
        dataStream.read(reinterpret_cast<char*>(&(ntyp->dataSize)), sizeof(ntyp->dataSize));
        dataStream.read(reinterpret_cast<char*>(&(ntyp->unknownData[0])), sizeof(uint8_t) * ntyp->dataSize);
        return 0;
    }

    int readDATA(std::ifstream &dataStream, std::shared_ptr<RBIC::DATA> data, std::shared_ptr<RBIC::ANIM> anim) {
        dataStream.read(reinterpret_cast<char*>(&(data->dataSize)), sizeof(data->dataSize));

        auto dataPos = dataStream.tellg();
        dataStream.seekg(data->dataSize, std::ifstream::cur);
        auto endPos = dataStream.tellg();
        dataStream.seekg(dataPos);
        while (dataStream.tellg() < endPos) {
            auto oldPos = dataStream.tellg();
            uint32_t dataChunkID;
            dataStream.read(reinterpret_cast<char*>(&dataChunkID), sizeof(uint32_t));
            dataStream.seekg(oldPos);

            switch (dataChunkID) {
                case SEQ_: {
                    std::cout << "RBICReader >> found SEQ_\n";
                    std::shared_ptr<RBIC::SEQ_> seq = std::make_shared<RBIC::SEQ_>();
                    data->data.push_back(seq);
                    readSEQ(dataStream, seq, anim);
                    break;
                }
                case 0x676D6962: {
                    std::cout << "RBICReader >> found BIMG\n";
                    std::shared_ptr<RBIC::BIMG> bimg = std::make_shared<RBIC::BIMG>();
                    data->data.push_back(bimg);
                    readBIMG(dataStream, bimg);
                    break;
                }
                case RATE: {
                    std::cout << "RBICReader >> found RATE\n";
                    std::shared_ptr<RBIC::RATE> rate = std::make_shared<RBIC::RATE>();
                    data->data.push_back(rate);
                    readRATE(dataStream, rate, anim);
                    break;
                }
                default: {
                    std::cout << "RBICReader >> found UNREADABLE : " << hexManip::toHex(dataChunkID, 8) << '\n';
                    auto remainingDataSize = endPos - dataStream.tellg();
                    data->unreadData = std::vector<uint8_t>(remainingDataSize);
                    dataStream.read((char*)&(data->unreadData[0]), sizeof(uint8_t) * remainingDataSize);
                }
            }
        }

        return 0;
    }

    int readSEQ(std::ifstream &dataStream, std::shared_ptr<RBIC::SEQ_> seq, std::shared_ptr<RBIC::ANIM> anim) {
        dataStream.seekg(4, std::ios_base::cur);
        dataStream.read(reinterpret_cast<char*>(&(seq->dataSize)), sizeof(seq->dataSize));
        std::cout << "RBICREADER >> dataSize : " << hexManip::toHex(seq->dataSize, 8) << '\n';

        seq->sequence = std::vector<uint32_t>(anim->stepCount);
        dataStream.read((char*)&(seq->sequence[0]), sizeof(uint32_t) * anim->stepCount);

        return 0;
    }

    int readRATE(std::ifstream& dataStream, std::shared_ptr<RBIC::RATE> rate, std::shared_ptr<RBIC::ANIM> anim) {
        dataStream.seekg(4, std::ios_base::cur);
        dataStream.read(reinterpret_cast<char*>(&(rate->dataSize)), sizeof(rate->dataSize));
        std::cout << "RBICREADER >> dataSize : " << hexManip::toHex(rate->dataSize, 8) << '\n';

        rate->rate = std::vector<uint32_t>(anim->stepCount);
        dataStream.read((char*)&(rate->rate[0]), sizeof(uint32_t) * anim->stepCount);

        return 0;
    }

    int readBIMG(std::ifstream &dataStream, std::shared_ptr<RBIC::BIMG> bimg) {
        dataStream.seekg(4, std::ios_base::cur);
        dataStream.read(reinterpret_cast<char*>(&(bimg->dataSize)), sizeof(bimg->dataSize));
        std::cout << "RBICREADER >> dataSize : " << hexManip::toHex(bimg->dataSize, 8) << '\n';

        dataStream.read(reinterpret_cast<char*>(&(bimg->width)), sizeof(bimg->width));
        dataStream.read(reinterpret_cast<char*>(&(bimg->height)), sizeof(bimg->height));

        
        uint32_t pixelCount = bimg->width * bimg->height;
        bimg->colorData = std::vector<uint32_t>(pixelCount);
        dataStream.read(reinterpret_cast<char*>(&(bimg->colorData[0])), sizeof(uint32_t) * pixelCount);
        return 0;
    }

    image::animatedImage getUniversalAnimatedImage(const RBIC::RBIC &rbic) {
        std::cout << "CONVERTER >> creating UAI" << endl;
        image::animatedImage res;
        std::shared_ptr<RBIC::ANIM> anim;

        if (std::strncmp((const char*)(rbic.fileType->header.data()), "ANIM", 4) != 0) {
            std::cout << "CONVERTER >> File type is not ANIM" << endl;
        }

        anim = std::dynamic_pointer_cast<RBIC::ANIM>(rbic.fileType);

        std::shared_ptr<RBIC::DATA> data;
        int fileSize = (int)(rbic.file.size());
        std::cout << "CONVERTER >> found " << fileSize << " headers" << endl;
        for (int i = 0; i < fileSize; i++) {
            if (std::strncmp((const char*)(rbic.file[i]->header.data()), "DATA", 4) == 0) {
                std::cout << "CONVERTER >> Found DATA" << endl;
                data = std::dynamic_pointer_cast<RBIC::DATA>(rbic.file[i]);
                continue;
            }
        }
        res.frames = std::vector<image::image>(anim->frameCount);

        std::cout << "CONVERTER >> Grabbing Images" << endl;
        auto it = res.frames.begin();
        for (int i = 0; i < (int)data->data.size(); i++) {
            if (std::strncmp((const char*)(data->data[i]->header.data()), "rate", 4) == 0) {
                std::cout << "CONVERTER >> Found rate" << endl;
                res.rate = (std::dynamic_pointer_cast<RBIC::RATE>(data->data[i]))->rate;
                continue;
            }
            if (std::strncmp((const char*)(data->data[i]->header.data()), "seq ", 4) == 0) {
                std::cout << "CONVERTER >> Found seq " << endl;
                res.frameOrder = (std::dynamic_pointer_cast<RBIC::SEQ_>(data->data[i]))->sequence;
                continue;
            }
            if (std::strncmp((const char*)(data->data[i]->header.data()), "bimg", 4) == 0) {
                std::cout << "CONVERTER >> Found bimg" << endl;
                RBIC::BIMG* bimg = static_cast<RBIC::BIMG*>(data->data[i].get());
                it->width = bimg->width;
                it->height = bimg->height;
                int size = (int)bimg->colorData.size();
                it->pixelData = std::vector<COLOR::COLOR>(size);
                std::cout << "CONVERTER >> Set dimensions" << endl;
                std::cout << "CONVERTER >> Found " << size << "pixels" << endl;

                for (int j = 0; j < size; j++) {
                    COLOR::COLOR c;
                    c.a = (bimg->colorData[j] >> 0) & 0xff;
                    c.b = (bimg->colorData[j] >> 8) & 0xff;
                    c.g = (bimg->colorData[j] >> 16) & 0xff;
                    c.r = (bimg->colorData[j] >> 24) & 0xff;
                    it->pixelData[j] = c;
                }
                std::advance(it, 1);
                continue;
            }
        }
        std::cout << "CONVERTER >> Finishing..." << endl;

        return res;
    }

    void printRBIC(const RBIC::RBIC& rbic) {
        std::string header(all(rbic.header));
        std::cout << "Header      : " << header << endl;
        std::cout << "File Size   : " << rbic.dataSize << endl;

        uint32_t fileType = RBIC::headerToID(rbic.fileType->header);
        switch (fileType) {
            case ANIM: {
                printANIM(std::dynamic_pointer_cast<RBIC::ANIM>(rbic.fileType));
                break;
            }
            case NTYP: {

            }
        }

        for (int i = 0; i < (int)rbic.file.size(); i++) {
            uint32_t fileID = RBIC::headerToID(rbic.file[i]->header);
            switch (fileID) {
                case DATA: {
                    printDATA(std::dynamic_pointer_cast<RBIC::DATA>(rbic.file[i]));
                    break;
                }
            }
        }

        binaryReader::printBytes(rbic.unreadData);
    }

    void printANIM(const std::shared_ptr<RBIC::ANIM> anim) {
        std::string fileType(all(anim->header));
        std::cout << "File Type   : " << fileType << endl;
        std::cout << "Header size : " << anim->dataSize << endl;
        std::cout << "Version     : " << anim->version << endl;
        std::cout << "Frame Count : " << anim->frameCount << endl;
        std::cout << "Step  Count : " << anim->stepCount << endl;
    }

    void printNTYP(const std::shared_ptr<RBIC::UNKNOWN_FILETYPE> ntyp) {
        std::string fileType(all(ntyp->header));
        std::cout << "File Type   : " << fileType << endl;
        std::cout << "Header size : " << ntyp->dataSize << endl;
        binaryReader::printBytes(ntyp->unknownData);
    }

    void printDATA(const std::shared_ptr<RBIC::DATA> data) {
        std::string fileType(all(data->header));
        std::cout << "Chunk Type  : " << fileType << endl;
        std::cout << "Header size : " << data->dataSize << endl;
        for (int i = 0; i < (int)data->data.size(); i++) {
            uint32_t dataID = RBIC::headerToID(data->data[i]->header);
            switch (dataID) {
                case 0x20716573: {
                    printSEQ(std::dynamic_pointer_cast<RBIC::SEQ_>(data->data[i]));
                    break;
                }
                case 0x676D6962: {
                    printBIMG(std::dynamic_pointer_cast<RBIC::BIMG>(data->data[i]));
                    break;
                }
            }
        }

        binaryReader::printBytes(data->unreadData);
    }

    void printSEQ(const std::shared_ptr<RBIC::SEQ_> seq) {
        std::string fileType(all(seq->header));
        std::cout << "Data Type   : " << fileType << endl;
        std::cout << "Header size : " << seq->dataSize << endl;
        std::cout << "Sequence    : ";
        for (int i = 0; i < (int)seq->sequence.size(); i++) {
            std::cout << (uint32_t)seq->sequence[i] << (i == (int)seq->sequence.size() - 1 ? endl : ',');
        }
    }

    void printBIMG(const std::shared_ptr<RBIC::BIMG> bimg) {
        std::string fileType(all(bimg->header));
        std::cout << "Data Type   : " << fileType << endl;
        std::cout << "Header size : " << bimg->dataSize << endl;
        std::cout << "Width       : " << bimg->width << endl;
        std::cout << "Height      : " << bimg->height << endl;
    }
}