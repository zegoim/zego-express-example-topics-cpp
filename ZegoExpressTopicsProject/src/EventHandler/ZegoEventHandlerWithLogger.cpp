#include "ZegoEventHandlerWithLogger.h"
#include <QScrollBar>
#include <QTime>
#include <QDebug>

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

void ZegoEventHandlerWithLogger::onRoomStateUpdate(const std::string &roomID, ZegoRoomState state, int errorCode, const std::string &extendData)
{
    QStringList roomStateExplain = {
        "ZEGO_ROOM_STATE_DISCONNECTED",
        "ZEGO_ROOM_STATE_CONNECTING",
        "ZEGO_ROOM_STATE_CONNECTED"
    };

    QString log = QString("onRoomStateUpdate: roomID=%1, state=%2, errorCode=%3, extendData=%4").arg(QString::fromStdString(roomID)).arg(roomStateExplain.value(state)).arg(errorCode).arg(QString::fromStdString(extendData));
    printLogToView(log);

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

void ZegoEventHandlerWithLogger::onPublisherStateUpdate(const std::string &streamID, ZegoPublisherState state, int errorCode, const std::string &extendData)
{
    QStringList stateExplain = {
        "ZEGO_PUBLISHER_STATE_NO_PUBLISH",
        "ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING",
        "ZEGO_PUBLISHER_STATE_PUBLISHING"
    };

    QString log = QString("onPublisherStateUpdate: streamID=%1, state=%2, errorCode=%3, extendData=%4").arg(streamID.c_str()).arg(stateExplain.value(state)).arg(errorCode).arg(QString::fromStdString(extendData));
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherStateUpdate(streamID, state, errorCode, extendData);
}

void ZegoEventHandlerWithLogger::onPublisherQualityUpdate(const std::string &streamID, const ZegoPublishStreamQuality &quality)
{
    ZegoEventHandlerQt::onPublisherQualityUpdate(streamID, quality);
}

void ZegoEventHandlerWithLogger::onPublisherCapturedAudioFirstFrame()
{
    QString log = QString("onPublisherCapturedAudioFirstFrame");
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherCapturedAudioFirstFrame();
}
void ZegoEventHandlerWithLogger::onPublisherCapturedVideoFirstFrame(ZegoPublishChannel channel)
{
    QString log = QString("onPublisherCapturedVideoFirstFrame: channel=%1").arg(channel);
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherCapturedVideoFirstFrame(channel);
}
void ZegoEventHandlerWithLogger::onPublisherVideoSizeChanged(int width, int height, ZegoPublishChannel channel)
{
    QString log = QString("onPublisherVideoSizeChanged: width=%1, height=%2 channel=%3").arg(width).arg(height).arg(channel);
    printLogToView(log);

    ZegoEventHandlerQt::onPublisherVideoSizeChanged(width, height, channel);
}

void ZegoEventHandlerWithLogger::onPublisherRelayCDNStateUpdate(const std::string &streamID, const std::vector<ZegoStreamRelayCDNInfo> &streamInfoList)
{
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
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerStateUpdate(streamID, state, errorCode, extendData);
}

void ZegoEventHandlerWithLogger::onPlayerQualityUpdate(const std::string &streamID, const ZegoPlayStreamQuality &quality)
{
    ZegoEventHandlerQt::onPlayerQualityUpdate(streamID,  quality);
}

void ZegoEventHandlerWithLogger::onPlayerMediaEvent(const std::string &streamID, ZegoPlayerMediaEvent event)
{
    ZegoEventHandlerQt::onPlayerMediaEvent(streamID, event);
}

void ZegoEventHandlerWithLogger::onPlayerRecvAudioFirstFrame(const std::string &streamID)
{
    QString log = QString("onPlayerRecvAudioFirstFrame: streamID=%1").arg(streamID.c_str());
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerRecvAudioFirstFrame(streamID);
}

void ZegoEventHandlerWithLogger::onPlayerRecvVideoFirstFrame(const std::string &streamID)
{
    QString log = QString("onPlayerRecvVideoFirstFrame: streamID=%1").arg(streamID.c_str());
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerRecvVideoFirstFrame(streamID);}

void ZegoEventHandlerWithLogger::onPlayerRenderVideoFirstFrame(const std::string &streamID)
{
    QString log = QString("onPlayerRenderVideoFirstFrame: streamID=%1").arg(streamID.c_str());
    printLogToView(log);

    ZegoEventHandlerQt::onPlayerRenderVideoFirstFrame(streamID);
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

void ZegoEventHandlerWithLogger::onMixerRelayCDNStateUpdate(const std::vector<ZegoStreamRelayCDNInfo> &infoList, const std::string &taskID)
{
    ZegoEventHandlerQt::onMixerRelayCDNStateUpdate(infoList, taskID);
}

void ZegoEventHandlerWithLogger::onIMRecvBroadcastMessage(const std::string &roomID, std::vector<ZegoBroadcastMessageInfo> messageList)
{
    for (const ZegoBroadcastMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv broadcast]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        printLogToView(log);
    }

    ZegoEventHandlerQt::onIMRecvBroadcastMessage(roomID, messageList);
}

void ZegoEventHandlerWithLogger::onIMRecvBarrageMessage(const std::string &roomID, std::vector<ZegoBarrageMessageInfo> messageList)
{
    for (const ZegoBarrageMessageInfo& messageInfo : messageList) {
        QString log = QString("[recv barrage]\t: roomID=%1,fromUser=%2, message=%3").arg(roomID.c_str()).arg(messageInfo.fromUser.userID.c_str()).arg(messageInfo.message.c_str());
        printLogToView(log);
    }

    ZegoEventHandlerQt::onIMRecvBarrageMessage(roomID, messageList);
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
        QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
        logView->append(QString("[ %1 ] %2").arg(time).arg(log));
        logView->verticalScrollBar()->setValue(logView->verticalScrollBar()->maximum());
    }
}
