
#ifndef __IZEGOEVENTHANDLER_H__
#define __IZEGOEVENTHANDLER_H__

#include "ZegoExpressDefine.h"

namespace ZEGO {
    namespace EXPRESS {

    class IZegoEventHandler
    {
    protected:

    virtual ~IZegoEventHandler() {

    }

    public:
    
    /**
     * 调试错误信息回调
     * 调用 SDK 接口出现异常时，会通过该回调提示详细的异常信息，该回调受 setDebugVerbose接口控制。
     * @param errorCode 错误码，详情请参考常用错误码文档
     * @param funcName 接口名
     * @param info 错误的详细信息
     */
    virtual void onDebugError(int /*errorCode*/, const std::string& /*funcName*/, const std::string& /*info*/) {

    }


    /**
     * 房间状态变化通知
     * 当房间的连接状态发生变化时将触发此回调，并告知发生变化的原因
     * @param roomID 房间 ID
     * @param state 变化后的的房间状态
     * @param errorCode 错误码，详情请参考常用错误码文档
     */
    virtual void onRoomStateUpdate(const std::string& /*roomID*/, ZegoRoomState /*state*/, int /*errorCode*/) {

    }


    /**
     * 房间内用户进出通知
     * 当有用户进入/退出房间时将触发此回调，并告知当前房间内存在的用户的列表；注意，只有在登录房间时传的 ZegoRoomConfig 中的 isUserStatusNotify 参数为 true 时才会触发这个回调
     * @param roomID 房间 ID
     * @param updateType 更新类型（添加/删除）
     * @param userList 当前房间内的用户列表
     */
    virtual void onRoomUserUpdate(const std::string& /*roomID*/, ZegoUpdateType /*updateType*/, const std::vector<ZegoUser>& /*userList*/) {

    }


    /**
     * 房间内流更新通知
     * 用户首次登录房间时会接收到房间内存在的所有流列表；后续当房间内新增/删除流时将触发此回调通知变更的流列表
     * @param roomID 房间ID
     * @param updateType 更新类型（添加/删除）
     * @param streamList 更新的流列表
     */
    virtual void onRoomStreamUpdate(const std::string& /*roomID*/, ZegoUpdateType /*updateType*/, const std::vector<ZegoStream>& /*streamList*/) {

    }


    /**
     * 房间内流附加信息更新通知
     * 当房间内有其他推流的用户更新了流的附加信息时，本端会收到该回调
     * @param roomID 房间ID
     * @param streamList 流附加信息更新的流列表
     */
    virtual void onRoomStreamExtraInfoUpdate(const std::string& /*roomID*/, const std::vector<ZegoStream>& /*streamList*/) {

    }


    /**
     * 推流状态回调
     * 在推流成功后，可以通过该回调接口获取推流状态变更的通知。
     * @param streamID 推流的流 ID
     * @param state 推流状态
     * @param errorCode 推流状态变更对应的错误码。请参考常见错误码文档
     */
    virtual void onPublisherStateUpdate(const std::string& /*streamID*/, ZegoPublisherState /*state*/, int /*errorCode*/) {

    }


    /**
     * 推流质量回调
     * 推流成功后每3秒会收到此回调，通过该回调可以获取推送的音视频流的采集帧率，码率，RTT，丢包率等质量数据，实时监控推送流的健康情况。
     * @param streamID 流 ID
     * @param quality 推流质量，包含了音视频帧率、码率、分辨率，RTT等值
     */
    virtual void onPublisherQualityUpdate(const std::string& /*streamID*/, const ZegoPublishStreamQuality& /*quality*/) {

    }


    /**
     * 推流端采集首帧回调
     * 
     * 调用推流接口成功后，SDK 采集到第一帧数据时会收到此回调，包含了音频/视频的采集首帧通知。
     * @param event 推流的首帧事件，包含音频首帧和视频首帧
     */
    virtual void onPublisherRecvFirstFrameEvent(ZegoPublisherFirstFrameEvent /*event*/) {

    }


    /**
     * 采集视频大小变更回调
     * 推流成功后，在推流中途如果有改变视频采集分辨率发生变化将会收到此回调。
     * @param width 视频采集分辨率宽
     * @param height 视频采集分辨率高
     */
    virtual void onPublisherVideoSizeChanged(int /*width*/, int /*height*/) {

    }


    /**
     * 添加/删除转推 CDN 地址状态回调
     * 在 ZEGO 实时音视频云将音视频流转推到 CDN 后，如果 CDN 转推状态发生变化，例如出现转推停止或转推重试，将会收到此回调。
     * @param streamID 流 ID
     * @param streamInfoList 当前 CDN 正在转推的信息列表
     */
    virtual void onPublisherRelayCDNStateUpdate(const std::string& /*streamID*/, const std::vector<ZegoStreamRelayCDNInfo>& /*streamInfoList*/) {

    }


    /**
     * 拉流状态回调
     * 在拉流成功后，可以通过该回调接口获取拉流状态变更的通知。
     * @param streamID 流 ID
     * @param state 拉流状态
     * @param errorCode 错误码
     */
    virtual void onPlayerStateUpdate(const std::string& /*streamID*/, ZegoPlayerState /*state*/, int /*errorCode*/) {

    }


    /**
     * 拉流质量回调
     * 拉流成功后每3秒会收到此回调，通过该回调可以获取拉取的音视频流的帧率，码率，RTT，丢包率等质量数据，实时监控拉取流的健康情况。
     * @param streamID 流 ID
     * @param quality 拉流质量
     */
    virtual void onPlayerQualityUpdate(const std::string& /*streamID*/, const ZegoPlayStreamQuality& /*quality*/) {

    }


    /**
     * 拉流媒体事件回调
     * 当拉流发生音视频卡顿以及恢复等事件发生时会触发此回调。
     * @param streamID 流 ID
     * @param event 拉流媒体事件枚举
     */
    virtual void onPlayerMediaEvent(const std::string& /*streamID*/, ZegoPlayerMediaEvent /*event*/) {

    }


    /**
     * 拉流首帧回调
     * 
     * 调用推流接口成功后，当拉取到音频首帧、视频首帧以及视频首帧被渲染都会触发此回调。
     * @param streamID 流 ID
     * @param event 拉流时收到的具体首帧事件
     */
    virtual void onPlayerRecvFirstFrameEvent(const std::string& /*streamID*/, ZegoPlayerFirstFrameEvent /*event*/) {

    }


    /**
     * 拉流分辨率改变回调
     * 拉流成功后，在拉流中途如果有视频分辨率发生变化将会收到此回调，用户可根据流的最终分辨率调整显示。
     * @param streamID 流 ID
     * @param width 视频分辨率宽
     * @param height 视频分辨率高
     */
    virtual void onPlayerVideoSizeChanged(const std::string& /*streamID*/, int /*width*/, int /*height*/) {

    }


    /**
     * 收到远端流的 SEI 内容
     * 拉流成功后，当远端流调用 sendSEI 后，本端会收到此回调
     * @param streamID 流 ID
     * @param data SEI 内容
     * @param dataLength SEI 内容长度
     */
    virtual void onPlayerRecvSEI(const std::string& /*streamID*/, const unsigned char* /*data*/, unsigned int /*dataLength*/) {

    }


    /**
     * 混流转推 CDN 状态更新通知
     * 
     * @param taskID 混流任务 ID
     * @param infoList 当前 CDN 正在混流的信息列表
     */
    virtual void onMixerRelayCDNStateUpdate(const std::string& /*taskID*/, const std::vector<ZegoStreamRelayCDNInfo>& /*infoList*/) {

    }


    /**
     * 音频设备状态改变
     * 监测到系统中有音频设备添加或移除时，会触发此回调。通过监听此回调，用户可在必要的时候更新使用特定设备进行声音采集或输出。
     * @param updateType 更新类型（添加/删除）
     * @param deviceType 音频设备类型
     * @param deviceInfo 音频设备信息
     */
    virtual void onAudioDeviceStateChanged(ZegoUpdateType /*updateType*/, ZegoAudioDeviceType /*deviceType*/, const ZegoDeviceInfo& /*deviceInfo*/) {

    }


    /**
     * 视频设备状态改变
     * 监测到系统中有视频设备添加或移除时，会触发此回调。通过监听此回调，用户可在必要的时候更新使用特定设备进行视频采集。
     * @param updateType 更新类型（添加/删除）
     * @param deviceInfo 视频设备信息
     */
    virtual void onVideoDeviceStateChanged(ZegoUpdateType /*updateType*/, const ZegoDeviceInfo& /*deviceInfo*/) {

    }


    /**
     * 本地采集音频声浪回调
     * 
     * @param soundLevel 本地采集的声浪值，取值范围为 0.0~100.0
     */
    virtual void onCapturedSoundLevelUpdate(double /*soundLevel*/) {

    }


    /**
     * 远端音频声浪回调
     * 
     * @param soundLevels 远端的声浪键值对，key 为流 ID，value 为对应的流的声浪值
     */
    virtual void onRemoteSoundLevelUpdate(const std::map<std::string, double>& /*soundLevels*/) {

    }


    /**
     * 本地采集音频频谱回调
     * 
     * @param audioSpectrum 本地采集的音频频谱值数组，频谱值范围为 [0-2^30]
     *  
     */
    virtual void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& /*audioSpectrum*/) {

    }


    /**
     * 远端拉流音频频谱回调
     * 
     * @param audioSpectrums 远端音频频谱键值对，key 是流 ID，value 为对应的流的音频频谱值数组，频谱值范围为 [0-2^30]
     *  
     */
    virtual void onRemoteAudioSpectrumUpdate(const std::map<std::string, ZegoAudioSpectrum>& /*audioSpectrums*/) {

    }


    /**
     * 设备异常通知
     * 当本地设备读写出现异常时会触发此回调。
     * @param errorCode 错误码
     * @param deviceName 设备名称
     */
    virtual void onDeviceError(int /*errorCode*/, const std::string& /*deviceName*/) {

    }


    /**
     * 远端摄像头设备状态通知
     * 远端摄像头设备状态发生变更时，例如开关摄像头等，通过监听此回调，能够获取远端摄像头相关的事件，可以用于提示用户可能导致视频异常的情况。
     * @param streamID 流 ID
     * @param state 远端摄像头状态
     */
    virtual void onRemoteCameraStateUpdate(const std::string& /*streamID*/, ZegoRemoteDeviceState /*state*/) {

    }


    /**
     * 远端麦克风设备状态通知
     * 远端麦克风设备状态发生变更时，例如开关麦克风等，通过监听此回调，能够获取远端麦克风相关的事件，可以用于提示用户可能导致音频异常的情况。
     * @param streamID 流 ID
     * @param state 远端麦克风状态
     */
    virtual void onRemoteMicStateUpdate(const std::string& /*streamID*/, ZegoRemoteDeviceState /*state*/) {

    }


    /**
     * 接收房间广播消息通知
     * 
     * @param roomID 房间 ID
     * @param messageList 收到的消息列表
     */
    virtual void onIMRecvBroadcastMessage(const std::string& /*roomID*/, std::vector<ZegoMessageInfo> /*messageList*/) {

    }


    /**
     * 接收自定义信令通知
     * 
     * @param roomID 房间 ID
     * @param fromUser 信令的发送人
     * @param command 信令内容
     */
    virtual void onIMRecvCustomCommand(const std::string& /*roomID*/, ZegoUser /*fromUser*/, const std::string& /*command*/) {

    }



    };

} //namespace EXPRESS
} //namespace ZEGO

#endif // __IZEGOEVENTHANDLER_H__
