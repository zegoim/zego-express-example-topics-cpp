#include "ZegoCustomVideoSourceMedia.h"
#include "AppSupport/ZegoConfigManager.h"

#include <QDebug>

ZegoCustomVideoSourceMedia::ZegoCustomVideoSourceMedia()
{
    auto engine = ZegoExpressSDK::getEngine();
    if(engine == nullptr){
        return;
    }

    mediaPlayer = engine->createMediaPlayer();

    auto mediaPlayerCallbackCenter = std::make_shared<ZegoExpressMediaPlayerCallbackCenter>();
    mediaPlayerCallbackCenter->setCallback(this);

    mediaPlayer->setEventHandler(mediaPlayerCallbackCenter);
    mediaPlayer->setVideoHandler(mediaPlayerCallbackCenter, ZEGO_VIDEO_FRAME_FORMAT_BGRA32);
}

ZegoCustomVideoSourceMedia::~ZegoCustomVideoSourceMedia()
{
    auto engine = ZegoExpressSDK::getEngine();
    if(engine){
        engine->destroyMediaPlayer(mediaPlayer);
    }
}

ZegoCustomVideoSourceType ZegoCustomVideoSourceMedia::videoSourceType()
{
    return ZegoCustomVideoSourceType_Media;
}

void ZegoCustomVideoSourceMedia::getVideoFrame(std::shared_ptr<ZegoCustomVideoFrame> &videoFrame)
{
    std::lock_guard<std::mutex> lock(mediaMutex);
    if(mVideoFrameQueue.size()>0){
         videoFrame = mVideoFrameQueue.front();
         mVideoFrameQueue.pop();
    }else{
        videoFrame = nullptr;
    }
}

void ZegoCustomVideoSourceMedia::startPlayMedia(std::string path)
{
    if(mediaPlayer == nullptr){
        return;
    }

    mediaPath = path;
    mediaPlayer->loadResource(mediaPath, [=](int errorCode){
        if(errorCode==0){
            mediaPlayer->enableRepeat(true);
            mediaPlayer->start();
        }
    });
}

void ZegoCustomVideoSourceMedia::stopPlayMedia()
{
    if(mediaPlayer == nullptr){
        return;
    }
    mediaPlayer->stop();
}

void ZegoCustomVideoSourceMedia::onMediaPlayerStateUpdate(IZegoMediaPlayer *mediaPlayer, ZegoMediaPlayerState state, int errorCode)
{
    Q_UNUSED(mediaPlayer)
    Q_UNUSED(state)
    Q_UNUSED(errorCode)
}

void ZegoCustomVideoSourceMedia::onVideoFrame(IZegoMediaPlayer *mediaPlayer, const unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam param)
{
    Q_UNUSED(mediaPlayer)
    std::lock_guard<std::mutex> lock(mediaMutex);
    if(mVideoFrameQueue.size() <= 2 ){
        auto videoFrame = std::make_shared<ZegoCustomVideoFrame>();
        videoFrame->dataLength = dataLength[0];
        videoFrame->data = std::unique_ptr<unsigned char[]>(new unsigned char[dataLength[0]]);
        memcpy(videoFrame->data.get(), data[0], videoFrame->dataLength);
        videoFrame->param = param;
        videoFrame->referenceFimeMillsecond = this->getCurTimeStampMs();

        mVideoFrameQueue.push(videoFrame);
    }
}

