#include "ZegoCustomVideoSourceImage.h"
#include <QImage>

ZegoCustomVideoSourceImage::ZegoCustomVideoSourceImage()
{

}

ZegoCustomVideoSourceImage::~ZegoCustomVideoSourceImage()
{

}

void ZegoCustomVideoSourceImage::setImagePath(std::string path)
{
    std::lock_guard<std::mutex>lock(imageMutex);
    imagePath = path;
}

ZegoCustomVideoSourceType ZegoCustomVideoSourceImage::videoSourceType()
{
    return ZegoCustomVideoSourceType_Image;
}

void ZegoCustomVideoSourceImage::getVideoFrame(std::shared_ptr<ZegoCustomVideoFrame> &videoFrame)
{
    std::lock_guard<std::mutex>lock(imageMutex);
    if(imagePath.empty())
        return;

    QImage image(imagePath.c_str());
    videoFrame = std::make_shared<ZegoCustomVideoFrame>();
    videoFrame->dataLength = (unsigned int)(image.byteCount());
    videoFrame->data = std::unique_ptr<unsigned char[]>(new unsigned char[videoFrame->dataLength]);
    memcpy(videoFrame->data.get(), image.constBits(), videoFrame->dataLength);

    videoFrame->param.format = ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_BGRA32;
    videoFrame->param.width = image.width();
    videoFrame->param.height = image.height();
    videoFrame->param.strides[0] = image.bytesPerLine();
    videoFrame->param.rotation = 0;

    videoFrame->referenceTimeMillsecond = this->getCurrentTimestampMS();
}
