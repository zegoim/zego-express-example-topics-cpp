#include "ZegoCustomVideoSourceBase.h"

ZegoCustomVideoSourceBase::ZegoCustomVideoSourceBase()
{

}

ZegoCustomVideoSourceBase::~ZegoCustomVideoSourceBase()
{

}

unsigned long long ZegoCustomVideoSourceBase::getCurTimeStampMs()
{
    auto timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return (unsigned long long) timeNow.count();
}

