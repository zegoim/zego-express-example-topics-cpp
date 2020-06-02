#ifndef ZEGOEVENTLOGGER_H
#define ZEGOEVENTLOGGER_H

#include "ZegoEventHandlerQt.h"
#include <QTextEdit>
#include <QDebug>

class ZegoEventHandlerWithLogger : public ZegoEventHandlerQt
{
    Q_OBJECT
public:
    explicit ZegoEventHandlerWithLogger(QTextEdit *logView = nullptr);
    ~ZegoEventHandlerWithLogger() override;

    void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;
    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string &extendData) override;
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList) override;
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList) override;
    void onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<ZegoStream>& streamList) override;

    void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode, const std::string &extendData) override;
    void onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality) override;
    void onPublisherCapturedAudioFirstFrame() override;

    void onPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<ZegoStreamRelayCDNInfo>& streamInfoList) override;

    void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode, const std::string &extendData) override;
    void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality) override;
    void onPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent event) override;
    void onPlayerRecvAudioFirstFrame(const std::string& streamID) override;

    void onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo) override;

    void onDeviceError(int errorCode, const std::string& deviceName) override;

    void onRemoteMicStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state) override;
    void onCapturedSoundLevelUpdate(float soundLevel) override;
    void onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float> &soundLevels) override;
    void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& frequencySpectrum) override;
    void onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum>& frequencySpectrums) override;

    void onMixerRelayCDNStateUpdate(const std::string &taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList) override;

    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoBroadcastMessageInfo> messageList) override;
    void onIMRecvBarrageMessage(const std::string& roomID, std::vector<ZegoBarrageMessageInfo> messageList) override;
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command) override;

private:
    void printLogToView(QString log);
    QTextEdit *logView = nullptr;
};

#endif // ZEGOEVENTLOGGER_H
