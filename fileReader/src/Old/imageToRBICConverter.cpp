#include "imageToRBICConverter.hpp"

namespace imageToRBICConverter{
    RBIC::RBIC convertImageToRBIC(const image::image& image) {
        std::shared_ptr<RBIC::BIMG> bimg = std::make_shared<RBIC::BIMG>();
        bimg->width = image.width;
        bimg->height = image.height;
        bimg->colorData = std::vector<uint32_t>(image.pixelData.begin(), image.pixelData.end());
        RBIC::recalculateDataSize(bimg);

        std::shared_ptr<RBIC::DATA> data = std::make_shared<RBIC::DATA>();
        data->data.push_back(bimg);
        
        std::shared_ptr<RBIC::IMAG> imag = std::make_shared<RBIC::IMAG>();
        imag->version = RBIC_VERSION;

        RBIC::RBIC rbic;
        rbic.fileType = imag;
        rbic.file.push_back(data);
    }
    
    RBIC::RBIC convertAnimatedImageToRBIC(const image::animatedImage& animatedImage) {
        return RBIC::RBIC();
    }
}

