#ifndef ZEGOCUSTOMVIDEOSOURCEIMAGE_H
#define ZEGOCUSTOMVIDEOSOURCEIMAGE_H

#include "ZegoCustomVideoSourceBase.h"

class ZegoCustomVideoSourceImage: public ZegoCustomVideoSourceBase
{
public:
    ZegoCustomVideoSourceImage();
    ~ZegoCustomVideoSourceImage() override;

    ZegoCustomVideoSourceType videoSourceType() override;
    void getVideoFrame(std::shared_ptr<ZegoCustomVideoFrame> & videoFrame) override;

    void setImagePath(std::string path);

private:
    std::mutex imageMutex;
    std::string imagePath;
};

#endif // ZEGOCUSTOMVIDEOSOURCEIMAGE_H
