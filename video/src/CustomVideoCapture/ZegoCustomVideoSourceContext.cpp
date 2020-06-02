#include "ZegoCustomVideoSourceContext.h"

ZegoCustomVideoSourceContext::ZegoCustomVideoSourceContext()
{

}

ZegoCustomVideoSourceContext::~ZegoCustomVideoSourceContext()
{
    if(currentVideoSource){
        delete currentVideoSource;
        currentVideoSource = nullptr;
    }
}

void ZegoCustomVideoSourceContext::getVideoFrame(std::shared_ptr<ZegoCustomVideoFrame> &videoFrame)
{
    std::lock_guard<std::mutex> lock(mVideoSouceMutex);
    if(currentVideoSource){
        currentVideoSource->getVideoFrame(videoFrame);
    }
}

ZegoCustomVideoSourceBase *ZegoCustomVideoSourceContext::getVideoSource(ZegoCustomVideoSourceType sourceType)
{
    std::lock_guard<std::mutex> lock(mVideoSouceMutex);
    if(currentVideoSource){
        if(currentVideoSource->videoSourceType() == sourceType){
            return currentVideoSource;
        }else{
            delete currentVideoSource;
            currentVideoSource = nullptr;
        }
    }

    switch (sourceType) {
    case ZegoCustomVideoSourceType_Image:
        currentVideoSource = new ZegoCustomVideoSourceImage;
        break;
    case ZegoCustomVideoSourceType_Media:
        currentVideoSource = new ZegoCustomVideoSourceMedia;
        break;
    }
    return currentVideoSource;
}

