
#ifndef __IZEGOEXPRESSENGINE_H__
#define __IZEGOEXPRESSENGINE_H__

#include "ZegoExpressDefine.h"
#include "IZegoEventHandler.h"
#include "IZegoMediaPlayer.h"

namespace ZEGO {
    namespace EXPRESS {

    class IZegoExpressEngine
    {
    protected:
        virtual ~IZegoExpressEngine() {}

    public:
    
    /**
     * 添加事件通知回调，最多支持添加 16 个
     * 
     * @param eventHandler 事件通知回调
     */
    virtual void addEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) = 0;


    /**
     * 移除事件通知回调
     * 
     * @param eventHandler 事件通知回调
     */
    virtual void removeEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) = 0;


    /**
     * 上传日志到 ZEGO 服务器
     * 
     */
    virtual void uploadLog() = 0;


    /**
     * 设置调试详细信息开关以及语言
     * 默认开启且调试信息的语言为英文。
     * @param enable 详细调试信息开关
     * @param language 调试信息语种
     */
    virtual void setDebugVerbose(bool enable, ZegoLanguage language) = 0;


    /**
     * 登录房间
     * 推拉流前必须登录房间，在同一个房间内的用户可以相互通话
     * 如果由于网络质量原因导致网络临时中断，SDK 内部会自动进行重连
     * 可通过监听  onRoomStateUpdate  代理方法获取本端当前房间连接状态的变化情况，同时同房间其他用户会接收到 onRoomUserUpdate 回调通知。
     * @param roomID 房间ID，最大长度为256字节的字符串
     * @param user 用户对象实例，配置用户 ID、用户名
     * @param config 房间高级配置，传空将使用默认配置
     */
    virtual void loginRoom(std::string roomID, ZegoUser user, ZegoRoomConfig* config) = 0;


    /**
     * 登录房间
     * 如果有较高的安全需求请使用此接口
     * 推拉流前必须登录房间，在同一个房间内的用户可以相互通话
     * 如果由于网络质量原因导致网络临时中断，SDK 内部会自动进行重连
     * 可通过监听 onRoomStateUpdate 代理方法获取本端当前房间连接状态的变化情况，同时同房间其他用户会接收到 onRoomUserUpdate 回调通知。
     * @param roomID 房间ID，最大长度为256字节的字符串
     * @param user 用户对象实例，配置用户 ID、用户名
     * @param config 房间高级配置，传空将使用默认配置
     * @param token 由开发者业务服务器下发的 token，用以保证安全性，生成规则详见 https://doc.zego.im/CN/565.html
     */
    virtual void loginRoom(std::string roomID, ZegoUser user, ZegoRoomConfig* config, std::string token) = 0;


    /**
     * 退出房间会停止该用户的所有推拉流，调用该接口后会收到  onRoomStateUpdate 回调通知成功退出房间，同时同房间其他用户会接收到 onRoomUserUpdate 回调通知。
     * 
     * @param roomID 用户已登录的房间 ID
     */
    virtual void logoutRoom(std::string roomID) = 0;


    /**
     * 开始推流
     * 可通过此接口让用户将自己本地的音视频流推送到 ZEGO 实时音视频云，同一房间的其他用户通过 streamID 就可以拉取该音视频流进行互通。
     * 在开始推流前，需要先加入房间，同房间内其他用户可通过监听 onRoomStreamUpdate 事件回调来获取该 streamID 新增。
     * 在网络质量不佳的情况下，用户推流可能出现中断， SDK 会尝试重新连接，可通过监听 onPublisherStateUpdate 事件来获知当前推流状态以及错误信息。
     * @param streamID 流 ID
     */
    virtual void startPublishing(std::string streamID) = 0;


    /**
     * 停止推流
     * 可通过此接口让用户停止发送本地的音视频流，结束通话。如果用户已经启动推流，在推新流（新的streamID）之前，必须要调用此接口停止当前流的推送，否则新流推送会返回失败。
     */
    virtual void stopPublishing() = 0;


    /**
     * 设置流附加信息
     * 可通过此接口设置当前推流的流附加信息，设置结果会通过 ZegoPublisherSetStreamExtraInfoCallback 回调通知。
     * @param extraInfo 流附加信息，长度不超过1024的字符串。
     * @param callback 更新流附加信息执行结果通知
     */
    virtual void setStreamExtraInfo(std::string extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback) = 0;


    /**
     * 启动/更新本地预览
     * 用户通过调用此接口可以看到自己本地的画面。预览功能不需要先登陆房间或推流。
     * 可以通过再次调用该接口来更新本地视图和视图预览模式。
     * 可以通过调用 setVideoMirrorMode 接口来设置预览的镜像模式，默认为开启预览镜像。
     * @param canvas 启动预览时用于显示画面的视图，视图设置为空则不进行预览。
     */
    virtual void startPreview(ZegoCanvas* canvas) = 0;


    /**
     * 停止本地预览
     * 当本地不需要看到预览画面时可调用此接口停止预览。
     */
    virtual void stopPreview() = 0;


    /**
     * 设置视频配置
     * 可通过此接口设置视频帧率、码率，视频采集分辨率，视频编码输出分辨率。如果不调用此接口，默认分辨率为 360p，码率为 600 kbps，帧率为 15 fps。需要在推流前设置好相关视频配置，在推流后仅支持编码分辨率和码率的修改。
     * @param videoConfig 视频配置，SDK 提供常用的分辨率、帧率和码率的组合值，也可自定义分辨率、帧率和码率
     */
    virtual void setVideoConfig(ZegoVideoConfig videoConfig) = 0;


    /**
     * 设置镜像模式
     * 可调用此接口来设置本地预览视频以及推送的视频是否开启镜像模式。
     * @param mirrorMode 预览或推流的镜像模式
     */
    virtual void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode) = 0;


    /**
     * 设置音频质量配置
     * 可通过此接口设置音频编码类型、码率，声道数的组合值。如果不调用此接口，默认为“普通延迟-标准音质”模式。仅支持推流前设置。
     * @param config 音频质量配置
     */
    virtual void setAudioConfig(ZegoAudioConfig config) = 0;


    /**
     * 停止或恢复发送音频流
     * 推流时可调用此接口实现只推视频流不推音频，本地仍会采集和处理音频，但不向网络发送音频数据。可以在推流前设置。
     * 如果在本地设置了停止发送音频流，对端可以通过监听 onRemoteMicStateUpdate 回调收到 ZEGO_REMOTE_DEVICE_STATE_MUTE 的状态变更通知。
     * @param mute 是否停止发送音频流，true 表示只发送视频流不发送音频流，false 表示同时发送音频和视频流。默认为false。
     */
    virtual void mutePublishStreamAudio(bool mute) = 0;


    /**
     * 停止或恢复发送视频流
     * 推流时可调用此接口实现只推音频流不推视频流，本地摄像头仍能正常工作，能正常采集，预览和处理视频画面，但不向网络发送视频数据。可以在推流前设置。
     * 如果在本地设置了停止发送视频流，对端可以通过监听 onRemoteCameraStateUpdate 回调收到 ZEGO_REMOTE_DEVICE_STATE_MUTE 的状态变更通知。
     * @param mute 是否停止发送视频流，true 表示只发送音频流不发送视频流，false 表示同时发送音频和视频流。默认为false。
     */
    virtual void mutePublishStreamVideo(bool mute) = 0;


    /**
     * 设置推流端采集音量
     * 此接口用于设置音频的采集音量，本端用户可控制往远端发送音频流的声音大小。可以在推流前设置。
     * @param volume 音量百分比，默认值为100
     */
    virtual void setCaptureVolume(int volume) = 0;


    /**
     * 增加转推至CDN的URL
     * 当需要将音视频流转推到其它指定的 CDN 时需要调用此接口进行设置。
     * @param streamID 需要转推的流 ID
     * @param targetURL CDN 转推地址，支持的转推地址格式有 rtmp，flv，hls
     * @param callback 配置转推CDN操作的执行结果通知，可根据执行结果进行下一步处理。
     */
    virtual void addPublishCDNURL(std::string streamID, std::string targetURL, ZegoPublisherUpdateCDNURLCallback callback) = 0;


    /**
     * 删除转推至CDN的URL 
     * 一般是和 addPublishCDNURL 配合使用，当添加了某个转推地址，后面想停止该地址的转推时调用此接口。
     * @param streamID 需要停止转推的流 ID
     * @param targetURL CDN 转推地址，支持的转推地址格式有 rtmp，flv，hls
     * @param callback 移除 CDN 转推结果通知
     */
    virtual void removePublishCDNURL(std::string streamID, std::string targetURL, ZegoPublisherUpdateCDNURLCallback callback) = 0;


    /**
     * 设置推流水印
     * 推流前设置，水印的布局不能超出推流的视频编码分辨率
     * @param watermark 水印布局左上角为坐标系原点，区域不能超过编码分辨率设置的大小
     * @param isPreviewVisible 是否本地预览能看见水印
     */
    virtual void setPublishWatermark(ZegoWatermark* watermark, bool isPreviewVisible) = 0;


    /**
     * 发送媒体增强补充信息
     * 此接口可在开发者推流传输音视频流数据同时，发送流媒体增强补充信息来同步一些其他附加信息。
     * 一般如同步音乐歌词或视频画面精准布局等场景，可选择使用发送 SEI。
     * 当推流方发送 SEI 后，拉流方可通过监听 onPlayerRecvSEI 的回调获取SEI内容。
     * @param data SEI 内容
     * @param dataLength SEI 内容长度
     */
    virtual void sendSEI(const unsigned char* data, unsigned int dataLength) = 0;


    /**
     * 开/关硬件编码
     * 推流时是否采用硬件编码的开关，开启硬解编码后会使用GPU进行编码，降低CPU使用率。在推流前设置才能生效，如果在推流后设置，停推后重新推流可以生效。
     * @param enable 是否开启硬件编码。true 表示开启硬件编码，false 表示关闭硬件编码。
     */
    virtual void enableHardwareEncoder(bool enable) = 0;


    /**
     * 设置采集时机
     * 
     * @param mode 采集时机
     */
    virtual void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) = 0;


    /**
     * 开始拉流
     * 可通过此接口让用户可以从 ZEGO 实时音视频云拉取远端用户的音视频流进行互通。
     * 在开始拉流前，需要先加入房间，可通过监听 onRoomStreamUpdate 事件回调来获取该房间内 streamID 的新增。
     * 在网络质量不佳的情况下，用户拉流可能出现中断， SDK 会尝试重新连接，可通过监听 onPlayerStateUpdate 事件来获知当前拉流状态以及错误信息。
     * 拉取不存在的流 ID，执行本接口后 SDK 持续尝试拉取，在该流 ID 被成功推送后，音视频流可以真正被拉取到。
     * 开发者可通过再次调用此接口实现更换拉流 Canvas 的操作（流 ID 必须一样）。
     * @param streamID 流 ID
     * @param canvas 用于显示拉流画面的视图，视图设置为空则不进行显示。
     */
    virtual void startPlayingStream(std::string streamID, ZegoCanvas* canvas) = 0;


    /**
     * 停止拉流
     * 可通过此接口让用户停止拉取远端的音视频流。停止拉流后对此条流此前设置的属性如 setPlayVolume、mutePlayStreamAudio、mutePlayStreamVideo等都会失效，需要在下次拉流时重新设置。
     * @param streamID 流 ID
     */
    virtual void stopPlayingStream(std::string streamID) = 0;


    /**
     * 设置拉流音量
     * 此接口用于设置流的播放音量。需要在调用拉流接口后调用。
     * @param streamID 流 ID
     * @param volume 音量百分比
     */
    virtual void setPlayVolume(std::string streamID, int volume) = 0;


    /**
     * 停止或恢复拉取音频流
     * 拉流时可调用此接口实现停止拉取/重新拉取远端用户的音频数据，需要在调用拉流接口后调用。
     * @param streamID 流 ID
     * @param mute 禁用标识。true:禁止拉取；false:恢复拉取。
     */
    virtual void mutePlayStreamAudio(std::string streamID, bool mute) = 0;


    /**
     * 停止或恢复拉取视频流
     * 拉流时可调用此接口实现停止拉取/重新拉取远端用户的视频数据，需要在调用拉流接口后调用。
     * @param streamID 流 ID
     * @param mute 禁用标识。true:禁止拉取；false:恢复拉取。
     */
    virtual void mutePlayStreamVideo(std::string streamID, bool mute) = 0;


    /**
     * 开/关硬件解码
     * 拉流时是否使用硬件解码，开启硬件解码后SDK会使用GPU进行解码，降低CPU使用率。在拉流前设置才能生效，如果在拉流后设置，停止拉流后重新拉流可以生效。
     * @param enable 是否开启硬解开关。true:启用硬解；false:关闭硬解。
     */
    virtual void enableHarewareDecoder(bool enable) = 0;


    /**
     * 开/关帧顺序检测
     * 
     * @param enable true  检测帧顺序，不支持B帧； false 不检测帧顺序，支持B帧，可能出现短暂花屏
     */
    virtual void enableCheckPoc(bool enable) = 0;


    /**
     * 开始混流任务
     * 
     * @param task 混流任务对象
     * @param callback 开始混流任务结果通知
     */
    virtual void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) = 0;


    /**
     * 停止混流任务
     * 
     * @param taskID 混流任务 ID
     */
    virtual void stopMixerTask(std::string taskID) = 0;


    /**
     * 开/关麦克风
     * 
     * 此接口用于控制是否使用采集到的音频数据，关闭麦克风即采集到数据后就丢弃，此时仍然会占用麦克风
     * @param mute 是否开启麦克风，true 表示关闭麦克风；false 表示开启麦克风
     */
    virtual void muteMicrophone(bool mute) = 0;


    /**
     * 开/关音频输出至设备
     * 关闭后SDK将不会再传递音频数据给输出设备，以达到静音的效果
     * @param mute 是否关闭音频输出到设备，true表示关闭音频输出；false 表示开启音频输出
     */
    virtual void muteAudioOutput(bool mute) = 0;


    /**
     * 选择使用某个音频设备
     * 
     * @param deviceType 音频设备类型
     * @param deviceID 通过getAudioDeviceList获取的某个设备的ID
     */
    virtual void useAudioDevice(ZegoAudioDeviceType deviceType, std::string deviceID) = 0;


    /**
     * 获取音频设备列表
     * 
     * @param deviceType 音频设备类型
     * @return (std::vector<ZegoDeviceInfo>), 音频设备列表
     */
    virtual std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) = 0;


    /**
     * 开/关音频采集设备
     * 
     * 此接口用于控制是否释放音频采集设备，关闭音频采集设备，则 SDK 不会再占用音频设备，当然如果此时正在推流，也没有音频数据。
     * @param enable 是否使能音频采集设备，true 表示开启音频采集设备，false 表示关闭音频采集设备
     */
    virtual void enableAudioCaptureDevice(bool enable) = 0;


    /**
     * 开/关摄像头
     * 此接口用于控制是否启动摄像头的采集，关闭摄像头后，将不会进行视频采集，此时推流也将没有视频数据
     * @param enable 是否打开摄像头，false 表示关闭摄像头；true 表示开启摄像头
     */
    virtual void enableCamera(bool enable) = 0;


    /**
     * 选择使用某个视频设备
     * 
     * @param deviceID 通过getVideoDeviceList获取的某个设备的ID
     */
    virtual void useVideoDevice(std::string deviceID) = 0;


    /**
     * 获取视频设备列表
     * 
     * @return (std::vector<ZegoDeviceInfo>), 视频设备列表
     */
    virtual std::vector<ZegoDeviceInfo> getVideoDeviceList() = 0;


    /**
     * 启动声浪监控
     * 
     */
    virtual void startSoundLevelMonitor() = 0;


    /**
     * 停止声浪监控
     * 
     */
    virtual void stopSoundLevelMonitor() = 0;


    /**
     * 启动音频频谱监控
     * 
     */
    virtual void startAudioSpectrumMonitor() = 0;


    /**
     * 停止音频频谱监控
     * 
     */
    virtual void stopAudioSpectrumMonitor() = 0;


    /**
     * 开/关回声消除
     * 打开回声消除， SDK 会对采集到的音频数据进行过滤以降低音频中的回音成分。需要在开始推流之前设置，推流开始后设置无效。
     * @param enable 是否开启回声消除，false 表示关闭回声消除，true 表示开启回声消除
     */
    virtual void enableAEC(bool enable) = 0;


    /**
     * 设置回声消除模式
     * 切换不同的回声消除模式以控制消除回声数据的程度。需要在开始推流之前设置。
     * @param mode 回声消除模式
     */
    virtual void setAECMode(ZegoAECMode mode) = 0;


    /**
     * 开/关自动增益
     * 开启自动增益后声音会被放大，但会一定程度上影响音质。需要在开始推流之前设置。
     * @param enable 是否开启自动增益，false 表示关闭自动增益，true 表示开启自动增益
     */
    virtual void enableAGC(bool enable) = 0;


    /**
     * 开/关噪声抑制
     * 
     * 开启噪声抑制开关后能降低音频数据中的噪音，使人声更加清晰。需要在开始推流之前设置。
     * @param enable 是否开启噪声抑制，false 表示关闭噪声抑制，true 表示开启噪声抑制
     */
    virtual void enableANS(bool enable) = 0;


    /**
     * 发送房间广播消息
     * 
     * @param roomID 房间 ID
     * @param message 消息内容，长度不超过256字节
     * @param callback 发送广播消息结果通知
     */
    virtual void sendBroadcastMassage(std::string roomID, std::string message, ZegoIMSendBroadcastMessageCallback callback) = 0;


    /**
     * 发送自定义信令
     * 
     * @param roomID 房间 ID
     * @param toUserList 信令的接收者
     * @param command 自定义信令内容，长度不超过256字节
     * @param callback 发送信令结果通知
     */
    virtual void sendCustomCommand(std::string roomID, std::vector<ZegoUser> toUserList, std::string command, ZegoIMSendCustomCommandCallback callback) = 0;



    /**
     * 创建媒体播放器实例
     * 引擎同一时间支持创建最多4个播放器实例，超出后再调用此接口将返回 nullptr
     */
    virtual IZegoExpressMediaPlayer* createMediaPlayer() = 0;


    /**
     * 销毁播放器实例，释放播放器占用资源
     * @param IMediaPlayer 先前调用 CreateMediaPlayer 接口返回播放器实例
     */
    virtual void destroyMediaPlayer(IZegoExpressMediaPlayer *& IMediaPlayer) = 0;
    
    };
}
}
    #endif // __IZEGOEXPRESSENGINE_H__
    