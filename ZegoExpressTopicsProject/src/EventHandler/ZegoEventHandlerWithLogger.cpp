#include "ZegoEventHandlerWithLogger.h"
#include <QScrollBar>

ZegoEventHandlerWithLogger::ZegoEventHandlerWithLogger(QTextEdit *logView)
    :logView(logView)
{

}

ZegoEventHandlerWithLogger::~ZegoEventHandlerWithLogger()
{

}

void ZegoEventHandlerWithLogger::onDebugError(int errorCode, const std::string &funcName, const std::string &info)
{
    QString log = QString("onDebugError: errorCode=%1, funcName=%2, info=%3").arg(errorCode).arg(funcName.c_str()).arg(info.c_str());
    printLogToView(log);

    ZegoEventHandlerQt::onDebugError(errorCode, funcName, info);
}

void ZegoEventHandlerWithLogger::onRoomStateUpdate(const std::string &roomID, ZegoRoomState state, int errorCode)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, state=%2, errorCode=%3").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(state)).arg(errorCode);
    printLogToView(log);

    ZegoEventHandlerQt::onRoomStateUpdate(roomID, state, errorCode);
}

void ZegoEventHandlerWithLogger::onRoomUserUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoUser> &userList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    QStringList userIDs;
    for (const ZegoUser &user : userList) {
        userIDs.append(QString::fromStdString(user.userID));
    }
    QString log = QString("onRoomUserUpdate: roomID=%1, updateType=%2, userIDs=%3").arg(roomID.c_str()).arg(updateTypeString).arg(userIDs.join(","));
    printLogToView(log);

    ZegoEventHandlerQt::onRoomUserUpdate(roomID, updateType, userList);
}

void ZegoEventHandlerWithLogger::onRoomStreamUpdate(const std::string &roomID, ZegoUpdateType updateType, const std::vector<ZegoStream> &streamList)
{
    QString updateTypeString = updateType == ZEGO_UPDATE_TYPE_ADD ? "Add" : "Remove";
    for(const ZegoStream &stream : streamList){
        QString log = QString("onStreamUpdate: roomID=%1, updateType=%2, streamID=%3, extraInfo=%4").arg(roomID.c_str()).arg(updateTypeString).arg(stream.streamID.c_str()).arg(stream.extraInfo.c_str());
        printLogToView(log);
    }

    ZegoEventHandlerQt::onRoomStreamUpdate(roomID, updateType, streamList);
}

void ZegoEventHandlerWithLogger::onRoomStreamExtraInfoUpdate(const std::string &roomID, const std::vector<ZegoStream> &streamList)
{
    for(const ZegoStream &stream : streamList){
        QString log = QString("onRoomStreamExtraInfoUpdate: roomID=%1, streamID=%2, extraInfo=%3").arg(roomID.c_str()).arg(stream.streamID.c_str()).arg(stream.extraInfo.c_str());
        printLogToView(log);
    }

    ZegoEventHandlerQt::onRoomStreamExtraInfoUpdate(roomID, streamList);
}

void ZegoEventHandlerWithLogger::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errorCode)
{
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NO_PUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING",
        "ZEGO_PUBLISHER_STATE_PUBLISHING"
    };

    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errorCode);
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherStateUpdate(streamID, state, errorCode);
}

void ZegoEventHandlerWithLogger::onPublisherQualityUpdate(const std::string &streamID, const ZegoPublishStreamQuality &quality)
{
    ZegoEventHandlerQt::onPublisherQualityUpdate(streamID, quality);
}

void ZegoEventHandlerWithLogger::onPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent event)
{
    QStringList firstFrameEventExplain = {
        "ZEGO_PUBLISHER_FIRST_FRAME_EVENT_AUDIO_CAPTURED",
        "ZEGO_PUBLISHER_FIRST_FRAME_EVENT_VIDEO_CAPTURED"
    };

    QString log = QString("onPublisherFirstFrameEvent: firstFrameEvent=%1").arg(firstFrameEventExplain.value(event));
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherRecvFirstFrameEvent(event);
}

void ZegoEventHandlerWithLogger::onPublisherVideoSizeChanged(int width, int height)
{
    QString log = QString("onPublisherVideoSizeChanged: width=%1, height=%2").arg(width).arg(height);
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherVideoSizeChanged(width, height);
}

void ZegoEventHandlerWithLogger::onPublisherRelayCDNStateUpdate(const std::string &streamID, const std::vector<ZegoStreamRelayCDNInfo> &streamInfoList)
{
    ZegoEventHandlerQt::onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
}

void ZegoEventHandlerWithLogger::onPlayerStateUpdate(const std::string &streamID, ZegoPlayerState state, int errorCode)
{
    QStringList stateExplain = {
        "ZEGO_PLAYER_STATE_NO_PLAY",
        "ZEGO_PLAYER_STATE_PLAY_REQUESTING",
        "ZEGO_PLAYER_STATE_PLAYING"
    };

    QString log = QString("onPlayerStateUpdate: streamID=%1, state=%2, errorCode=%3").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errorCode);
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerStateUpdate(streamID, state, errorCode);
}

void ZegoEventHandlerWithLogger::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    ZegoEventHandlerQt::onPlayerQualityUpdate(streamID,  quality);
}

void ZegoEventHandlerWithLogger::onPlayerMediaEvent(const std::string &streamID, ZegoPlayerMediaEvent event)
{
    ZegoEventHandlerQt::onPlayerMediaEvent(streamID, event);
}

void ZegoEventHandlerWithLogger::onPlayerRecvFirstFrameEvent(const std::string &streamID, ZegoPlayerFirstFrameEvent event)
{
    QStringList firstFrameEventExplain = {
        "ZEGO_PLAYER_FIRST_FRAME_EVENT_AUDIO_RCV",
        "ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RCV",
        "ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDER"
    };
    QString log = QString("onPlayerFirstFrameEvent: streamID=%1, firstFrameEvent=%2").arg(streamID.c_str()).arg(firstFrameEventExplain.value(event));
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerRecvFirstFrameEvent(streamID,  event);
}

void ZegoEventHandlerWithLogger::onPlayerVideoSizeChanged(const std::string &streamID, int width, int height)
{
    QString log = QString("onPlayerVideoSizeChanged: streamID=%1, width=%2, height=%3").arg(streamID.c_str()).arg(width).arg(height);
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerVideoSizeChanged(streamID,  width,  height);
}

void ZegoEventHandlerWithLogger::onPlayerRecvSEI(const std::string &streamID, const unsigned char *data, unsigned int dataLength)
{
    ZegoEventHandlerQt::onPlayerRecvSEI(streamID, data, dataLength);
}

void ZegoEventHandlerWithLogger::onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo &deviceInfo)
{
    ZegoEventHandlerQt::onAudioDeviceStateChanged(updateType,  deviceType,deviceInfo);
}

void ZegoEventHandlerWithLogger::onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo &deviceInfo)
{
    ZegoEventHandlerQt::onVideoDeviceStateChanged(updateType,   deviceInfo);
}

void ZegoEventHandlerWithLogger::onDeviceError(int errorCode, const std::string &deviceName)
{
    ZegoEventHandlerQt::onDeviceError(errorCode, deviceName);
}

void ZegoEventHandlerWithLogger::onRemoteCameraStateUpdate(const std::string &streamID, ZegoRemoteDeviceState state)
{
    ZegoEventHandlerQt::onRemoteCameraStateUpdate(streamID,  state);
}

void ZegoEventHandlerWithLogger::onRemoteMicStateUpdate(const std::string &streamID, ZegoRemoteDeviceState state)
{
    ZegoEventHandlerQt::onRemoteMicStateUpdate(streamID,  state);
}

void ZegoEventHandlerWithLogger::onCapturedSoundLevelUpdate(double soundLevel)
{
    ZegoEventHandlerQt::onCapturedSoundLevelUpdate(soundLevel);
}

void ZegoEventHandlerWithLogger::onRemoteSoundLevelUpdate(const std::map<std::string, double> &soundLevels)
{
    ZegoEventHandlerQt::onRemoteSoundLevelUpdate(soundLevels);
}

void ZegoEventHandlerWithLogger::onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum &frequencySpectrum)
{
    ZegoEventHandlerQt::onCapturedAudioSpectrumUpdate(frequencySpectrum);
}

void ZegoEventHandlerWithLogger::onRemoteAudioSpectrumUpdate(const std::map<std::string, ZegoAudioSpectrum> &frequencySpectrums)
{
    ZegoEventHandlerQt::onRemoteAudioSpectrumUpdate(frequencySpectrums);
}

void ZegoEventHandlerWithLogger::onMixerRelayCDNStateUpdate(const std::string &taskID, const std::vector<ZegoStreamRelayCDNInfo> &infoList)
{
    ZegoEventHandlerQt::onMixerRelayCDNStateUpdate(taskID, infoList);
}

void ZegoEventHandlerWithLogger::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoMessageInfo> messageList)
{
    for (const ZegoMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv message]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        printLogToView(log);
    }

    ZegoEventHandlerQt::onIMRecvBroadcastMessage(roomID, messageList);
}

void ZegoEventHandlerWithLogger::onIMRecvCustomCommand(const std::string &roomID, ZegoUser fromUser, const std::string &command)
{
    QString log = QString("[recv command]\t: roomID=%1, fromUser=%2, command=%3").arg(roomID.c_str()).arg(fromUser.userID.c_str()).arg(command.c_str());
    printLogToView(log);

    ZegoEventHandlerQt::onIMRecvCustomCommand(roomID, fromUser, command);
}

void ZegoEventHandlerWithLogger::printLogToView(QString log)
{
    if(logView){
        logView->append(log);
        logView->verticalScrollBar()->setValue(logView->verticalScrollBar()->maximum());
    }
}
