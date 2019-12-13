#ifndef ZEGOEVENTLOGGER_H
#define ZEGOEVENTLOGGER_H

#include "ZegoEventHandlerQt.h"
#include <QTextEdit>

class ZegoEventHandlerWithLogger : public ZegoEventHandlerQt
{
    Q_OBJECT
public:
    explicit ZegoEventHandlerWithLogger(QTextEdit *logView = nullptr);
    ~ZegoEventHandlerWithLogger() override;

    void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode) override;
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList) override;
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList) override;
    void onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<ZegoStream>& streamList) override;

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
    void onCapturedSoundLevelUpdate(double soundLevel) override;
    void onRemoteSoundLevelUpdate(const std::map<std::string, double>& soundLevels) override;
    void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& frequencySpectrum) override;
    void onRemoteAudioSpectrumUpdate(const std::map<std::string, ZegoAudioSpectrum>& frequencySpectrums) override;

    void onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList) override;

    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoMessageInfo> messageList) override;
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command) override;

private:
    void printLogToView(QString log);
    QTextEdit *logView = nullptr;
};

#endif // ZEGOEVENTLOGGER_H
