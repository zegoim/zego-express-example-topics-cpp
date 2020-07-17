#include "ZegoEventHandlerWithLogger.h"
#include "AppSupport/ZegoUtilHelper.h"

QJsonArray ConvertStreamRelayCDNInfo(const std::vector<ZegoStreamRelayCDNInfo> &streamInfoList)
{
    QStringList ZegoStreamRelayCDNStateList =
    {
        "ZEGO_STREAM_RELAY_CDN_STATE_NO_RELAY",
        "ZEGO_STREAM_RELAY_CDN_STATE_RELAY_REQUESTING",
        "ZEGO_STREAM_RELAY_CDN_STATE_RELAYING"
    };

    QStringList ZegoStreamRelayCDNUpdateReasonList =
    {
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_NONE",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_SERVER_ERROR",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_HANDSHAKE_FAILED",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_ACCESS_POINT_ERROR",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_CREATE_STREAM_FAILED",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_BAD_NAME",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_CDN_SERVER_DISCONNECTED",
        "ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_DISCONNECTED"
    };

    QJsonArray streamInfoArray;
        for(ZegoStreamRelayCDNInfo cdnInfo: streamInfoList){
        QJsonObject cdnInfoObject;
        cdnInfoObject["url"] = cdnInfo.url.c_str();
        cdnInfoObject["state"] = ZegoStreamRelayCDNStateList.value(cdnInfo.state);
        cdnInfoObject["updateReason"] = ZegoStreamRelayCDNUpdateReasonList.value(cdnInfo.updateReason);
        streamInfoArray.append(cdnInfoObject);
    }
    return streamInfoArray;
}

ZegoEventHandlerWithLogger::ZegoEventHandlerWithLogger()
{

}

ZegoEventHandlerWithLogger::~ZegoEventHandlerWithLogger()
{

}

void ZegoEventHandlerWithLogger::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(funcName.c_str()).arg(info.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onDebugError(errorCode, funcName, info);
}

void ZegoEventHandlerWithLogger::onEngineStateUpdate(ZegoEngineState state)
{
    QStringList engineStateExplain = {
        "ZEGO_ENGINE_STATE_START",
        "ZEGO_ENGINE_STATE_STOP",
    };
    QString log = QString("onEngineStateUpdate: state=%1").arg(engineStateExplain.value(state));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onEngineStateUpdate(state);
}

void ZegoEventHandlerWithLogger::onRoomStateUpdate(const std::string &roomID, ZegoRoomState state, int errorCode, const std::string &extendData)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, state=%2, errorCode=%3, extendData=%4").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(state)).arg(errorCode).arg(QString::fromStdString(extendData));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onRoomStateUpdate(roomID, state, errorCode, extendData);
}

void ZegoEventHandlerWithLogger::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onRoomUserUpdate(roomID, updateType, userList);
}

void ZegoEventHandlerWithLogger::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    for(const ZegoStream &stream : streamList){
        QString log = QString("onStreamUpdate: roomID=%1, updateType=%2, streamID=%3, extraInfo=%4").arg(roomID.c_str()).arg(updateTypeString).arg(stream.streamID.c_str()).arg(stream.extraInfo.c_str());
        emit sigPrintLogToView(log);
    }

    ZegoEventHandlerQt::onRoomStreamUpdate(roomID, updateType, streamList);
}

void ZegoEventHandlerWithLogger::onRoomStreamExtraInfoUpdate(const std::string &roomID, const std::vector<ZegoStream> &streamList)
{
    for(const ZegoStream &stream : streamList){
        QString log = QString("onRoomStreamExtraInfoUpdate: roomID=%1, streamID=%2, extraInfo=%3").arg(roomID.c_str()).arg(stream.streamID.c_str()).arg(stream.extraInfo.c_str());
        emit sigPrintLogToView(log);
    }

    ZegoEventHandlerQt::onRoomStreamExtraInfoUpdate(roomID, streamList);
}

void ZegoEventHandlerWithLogger::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errorCode, const std::string &extendData)
{
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NO_PUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING",
        "ZEGO_PUBLISHER_STATE_PUBLISHING"
    };

    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3, extendData=%4").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errorCode).arg(QString::fromStdString(extendData));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPublisherStateUpdate(streamID, state, errorCode, extendData);
}

void ZegoEventHandlerWithLogger::onPublisherQualityUpdate(const std::string &streamID, const ZegoPublishStreamQuality &quality)
{
    ZegoEventHandlerQt::onPublisherQualityUpdate(streamID, quality);
}

void ZegoEventHandlerWithLogger::onPublisherCapturedAudioFirstFrame()
{
    QString log = QString("onPublisherCapturedAudioFirstFrame");
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPublisherCapturedAudioFirstFrame();
}
void ZegoEventHandlerWithLogger::onPublisherCapturedVideoFirstFrame(ZegoPublishChannel channel)
{
    QString log = QString("onPublisherCapturedVideoFirstFrame: channel=%1").arg(channel);
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPublisherCapturedVideoFirstFrame(channel);
}
void ZegoEventHandlerWithLogger::onPublisherVideoSizeChanged(int width, int height, ZegoPublishChannel channel)
{
    QString log = QString("onPublisherVideoSizeChanged: width=%1, height=%2 channel=%3").arg(width).arg(height).arg(channel);
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPublisherVideoSizeChanged(width, height, channel);
}

void ZegoEventHandlerWithLogger::onPublisherRelayCDNStateUpdate(const std::string &streamID, const std::vector<ZegoStreamRelayCDNInfo> &streamInfoList)
{
    QJsonObject relayCDNObject;
    relayCDNObject["streamID"] = streamID.c_str();
    relayCDNObject["relayCNDInfoList"] = ConvertStreamRelayCDNInfo(streamInfoList);
    auto relayCDNString = ZegoUtilHelper::jsonObjectToString(relayCDNObject);
    QString log = QString("onPublisherRelayCDNStateUpdate: %1").arg(relayCDNString);
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
}

void ZegoEventHandlerWithLogger::onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errorCode, const std::string &extendData)
{
    QStringList stateExplain = {
        "ZEGO_PLAYER_STATE_NO_PLAY",
        "ZEGO_PLAYER_STATE_PLAY_REQUESTING",
        "ZEGO_PLAYER_STATE_PLAYING"
    };

    QString log = QString("onPlayerStateUpdate: streamID=%1, state=%2, errorCode=%3, extenData=%4").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errorCode).arg(QString::fromStdString(extendData));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPlayerStateUpdate(streamID, state, errorCode, extendData);
}

void ZegoEventHandlerWithLogger::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    ZegoEventHandlerQt::onPlayerQualityUpdate(streamID,  quality);
}

void ZegoEventHandlerWithLogger::onPlayerMediaEvent(const std::string &streamID, ZegoPlayerMediaEvent event)
{
    QStringList eventExplain = {
        "ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_OCCUR",
        "ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_RESUME",
        "ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_OCCUR",
        "ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_RESUME"
    };

    QString log = QString("onPlayerMediaEvent: streamID=%1, event=%2").arg(streamID.c_str()).arg(eventExplain.value(event));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPlayerMediaEvent(streamID, event);
}

void ZegoEventHandlerWithLogger::onPlayerRecvAudioFirstFrame(const std::string &streamID)
{
    QString log = QString("onPlayerRecvAudioFirstFrame: streamID=%1").arg(streamID.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPlayerRecvAudioFirstFrame(streamID);
}

void ZegoEventHandlerWithLogger::onPlayerRecvVideoFirstFrame(const std::string &streamID)
{
    QString log = QString("onPlayerRecvVideoFirstFrame: streamID=%1").arg(streamID.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPlayerRecvVideoFirstFrame(streamID);}

void ZegoEventHandlerWithLogger::onPlayerRenderVideoFirstFrame(const std::string &streamID)
{
    QString log = QString("onPlayerRenderVideoFirstFrame: streamID=%1").arg(streamID.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPlayerRenderVideoFirstFrame(streamID);
}

void ZegoEventHandlerWithLogger::onPlayerVideoSizeChanged(const std::string &streamID, int width, int height)
{
    QString log = QString("onPlayerVideoSizeChanged: streamID=%1, width=%2, height=%3").arg(streamID.c_str()).arg(width).arg(height);
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onPlayerVideoSizeChanged(streamID,  width,  height);
}

void ZegoEventHandlerWithLogger::onPlayerRecvSEI(const std::string &streamID, const unsigned char *data, unsigned int dataLength)
{
    ZegoEventHandlerQt::onPlayerRecvSEI(streamID, data, dataLength);
}

void ZegoEventHandlerWithLogger::onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo &deviceInfo)
{       
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QString deviceTypeString = deviceType == ZEGO_AUDIO_DEVICE_TYPE_INPUT ? "Input": "Output";
    QString log = QString("onAudioDeviceStateChanged: updateType=%1, deviceType=%2, deviceID=%3, deviceName=%4").arg(updateTypeString).arg(deviceTypeString).arg(deviceInfo.deviceID.c_str()).arg(deviceInfo.deviceName.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onAudioDeviceStateChanged(updateType,  deviceType,deviceInfo);
}

void ZegoEventHandlerWithLogger::onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo &deviceInfo)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QString log = QString("onVideoDeviceStateChanged: updateType=%1, deviceID=%2, deviceName=%3").arg(updateTypeString).arg(deviceInfo.deviceID.c_str()).arg(deviceInfo.deviceName.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onVideoDeviceStateChanged(updateType,   deviceInfo);
}

void ZegoEventHandlerWithLogger::onDeviceError(int errorCode, const std::string &deviceName)
{
    QString log = QString("onDeviceError: errorCode=%1, deviceName=%2").arg(errorCode).arg(deviceName.c_str());
    emit sigPrintLogToView(log);
    ZegoEventHandlerQt::onDeviceError(errorCode, deviceName);
}

void ZegoEventHandlerWithLogger::onRemoteCameraStateUpdate(const std::string &streamID, ZegoRemoteDeviceState state)
{
    QStringList stateExplain = {
        "ZEGO_REMOTE_DEVICE_STATE_OPEN",
        "ZEGO_REMOTE_DEVICE_STATE_GENERIC_ERROR",
        "ZEGO_REMOTE_DEVICE_STATE_INVALID_ID",
        "ZEGO_REMOTE_DEVICE_STATE_NO_AUTHORIZATION",
        "ZEGO_REMOTE_DEVICE_STATE_ZERO_FPS",
        "ZEGO_REMOTE_DEVICE_STATE_IN_USE_BY_OTHER",
        "ZEGO_REMOTE_DEVICE_STATE_UNPLUGGED",
        "ZEGO_REMOTE_DEVICE_STATE_REBOOT_REQUIRED",
        "ZEGO_REMOTE_DEVICE_STATE_SYSTEM_MEDIA_SERVICES_LOST",
        "ZEGO_REMOTE_DEVICE_STATE_DISABLE",
        "ZEGO_REMOTE_DEVICE_STATE_MUTE",
        "ZEGO_REMOTE_DEVICE_STATE_INTERRUPTION",
        "ZEGO_REMOTE_DEVICE_STATE_IN_BACKGROUND",
        "ZEGO_REMOTE_DEVICE_STATE_MULTI_FOREGROUND_APP",
        "ZEGO_REMOTE_DEVICE_STATE_BY_SYSTEM_PRESSURE"
    };

    QString log = QString("onRemoteCameraStateUpdate: streamID=%1, state=%2").arg(streamID.c_str()).arg(stateExplain.value(state));
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onRemoteCameraStateUpdate(streamID,  state);
}

void ZegoEventHandlerWithLogger::onRemoteMicStateUpdate(const std::string &streamID, ZegoRemoteDeviceState state)
{
    QStringList stateExplain = {
        "ZEGO_REMOTE_DEVICE_STATE_OPEN",
        "ZEGO_REMOTE_DEVICE_STATE_GENERIC_ERROR",
        "ZEGO_REMOTE_DEVICE_STATE_INVALID_ID",
        "ZEGO_REMOTE_DEVICE_STATE_NO_AUTHORIZATION",
        "ZEGO_REMOTE_DEVICE_STATE_ZERO_FPS",
        "ZEGO_REMOTE_DEVICE_STATE_IN_USE_BY_OTHER",
        "ZEGO_REMOTE_DEVICE_STATE_UNPLUGGED",
        "ZEGO_REMOTE_DEVICE_STATE_REBOOT_REQUIRED",
        "ZEGO_REMOTE_DEVICE_STATE_SYSTEM_MEDIA_SERVICES_LOST",
        "ZEGO_REMOTE_DEVICE_STATE_DISABLE",
        "ZEGO_REMOTE_DEVICE_STATE_MUTE",
        "ZEGO_REMOTE_DEVICE_STATE_INTERRUPTION",
        "ZEGO_REMOTE_DEVICE_STATE_IN_BACKGROUND",
        "ZEGO_REMOTE_DEVICE_STATE_MULTI_FOREGROUND_APP",
        "ZEGO_REMOTE_DEVICE_STATE_BY_SYSTEM_PRESSURE"
    };

    QString log = QString("onRemoteMicStateUpdate: streamID=%1, state=%2").arg(streamID.c_str()).arg(stateExplain.value(state));
    emit sigPrintLogToView(log);
    ZegoEventHandlerQt::onRemoteMicStateUpdate(streamID,  state);
}

void ZegoEventHandlerWithLogger::onCapturedSoundLevelUpdate(float soundLevel)
{
    ZegoEventHandlerQt::onCapturedSoundLevelUpdate(soundLevel);
}

void ZegoEventHandlerWithLogger::onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float> &soundLevels)
{
    ZegoEventHandlerQt::onRemoteSoundLevelUpdate(soundLevels);
}

void ZegoEventHandlerWithLogger::onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum &frequencySpectrum)
{
    ZegoEventHandlerQt::onCapturedAudioSpectrumUpdate(frequencySpectrum);
}

void ZegoEventHandlerWithLogger::onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum> &frequencySpectrums)
{
    ZegoEventHandlerQt::onRemoteAudioSpectrumUpdate(frequencySpectrums);
}

void ZegoEventHandlerWithLogger::onMixerRelayCDNStateUpdate(const std::string &taskID, const std::vector<ZegoStreamRelayCDNInfo> &infoList)
{
    QJsonObject relayCDNObject;
    relayCDNObject["taskID"] = taskID.c_str();
    relayCDNObject["relayCNDInfoList"] = ConvertStreamRelayCDNInfo(infoList);
    auto relayCDNString = ZegoUtilHelper::jsonObjectToString(relayCDNObject);
    QString log = QString("onMixerRelayCDNStateUpdate: %1").arg(relayCDNString);
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onMixerRelayCDNStateUpdate(taskID, infoList);
}

void ZegoEventHandlerWithLogger::onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float> &soundLevels)
{
   for (auto& soundLevel : soundLevels) {
       QString log = QString("onMixerSoundLevelUpdate\t: soudnLevelID=%1, soundLevel=%2").arg(soundLevel.first).arg(soundLevel.second);
       emit sigPrintLogToView(log);
   }
   ZegoEventHandlerQt::onMixerSoundLevelUpdate(soundLevels);
}

void ZegoEventHandlerWithLogger::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoBroadcastMessageInfo> messageList)
{
    for (const ZegoBroadcastMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv broadcast]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        emit sigPrintLogToView(log);
    }

    ZegoEventHandlerQt::onIMRecvBroadcastMessage(roomID, messageList);
}

void ZegoEventHandlerWithLogger::onIMRecvBarrageMessage(const std::string &roomID, std::vector<ZegoBarrageMessageInfo> messageList)
{
    for (const ZegoBarrageMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv barrage]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        emit sigPrintLogToView(log);
    }

    ZegoEventHandlerQt::onIMRecvBarrageMessage(roomID, messageList);
}

void ZegoEventHandlerWithLogger::onIMRecvCustomCommand(const std::string &roomID, ZegoUser fromUser, const std::string &command)
{
    QString log = QString("[recv command]\t: roomID=%1, fromUser=%2, command=%3").arg(roomID.c_str()).arg(fromUser.userID.c_str()).arg(command.c_str());
    emit sigPrintLogToView(log);

    ZegoEventHandlerQt::onIMRecvCustomCommand(roomID, fromUser, command);
}
