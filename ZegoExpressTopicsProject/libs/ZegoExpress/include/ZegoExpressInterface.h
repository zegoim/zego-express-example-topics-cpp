
#ifndef __ZEGOEXPRESSINTERFACE_H__
#define __ZEGOEXPRESSINTERFACE_H__

#include "ZegoExpressDefines.h"
#include "ZegoExpressEventHandler.h"

namespace ZEGO {
    namespace EXPRESS {

        class IZegoMediaPlayer;


        class IZegoExpressEngine
        {
        protected:

            virtual ~IZegoExpressEngine() {}

        public:

            /**
             * 设置事件通知回调，传 [nullptr] 则清空已设置的回调
             *
             * 调用此接口将覆盖 [createEngine] 时传入的回调
             * @param eventHandler 事件通知回调。开发者应根据自身业务场景，重写回调的相关方法以关注特定的通知。SDK 主要的回调方法都在 [IZegoEventHandler] 里。
             */
            virtual void setEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) = 0;

            /**
             * 上传日志到 ZEGO 服务器
             *
             * 默认情况下，SDK 会在 App 默认目录创建日志文件并打印，每个日志文件默认最大 5MB，三个日志文件循环覆盖写入。当调用此接口时 SDK 会自动将日志文件打包并上传到 ZEGO 服务器。
             * 开发者可在 App 提供业务上的”反馈“渠道，当用户反馈问题时可调用此接口将 SDK 的本地日志信息上传以协助定位用户问题。
             */
            virtual void uploadLog() = 0;

            /**
             * 设置调试详细信息开关以及语言
             *
             * 默认开启且调试信息的语言为英文。
             * @param enable 详细调试信息开关
             * @param language 调试信息语种
             */
            virtual void setDebugVerbose(bool enable, ZegoLanguage language) = 0;

            /**
             * 登录房间，推拉流前必须登录房间
             *
             * 登录同一个房间的不同用户可以得到以相同房间为单位的房间信令通知（例如：[onRoomUserUpdate], [onRoomStreamUpdate] 等），一个房间内的用户收不到另一个房间房间信令的通知。
             * 一个房间发的消息（例如 [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand] 等）在别的房间无法收到（例如 [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand] 等），目前 SDK 未提供跨房间消息的能力。开发者可以集成第三方 IM 的 SDK 来实现。
             * SDK 支持拉相同 appID 下非同一个房间的流，即跨房间拉流。由于 SDK 的房间信令的相关回调通知是以相同房间为单位，当开发者想要跨房间拉流时，开发者需自行维护相关的消息及信令通知。
             * 如果由于网络质量原因导致网络临时中断，SDK 内部会自动进行重连。可通过监听 [onRoomStateUpdate] 回调获取本端当前房间连接状态的变化情况，同时同房间其他用户会接收到 [onRoomUserUpdate] 回调通知。
             * @param roomID 房间 ID，最大长度为 128 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param user 用户对象实例，配置用户 ID、用户名。 注意用户 ID 需要在相同的 appID 下全局唯一，否则会出现后登陆的用户踢掉先登陆的用户的情况。
             */
            virtual void loginRoom(const std::string& roomID, ZegoUser user) = 0;

            /**
             * 登录房间，推拉流前必须登录房间
             *
             * 为了防止 App 被恶意用户模拟登录，可以在登录房间之前加上鉴权验证，即 [config] 参数传入的 ZegoRoomConfig 对象中的 [token] 参数
             * 登录同一个房间的不同用户可以得到以相同房间为单位的房间信令通知（例如：[onRoomUserUpdate], [onRoomStreamUpdate] 等），一个房间内的用户收不到另一个房间房间信令的通知。
             * 一个房间发的消息（例如 [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand] 等）在别的房间无法收到（例如 [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand] 等），目前 SDK 未提供跨房间消息的能力。开发者可以集成第三方 IM 的 SDK 来实现。
             * SDK 支持拉相同 appID 下非同一个房间的流，即跨房间拉流。由于 SDK 的房间信令的相关回调通知是以相同房间为单位，当开发者想要跨房间拉流时，开发者需自行维护相关的消息及信令通知。
             * 如果由于网络质量原因导致网络临时中断，SDK 内部会自动进行重连。可通过监听 [onRoomStateUpdate] 回调获取本端当前房间连接状态的变化情况，同时同房间其他用户会接收到 [onRoomUserUpdate] 回调通知。
             * @param roomID 房间 ID，最大长度为 128 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param user 用户对象实例，配置用户 ID、用户名。 注意用户 ID 需要在相同的 appID 下全局唯一，否则会出现后登陆的用户踢掉先登陆的用户的情况。
             * @param config 房间进阶配置
             */
            virtual void loginRoom(const std::string& roomID, ZegoUser user, ZegoRoomConfig config) = 0;

            /**
             * 退出房间
             *
             * 退出房间会停止该用户的所有推拉流并且 SDK 内部会主动停止本地预览，调用该接口后会收到 [onRoomStateUpdate] 回调通知成功退出房间，同时同房间其他用户会接收到 [onRoomUserUpdate] 回调通知。
             * @param roomID 房间 ID，最大长度为 128 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             */
            virtual void logoutRoom(const std::string& roomID) = 0;

            /**
             * 开始推流
             *
             * 可通过此接口让用户将自己本地的音视频流推送到 ZEGO 实时音视频云，同一房间的其他用户通过 streamID 就可以拉取该音视频流进行互通。
             * 在开始推流前，需要先调用 [loginRoom] 加入房间，当推流成功之后，同房间内其他用户可通过监听 [onRoomStreamUpdate] 事件回调来获取该 streamID 新增。
             * 在网络质量不佳的情况下，用户推流可能出现中断，SDK 会尝试重新连接，可通过监听 [onPublisherStateUpdate] 事件来获知当前推流状态以及错误信息。
             * @param streamID 流 ID，长度不超过256的字符串，需要在整个 AppID 内全局唯一，若出现在同一个 AppID 内，不同的用户各推了一条流且流名相同，将会导致后推流的用户推流失败。不可以包含 URL 关键字，否则推拉流失败。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。
             */
            virtual void startPublishingStream(const std::string& streamID) = 0;

            /**
             * 开始推流，可选择推第二路流
             *
             * 可通过此接口让用户将自己本地的音视频流推送到 ZEGO 实时音视频云，同一房间的其他用户通过 streamID 就可以拉取该音视频流进行互通。
             * 在开始推流前，需要先调用 [loginRoom] 加入房间，当推流成功之后，同房间内其他用户可通过监听 [onRoomStreamUpdate] 事件回调来获取该 streamID 新增。
             * 在网络质量不佳的情况下，用户推流可能出现中断， SDK 会尝试重新连接，可通过监听 [onPublisherStateUpdate] 事件来获知当前推流状态以及错误信息。
             * @param streamID 流 ID，长度不超过256的字符串，需要在整个 AppID 内全局唯一，若出现在同一个 AppID 内，不同的用户各推了一条流且流名相同，将会导致后推流的用户推流失败。不可以包含 URL 关键字，否则推拉流失败。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。
             * @param channel 推流通道
             */
            virtual void startPublishingStream(const std::string& streamID, ZegoPublishChannel channel) = 0;

            /**
             * 停止推流
             *
             * 可通过此接口让用户停止发送本地的音视频流，结束通话。
             * 如果用户已经启动推流，在推新流（新的 streamID）之前，必须要调用此接口停止当前流的推送，否则新流推送会返回失败。
             * 在停止推流之后，开发者应该根据业务情况是否需要而停止本地预览。
             */
            virtual void stopPublishingStream() = 0;

            /**
             * 停止推流，可停止指定通道的音视频流
             *
             * 可通过此接口让用户停止发送本地的音视频流，结束通话。
             * 如果用户已经启动推流，在推新流（新的 streamID）之前，必须要调用此接口停止当前流的推送，否则新流推送会返回失败。
             * 在停止推流之后，开发者应该根据业务情况是否需要而停止本地预览。
             * 当使用推第二路音视频流的接口推流时使用此接口进行停止推流。
             * @param channel 推流通道
             */
            virtual void stopPublishingStream(ZegoPublishChannel channel) = 0;

            /**
             * 设置流附加信息
             *
             * 可通过此接口设置当前推流的流附加信息，设置结果会通过 [ZegoPublisherSetStreamExtraInfoCallback] 回调通知。
             * 流附加信息是流 ID 的附加信息标识，不同于流 ID 在推流过程中不可修改，流附加信息可以在对应流 ID 的推流中途修改。
             * 开发者可根据流附加信息来实现流 ID 相关的可变内容的同步。
             * @param extraInfo 流附加信息，长度不超过1024的字符串
             * @param callback 更新流附加信息执行结果通知
             */
            virtual void setStreamExtraInfo(const std::string& extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback) = 0;

            /**
             * 设置流附加信息，支持设置其他通道的推流
             *
             * 可通过此接口设置当前推流的流附加信息，设置结果会通过 [ZegoPublisherSetStreamExtraInfoCallback] 回调通知。
             * 流附加信息是流 ID 的附加信息标识，不同于流 ID 在推流过程中不可修改，流附加信息可以在对应流 ID 的推流中途修改。
             * 开发者可根据流附加信息来实现流 ID 相关的可变内容的同步。
             * @param extraInfo 流附加信息，长度不超过1024的字符串
             * @param channel 推流通道
             * @param callback 更新流附加信息执行结果通知
             */
            virtual void setStreamExtraInfo(const std::string& extraInfo, ZegoPublishChannel channel, ZegoPublisherSetStreamExtraInfoCallback callback) = 0;

            /**
             * 启动/更新本地预览
             *
             * 用户通过调用此接口可以看到自己本地的画面。预览功能不需要先登陆房间或推流。但是在退出房间之后 SDK 内部默认会主动停止预览。
             * 可以通过再次调用该接口来更新本地视图和视图预览模式。
             * 可以通过调用 [setVideoMirrorMode] 接口来设置预览的镜像模式，默认为开启预览镜像。
             * 当调用此接口后 SDK 内部的音视频引擎模块会启动，会开始尝试开始采集音频与视频。开发者除了正常调用此接口以便预览本端画面之外，还可以传 [nullptr] 给 canvas 参数，配合 SDK 音浪的功能以便达到在登录房间之前检测音频设备是否正常工作的目的。
             * @param canvas 启动预览时用于显示画面的视图，视图设置为 nullptr 则不进行预览
             */
            virtual void startPreview(ZegoCanvas* canvas) = 0;

            /**
             * 启动/更新本地预览，支持设置其他通道的推流
             *
             * 用户通过调用此接口可以看到自己本地的画面。预览功能不需要先登陆房间或推流。但是在退出房间之后 SDK 内部默认会主动停止预览。
             * 可以通过再次调用该接口来更新本地视图和视图预览模式。
             * 可以通过调用 [setVideoMirrorMode] 接口来设置预览的镜像模式，默认为开启预览镜像。
             * 当调用此接口后 SDK 内部的音视频引擎模块会启动，会开始尝试开始采集音频与视频。开发者除了正常调用此接口以便预览本端画面之外，还可以传 [nullptr] 给 canvas 参数，配合 SDK 音浪的功能以便达到在登录房间之前检测音频设备是否正常工作的目的。
             * @param canvas 启动预览时用于显示画面的视图，视图设置为 nullptr 则不进行预览
             * @param channel 推流通道
             */
            virtual void startPreview(ZegoCanvas* canvas, ZegoPublishChannel channel) = 0;

            /**
             * 停止本地预览
             *
             * 当本地不需要看到预览时可调用此接口停止预览。
             */
            virtual void stopPreview() = 0;

            /**
             * 停止本地预览
             *
             * 当本地不需要看到预览时可调用此接口停止预览。
             * @param channel 推流通道
             */
            virtual void stopPreview(ZegoPublishChannel channel) = 0;

            /**
             * 设置视频配置
             *
             * 可通过此接口设置视频帧率、码率，视频采集分辨率，视频编码输出分辨率。如果不调用此接口，默认分辨率为 360p，码率为 600 kbps，帧率为 15 fps。
             * 需要在推流前设置好相关视频配置，在推流后仅支持编码分辨率和码率的修改。
             * 开发者应该注意的是，移动端的宽高分辨率与 PC 端的宽高分辨率是相反的，例如移动端的 360p 的分辨率，移动端的分辨率为 360x640，而 PC 端 360p 的分辨率为 640x360。
             * @param config 视频配置，SDK 提供常用的分辨率、帧率和码率的组合值，也可自定义分辨率、帧率和码率
             */
            virtual void setVideoConfig(ZegoVideoConfig config) = 0;

            /**
             * 设置视频配置，支持设置其他推流
             *
             * 可通过此接口设置视频帧率、码率，视频采集分辨率，视频编码输出分辨率。如果不调用此接口，默认分辨率为 360p，码率为 600 kbps，帧率为 15 fps。
             * 需要在推流前设置好相关视频配置，在推流后仅支持编码分辨率和码率的修改。
             * 开发者应该注意的是，移动端的宽高分辨率与 PC 端的宽高分辨率是相反的，例如移动端的 360p 的分辨率，移动端的分辨率为 360x640，而 PC 端 360p 的分辨率为 640x360。
             * @param config 视频配置，SDK 提供常用的分辨率、帧率和码率的组合值，也可自定义分辨率、帧率和码率
             * @param channel 推流通道
             */
            virtual void setVideoConfig(ZegoVideoConfig config, ZegoPublishChannel channel) = 0;

            /**
             * 设置镜像模式
             *
             * 可调用此接口来设置本地预览视频以及推送的视频是否开启镜像模式。
             * @param mirrorMode 预览或推流的镜像模式
             */
            virtual void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode) = 0;

            /**
             * 设置镜像模式，支持设置其他通道的推流
             *
             * 可调用此接口来设置本地预览视频以及推送的视频是否开启镜像模式。
             * @param mirrorMode 预览或推流的镜像模式
             * @param channel 推流通道
             */
            virtual void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, ZegoPublishChannel channel) = 0;

            /**
             * 设置音频质量配置
             *
             * 可通过此接口设置音频编码类型、码率，音频声道的组合值。如果不调用此接口，默认为标准音质模式。仅支持推流前设置。
             * 若预设的值无法满足开发者的场景，开发者可自行根据业务要求设置符合的参数。
             * @param config 音频质量配置
             */
            virtual void setAudioConfig(ZegoAudioConfig config) = 0;

            /**
             * 停止或恢复发送音频流
             *
             * 推流时可调用此接口实现只推视频流不推音频，本地仍会采集和处理音频，但不向网络发送音频数据。可以在推流前设置。
             * 如果在本地设置了停止发送音频流，拉本地流的用户可以通过监听 [onRemoteMicStateUpdate] 回调收到 `Mute` 的状态变更通知。
             * @param mute 是否停止发送音频流；true 表示只发送视频流不发送音频流；false 表示同时发送音频和视频流；默认为 false
             */
            virtual void mutePublishStreamAudio(bool mute) = 0;

            /**
             * 停止或恢复发送音频流，支持设置其他路推流
             *
             * 推流时可调用此接口实现只推视频流不推音频，本地仍会采集和处理音频，但不向网络发送音频数据。可以在推流前设置。
             * 如果在本地设置了停止发送音频流，拉本地流的用户可以通过监听 [onRemoteMicStateUpdate] 回调收到 `Mute` 的状态变更通知。
             * @param mute 是否停止发送音频流；true 表示只发送视频流不发送音频流；false 表示同时发送音频和视频流；默认为 false
             * @param channel 推流通道
             */
            virtual void mutePublishStreamAudio(bool mute, ZegoPublishChannel channel) = 0;

            /**
             * 停止或恢复发送视频流
             *
             * 推流时可调用此接口实现只推音频流不推视频流，本地摄像头仍能正常工作，能正常采集，预览和处理视频画面，但不向网络发送视频数据。可以在推流前设置。
             * 如果在本地设置了停止发送视频流，拉本地流的用户可以通过监听 [onRemoteCameraStateUpdate] 回调收到 `Mute` 的状态变更通知。
             * @param mute 是否停止发送视频流；true 表示只发送音频流不发送视频流；false 表示同时发送音频和视频流；默认为 false
             */
            virtual void mutePublishStreamVideo(bool mute) = 0;

            /**
             * 停止或恢复发送视频流，支持设置其他路推流
             *
             * 推流时可调用此接口实现只推音频流不推视频流，本地摄像头仍能正常工作，能正常采集，预览和处理视频画面，但不向网络发送视频数据。可以在推流前设置。
             * 如果在本地设置了停止发送视频流，拉本地流的用户可以通过监听 [onRemoteCameraStateUpdate] 回调收到 `Mute` 的状态变更通知。
             * @param mute 是否停止发送视频流；true 表示只发送音频流不发送视频流；false 表示同时发送音频和视频流；默认为 false
             * @param channel 推流通道
             */
            virtual void mutePublishStreamVideo(bool mute, ZegoPublishChannel channel) = 0;

            /**
             * 开始或停止流量控制
             *
             * 流量控制可以使 SDK 根据自己以及拉本地流的用户当前网络环境状态来动态调整音视频推流的码率。
             * 自动适应当前网络环境及网络波动，从而保证直播的流畅发布
             * @param enable 是否使用流量控制；true 表示开启流控；false 表示关闭流控；默认为 true
             * @param property 流量控制的可调节的属性，位掩码格式。具体可设置为 [ZegoTrafficControlProperty] 的属性的一个或多个枚举组合。默认为 AdaptiveFPS
             */
            virtual void enableTrafficControl(bool enable, int property) = 0;

            /**
             * 设置流量控制视频码率最低值
             *
             * 当网络不足以发送这个码率最低值，可选择视频发送模式。
             * 不调用此接口时，默认情况 SDK 会根据当前网络上行情况自动调整所发送的视频数据帧。
             * @param bitrate 最低视频码率，单位为 kbps
             * @param mode 低于最低码率时的视频发送模式
             */
            virtual void setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode) = 0;

            /**
             * 设置推流端采集音量
             *
             * 此接口用于设置音频的采集音量，本端用户可控制往远端发送音频流的声音大小。可以在推流前设置。
             * @param volume 音量百分比，范围为 0 ~ 100，默认值为 100。
             */
            virtual void setCaptureVolume(int volume) = 0;

            /**
             * 增加转推至 CDN 的 URL
             *
             * 调用此接口，将已推往 ZEGO 实时音视频云的音视频流转推至延迟高但是支持高并发拉流的自定义的 CDN 内容分发网络。
             * 由于调用该接口时本质上是将推往 ZEGO 音视频云端的音视频流动态转推至不同的 CDN ，因此此接口需要在推流成功后调用。
             * 由于 ZEGO 的音视频云服务本身可配置支持 CDN 内容分发网络，该接口主要为自身拥有 CDN 内容分发服务的开发者使用。
             * 可以同时在使用 ZEGO 的 CDN 音视频流内容分发服务的同时通过调用此接口再使用开发者自身拥有 CDN 内容分发服务的开发者使用。
             * 该接口支持动态转推至 CDN 内容分发网络，因此开发者可以使用该接口来作为 CDN 内容分发服务的一个容灾方案。
             * 当调用 [enablePublishDirectToCDN] 接口设置为 true 将流直推到 CDN 时，再调用本接口将无效。
             * @param streamID 流 ID
             * @param targetURL CDN 转推地址，支持的转推地址格式为 rtmp。
             * @param callback 添加 CDN 转推结果通知
             */
            virtual void addPublishCdnUrl(const std::string& streamID, const std::string& targetURL, ZegoPublisherUpdateCdnUrlCallback callback) = 0;

            /**
             * 删除转推至 CDN 的 URL
             *
             * 当已经添加了某个 CDN 转推地址，需要将流停止转推至该接口时调用此接口。
             * 该接口并不会停止推往 ZEGO 音视频云的音视频流。
             * @param streamID 流 ID
             * @param targetURL CDN 转推地址，支持的转推地址格式有 rtmp，flv，hls
             * @param callback 移除 CDN 转推结果通知
             */
            virtual void removePublishCdnUrl(const std::string& streamID, const std::string& targetURL, ZegoPublisherUpdateCdnUrlCallback callback) = 0;

            /**
             * 是否不经过 Zego 实时视频云服务器直接推流到 CDN
             *
             * 此接口需要推流前设置。
             * 调用此接口将音视频流直接推往 CDN 之后，调用 [addPublishCdnUrl] 与 [removePublishCdnUrl] 动态转推至 CDN 不再生效，因为这两个接口是从 ZEGO 实时音视频云将音视频流转推或停止转推到 CDN，若直接音视频流直接推往 CDN 将无法通过 ZEGO 实时音视频云将音视频流再动态转推至 CDN。
             * @param enable 是否开启直推 CDN；true 表示开启直推 CDN；false 表示不开启直推 CDN；默认为 false
             * @param config CDN 配置，若为 nullptr 则使用 Zego 的后台配置
             */
            virtual void enablePublishDirectToCDN(bool enable, ZegoCDNConfig* config) = 0;

            /**
             * 是否不经过 Zego 实时视频云服务器直接推流到 CDN，支持设置其他路推流
             *
             * 此接口需要推流前设置。
             * 调用此接口将音视频流直接推往 CDN 之后，调用 [addPublishCdnUrl] 与 [removePublishCdnUrl] 动态转推至 CDN 不再生效，因为这两个接口是从 ZEGO 实时音视频云将音视频流转推或停止转推到 CDN，若直接音视频流直接推往 CDN 将无法通过 ZEGO 实时音视频云将音视频流再动态转推至 CDN。
             * @param enable 是否开启直推 CDN；true 表示开启直推 CDN；false 表示不开启直推 CDN；默认为 false
             * @param config CDN 配置，若为 nullptr 则使用 Zego 的后台配置
             * @param channel 推流通道
             */
            virtual void enablePublishDirectToCDN(bool enable, ZegoCDNConfig* config, ZegoPublishChannel channel) = 0;

            /**
             * 设置推流水印
             *
             * 推流前设置，水印的布局不能超出推流的视频编码分辨率。
             * @param watermark 水印布局左上角为坐标系原点，区域不能超过编码分辨率设置的大小。若为空表示取消水印。
             * @param isPreviewVisible 是否本地预览能看见水印
             */
            virtual void setPublishWatermark(ZegoWatermark* watermark, bool isPreviewVisible) = 0;

            /**
             * 设置推流水印，支持设置其他路推流
             *
             * 推流前设置，水印的布局不能超出推流的视频编码分辨率。
             * @param watermark 水印布局左上角为坐标系原点，区域不能超过编码分辨率设置的大小。若为空表示取消水印。
             * @param isPreviewVisible 是否本地预览能看见水印
             * @param channel 推流通道
             */
            virtual void setPublishWatermark(ZegoWatermark* watermark, bool isPreviewVisible, ZegoPublishChannel channel) = 0;

            /**
             * 发送媒体增强补充信息
             *
             * 此接口可在开发者推流传输音视频流数据同时，发送流媒体增强补充信息来同步一些其他附加信息。
             * 一般如同步音乐歌词或视频画面精准布局等场景，可选择使用发送 SEI。
             * 当推流方发送 SEI 后，拉流方可通过监听 [onPlayerRecvSEI] 的回调获取 SEI 内容。
             * 由于 SEI 信息跟随视频帧或音频帧，由于网络问题有可能丢帧，因此 SEI 信息也有可能丢，为解决这种情况，应该在限制频率内多发几次。
             * 限制频率：1秒钟不要超过30次。
             * 注意：该接口只在有推视频数据的时候有效果，不推视频数据 SEI 信息将发不出去。即关闭摄像头，音视频流不推视频数据，拉流不拉视频数据都将收不到 SEI 信息。
             * SEI 数据长度限制为 4096 字节。
             * @param data SEI 内容
             * @param dataLength SEI 内容长度
             */
            virtual void sendSEI(const unsigned char* data, unsigned int dataLength) = 0;

            /**
             * 发送媒体增强补充信息， 支持设置其他路推流
             *
             * 此接口可在开发者推流传输音视频流数据同时，发送流媒体增强补充信息来同步一些其他附加信息。
             * 一般如同步音乐歌词或视频画面精准布局等场景，可选择使用发送 SEI。
             * 当推流方发送 SEI 后，拉流方可通过监听 [onPlayerRecvSEI] 的回调获取 SEI 内容。
             * 由于 SEI 信息跟随视频帧，由于网络问题有可能丢帧，因此 SEI 信息也有可能丢，为解决这种情况，应该在限制频率内多发几次。
             * 限制频率：1秒钟不要超过30次。
             * 注意：该接口只在有推视频数据的时候有效果，不推视频数据 SEI 信息将发不出去。即关闭摄像头，音视频流不推视频数据，拉流不拉视频数据都将收不到 SEI 信息。
             * SEI 数据长度限制为 4096 字节。
             * @param data SEI 内容
             * @param dataLength SEI 内容长度
             * @param channel 推流通道
             */
            virtual void sendSEI(const unsigned char* data, unsigned int dataLength, ZegoPublishChannel channel) = 0;

            /**
             * 开/关硬件编码
             *
             * 推流时是否采用硬件编码的开关，开启硬解编码后会使用 GPU 进行编码，降低 CPU 使用率。在推流前设置才能生效，如果在推流后设置，停推后重新推流可以生效。
             * 由于少部分机型设备硬编支持不是特别好，SDK 默认使用软件编码的方式。若开发者在某些机型测试时发现推大分辨率音视频流时设备发热严重，可考虑调用此接口开启硬编的方式。
             * @param enable 是否开启硬件编码；true 表示开启硬编；false 表示关闭硬编；默认为 false
             */
            virtual void enableHardwareEncoder(bool enable) = 0;

            /**
             * 设置采集缩放时机，视频数据是采集的时候立即缩放还是编码时才进行缩放
             *
             * 该接口需在预览或推流前设置有效。
             * 主要影响是当采集分辨率与编码分辨率不同的时候，是否影响本地预览情况。
             * @param mode 采集缩放时机
             */
            virtual void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) = 0;

            /**
             * 开始拉流
             *
             * 可通过此接口让用户可以从 ZEGO 音视频云拉取远端用户的音视频流进行互通。
             * 在开始拉流前，需要先加入房间，可通过监听 [onRoomStreamUpdate] 事件回调来获取该房间内 streamID 的新增。
             * 在网络质量不佳的情况下，用户拉流可能出现中断， SDK 会尝试重新连接，可通过监听 [onPlayerStateUpdate] 事件来获知当前拉流状态以及错误信息。
             * 拉取不存在的流 ID，执行本接口后 SDK 持续尝试拉取，在该流 ID 被成功推送后，音视频流可以真正被拉取到。
             * 开发者可通过再次调用此接口实现更新拉流 Canvas 的操作（流 ID 必须一样）。
             * @param streamID 流 ID，长度不超过256的字符串。不可以包含 URL 关键字，否则推拉流失败。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。
             * @param canvas 用于显示拉流画面的视图，视图设置为 [nullptr] 则不进行显示
             */
            virtual void startPlayingStream(const std::string& streamID, ZegoCanvas* canvas) = 0;

            /**
             * 开始拉流
             *
             * 除了可通过此接口让用户可以从 ZEGO 实时音视频云拉取远端用户的音视频流进行互通外，还可以通过此接口让用户可以从第三方 URL 拉取远端用户的音视频流。
             * 在开始拉流前，需要先加入房间，可通过监听 [onRoomStreamUpdate] 事件回调来获取该房间内 streamID 的新增。
             * 在网络质量不佳的情况下，用户拉流可能出现中断， SDK 会尝试重新连接，可通过监听 [onPlayerStateUpdate] 事件来获知当前拉流状态以及错误信息。
             * 拉取不存在的流 ID，执行本接口后 SDK 持续尝试拉取，在该流 ID 被成功推送后，音视频流可以真正被拉取到。
             * 开发者可通过再次调用此接口实现更新拉流 Canvas 的操作（流 ID 必须一样）。
             * @param streamID 流 ID，长度不超过256的字符串。不可以包含 URL 关键字，否则推拉流失败。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。
             * @param canvas 用于显示拉流画面的视图，视图设置为 [nullptr] 则不进行显示
             * @param config 拉流进阶配置
             */
            virtual void startPlayingStream(const std::string& streamID, ZegoCanvas* canvas, ZegoPlayerConfig config) = 0;

            /**
             * 停止拉流
             *
             * 可通过此接口让用户停止拉取远端的音视频流。停止拉流后对此条流此前设置的属性如 [setPlayVolume]、[mutePlayStreamAudio]、[mutePlayStreamVideo] 等都会失效，需要在下次拉流时重新设置。
             * @param streamID 流 ID
             */
            virtual void stopPlayingStream(const std::string& streamID) = 0;

            /**
             * 设置拉流音量
             *
             * 此接口用于设置拉流的声音大小，本端用户可控制音频流的播放音量。需要在调用拉流接口后调用。
             * 在停止拉流，重新拉流之后需要重新设置。
             * @param streamID 流 ID
             * @param volume 音量百分比，取值范围为 0 ~ 100，默认值为 100
             */
            virtual void setPlayVolume(const std::string& streamID, int volume) = 0;

            /**
             * 停止或恢复拉取音频流
             *
             * 拉流时可调用此接口实现不拉取远端用户的音频数据，需要在调用拉流接口后调用。
             * 该接口仅对从 ZEGO 实时音视频云(非 ZEGO CDN 或 第三方 CDN)拉流才有效果。
             * @param streamID 流 ID
             * @param mute 禁用标识；true 表示禁止拉取；false 表示恢复拉取
             */
            virtual void mutePlayStreamAudio(const std::string& streamID, bool mute) = 0;

            /**
             * 停止或恢复拉取视频流
             *
             * 拉流时可调用此接口实现不拉取远端用户的视频数据，需要在调用拉流接口后调用。
             * 该接口仅对从 ZEGO 实时音视频云(非 ZEGO CDN 或 第三方 CDN)拉流才有效果。
             * @param streamID 流 ID
             * @param mute 禁用标识；true 表示禁止拉取；false 表示恢复拉取
             */
            virtual void mutePlayStreamVideo(const std::string& streamID, bool mute) = 0;

            /**
             * 开/关硬件解码
             *
             * 拉流时是否使用硬件解码，开启硬件解码后 SDK 会使用 GPU 进行解码，降低 CPU 使用率。在拉流前设置才能生效，如果在拉流后设置，停止拉流后重新拉流可以生效。
             * 由于少部分机型设备硬解支持不是特别好，SDK 默认使用软件解码的方式。若开发者在某些机型测试时发现拉大分辨率音视频流时设备发热严重，可考虑调用此接口开启解编的方式。
             * @param enable 是否开启硬解开关；true 表示开启硬解；false 表示关闭硬解；默认为 false
             */
            virtual void enableHardwareDecoder(bool enable) = 0;

            /**
             * 开/关帧顺序检测
             *
             * @param enable 是否开启帧顺序检测；true 表示开启帧顺序检测，不支持 B 帧；false 表示关闭帧顺序检测，支持 B 帧，可能出现短暂花屏；默认为 true
             */
            virtual void enableCheckPoc(bool enable) = 0;

            /**
             * 开始混流任务
             *
             * 由于客户端设备的性能考虑，SDK 的混流是在 ZEGO 音视频云的服务端开启混流任务进行混流。
             * 调用此接口后 SDK 向 ZEGO 音视频云发起混流请求，ZEGO 音视频云会寻找当前正在推得流，并根据 SDK 请求的混流任务的参数进行图层混合。
             * 若请求开启混流任务发生异常，例如最常见的混流的输入流不存在，将会从 callback 回调的错误码给出。具体错误码请参考常见错误码文档 [https://doc-zh.zego.im/zh/308.html]
             * 若中途某条输入流不存在了，混流任务会自动重试拉这条输入流 90 秒，90 秒之后不再重试。
             * @param task 混流任务对象
             * @param callback 开始混流任务结果通知
             */
            virtual void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) = 0;

            /**
             * 停止混流任务
             *
             * 与 [startMixerTask] 类似，调用该接口之后 SDK 向 ZEGO 音视频云的服务端发起结束混流的请求。
             * 若开发者在未停止上一个混流任务的情况下启动下一个混流任务，上一个混流任务不会自动停止，直到上一个混流任务的输入流都不存在之后 90 秒之后，上一个混流任务才会自动结束。
             * 开发者在使用 ZEGO 音视频云的服务的混流功能时应该注意再启动下一个混流任务时，应该停止上一个混流任务，以免造成主播已经开启下一个混流任务与其他主播混流，观众依然在一直拉上一个混流任务的输出流的情况。
             * @param task 混流任务对象
             * @param callback 停止混流任务结果通知
             */
            virtual void stopMixerTask(ZegoMixerTask task, ZegoMixerStopCallback callback) = 0;

            /**
             * 是否关闭麦克风输入
             *
             * 此接口用于控制是否使用采集到的音频数据，关闭麦克风即采集到数据后就丢弃，此时仍然会占用麦克风。
             * 依然占用麦克风是因为硬件上关闭或打开麦克风是比较重的操作，有一定的性能开销，且真实用户存在频繁操作的可能。为了权衡性能原因，此接口仅是简单的丢弃采集到的数据。
             * 若想要真正让 SDK 放弃占用麦克风，如实现退到后台释放麦克风占用等场景，可调用 [enableAudioCaptureDevice] 接口。
             * 开发者若想在 UI 上控制是否使用麦克风采集，应使用此接口，避免使用 [enableAudioCaptureDevice] 而带来不必要的性能开销。
             * @param mute 是否关闭麦克风；true 表示关闭麦克风；false 表示开启麦克风，默认为 true。
             */
            virtual void muteMicrophone(bool mute) = 0;

            /**
             * 是否关闭音频输出
             *
             * 关闭后 SDK 所有声音都不会播放，包括拉流、媒体播放器等。但 SDK 仍会占用输出设备。
             * @param mute 是否关闭音频输出；true 表示关闭音频输出；false 表示开启音频输出。默认为 false
             */
            virtual void muteSpeaker(bool mute) = 0;

            /**
             * 选择使用某个音频设备
             *
             * @param deviceType 音频设备类型
             * @param deviceID 通过 getAudioDeviceList: 获取的某个设备的 ID
             */
            virtual void useAudioDevice(ZegoAudioDeviceType deviceType, const std::string& deviceID) = 0;

            /**
             * 获取音频设备列表
             *
             * @param deviceType 音频设备类型
             * @return 音频设备列表
             */
            virtual std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) = 0;

            /**
             * 开/关音频采集设备
             *
             * 此接口用于控制是否释放音频采集设备，关闭音频采集设备，则 SDK 不会再占用音频设备，当然如果此时正在推流，也不会产生音频数据。
             * 占用音频采集设备与放弃占用音频设备是比较重的操作，一般推荐使用 [muteMicrophone] 接口。
             * @param enable 是否开启音频采集设备；true 表示打开音频采集设备；false 表示关闭音频采集设备
             */
            virtual void enableAudioCaptureDevice(bool enable) = 0;

            /**
             * 开/关摄像头
             *
             * 此接口用于控制是否启动摄像头的采集，关闭摄像头后，将不会进行视频采集，此时推流也将没有视频数据
             * @param enable 是否打开摄像头；true 表示打开摄像头；false 表示关闭摄像头
             */
            virtual void enableCamera(bool enable) = 0;

            /**
             * 开/关摄像头，支持设置其他通道的推流
             *
             * 此接口用于控制是否启动摄像头的采集，关闭摄像头后，将不会进行视频采集，此时推流也将没有视频数据
             * @param enable 是否打开摄像头；true 表示打开摄像头；false 表示关闭摄像头
             * @param channel 推流通道
             */
            virtual void enableCamera(bool enable, ZegoPublishChannel channel) = 0;

            /**
             * 选择使用某个视频设备
             *
             * @param deviceID 通过 getVideoDeviceList: 获取的某个设备的 ID
             */
            virtual void useVideoDevice(const std::string& deviceID) = 0;

            /**
             * 选择使用某个视频设备，支持设置其他通道的推流
             *
             * @param deviceID 通过 getVideoDeviceList: 获取的某个设备的 ID
             * @param channel 推流通道
             */
            virtual void useVideoDevice(const std::string& deviceID, ZegoPublishChannel channel) = 0;

            /**
             * 获取视频设备列表
             *
             * @return 视频设备列表
             */
            virtual std::vector<ZegoDeviceInfo> getVideoDeviceList() = 0;

            /**
             * 启动声浪监控
             *
             * 启动监控后可通过 [onCapturedSoundLevelUpdate] 回调接收本地采集音频声浪回调，以及 [onRemoteSoundLevelUpdate] 回调接收远端拉流音频声浪回调。
             * 开发者可在进入房间之前，调用 [startPreview] 与此接口，并与 [onCapturedSoundLevelUpdate] 结合来判断音频设备是否正常工作。
             * [onCapturedSoundLevelUpdate] 与 [onRemoteSoundLevelUpdate] 回调通知周期为 100 ms。
             */
            virtual void startSoundLevelMonitor() = 0;

            /**
             * 停止声浪监控
             *
             * 停止监控后将停止回调本地采集以及远端拉流的音频声浪回调。
             */
            virtual void stopSoundLevelMonitor() = 0;

            /**
             * 启动音频频谱监控
             *
             * 启动监控后可通过 [onCapturedAudioSpectrumUpdate] 回调接收本地采集音频频谱回调，以及 [onRemoteAudioSpectrumUpdate] 回调接收远端音频声浪回调。
             * [onCapturedAudioSpectrumUpdate] 与 [onRemoteAudioSpectrumUpdate] 回调通知周期为 100 ms。
             */
            virtual void startAudioSpectrumMonitor() = 0;

            /**
             * 停止音频频谱监控
             *
             * 停止监控后将停止回调本地采集以及远端拉流的音频频谱回调。
             */
            virtual void stopAudioSpectrumMonitor() = 0;

            /**
             * 开/关回声消除
             *
             * 打开回声消除， SDK 会对采集到的音频数据进行过滤以降低音频中的回音成分。需要在开始推流之前设置，推流开始后设置无效。
             * @param enable 是否开启回声消除；true 表示开启回声消除；false 表示关闭回声消除
             */
            virtual void enableAEC(bool enable) = 0;

            /**
             * 设置回声消除模式
             *
             * 切换不同的回声消除模式以控制消除回声数据的程度。需要在开始推流之前设置。
             * @param mode 回声消除模式
             */
            virtual void setAECMode(ZegoAECMode mode) = 0;

            /**
             * 开/关自动增益
             *
             * 开启自动增益后声音会被放大，但会一定程度上影响音质。需要在开始推流之前设置。
             * @param enable 是否开启自动增益；true 表示开启自动增益；false 表示关闭自动增益
             */
            virtual void enableAGC(bool enable) = 0;

            /**
             * 开/关噪声抑制
             *
             * 开启噪声抑制可以使人声更加清晰。需要在开始推流之前设置，推流开始后设置无效。
             * @param enable 是否开启噪声抑制；true 表示开启噪声抑制；false 表示关闭噪声抑制
             */
            virtual void enableANS(bool enable) = 0;

            /**
             * 发送房间广播消息
             *
             * 广播消息 与 自定义信令 消息的合计的发送频率限制默认为每分钟 600 次。
             * 同房间内的最多前 500 个先进房间的用户能收到，一般用于在直播房间人数小于 500 时使用。
             * @param roomID 房间 ID，最大长度为 128 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param message 消息内容，长度不超过256字节
             * @param callback 发送广播消息结果通知
             */
            virtual void sendBroadcastMessage(const std::string& roomID, const std::string& message, ZegoIMSendBroadcastMessageCallback callback) = 0;

            /**
             * 发送房间弹幕消息
             *
             * 没有发送次数限制，但如果发送太频繁服务端会主动丢消息。
             * 支持同房间内大于 500 人的用户能收到，但不可靠，即房间内用户很多或者用户间发送消息很频繁时，接收消息的用户不一定能收到。一般用于直播弹幕的发送。
             * @param roomID 房间 ID，最大长度为 128 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param message 消息内容，长度不超过256字节
             * @param callback 发送弹幕消息结果通知
             */
            virtual void sendBarrageMessage(const std::string& roomID, const std::string& message, ZegoIMSendBarrageMessageCallback callback) = 0;

            /**
             * 发送自定义信令
             *
             * 广播消息 与 自定义信令 消息的合计的发送频率限制默认为每分钟 600 次。
             * 同房间内点对点的信令类型，一般用于远程控制信令或用户与用户之间的消息发送。
             * @param roomID 房间 ID，最大长度为 128 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param command 自定义信令内容，长度不超过256字节
             * @param toUserList 信令的接收者
             * @param callback 发送信令结果通知
             */
            virtual void sendCustomCommand(const std::string& roomID, const std::string& command, std::vector<ZegoUser> toUserList, ZegoIMSendCustomCommandCallback callback) = 0;

            /**
             * 创建媒体播放器实例对象
             *
             * 目前最多支持创建 4 个实例，超过后将返回 nullptr。媒体播放器的实例越多，对设备的性能开销越大。
             * @return 媒体播放器实例，超过最大数量限制后将返回 null
             */
            virtual IZegoMediaPlayer* createMediaPlayer() = 0;

            /**
             * 销毁媒体播放器实例对象
             *
             * @param mediaPlayer 媒体播放器实例对象
             */
            virtual void destroyMediaPlayer(IZegoMediaPlayer*& mediaPlayer) = 0;

            /**
             * 设置自定义视频渲染回调
             *
             * 自定义视频渲染，由开发者负责把视频数据渲染到 UI 组件上。该功能一般为使用第三方美颜功能或使用第三方渲染框架的开发者使用。
             * 当开发者使用 SDK 的自定义视频渲染的高级功能时需要调用此接口来设置给开发者抛视频数据的回调对象。
             * 当开发者调用启动预览 [startPreview]、开始推流 [startPublishingStream]、开始拉流[startPlayingStream] 时会触发向开发者抛视频数据的回调方法。
             * 开发者可根据 SDK 抛视频数据的回调进行视频画面的渲染。
             * 自定义视频渲染功能可以与自定义视频采集功能同时使用。
             * @param handler 自定义视频渲染回调对象
             */
            virtual void setCustomVideoRenderHandler(std::shared_ptr<IZegoCustomVideoRenderHandler> handler) = 0;

            /**
             * 设置自定义视频采集回调
             *
             * 自定义视频采集，即由开发者负责采集视频数据，并将采集到的视频数据发送给 SDK 进行视频数据的编码与推送到 ZEGO 音视频云端。该功能一般为使用第三方美颜功能或游戏录屏直播的开发者使用。
             * 当开发者使用 SDK 的自定义视频采集的高级功能时，需要调用此接口来设置 SDK 以通知开发者可以开始发送的视频数据给 ZegoExpressEngine。
             * 当开发者调用启动预览 [startPreview]、开始推流 [startPublishingStream] 时会触发通知开发者可以开始发送视频数据的回调方法。当停止预览 [stopPreview] 且 停止推流[stopPublishingStream] 时会触发通知开发者可以停止发送视频数据的回调方法。
             * 由于使用自定义视频采集时，SDK 将不再启动摄像头去采集视频数据，开发者需自行对视频采集源进行视频数据的采集。
             * 自定义视频采集功能可以与自定义视频渲染功能同时使用。
             * @param handler 自定义视频采集回调对象
             */
            virtual void setCustomVideoCaptureHandler(std::shared_ptr<IZegoCustomVideoCaptureHandler> handler) = 0;

            /**
             * 给 SDK 发送自定义采集的视频帧原始数据
             *
             * 该接口应该在 [onStart] 通知的之后开始调用，在 [onStop] 通知之后结束调用。
             * @param data 要向 SDK 发送的视频帧数据
             * @param dataLength 视频帧数据长度
             * @param params 视频帧的参数
             * @param referenceTimeMillisecond 视频帧的索引时间，单位为毫秒
             */
            virtual void sendCustomVideoCaptureRawData(const unsigned char* data, unsigned int dataLength, ZegoVideoFrameParam params, unsigned long long referenceTimeMillisecond) = 0;

            /**
             * 给 SDK 发送自定义采集的视频帧裸数据，支持其他路推流
             *
             * 该接口应该在 [onStart] 通知的之后开始调用，在 [onStop] 通知之后结束调用。
             * @param data 要向 SDK 发送的视频帧数据
             * @param dataLength 视频帧数据长度
             * @param params 视频帧的参数
             * @param referenceTimeMillisecond 视频帧的索引时间，单位为毫秒
             * @param channel 推流通道
             */
            virtual void sendCustomVideoCaptureRawData(const unsigned char* data, unsigned int dataLength, ZegoVideoFrameParam params, unsigned long long referenceTimeMillisecond, ZegoPublishChannel channel) = 0;

            /**
             * 设置自定义视频采集画面缩放填充模式
             *
             * @param mode 画面填充缩放模式
             */
            virtual void setCustomVideoCaptureFillMode(ZegoViewMode mode) = 0;

            /**
             * 设置自定义视频采集画面缩放填充模式，支持其他路推流
             *
             * @param mode 画面填充缩放模式
             * @param channel 推流通道
             */
            virtual void setCustomVideoCaptureFillMode(ZegoViewMode mode, ZegoPublishChannel channel) = 0;

            /**
             * 开/关音频输出至设备
             *
             * 关闭后 SDK 将不会再传递音频数据给输出设备，以达到静音的效果
             * @deprecated 该接口已废弃，请使用 [muteSpeaker]
             * @param mute 是否关闭音频输出到设备；true 表示关闭音频输出；false 表示开启音频输出
             */
            virtual void muteAudioOutput(bool mute) = 0;

        };
        class IZegoMediaPlayer
        {
        protected:

            virtual ~IZegoMediaPlayer() {}

        public:

            /**
             * 设置媒体播放器的事件回调 handler
             *
             * 开发者可以根据媒体播放器的相关事件回调进行播放器 UI 界面的变化
             * @param handler 媒体播放器的事件回调对象
             */
            virtual void setEventHandler(std::shared_ptr<IZegoMediaPlayerEventHandler> handler) = 0;

            /**
             * 设置视频回调 handler
             *
             * 可以通过设置此回调将媒体播放器播放的媒体资源文件的视频数据抛出来
             * @param handler 媒体播放器的视频事件回调对象
             * @param format 视频数据的视频帧格式
             */
            virtual void setVideoHandler(std::shared_ptr<IZegoMediaPlayerVideoHandler> handler, ZegoVideoFrameFormat format) = 0;

            /**
             * 设置视频回调 handler
             *
             * 可以通过设置此回调将媒体播放器播放的媒体资源文件的音频数据抛出来
             * @param handler 媒体播放器的音频事件回调对象
             */
            virtual void setAudioHandler(std::shared_ptr<IZegoMediaPlayerAudioHandler> handler) = 0;

            /**
             * 加载媒体资源
             *
             * 可传本地资源的绝对路径或者网络资源的 URL
             * @param path 本地资源路径或网络资源的 URL
             * @param callback 资源加载结果的通知
             */
            virtual void loadResource(std::string path, ZegoMediaPlayerLoadResourceCallback callback) = 0;

            /**
             * 开始播放
             *
             * 必须在加载资源完成后才能调用
             */
            virtual void start() = 0;

            /**
             * 停止播放
             *
             */
            virtual void stop() = 0;

            /**
             * 暂停播放
             *
             */
            virtual void pause() = 0;

            /**
             * 恢复播放
             *
             */
            virtual void resume() = 0;

            /**
             * 设置指定的播放进度
             *
             * 单位为毫秒
             * @param millisecond 指定的播放进度的时间点
             * @param callback 设置指定的播放进度的结果通知
             */
            virtual void seekTo(unsigned long long millisecond, ZegoMediaPlayerSeekToCallback callback) = 0;

            /**
             * 是否重复播放
             *
             * @param enable 重复播放标记，默认为 false
             */
            virtual void enableRepeat(bool enable) = 0;

            /**
             * 是否将播放器的声音混入正在推的流中
             *
             * @param enable 是否混音标记，默认为 false
             */
            virtual void enableAux(bool enable) = 0;

            /**
             * 是否静默本地播放
             *
             * 若开启了混音入流则推的流中仍然有声音，默认为 false。
             * @param mute 本地静音标记，默认为 false。
             */
            virtual void muteLocal(bool mute) = 0;

            /**
             * 设置播放器播放视频的视图
             *
             * @param canvas 视频渲染的画布对象
             */
            virtual void setPlayerCanvas(ZegoCanvas* canvas) = 0;

            /**
             * 设置播放器音量
             *
             * @param volume 范围为 0 ~ 100，默认为 50。
             */
            virtual void setVolume(int volume) = 0;

            /**
             * 设置播放进度回调间隔
             *
             * 可通过此接口控制 [onMediaPlayerPlayingProgress] 的回调频率，当设置回调间隔为 0 时，停止回调。默认回调间隔为 1s
             * 回调不会严格按照设定的回调间隔值返回，而是以处理音频帧或者视频帧的频率来判断是否需要回调
             * @param millisecond 播放进度回调间隔时间，单位为毫秒
             */
            virtual void setProgressInterval(unsigned long long millisecond) = 0;

            /**
             * 获取当前音量
             *
             * 范围为 0 ~ 100，默认为 50
             */
            virtual int getVolume() = 0;

            /**
             * 获取媒体资源的总进度
             *
             * 必须在加载资源完成后才能调用，否则返回值为 0
             * @return 单位为毫秒
             */
            virtual unsigned long long getTotalDuration() = 0;

            /**
             * 获取当前播放进度
             *
             * 必须在加载资源完成后才能调用，否则返回值为 0
             */
            virtual unsigned long long getCurrentProgress() = 0;

            /**
             * 获取当前播放状态
             *
             */
            virtual ZegoMediaPlayerState getCurrentState() = 0;

        };


    }
}
#endif // __ZEGOEXPRESSINTERFACE_H__
