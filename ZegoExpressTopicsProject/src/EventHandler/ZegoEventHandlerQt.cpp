#include "ZegoEventHandlerQt.h"
#include <QMetaType>

ZegoEventHandlerQt::ZegoEventHandlerQt(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<std::string>("std::string");
}

ZegoEventHandlerQt::~ZegoEventHandlerQt()
{

}

void ZegoEventHandlerQt::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    emit sigDebugError(errorCode, funcName, info);
}

void ZegoEventHandlerQt::onRoomStateUpdate(const std::string &roomID, ZegoRoomState state, int errorCode)
{
    emit sigRoomStateUpdate(roomID, state, errorCode);
}

void ZegoEventHandlerQt::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    emit sigRoomUserUpdate(roomID, updateType, userList);
}

void ZegoEventHandlerQt::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    emit sigRoomStreamUpdate(roomID, updateType, streamList);
}

void ZegoEventHandlerQt::onRoomStreamExtraInfoUpdate(const std::string &roomID, const std::vector<ZegoStream> &streamList)
{
    emit sigRoomStreamExtraInfoUpdate(roomID, streamList);
}

void ZegoEventHandlerQt::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errorCode)
{
    emit sigPublisherStateUpdate(streamID, state, errorCode);
}

void ZegoEventHandlerQt::onPublisherQualityUpdate(const std::string &streamID, const ZegoPublishStreamQuality &quality)
{
    emit sigPublisherQualityUpdate(streamID, quality);
}

void ZegoEventHandlerQt::onPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent event)
{
    emit sigPublisherRecvFirstFrameEvent(event);
}

void ZegoEventHandlerQt::onPublisherVideoSizeChanged(int width, int height)
{
    emit sigPublisherVideoSizeChanged(width, height);
}

void ZegoEventHandlerQt::onPublisherRelayCDNStateUpdate(const std::string &streamID, const std::vector<ZegoStreamRelayCDNInfo> &streamInfoList)
{
    emit sigPublisherRelayCDNStateUpdate(streamID, streamInfoList);
}

void ZegoEventHandlerQt::onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errorCode)
{
    emit sigPlayerStateUpdate(streamID, state, errorCode);
}

void ZegoEventHandlerQt::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    emit sigPlayerQualityUpdate(streamID,  quality);
}

void ZegoEventHandlerQt::onPlayerMediaEvent(const std::string &streamID, ZegoPlayerMediaEvent event)
{
    emit sigPlayerMediaEvent(streamID, event);
}

void ZegoEventHandlerQt::onPlayerRecvFirstFrameEvent(const std::string &streamID, ZegoPlayerFirstFrameEvent event)
{
    emit sigPlayerRecvFirstFrameEvent(streamID,  event);
}

void ZegoEventHandlerQt::onPlayerVideoSizeChanged(const std::string &streamID, int width, int height)
{
    emit sigPlayerVideoSizeChanged(streamID,  width,  height);
}

void ZegoEventHandlerQt::onPlayerRecvSEI(const std::string &streamID, const unsigned char *data, unsigned int dataLength)
{
    emit sigPlayerRecvSEI(streamID, data, dataLength);
}

void ZegoEventHandlerQt::onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo &deviceInfo)
{
    emit sigAudioDeviceStateChanged(updateType,  deviceType,deviceInfo);
}

void ZegoEventHandlerQt::onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo &deviceInfo)
{
    emit sigVideoDeviceStateChanged( updateType,   deviceInfo);
}

void ZegoEventHandlerQt::onDeviceError(int errorCode, const std::string &deviceName)
{
    emit sigDeviceError( errorCode, deviceName);
}

void ZegoEventHandlerQt::onRemoteCameraStateUpdate(const std::string &streamID, ZegoRemoteDeviceState state)
{
    emit sigRemoteCameraStateUpdate(streamID,  state);
}

void ZegoEventHandlerQt::onRemoteMicStateUpdate(const std::string &streamID, ZegoRemoteDeviceState state)
{
    emit sigRemoteMicStateUpdate(streamID,  state);
}

void ZegoEventHandlerQt::onCapturedSoundLevelUpdate(double soundLevel)
{
    emit sigCapturedSoundLevelUpdate(soundLevel);
}

void ZegoEventHandlerQt::onRemoteSoundLevelUpdate(const std::map<std::string, double> &soundLevels)
{
    emit sigRemoteSoundLevelUpdate(soundLevels);
}

void ZegoEventHandlerQt::onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum &frequencySpectrum)
{
    emit sigCapturedAudioSpectrumUpdate(frequencySpectrum);
}

void ZegoEventHandlerQt::onRemoteAudioSpectrumUpdate(const std::map<std::string, ZegoAudioSpectrum> &frequencySpectrums)
{
    emit sigRemoteAudioSpectrumUpdate(frequencySpectrums);
}

void ZegoEventHandlerQt::onMixerRelayCDNStateUpdate(const std::string &taskID, const std::vector<ZegoStreamRelayCDNInfo> &infoList)
{
    emit sigMixerRelayCDNStateUpdate(taskID, infoList);
}

void ZegoEventHandlerQt::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoMessageInfo> messageList)
{
    emit sigIMRecvBroadcastMessage(roomID, messageList);
}

void ZegoEventHandlerQt::onIMRecvCustomCommand(const std::string &roomID, ZegoUser fromUser, const std::string &command)
{
    emit sigIMRecvCustomCommand(roomID, fromUser, command);
}
