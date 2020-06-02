#ifndef ZEGOCUSTOMVIDEOSOURCEBASE_H
#define ZEGOCUSTOMVIDEOSOURCEBASE_H

#include "ZegoExpressSDK.h"

enum ZegoCustomVideoSourceType{
    ZegoCustomVideoSourceType_Image = 1,
    ZegoCustomVideoSourceType_Media = 2,
};

struct ZegoCustomVideoFrame
{
    std::unique_ptr<unsigned char[]> data;
    unsigned int dataLength = 0;
    ZEGO::EXPRESS::ZegoVideoFrameParam param;
    unsigned long long referenceTimeMillsecond = 0;
};

class ZegoCustomVideoSourceBase
{
public:
    ZegoCustomVideoSourceBase();
    virtual ~ZegoCustomVideoSourceBase();

    virtual ZegoCustomVideoSourceType videoSourceType() = 0;
    virtual void getVideoFrame(std::shared_ptr<ZegoCustomVideoFrame> & videoFrame) = 0;

    unsigned long long getCurrentTimestampMS();
};


#endif // ZEGOCUSTOMVIDEOSOURCEBASE_H
