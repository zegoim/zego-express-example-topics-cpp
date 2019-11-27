#ifndef ZEGOEVENTHANDLERQT_H
#define ZEGOEVENTHANDLERQT_H

#include <QObject>
#include "ZegoExpressEngine.h"
using namespace ZEGO::EXPRESS;

class ZegoEventHandlerQt : public QObject, public IZegoEventHandler
{
    Q_OBJECT
public:
    explicit ZegoEventHandlerQt(QObject *parent = nullptr);
    ~ZegoEventHandlerQt() override;

    void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode) override;
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList) override;
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList) override;
    void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode) override;
    void onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality) override;
    void onPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent event) override;
    void onPublisherVideoSizeChanged(int width, int height) override;
    void onPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<ZegoStreamRelayCDNInfo>& streamInfoList) override;
    void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode) override;
    void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality) override;
    void onPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent event) override;
    void onPlayerRecvFirstFrameEvent(const std::string& streamID, ZegoPlayerFirstFrameEvent event) override;
    void onPlayerVideoSizeChanged(const std::string& streamID, int width, int height) override;
    void onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo) override;
    void onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo& deviceInfo) override;
    void onDeviceError(int errorCode, const std::string& deviceName) override;
    void onRemoteCameraStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state) override;
    void onRemoteMicStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state) override;
    void onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList) override;
    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoMessageInfo> messageList) override;
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command) override;

signals:
    void sigDebugError(int errorCode, const std::string& funcName, const std::string& info);
    void sigRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode);
    void sigRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void sigRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
    void sigPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode);
    void sigPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality);
    void sigPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent event);
    void sigPublisherVideoSizeChanged(int width, int height);
    void sigPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<ZegoStreamRelayCDNInfo>& streamInfoList);
    void sigPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode);
    void sigPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality);
    void sigPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent event);
    void sigPlayerRecvFirstFrameEvent(const std::string& streamID, ZegoPlayerFirstFrameEvent event);
    void sigPlayerVideoSizeChanged(const std::string& streamID, int width, int height);
    void sigAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo);
    void sigVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo& deviceInfo);
    void sigDeviceError(int errorCode, const std::string& deviceName);
    void sigRemoteCameraStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state);
    void sigRemoteMicStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state);
    void sigMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList);
    void sigIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoMessageInfo> messageList);
    void sigIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command);

public slots:
};

#endif // ZEGOEVENTHANDLERQT_H









