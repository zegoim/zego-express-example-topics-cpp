#ifndef ZEGOEVENTHANDLERQT_H
#define ZEGOEVENTHANDLERQT_H

#include <QObject>
#include "ZegoExpressSDK.h"
using namespace ZEGO::EXPRESS;


Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(ZegoEngineState)
Q_DECLARE_METATYPE(ZegoRoomState)
Q_DECLARE_METATYPE(ZegoUpdateType)
Q_DECLARE_METATYPE(std::vector<ZegoUser>)
Q_DECLARE_METATYPE(std::vector<ZegoStream>)
Q_DECLARE_METATYPE(ZegoPublisherState)
Q_DECLARE_METATYPE(ZegoPublishStreamQuality)
Q_DECLARE_METATYPE(ZegoPublishChannel)
Q_DECLARE_METATYPE(std::vector<ZegoStreamRelayCDNInfo>)
Q_DECLARE_METATYPE(ZegoPlayerState)
Q_DECLARE_METATYPE(ZegoPlayStreamQuality)
Q_DECLARE_METATYPE(ZegoPlayerMediaEvent)
Q_DECLARE_METATYPE(ZegoAudioDeviceType)
Q_DECLARE_METATYPE(ZegoDeviceInfo)
Q_DECLARE_METATYPE(ZegoRemoteDeviceState)
Q_DECLARE_METATYPE(ZegoAudioSpectrum)
Q_DECLARE_METATYPE(std::vector<ZegoBroadcastMessageInfo>)
Q_DECLARE_METATYPE(std::vector<ZegoBarrageMessageInfo>)
Q_DECLARE_METATYPE(ZegoUser)
Q_DECLARE_METATYPE(std::vector<ZegoRoomExtraInfo>)
Q_DECLARE_METATYPE(ZegoPerformanceStatus)

using UnorderedMapWithStringFloat = std::unordered_map<std::string, float>;
using UnorderedMapWithStringZegoAudioSpectrum = std::unordered_map<std::string, ZegoAudioSpectrum>;
using UnorderedMapWithUnsignedIntFloat = std::unordered_map<unsigned int, float>;
Q_DECLARE_METATYPE(UnorderedMapWithStringFloat)
Q_DECLARE_METATYPE(UnorderedMapWithStringZegoAudioSpectrum)
Q_DECLARE_METATYPE(UnorderedMapWithUnsignedIntFloat)

class ZegoEventHandlerQt : public QObject, public IZegoEventHandler
{
    Q_OBJECT
public:
    explicit ZegoEventHandlerQt(QObject *parent = nullptr);
    ~ZegoEventHandlerQt() override;

    void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;

    void onEngineStateUpdate(ZegoEngineState state) override;

    void onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string &extendData) override;
    void onRoomExtraInfoUpdate(const std::string& roomID, const std::vector<ZegoRoomExtraInfo>& roomExtraInfoList) override;
    void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList) override;
    void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList, const std::string& extendData) override;
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
    void onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels) override;

    void onIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoBroadcastMessageInfo> messageList) override;
    void onIMRecvBarrageMessage(const std::string& roomID, std::vector<ZegoBarrageMessageInfo> messageList) override;
    void onIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command) override;

    void onPerformanceStatusUpdate(const ZegoPerformanceStatus& status) override;
    void onNetworkSpeedTestError(int errorCode, ZegoNetworkSpeedTestType type) override;
    void onNetworkSpeedTestQualityUpdate(const ZegoNetworkSpeedTestQuality& quality, ZegoNetworkSpeedTestType type) override;

signals:
    void sigDebugError(int errorCode, const std::string& funcName, const std::string& info);

    void sigEngineStateUpdate(ZegoEngineState state);

    void sigRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string &extendData);
    void sigRoomExtraInfoUpdate(const std::string& roomID, const std::vector<ZegoRoomExtraInfo>& roomExtraInfoList);
    void sigRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList);
    void sigRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList, const std::string& extendData);
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
    void sigMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels);

    void sigIMRecvBroadcastMessage(const std::string& roomID, std::vector<ZegoBroadcastMessageInfo> messageList);
    void sigIMRecvBarrageMessage(const std::string& roomID, std::vector<ZegoBarrageMessageInfo> messageList);
    void sigIMRecvCustomCommand(const std::string& roomID, ZegoUser fromUser, const std::string& command);

    void sigPerformanceStatusUpdate(const ZegoPerformanceStatus& status);
    void sigNetworkSpeedTestError(int errorCode, ZegoNetworkSpeedTestType type);
    void sigNetworkSpeedTestQualityUpdate(const ZegoNetworkSpeedTestQuality& quality, ZegoNetworkSpeedTestType type);

public slots:
};

#endif // ZEGOEVENTHANDLERQT_H









