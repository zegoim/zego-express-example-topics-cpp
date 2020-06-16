#ifndef ZEGOEVENTHANDLERQT_H
#define ZEGOEVENTHANDLERQT_H

#include <QObject>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;

class ZegoEventHandlerQt : public QObject, public IZegoEventHandler
{
    Q_OBJECT
public:
    explicit ZegoEventHandlerQt(QObject *parent = nullptr);
    ~ZegoEventHandlerQt() override;

    void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;

    void onEngineStateUpdate(ZegoEngineState state) override;

    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string &extendData) override;
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList) override;
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList) override;
    void onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<ZegoStream>& streamList) override;

    void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode, const std::string &extendData) override;
    void onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality) override;
    void onPublisherCapturedAudioFirstFrame() override;
    void onPublisherCapturedVideoFirstFrame(ZegoPublishChannel channel) override;
    void onPublisherVideoSizeChanged(int width, int height, ZegoPublishChannel channel) override;
    void onPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<ZegoStreamRelayCDNInfo>& streamInfoList) override;

    void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode, const std::string &extendData) override;
    void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality) override;
    void onPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent event) override;
    void onPlayerRecvAudioFirstFrame(const std::string& streamID) override;
    void onPlayerRecvVideoFirstFrame(const std::string& streamID) override;
    void onPlayerRenderVideoFirstFrame(const std::string& streamID) override;
    void onPlayerVideoSizeChanged(const std::string& streamID, int width, int height) override;
    void onPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength) override;

    void onAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo) override;
    void onVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo& deviceInfo) override;
    void onDeviceError(int errorCode, const std::string& deviceName) override;
    void onRemoteCameraStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state) override;
    void onRemoteMicStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state) override;
    void onCapturedSoundLevelUpdate(float soundLevel) override;
    void onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels) override;
    void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& frequencySpectrum) override;
    void onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum>& frequencySpectrums) override;

    void onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList) override;

    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoBroadcastMessageInfo> messageList) override;
    void onIMRecvBarrageMessage(const std::string& roomID, std::vector<ZegoBarrageMessageInfo> messageList) override;
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command) override;

signals:
    void sigDebugError(int errorCode, const std::string& funcName, const std::string& info);

    void sigEngineStateUpdate(ZegoEngineState state);

    void sigRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string &extendData);
    void sigRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void sigRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList);
    void sigRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<ZegoStream>& streamList);

    void sigPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode, const std::string &extendData);
    void sigPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality);
    void sigPublisherCapturedAudioFirstFrame();
    void sigPublisherCapturedVideoFirstFrame(ZegoPublishChannel channel);
    void sigPublisherVideoSizeChanged(int width, int height, ZegoPublishChannel channel);
    void sigPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<ZegoStreamRelayCDNInfo>& streamInfoList);

    void sigPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode, const std::string &extendData);
    void sigPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality);
    void sigPlayerMediaEvent(const std::string& streamID, ZegoPlayerMediaEvent event);
    void sigPlayerRecvAudioFirstFrame(const std::string& streamID);
    void sigPlayerRecvVideoFirstFrame(const std::string& streamID);
    void sigPlayerRenderVideoFirstFrame(const std::string& streamID);
    void sigPlayerVideoSizeChanged(const std::string& streamID, int width, int height);
    void sigPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength);

    void sigAudioDeviceStateChanged(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, const ZegoDeviceInfo& deviceInfo);
    void sigVideoDeviceStateChanged(ZegoUpdateType updateType, const ZegoDeviceInfo& deviceInfo);
    void sigDeviceError(int errorCode, const std::string& deviceName);
    void sigRemoteCameraStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state);
    void sigRemoteMicStateUpdate(const std::string& streamID, ZegoRemoteDeviceState state);
    void sigCapturedSoundLevelUpdate(float soundLevel);
    void sigRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels);
    void sigCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& frequencySpectrum);
    void sigRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum>& frequencySpectrums);

    void sigMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<ZegoStreamRelayCDNInfo>& infoList);

    void sigIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoBroadcastMessageInfo> messageList);
    void sigIMRecvBarrageMessage(const std::string& roomID, std::vector<ZegoBarrageMessageInfo> messageList);
    void sigIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command);



public slots:
};

#endif // ZEGOEVENTHANDLERQT_H









