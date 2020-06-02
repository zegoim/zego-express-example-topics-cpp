#ifndef ZEGOCUSTOMVIDEOSOURCEMEDIA_H
#define ZEGOCUSTOMVIDEOSOURCEMEDIA_H

#include "ZegoCustomVideoSourceBase.h"
#include "../MediaPlayer/ZegoMediaPlayerDemo.h"
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;
#include <queue>

class ZegoCustomVideoSourceMedia: public ZegoCustomVideoSourceBase, public IMediaPlayerCallback
{
public:
    ZegoCustomVideoSourceMedia();
    ~ZegoCustomVideoSourceMedia() override;

    ZegoCustomVideoSourceType videoSourceType() override;
    void getVideoFrame(std::shared_ptr<ZegoCustomVideoFrame> & videoFrame) override;

    void startPlayMedia(std::string path);
    void stopPlayMedia();

    void onMediaPlayerStateUpdate(IZegoMediaPlayer* mediaPlayer, ZegoMediaPlayerState state, int errorCode) override;
    void onVideoFrame(IZegoMediaPlayer* mediaPlayer, const unsigned char** data, unsigned int* dataLength, ZegoVideoFrameParam param) override;

private:
    std::mutex mediaMutex;
    std::queue<std::shared_ptr<ZegoCustomVideoFrame>> mVideoFrameQueue;
    ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer = nullptr;
    std::string mediaPath;
};

#endif // ZEGOCUSTOMVIDEOSOURCEMEDIA_H
