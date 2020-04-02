
#ifndef __ZEGOEXPRESSDEFINES_H__
#define __ZEGOEXPRESSDEFINES_H__

#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#include <sstream>
#include <locale>
#include <codecvt>
#define ZegoStrncpy strncpy_s
#else
#import <Foundation/Foundation.h>
#define ZegoStrncpy strncpy
#endif

#ifndef ZEGO_DISABLE_SWTICH_THREAD
#ifdef WIN32
#define ZEGO_SWITCH_THREAD_PRE  auto hCommWnd = \
        ::FindWindow(ZegoMainThreadTool::ZegoCommWndClassName, ZegoMainThreadTool::ZegoCommWndName); \
        std::function<void(void)>* pFunc = new  std::function<void(void)>;*pFunc = [=](void) {
#define ZEGO_SWITCH_THREAD_ING }; PostMessage(hCommWnd, WM_APP+10086, WPARAM(pFunc), 0);
#else
#define ZEGO_SWITCH_THREAD_PRE dispatch_async(dispatch_get_main_queue(), ^{;
#define ZEGO_SWITCH_THREAD_ING });
#endif
#else
#define ZEGO_SWITCH_THREAD_PRE {
#define ZEGO_SWITCH_THREAD_ING }
#endif

#define ZegoCBridge ZegoExpressEngineBridge::GetInstance()
#define ZegoExpressGD ZegoSingleton<ZegoExpressGlobalData>::CreateInstance()
#define ZEGO_UNUSED_VARIABLE(x) ((void)(x))

#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

namespace ZEGO {
    namespace EXPRESS {
    
        /** 应用使用场景 */
        enum ZegoScenario
        {
            /** 通用场景 */
            ZEGO_SCENARIO_GENERAL = 0,

            /** 实时通讯 */
            ZEGO_SCENARIO_COMMUNICATION = 1,

            /** 直播场景 */
            ZEGO_SCENARIO_LIVE = 2

        };

        /** 语言 */
        enum ZegoLanguage
        {
            /** 英文 */
            ZEGO_LANGUAGE_ENGLISH = 0,

            /** 中文 */
            ZEGO_LANGUAGE_CHINESE = 1

        };

        /** 房间状态 */
        enum ZegoRoomState
        {
            /** 未连接状态，在登陆房间前和退出房间之后进入该状态。如果登陆房间的过程出现稳态异常，例如 AppID 和 AppSign 不正确，或者有相同用户名在其他地方登陆导致本端被 KickOut，都会进入该状态 */
            ZEGO_ROOM_STATE_DISCONNECTED = 0,

            /** 正在请求连接状态，登陆房间动作执行成功后会进入此状态。通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断， SDK 会进行内部重试，也会回到正在请求连接状态 */
            ZEGO_ROOM_STATE_CONNECTING = 1,

            /** 连接成功状态，进入该状态表示登陆房间已经成功，用户可以正常收到房间内的用户和流信息增删的回调通知 */
            ZEGO_ROOM_STATE_CONNECTED = 2

        };

        /** 推流通道 */
        enum ZegoPublishChannel
        {
            /** 主推流通道 */
            ZEGO_PUBLISH_CHANNEL_MAIN = 0,

            /** 辅推流通道 */
            ZEGO_PUBLISH_CHANNEL_AUX = 1

        };

        /** 视频渲染模式 */
        enum ZegoViewMode
        {
            /** 等比缩放，可能有黑边 */
            ZEGO_VIEW_MODE_ASPECT_FIT = 0,

            /** 等比缩放填充整个 View，可能有部分被裁减 */
            ZEGO_VIEW_MODE_ASPECT_FILL = 1,

            /** 填充整个 View，图像可能被拉伸 */
            ZEGO_VIEW_MODE_SCALE_TO_FILL = 2

        };

        /** 预览或拉流端的镜像模式 */
        enum ZegoVideoMirrorMode
        {
            /** 只有本地预览时才是镜像画面，默认采用此模式 */
            ZEGO_VIDEO_MIRROR_MODE_ONLY_PREVIEW_MIRROR = 0,

            /** 本地预览和拉流端看到的视频都是镜像画面 */
            ZEGO_VIDEO_MIRROR_MODE_BOTH_MIRROR = 1,

            /** 本地预览和拉流端看到的视频都不是镜像画面 */
            ZEGO_VIDEO_MIRROR_MODE_NO_MIRROR = 2,

            /** 只有拉流端看到的视频才是镜像画面 */
            ZEGO_VIDEO_MIRROR_MODE_ONLY_PUBLISH_MIRROR = 3

        };

        /** 推流状态 */
        enum ZegoPublisherState
        {
            /** 未推流状态，在推流前处于该状态。如果推流过程出现稳态的异常，例如 AppID 和 AppSign 不正确，或者如果其他用户已经在推送流，推送相同流 ID 的流会失败，都会进入未推流状态 */
            ZEGO_PUBLISHER_STATE_NO_PUBLISH = 0,

            /** 正在请求推流状态，推流操作执行成功后会进入正在请求推流状态，通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断，SDK 会进行内部重试，也会回到正在请求推流状态 */
            ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING = 1,

            /** 正在推流状态，进入该状态表明推流已经成功，用户可以正常通信 */
            ZEGO_PUBLISHER_STATE_PUBLISHING = 2

        };

        /** 视频配置分辨率与比特率预设枚举。预设的分辨率分别针对移动端与桌面端做了适配。在移动端上 height 长于 width，而桌面端相反。例如 1080p 在移动端上实际为 1080(w) x 1920(h)，而在桌面端上实际为 1920(w) x 1080(h) */
        enum ZegoVideoConfigPreset
        {
            /** 设置分辨率为 320x180，默认采用 15 fps，码率 300 kbps */
            ZEGO_VIDEO_CONFIG_PRESET_180P = 0,

            /** 设置分辨率为 480x270，默认采用 15 fps，码率 400 kbps */
            ZEGO_VIDEO_CONFIG_PRESET_270P = 1,

            /** 设置分辨率为 640x360，默认采用 15 fps，码率 600 kbps */
            ZEGO_VIDEO_CONFIG_PRESET_360P = 2,

            /** 设置分辨率为 960x540，默认采用 15 fps，码率 1200 kbps */
            ZEGO_VIDEO_CONFIG_PRESET_540P = 3,

            /** 设置分辨率为 1280x720，默认采用 15 fps，码率 1500 kbps */
            ZEGO_VIDEO_CONFIG_PRESET_720P = 4,

            /** 设置分辨率为 1920x1080，默认采用 15 fps，码率 3000 kbps */
            ZEGO_VIDEO_CONFIG_PRESET_1080P = 5

        };

        /** 流质量等级 */
        enum ZegoStreamQualityLevel
        {
            /** 质量极好 */
            ZEGO_STREAM_QUALITY_LEVEL_EXCELLENT = 0,

            /** 质量好 */
            ZEGO_STREAM_QUALITY_LEVEL_GOOD = 1,

            /** 质量正常 */
            ZEGO_STREAM_QUALITY_LEVEL_MEDIUM = 2,

            /** 质量差 */
            ZEGO_STREAM_QUALITY_LEVEL_BAD = 3,

            /** 质量异常 */
            ZEGO_STREAM_QUALITY_LEVEL_DIE = 4

        };

        /** 音频声道 */
        enum ZegoAudioChannel
        {
            /** 单声道 */
            ZEGO_AUDIO_CHANNEL_MONO = 0,

            /** 双声道 */
            ZEGO_AUDIO_CHANNEL_STEREO = 1

        };

        /** 音频编码器 */
        enum ZegoAudioCodecID
        {
            /** default */
            ZEGO_AUDIO_CODEC_ID_DEFAULT = 0,

            /** Normal */
            ZEGO_AUDIO_CODEC_ID_NORMAL = 1,

            /** Normal2 */
            ZEGO_AUDIO_CODEC_ID_NORMAL2 = 2,

            /** Normal3 */
            ZEGO_AUDIO_CODEC_ID_NORMAL3 = 3,

            /** Low */
            ZEGO_AUDIO_CODEC_ID_LOW = 4,

            /** Low2 */
            ZEGO_AUDIO_CODEC_ID_LOW2 = 5,

            /** Low3 */
            ZEGO_AUDIO_CODEC_ID_LOW3 = 6

        };

        /** 视频编码器 */
        enum ZegoVideoCodecID
        {
            /** 默认编码 */
            ZEGO_VIDEO_CODEC_ID_DEFAULT = 0,

            /** SVC */
            ZEGO_VIDEO_CODEC_ID_SVC = 1,

            /** VP8 */
            ZEGO_VIDEO_CODEC_ID_VP8 = 2

        };

        /** 拉流播放的视频分层 */
        enum ZegoPlayerVideoLayer
        {
            /** 根据网络状态选择图层 */
            ZEGO_PLAYER_VIDEO_LAYER_AUTO = 0,

            /** 指定拉基本层（小分辨率） */
            ZEGO_PLAYER_VIDEO_LAYER_BASE = 1,

            /** 指定拉扩展层（大分辨率) */
            ZEGO_PLAYER_VIDEO_LAYER_BASE_EXTEND = 2

        };

        /** 回声消除模式 */
        enum ZegoAECMode
        {
            /** 激进的回声抵消，可能会影响音质稍微明显，但是回声会消除得很干净 */
            ZEGO_AEC_MODE_AGGRESSIVE = 0,

            /** 适度的回声抵消，就是可能会稍微影响一点点音质，但是残留的回声会更少 */
            ZEGO_AEC_MODE_MEDIUM = 1,

            /** 舒适的回声抵消，就是回声抵消基本不会影响声音的音质，可能有时会残留一点回声，但不会影响正常听音 */
            ZEGO_AEC_MODE_SOFT = 2

        };

        /** 当发生流量控制时可供调节的属性（位掩码枚举） */
        enum ZegoTrafficControlProperty
        {
            /** 基础属性 */
            ZEGO_TRAFFIC_CONTROL_PROPERTY_BASIC = 0,

            /** 调整帧率 */
            ZEGO_TRAFFIC_CONTROL_PROPERTY_ADAPTIVE_FPS = 1,

            /** 调整分辨率 */
            ZEGO_TRAFFIC_CONTROL_PROPERTY_ADAPTIVE_RESOLUTION = 1 << 1,

            /** 调整音频码率 */
            ZEGO_TRAFFIC_CONTROL_PROPERTY_ADAPTIVE_AUDIO_BITRATE = 1 << 2

        };

        /** 流控触发最低码率时的视频发送模式 */
        enum ZegoTrafficControlMinVideoBitrateMode
        {
            /** 低于设置的最低码率时，停止视频发送 */
            ZEGO_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_MODE_NO_VIDEO = 0,

            /** 低于设置的最低码率时，视频以极低的频率发送（不超过 2 FPS) */
            ZEGO_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_MODE_ULTRA_LOW_FPS = 1

        };

        /** 拉流状态 */
        enum ZegoPlayerState
        {
            /** 未拉流状态，在拉流前处于该状态。如果拉流过程出现稳态的异常，例如 AppID 和 AppSign 不正确，都会进入未拉流状态 */
            ZEGO_PLAYER_STATE_NO_PLAY = 0,

            /** 正在请求拉流状态，拉流操作执行成功后会进入正在请求拉流状态，通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断，SDK 会进行内部重试，也会回到正在请求拉流状态 */
            ZEGO_PLAYER_STATE_PLAY_REQUESTING = 1,

            /** 正在拉流状态，进入该状态表明拉流已经成功，用户可以正常通信 */
            ZEGO_PLAYER_STATE_PLAYING = 2

        };

        /** 拉流媒体事件 */
        enum ZegoPlayerMediaEvent
        {
            /** 拉流端出现音频卡顿事件 */
            ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_OCCUR = 0,

            /** 拉流端音频卡顿事件结束 */
            ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_RESUME = 1,

            /** 拉流端出现视频卡顿事件 */
            ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_OCCUR = 2,

            /** 拉流端视频卡顿事件结束 */
            ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_RESUME = 3

        };

        /** 更新类型 */
        enum ZegoUpdateType
        {
            /** 添加 */
            ZEGO_UPDATE_TYPE_ADD = 0,

            /** 删除 */
            ZEGO_UPDATE_TYPE_DELETE = 1

        };

        /** 转推 CDN 状态 */
        enum ZegoStreamRelayCDNState
        {
            /** 未转推状态，在转推前处于该状态。如果转推过程出现稳态的异常，例如 转推地址 不正确，都会进入未转推状态 */
            ZEGO_STREAM_RELAY_CDN_STATE_NO_RELAY = 0,

            /** 正在请求转推状态，转推操作执行成功后会进入正在请求转推状态，通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断，SDK 会进行内部重试，也会回到正在转推状态 */
            ZEGO_STREAM_RELAY_CDN_STATE_RELAY_REQUESTING = 1,

            /** 正在转推状态，进入该状态表明转推已成功 */
            ZEGO_STREAM_RELAY_CDN_STATE_RELAYING = 2

        };

        /** 转发 CDN 状态改变原因 */
        enum ZegoStreamRelayCDNUpdateReason
        {
            /** 无 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_NONE = 0,

            /** 服务器错误 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_SERVER_ERROR = 1,

            /** 握手失败 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_HANDSHAKE_FAILED = 2,

            /** 接入点错误 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_ACCESS_POINT_ERROR = 3,

            /** 创建流失败 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_CREATE_STREAM_FAILED = 4,

            /** BAD NAME */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_BAD_NAME = 5,

            /** CDN 服务器主动断开 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_CDN_SERVER_DISCONNECTED = 6,

            /** 主动断开 */
            ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_DISCONNECTED = 7

        };

        /** 远端设备状态 */
        enum ZegoRemoteDeviceState
        {
            /** 设备开启 */
            ZEGO_REMOTE_DEVICE_STATE_OPEN = 0,

            /** 设备关闭：一般性设备错误 */
            ZEGO_REMOTE_DEVICE_STATE_GENERIC_ERROR = 1,

            /** 设备关闭：无效的设备 ID */
            ZEGO_REMOTE_DEVICE_STATE_INVALID_ID = 2,

            /** 设备关闭：无权限 */
            ZEGO_REMOTE_DEVICE_STATE_NO_AUTHORIZATION = 3,

            /** 设备关闭：采集帧率为 0 */
            ZEGO_REMOTE_DEVICE_STATE_ZERO_FPS = 4,

            /** 设备关闭：设备被占用 */
            ZEGO_REMOTE_DEVICE_STATE_IN_USE_BY_OTHER = 5,

            /** 设备关闭：设备未插入或被拔出 */
            ZEGO_REMOTE_DEVICE_STATE_UNPLUGGED = 6,

            /** 设备关闭：由于系统原因需要重启后才能进行下一次修改，否则重新打开也不生效 */
            ZEGO_REMOTE_DEVICE_STATE_REBOOT_REQUIRED = 7,

            /** 设备关闭：系统媒体服务停止，如 iOS 平台下，当系统检测到当前压力巨大（如播放大量动画），则有可能会将媒体相关服务全部停用 */
            ZEGO_REMOTE_DEVICE_STATE_SYSTEM_MEDIA_SERVICES_LOST = 8,

            /** 设备关闭：禁用采集 */
            ZEGO_REMOTE_DEVICE_STATE_DISABLE = 9,

            /** 设备关闭：远端设备被静音 */
            ZEGO_REMOTE_DEVICE_STATE_MUTE = 10,

            /** 设备关闭：设备被中断，如电话事件打断等 */
            ZEGO_REMOTE_DEVICE_STATE_INTERRUPTION = 11,

            /** 设备关闭：用户 App 退到后台 */
            ZEGO_REMOTE_DEVICE_STATE_IN_BACKGROUND = 12,

            /** 设备关闭：当前前台同时存在多个 App，如 iPad 应用分屏下，系统会禁止所有应用使用摄像头 */
            ZEGO_REMOTE_DEVICE_STATE_MULTI_FOREGROUND_APP = 13,

            /** 设备关闭：系统处于高负载压力下，可能导致设备异常 */
            ZEGO_REMOTE_DEVICE_STATE_BY_SYSTEM_PRESSURE = 14

        };

        /** 音频设备类型 */
        enum ZegoAudioDeviceType
        {
            /** 音频输入设备 */
            ZEGO_AUDIO_DEVICE_TYPE_INPUT = 0,

            /** 音频输出设备 */
            ZEGO_AUDIO_DEVICE_TYPE_OUTPUT = 1

        };

        /** 混流内容类型 */
        enum ZegoMixerInputContentType
        {
            /** 音频混流内容类型 */
            ZEGO_MIXER_INPUT_CONTENT_TYPE_AUDIO = 0,

            /** 视频混流内容类型 */
            ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO = 1

        };

        /** 视频采集缩放时机 */
        enum ZegoCapturePipelineScaleMode
        {
            /** 采集后立即进行缩放，默认 */
            ZEGO_CAPTURE_PIPELINE_SCALE_MODE_PRE = 0,

            /** 编码时进行缩放 */
            ZEGO_CAPTURE_PIPELINE_SCALE_MODE_POST = 1

        };

        /** 视频帧格式 */
        enum ZegoVideoFrameFormat
        {
            /** 未知格式，将取平台默认值 */
            ZEGO_VIDEO_FRAME_FORMAT_UNKNOWN = 0,

            /** I420 (YUV420Planar) 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_I420 = 1,

            /** NV12 (YUV420SemiPlanar) 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_NV12 = 2,

            /** NV21 (YUV420SemiPlanar) 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_NV21 = 3,

            /** BGRA32 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_BGRA32 = 4,

            /** RGBA32 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_RGBA32 = 5,

            /** ARGB32 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_ARGB32 = 6,

            /** ABGR32 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_ABGR32 = 7,

            /** I422 (YUV422Planar) 格式 */
            ZEGO_VIDEO_FRAME_FORMAT_I422 = 8

        };

        /** 视频帧数据类型 */
        enum ZegoVideoBufferType
        {
            /** 未知类型 */
            ZEGO_VIDEO_BUFFER_TYPE_UNKNOWN = 0,

            /** 裸数据类型视频帧 */
            ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA = 1

        };

        /** 视频帧格式系列 */
        enum ZegoVideoFrameFormatSeries
        {
            /** RGB 系列 */
            ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB = 0,

            /** YUV 系列 */
            ZEGO_VIDEO_FRAME_FORMAT_SERIES_YUV = 1

        };

        /** 视频帧翻转模式 */
        enum ZegoVideoFlipMode
        {
            /** 不翻转 */
            ZEGO_VIDEO_FLIP_MODE_NONE = 0,

            /** X 轴翻转 */
            ZEGO_VIDEO_FLIP_MODE_X = 1

        };

        /** 音频配置类型 */
        enum ZegoAudioConfigPreset
        {
            /** 基础音质 */
            ZEGO_AUDIO_CONFIG_PRESET_BASIC_QUALITY = 0,

            /** 标准音质 */
            ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY = 1,

            /** 标准音质-双声道 */
            ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY_STEREO = 2,

            /** 高音质 */
            ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY = 3,

            /** 高音质-双声道 */
            ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY_STEREO = 4

        };

        /** 播放器状态 */
        enum ZegoMediaPlayerState
        {
            /** 不在播放 */
            ZEGO_MEDIA_PLAYER_STATE_NO_PLAY = 0,

            /** 播放中 */
            ZEGO_MEDIA_PLAYER_STATE_PLAYING = 1,

            /** 暂停播放 */
            ZEGO_MEDIA_PLAYER_STATE_PAUSING = 2,

            /** 播放结束 */
            ZEGO_MEDIA_PLAYER_STATE_PLAY_ENDED = 3

        };

        /** 播放器网络事件 */
        enum ZegoMediaPlayerNetworkEvent
        {
            /** 网络资源播放不畅，开始尝试缓存数据 */
            ZEGO_MEDIA_PLAYER_NETWORK_EVENT_BUFFER_BEGIN = 0,

            /** 网络资源可以顺畅播放 */
            ZEGO_MEDIA_PLAYER_NETWORK_EVENT_BUFFER_ENDED = 1

        };

        struct ZegoLogConfig
        {
            /** 日志文件保存路径 */
            std::string logPath;

            /** 日志文件大小上限 (Bytes)，默认为 5MB (5 * 1024 * 1024 Bytes) */
            unsigned long long logSize;

            ZegoLogConfig(){
                logSize = 5 * 1024 * 1024;
            }

        };

        struct ZegoCustomVideoCaptureConfig
        {
            /** 自定义视频采集视频帧数据类型 */
            ZegoVideoBufferType bufferType;

        };

        struct ZegoCustomVideoRenderConfig
        {
            /** 自定义视频渲染视频帧数据类型 */
            ZegoVideoBufferType bufferType;

            /** 自定义视频渲染视频帧数据格式 */
            ZegoVideoFrameFormatSeries frameFormatSeries;

            /** 是否在自定义视频渲染的同时，引擎也渲染，默认为 [false] */
            bool enableEngineRender;

        };

        struct ZegoEngineConfig
        {
            /** 日志配置，不设则使用默认配置 */
            ZegoLogConfig* logConfig;

            /** 主流自定义视频采集配置，不设则默认主流不开启自定义视频采集 */
            ZegoCustomVideoCaptureConfig* customVideoCaptureMainConfig;

            /** 辅流自定义视频采集配置，不设则默认辅流不开启自定义视频采集 */
            ZegoCustomVideoCaptureConfig* customVideoCaptureAuxConfig;

            /** 自定义视频渲染配置，不设则默认不开启自定义视频渲染 */
            ZegoCustomVideoRenderConfig* customVideoRenderConfig;

            /** 其他特殊功能开关，不设则默认不使用任何其他特殊功能，这里所指的特殊功能不包括上述的自定义视频采集功能与自定义视频渲染等参数字段所列举的功能。 */
            std::string advancedConfig;

            ZegoEngineConfig(){
                logConfig = nullptr;
                customVideoCaptureMainConfig = nullptr;
                customVideoCaptureAuxConfig = nullptr;
                customVideoRenderConfig = nullptr;
            }

        };

        struct ZegoRoomConfig
        {
            /** 房间最大用户数量，传 0 视为不限制，默认无限制 */
            unsigned int maxMemberCount;

            /** 是否开启用户进出房间回调通知 [onRoomUserUpdate]，默认关闭 */
            bool isUserStatusNotify;

            /** 由开发者业务服务器下发的 token，用以保证安全性，生成规则详见 [https://doc.zego.im/CN/565.html](https://doc.zego.im/CN/565.html)，默认为空字符串，即不鉴权 */
            std::string token;

            ZegoRoomConfig():maxMemberCount(0), isUserStatusNotify(false), token(""){
            }

        };

        struct ZegoVideoConfig
        {
            /** 采集分辨率宽度 */
            int captureWidth;

            /** 采集分辨率宽度 */
            int captureHeight;

            /** 编码分辨率宽度 */
            int encodeWidth;

            /** 采集分辨率宽度 */
            int encodeHeight;

            /** 帧率 */
            int fps;

            /** 码率，单位为 kbps */
            int bitrate;

            /** 要使用的编码器 */
            ZegoVideoCodecID codecID;

            ZegoVideoConfig(ZegoVideoConfigPreset preset = ZEGO_VIDEO_CONFIG_PRESET_360P){
            codecID = ZEGO_VIDEO_CODEC_ID_DEFAULT;
            switch (preset) {
            case ZEGO_VIDEO_CONFIG_PRESET_180P:
                captureWidth = 320;
                captureHeight = 180;
                encodeWidth = 320;
                encodeHeight = 180;
                bitrate = 300;
                fps = 15;
                break;
            case ZEGO_VIDEO_CONFIG_PRESET_270P:
                captureWidth = 480;
                captureHeight = 270;
                encodeWidth = 480;
                encodeHeight = 270;
                bitrate = 400;
                fps = 15;
                break;
            case ZEGO_VIDEO_CONFIG_PRESET_360P:
                captureWidth = 640;
                captureHeight = 360;
                encodeWidth = 640;
                encodeHeight = 360;
                bitrate = 600;
                fps = 15;
                break;
            case ZEGO_VIDEO_CONFIG_PRESET_540P:
                captureWidth = 960;
                captureHeight = 540;
                encodeWidth = 960;
                encodeHeight = 540;
                bitrate = 1200;
                fps = 15;
                break;
            case ZEGO_VIDEO_CONFIG_PRESET_720P:
                captureWidth = 1280;
                captureHeight = 720;
                encodeWidth = 1280;
                encodeHeight = 720;
                bitrate = 1500;
                fps = 15;
                break;
            case ZEGO_VIDEO_CONFIG_PRESET_1080P:
                captureWidth = 1920;
                captureHeight = 1080;
                encodeWidth = 1920;
                encodeHeight = 1080;
                bitrate = 3000;
                fps = 15;
                break;
            }
            }

        };

        struct ZegoUser
        {
            /** 用户 ID，最大 64 字节的字符串。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。 */
            std::string userID;

            /** 用户名，最大长度不超过 256 字节的字符串 */
            std::string userName;

            ZegoUser(){
            }

            ZegoUser(std::string userID, std::string userName):userID(userID),userName(userName){
            }

        };

        struct ZegoStream
        {
            /** 用户对象实例 */
            ZegoUser user;

            /** 流 ID，长度不超过256的字符串。不可以包含 URL 关键字，否则推拉流失败。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。 */
            std::string streamID;

            /** 流附加信息 */
            std::string extraInfo;

        };

        struct ZegoRect
        {
            /** 矩形左上角在坐标系 X 轴上的值 */
            int x;

            /** 矩形左上角在坐标系 Y 轴上的值 */
            int y;

            /** 矩形宽度 */
            int width;

            /** 矩形高度 */
            int height;

            ZegoRect() : x(0), y(0), width(0), height(0){
            }

            ZegoRect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h){
            }

        };

        struct ZegoCanvas
        {
            /** View 对象 */
            void * view;

            /** 视图模式，默认为 ZegoViewModeAspectFit */
            ZegoViewMode viewMode;

            /** 背景颜色，格式为 0xRRGGBB，默认为黑色即 0x000000 */
            int backgroundColor;

            ZegoCanvas(void *view=nullptr, ZegoViewMode viewMode=ZEGO_VIEW_MODE_ASPECT_FIT, int backgroundColor=0x000000):view(view),viewMode(viewMode),backgroundColor(backgroundColor){
            }

        };

        struct ZegoPublishStreamQuality
        {
            /** 视频采集帧率，单位为 f/s */
            double videoCaptureFPS;

            /** 视频编码帧率，单位为 f/s */
            double videoEncodeFPS;

            /** 视频发送帧率，单位为 f/s */
            double videoSendFPS;

            /** 视频码率，单位为 kbps */
            double videoKBPS;

            /** 音频采集帧率，单位为 f/s */
            double audioCaptureFPS;

            /** 音频发送帧率，单位为 f/s */
            double audioSendFPS;

            /** 音频码率，单位为 kbps */
            double audioKBPS;

            /** 本端至服务端的延迟，单位为毫秒 */
            int rtt;

            /** 丢包率，单位为百分比，0.0 ~ 1.0 */
            double packetLostRate;

            /** 推流质量级别 */
            ZegoStreamQualityLevel level;

            /** 是否开启硬件编码 */
            bool isHardwareEncode;

        };

        struct ZegoCDNConfig
        {
            /** CDN 的 URL */
            std::string url;

            /** URL 的鉴权参数 */
            std::string authParam;

        };

        struct ZegoStreamRelayCDNInfo
        {
            /** CDN 推流的 URL */
            std::string url;

            /** 转推状态 */
            ZegoStreamRelayCDNState state;

            /** 转推状态变更的原因 */
            ZegoStreamRelayCDNUpdateReason updateReason;

            /** 状态发生的时间戳，单位为毫秒 */
            unsigned long long stateTime;

        };

        struct ZegoPlayerConfig
        {
            /** 拉流的 CDN 配置，若设置后，则按照 URL 拉流而不是按照 streamID 拉流，此后 streamID 仅作为 SDK 内部回调的标识 */
            ZegoCDNConfig * cdnConfig;

            /** 设置拉流的视频分层 */
            ZegoPlayerVideoLayer videoLayer;

            ZegoPlayerConfig(): cdnConfig(nullptr), videoLayer(ZEGO_PLAYER_VIDEO_LAYER_AUTO){
            }

        };

        struct ZegoPlayStreamQuality
        {
            /** 视频接收帧率，单位为 f/s */
            double videoRecvFPS;

            /** 视频解码帧率，单位为 f/s */
            double videoDecodeFPS;

            /** 视频渲染帧率，单位为 f/s */
            double videoRenderFPS;

            /** 视频码率，单位为 kbps */
            double videoKBPS;

            /** 音频接收帧率，单位为 f/s */
            double audioRecvFPS;

            /** 音频解码帧率，单位为 f/s */
            double audioDecodeFPS;

            /** 音频渲染帧率，单位为 f/s */
            double audioRenderFPS;

            /** 音频码率，单位为 kbps */
            double audioKBPS;

            /** 服务端至本端的延迟，单位为毫秒 */
            int rtt;

            /** 丢包率，单位为百分比，0.0 ~ 1.0 */
            double packetLostRate;

            /** 拉流质量级别 */
            ZegoStreamQualityLevel level;

            /** 本端接收到数据后到播放的延迟，单位为毫秒 */
            int delay;

            /** 是否开启硬件解码 */
            bool isHardwareDecode;

        };

        struct ZegoDeviceInfo
        {
            /** 设备 ID */
            std::string deviceID;

            /** 设备名称 */
            std::string deviceName;

        };

        struct ZegoMixerAudioConfig
        {
            /** 音频码率，单位为 kbps，默认为 48 kbps，开始混流任务后不能修改 */
            int bitrate;

            /** 音频声道，默认为 Mono 单声道 */
            ZegoAudioChannel channel;

            /** 编码 ID，默认为 ZegoAudioCodecIDDefault */
            ZegoAudioCodecID codecID;

            ZegoMixerAudioConfig():bitrate(48), channel(ZEGO_AUDIO_CHANNEL_MONO), codecID(ZEGO_AUDIO_CODEC_ID_DEFAULT){
            }

            ZegoMixerAudioConfig(int bitrate, ZegoAudioChannel channel, ZegoAudioCodecID codecID): bitrate(bitrate), channel(channel), codecID(codecID){
            }

        };

        struct ZegoMixerVideoConfig
        {
            /** 视频分辨率宽 */
            int width;

            /** 视频分辨率高 */
            int height;

            /** 视频帧率，开始混流任务后不能修改 */
            int fps;

            /** 视频码率，单位为 kbps */
            int bitrate;

            ZegoMixerVideoConfig(): width(640), height(360), fps(15), bitrate(600){
            }

            ZegoMixerVideoConfig(int width, int height, int fps, int bitrate): width(width), height(height), fps(fps), bitrate(bitrate){
            }

        };

        struct ZegoMixerInput
        {
            /** 流 ID，长度不超过256的字符串。不可以包含 URL 关键字，否则推拉流失败。仅支持数字，英文字符 和 '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'。 */
            std::string streamID;

            /** 混流内容类型 */
            ZegoMixerInputContentType contentType;

            /** 流的布局 */
            ZegoRect layout;

            /** 当混流任务开启了声浪回调时，需要为每条输入流指定唯一的 soundLevelID */
            unsigned int soundLevelID;

            ZegoMixerInput() : contentType(ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO), soundLevelID(0){
            }

            ZegoMixerInput(std::string streamID, ZegoMixerInputContentType contentType, ZegoRect layout, unsigned int soundLevelID): streamID(streamID), contentType(contentType), layout(layout), soundLevelID(soundLevelID){
            }

        };

        struct ZegoMixerOutput
        {
            /** 混流输出目标，URL 或者流 ID */
            std::string target;

            ZegoMixerOutput(){
            }

            ZegoMixerOutput(std::string target) :target(target){
            }

        };

        struct ZegoWatermark
        {
            /** 水印图片 URL */
            std::string imageURL;

            /** 水印图片的大小方位 */
            ZegoRect layout;

            ZegoWatermark(){
            }

            ZegoWatermark(std::string imageURL, ZegoRect layout) :imageURL(imageURL), layout(layout){
            }

        };

        struct ZegoMixerTask
        {
            /** 混流任务 ID */
            std::string taskID;

            /** 混流任务的音频配置 */
            ZegoMixerAudioConfig audioConfig;

            /** 混流任务的视频配置 */
            ZegoMixerVideoConfig videoConfig;

            /** 混流任务的输入流列表 */
            std::vector<ZegoMixerInput> inputList;

            /** 混流任务的输出流列表 */
            std::vector<ZegoMixerOutput> outputList;

            /** 混流任务的水印 */
            ZegoWatermark* watermark;

            /** 混流任务的背景图 URL */
            std::string backgroundImageURL;

            /** 是否开启混流的声浪回调通知，开启后拉混流时可通过 [onMixerSoundLevelUpdate] 回调收到每条单流的声浪信息 */
            bool enableSoundLevel;

            ZegoMixerTask(std::string taskID=""): taskID(taskID), watermark(nullptr){
            }

        };

        struct ZegoBroadcastMessageInfo
        {
            /** 消息内容 */
            std::string message;

            /** 消息 ID */
            unsigned long long messageID;

            /** 消息的发送时间 */
            unsigned long long sendTime;

            /** 消息的发送者 */
            ZegoUser fromUser;

        };

        struct ZegoBarrageMessageInfo
        {
            /** 消息内容 */
            std::string message;

            /** 消息 ID */
            std::string messageID;

            /** 消息的发送时间 */
            unsigned long long sendTime;

            /** 消息的发送者 */
            ZegoUser fromUser;

        };

        struct ZegoVideoFrameParam
        {
            /** 视频帧的格式 */
            ZegoVideoFrameFormat format;

            /** 每个平面一行字节数（此参数为 int 数组，数组长度为4，RGBA 只需考虑 strides[0]，I420 需考虑 strides[0,1,2]） */
            int strides[4];

            /** 视频帧的画面宽 */
            int width;

            /** 视频帧的画面高 */
            int height;

        };

        struct ZegoAudioFrameParam
        {
            /** 采样率 */
            int sampleRate;

            /** 音频声道，默认为 Mono 单声道 */
            ZegoAudioChannel channel;

        };

        struct ZegoAudioConfig
        {
            /** 音频码率，单位为 kbps，默认为 48 kbps */
            int bitrate;

            /** 音频声道，默认为 Mono 单声道 */
            ZegoAudioChannel channel;

            /** 编码 ID，默认为 ZegoAudioCodecIDDefault */
            ZegoAudioCodecID codecID;

            ZegoAudioConfig(ZegoAudioConfigPreset preset = ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY){
                switch (preset) {
                codecID = ZEGO_AUDIO_CODEC_ID_DEFAULT;
                case ZEGO_AUDIO_CONFIG_PRESET_BASIC_QUALITY:
                    bitrate = 16;
                    channel = ZEGO_AUDIO_CHANNEL_MONO;
                    break;
                case ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY:
                    bitrate = 48;
                    channel = ZEGO_AUDIO_CHANNEL_MONO;
                    break;
                case ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY_STEREO:
                    bitrate = 56;
                    channel = ZEGO_AUDIO_CHANNEL_STEREO;
                    break;
                case ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY:
                    bitrate = 128;
                    channel = ZEGO_AUDIO_CHANNEL_MONO;
                    break;
                case ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY_STEREO:
                    bitrate = 192;
                    channel = ZEGO_AUDIO_CHANNEL_STEREO;
                    break;
                }
            }

        };

        /**
         * 引擎异步销毁完成的回调
         *
         * 一般情况下开发者无需关注此回调
         */
        using ZegoDestroyCompletionCallback = std::function<void ()>;


        /**
         * 更新流附加信息接口的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         */
        using ZegoPublisherSetStreamExtraInfoCallback = std::function<void (int errorCode)>;


        /**
         * 添加/删除转推 CDN 接口的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         */
        using ZegoPublisherUpdateCdnUrlCallback = std::function<void (int errorCode)>;


        /**
         * 开始混流任务的结果的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         * @param extendedData 扩展信息
         */
        using ZegoMixerStartCallback = std::function<void (int errorCode, std::string extendedData)>;


        /**
         * 结束混流任务的结果的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         */
        using ZegoMixerStopCallback = std::function<void (int errorCode)>;


        /**
         * 发送房间聊天消息的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         * @param messageID 此消息的 ID
         */
        using ZegoIMSendBroadcastMessageCallback = std::function<void (int errorCode, unsigned long long messageID)>;


        /**
         * 发送房间弹幕消息的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         * @param messageID 此消息的 ID
         */
        using ZegoIMSendBarrageMessageCallback = std::function<void (int errorCode, std::string messageID)>;


        /**
         * 发送房间自定义信令的回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         */
        using ZegoIMSendCustomCommandCallback = std::function<void (int errorCode)>;


        /**
         * 播放器加载资源完成回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         */
        using ZegoMediaPlayerLoadResourceCallback = std::function<void (int errorCode)>;


        /**
         * 播放器设置指定播放进度回调
         *
         * @param errorCode 错误码，详情请参考常用错误码文档 [https://doc-zh.zego.im/zh/308.html]
         */
        using ZegoMediaPlayerSeekToCallback = std::function<void (int errorCode)>;



        using ZegoAudioSpectrum = std::vector<double>;
        using ZegoView = void*; 

    }
}

#endif /* __ZEGOEXPRESSDEFINES_H__ */
