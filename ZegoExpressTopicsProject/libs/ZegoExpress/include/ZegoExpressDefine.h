
#ifndef __ZEGOEXPRESSDEFINE_H__
#define __ZEGOEXPRESSDEFINE_H__

#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#include <sstream>
#include <locale>
#include <codecvt>

#define WM_ZEGO_SWITCH_THREAD   WM_APP+10086
#define ZEGO_SWITCH_THREAD_PRE  auto hCommWnd = ::FindWindow(pthis->ZegoCommWndClassName, pthis->ZegoCommWndName); std::function<void(void)>* pFunc = new  std::function<void(void)>;*pFunc = [=](void) {
#define ZEGO_SWITCH_THREAD_ING }; PostMessage(hCommWnd, WM_ZEGO_SWITCH_THREAD, (WPARAM)pFunc, 0);

#else	

#import <Foundation/Foundation.h>
#define ZEGO_SWITCH_THREAD_PRE dispatch_async(dispatch_get_main_queue(), ^{;
#define ZEGO_SWITCH_THREAD_ING });

#endif // WIN32

#define ZEGO_UNUSED_VARIABLE(x) ((void)(x))

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace ZEGO {
    namespace EXPRESS {
    
        #define ZEGO_MAX_EVENT_HANDLER_COUNT 16    /**  */

        enum ZegoScenario
        {
            /** 通用场景
             */
            ZEGO_SCENARIO_GENERAL = 0,

        };

        enum ZegoLanguage
        {
            /** 英文
             */
            ZEGO_LANGUAGE_ENGLISH = 0,

            /** 中文
             */
            ZEGO_LANGUAGE_CHINESE = 1,

        };

        enum ZegoRoomState
        {
            /** 未连接状态，在登陆房间前和退出房间之后进入该状态。如果登陆房间的过程出现稳态异常，例如 AppID 和 AppSign 不正确，或者有相同用户名在其他地方登陆导致本端被 KickOut，都会进入该状态
             */
            ZEGO_ROOM_STATE_DISCONNECTED = 0,

            /** 正在请求连接状态，登陆房间动作执行成功后会进入此状态。通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断， SDK 会进行内部重试，也会回到正在请求连接状态
             */
            ZEGO_ROOM_STATE_CONNECTING = 1,

            /** 连接成功状态，进入该状态表示登陆房间已经成功，用户可以正常收到房间内的用户和流信息增删的回调通知
             */
            ZEGO_ROOM_STATE_CONNECTED = 2,

        };

        enum ZegoViewMode
        {
            /** 等比缩放，可能有黑边
             */
            ZEGO_VIEW_MODE_ASPECT_FIT = 0,

            /** 等比缩放填充整个 View ，可能有部分被裁减
             */
            ZEGO_VIEW_MODE_ASPECT_FILL = 1,

            /** 填充整个 View，图像可能被拉伸
             */
            ZEGO_VIEW_MODE_SCALE_TO_FILL = 2,

        };

        enum ZegoVideoMirrorMode
        {
            /** 只有本地预览时才是镜像画面，默认采用此模式
             */
            ZEGO_VIDEO_MIRROR_MODE_ONLY_PREVIEW_MIRROR = 0,

            /** 本地预览和拉流端看到的视频都是镜像画面
             */
            ZEGO_VIDEO_MIRROR_MODE_BOTH_MIRROR = 1,

            /** 本地预览和拉流端看到的视频都不是镜像画面
             */
            ZEGO_VIDEO_MIRROR_MODE_NO_MIRROR = 2,

            /** 只有拉流端看到的视频才是镜像画面
             */
            ZEGO_VIDEO_MIRROR_MODE_ONLY_PUBLISH_MIRROR = 3,

        };

        enum ZegoPublisherState
        {
            /** 未推流状态，在推流前处于该状态。如果推流过程出现稳态的异常，例如 AppID 和 AppSign 不正确，或者如果其他用户已经在推送流，推送相同流ID的流会失败，都会进入未推流状态
             */
            ZEGO_PUBLISHER_STATE_NO_PUBLISH = 0,

            /** 正在请求推流状态，推流操作执行成功后会进入正在请求推流状态，通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断，SDK 会进行内部重试，也会回到正在请求推流状态
             */
            ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING = 1,

            /** 正在推流状态，进入该状态表明推流已经成功，用户可以正常通信
             */
            ZEGO_PUBLISHER_STATE_PUBLISHING = 2,

        };

        enum ZegoResolution
        {
            /** 设置分辨率为 320x180，默认采用 15 fps，码率 300000 bps
             */
            ZEGO_RESOLUTION_320x180 = 0,

            /** 设置分辨率为 480x270，默认采用 15 fps，码率 400000 bps
             */
            ZEGO_RESOLUTION_480x270 = 1,

            /** 设置分辨率为 640x360，默认采用 15 fps，码率 600000 bps
             */
            ZEGO_RESOLUTION_640x360 = 2,

            /** 设置分辨率为 960x540，默认采用 15 fps，码率 1200000 bps
             */
            ZEGO_RESOLUTION_960x540 = 3,

            /** 设置分辨率为 1280x720，默认采用 15 fps，码率 1500000 bps
             */
            ZEGO_RESOLUTION_1280x720 = 4,

            /** 设置分辨率为 1920x1080，默认采用 15 fps，码率 3000000 bps
             */
            ZEGO_RESOLUTION_1920x1080 = 5,

        };

        enum ZegoPublisherFirstFrameEvent
        {
            /** 推流端采集到音频首帧事件
             */
            ZEGO_PUBLISHER_FIRST_FRAME_EVENT_AUDIO_CAPTURED = 0,

            /** 推流端采集到视频首帧事件
             */
            ZEGO_PUBLISHER_FIRST_FRAME_EVENT_VIDEO_CAPTURED = 1,

        };

        enum ZegoStreamQualityLevel
        {
            /** 质量极好
             */
            ZEGO_STREAM_QUALITY_LEVEL_EXCELLENT = 0,

            /** 质量好
             */
            ZEGO_STREAM_QUALITY_LEVEL_GOOD = 1,

            /** 质量正常
             */
            ZEGO_STREAM_QUALITY_LEVEL_MEDIUM = 2,

            /** 质量差
             */
            ZEGO_STREAM_QUALITY_LEVEL_BAD = 3,

            /** 质量异常
             */
            ZEGO_STREAM_QUALITY_LEVEL_DIE = 4,

        };

        enum ZegoLatencyMode
        {
            /** 普通延迟模式
             */
            ZEGO_LATENCY_MODE_NORMAL = 0,

            /** 低延迟模式，无法用于 RTMP 流 
             */
            ZEGO_LATENCY_MODE_LOW = 1,

            /** 普通延迟模式，最高码率可达 192 kbps
             */
            ZEGO_LATENCY_MODE_NORMAL2 = 2,

            /** 低延迟模式，无法用于 RTMP 流。相对于 ZEGO_LATENCY_MODE_LOW 而言，CPU 开销稍低
             */
            ZEGO_LATENCY_MODE_LOW2 = 3,

            /** 低延迟模式，无法用于 RTMP 流。支持WebRTC必须使用此模式
             */
            ZEGO_LATENCY_MODE_LOW3 = 4,

            /** 普通延迟模式
             */
            ZEGO_LATENCY_MODE_NORMAL3 = 5,

        };

        enum ZegoAudioChannelType
        {
            /** 单声道
             */
            ZEGO_AUDIO_CHANNEL_TYPE_MONO = 0,

            /** 双声道
             */
            ZEGO_AUDIO_CHANNEL_TYPE_STEREO = 1,

        };

        enum ZegoAECMode
        {
            /** 激进的回声抵消，可能会影响音质稍微明显，但是回声会消除得很干净
             */
            ZEGO_AEC_MODE_AGGRESSIVE = 0,

            /** 适度的回声抵消，就是可能会稍微影响一点点音质，但是残留的回声会更少
             */
            ZEGO_AEC_MODE_MEDIUM = 1,

            /** 舒适的回声抵消，就是回声抵消基本不会影响声音的音质，可能有时会残留一点回声，但不会影响正常听音
             */
            ZEGO_AEC_MODE_SOFT = 2,

        };

        enum ZegoPlayerState
        {
            /** 未拉流状态，在拉流前处于该状态。如果拉流过程出现稳态的异常，例如 AppID 和 AppSign 不正确，都会进入未拉流状态
             */
            ZEGO_PLAYER_STATE_NO_PLAY = 0,

            /** 正在请求拉流状态，拉流操作执行成功后会进入正在请求拉流状态，通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断，SDK 会进行内部重试，也会回到正在请求拉流状态
             */
            ZEGO_PLAYER_STATE_PLAY_REQUESTING = 1,

            /** 正在拉流状态，进入该状态表明拉流已经成功，用户可以正常通信
             */
            ZEGO_PLAYER_STATE_PLAYING = 2,

        };

        enum ZegoPlayerMediaEvent
        {
            /** 拉流端出现音频卡顿事件
             */
            ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_OCCUR = 0,

            /** 拉流端音频卡顿事件结束
             */
            ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_RESUME = 1,

            /** 拉流端出现视频卡顿事件
             */
            ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_OCCUR = 2,

            /** 拉流端视频卡顿事件结束
             */
            ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_RESUME = 3,

        };

        enum ZegoPlayerFirstFrameEvent
        {
            /** 拉流端拉取到音频首帧事件
             */
            ZEGO_PLAYER_FIRST_FRAME_EVENT_AUDIO_RCV = 0,

            /** 拉流端拉取到视频首帧事件
             */
            ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RCV = 1,

            /** 拉流端视频首帧渲染完成事件
             */
            ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDER = 2,

        };

        enum ZegoUpdateType
        {
            /** 添加
             */
            ZEGO_UPDATE_TYPE_ADD = 0,

            /** 删除
             */
            ZEGO_UPDATE_TYPE_DEL = 1,

        };

        enum ZegoStreamRelayCDNState
        {
            /** 未转推状态，在转推前处于该状态。如果转推过程出现稳态的异常，例如 转推地址 不正确，都会进入未转推状态
             */
            ZEGO_STREAM_RELAY_CDN_STATE_STOP = 0,

            /** 正在转推状态，进入该状态表明转推已成功
             */
            ZEGO_STREAM_RELAY_CDN_STATE_START = 1,

            /** 正在请求转推状态，转推操作执行成功后会进入正在请求转推状态，通常通过该状态进行应用界面的展示。如果因为网络质量不佳产生的中断，SDK 会进行内部重试，也会回到正在转推状态
             */
            ZEGO_STREAM_RELAY_CDN_STATE_RETRY = 2,

        };

        enum ZegoStreamRelayCDNReason
        {
            /** 无
             */
            ZEGO_STREAM_RELAY_CDN_REASON_NONE = 0,

            /** 服务器错误
             */
            ZEGO_STREAM_RELAY_CDN_REASON_SERVER_ERROR = 1,

            /** 握手失败
             */
            ZEGO_STREAM_RELAY_CDN_REASON_HANDSHAKE_FAILED = 2,

            /** 接入点错误
             */
            ZEGO_STREAM_RELAY_CDN_REASON_ACCESS_POINT_ERROR = 3,

            /** 创建流失败
             */
            ZEGO_STREAM_RELAY_CDN_REASON_CREATE_STREAM_FAILED = 4,

            /** BAD NAME
             */
            ZEGO_STREAM_RELAY_CDN_REASON_BAD_NAME = 5,

            /** CDN 服务器主动断开
             */
            ZEGO_STREAM_RELAY_CDN_REASON_CDN_SERVER_DISCONNECTED = 6,

            /** 主动断开
             */
            ZEGO_STREAM_RELAY_CDN_REASON_DISCONNECTED = 7,

        };

        enum ZegoBeautifyFeature
        {
            /** 无美颜
             */
            ZEGO_BEAUTIFY_FEATURE_NONE = 0,

            /** 磨皮
             */
            ZEGO_BEAUTIFY_FEATURE_POLISH = 1,

            /** 锐化
             */
            ZEGO_BEAUTIFY_FEATURE_SHARPEN = 1<<1,

            /** 皮肤美白
             */
            ZEGO_BEAUTIFY_FEATURE_SKIN_WHITEN = 1<<2,

            /** 全屏美白
             */
            ZEGO_BEAUTIFY_FEATURE_WHITEN = 1<<3,

        };

        enum ZegoRemoteDeviceState
        {
            /** 设备开启
             */
            ZEGO_REMOTE_DEVICE_STATE_OPEN = 0,

            /** 设备关闭：一般性设备错误
             */
            ZEGO_REMOTE_DEVICE_STATE_GENERIC_ERROR = 1,

            /** 设备关闭：无效的设备 ID 
             */
            ZEGO_REMOTE_DEVICE_STATE_INVALID_ID = 2,

            /** 设备关闭：无权限
             */
            ZEGO_REMOTE_DEVICE_STATE_NO_AUTHORIZATION = 3,

            /** 设备关闭：采集帧率为 0
             */
            ZEGO_REMOTE_DEVICE_STATE_ZERO_FPS = 4,

            /** 设备关闭：设备被占用
             */
            ZEGO_REMOTE_DEVICE_STATE_IN_USE_BY_OTHER = 5,

            /** 设备关闭：设备未插入或被拔出
             */
            ZEGO_REMOTE_DEVICE_STATE_UNPLUGGED = 6,

            /** 设备关闭：由于系统原因需要重启后才能进行下一次修改，否则重新打开也不生效
             */
            ZEGO_REMOTE_DEVICE_STATE_REBOOT_REQUIRED = 7,

            /** 设备关闭：系统媒体服务停止，如 iOS 平台下，当系统检测到当前压力巨大（如播放大量动画），则有可能会将媒体相关服务全部停用
             */
            ZEGO_REMOTE_DEVICE_STATE_SYSTEM_MEDIA_SERVICES_LOST = 8,

            /** 设备关闭：禁用采集
             */
            ZEGO_REMOTE_DEVICE_STATE_DISABLE = 9,

            /** 设备关闭：远端设备被静音
             */
            ZEGO_REMOTE_DEVICE_STATE_MUTE = 10,

            /** 设备关闭：设备被中断，如电话事件打断等
             */
            ZEGO_REMOTE_DEVICE_STATE_INTERRUPTION = 11,

            /** 设备关闭：用户app退到后台
             */
            ZEGO_REMOTE_DEVICE_STATE_IN_BACKGROUND = 12,

            /** 设备关闭：当前前台同时存在多个 App，如 iPad 应用分屏下，系统会禁止所有应用使用摄像头
             */
            ZEGO_REMOTE_DEVICE_STATE_MULTI_FOREGROUND_APP = 13,

            /** 设备关闭：系统处于高负载压力下，可能导致设备异常
             */
            ZEGO_REMOTE_DEVICE_STATE_BY_SYSTEM_PRESSURE = 14,

        };

        enum ZegoAudioDeviceType
        {
            /** 音频输入设备
             */
            ZEGO_AUDIO_DEVICE_TYPE_INPUT = 0,

            /** 音频输出设备
             */
            ZEGO_AUDIO_DEVICE_TYPE_OUTPUT = 1,

        };

        enum ZegoMixerInputContentType
        {
            /** 音频混流内容类型
             */
            ZegoMixerInputContentTypeAudio = 0,

            /** 视频混流内容类型
             */
            ZegoMixerInputContentTypeVideo = 1,

        };

        enum ZegoCapturePipelineScaleMode
        {
            /** 采集后立即进行缩放，默认
             */
            ZegoCapturePipelineScaleModePre = 0,

            /** 编码时进行缩放
             */
            ZegoCapturePipelineScaleModePost = 1,

        };

        struct ZegoRoomConfig
        {
            /** 
             */
            ZegoRoomConfig(): maxMemberCount(UINT_MAX), isUserStatusNotify(false) {}
            
            ZegoRoomConfig(unsigned int maxMemberCount, bool isUserStatusNotify)
            :maxMemberCount(maxMemberCount), isUserStatusNotify(isUserStatusNotify){}

            /** 房间最大用户数量，默认无限制
             */
            unsigned int maxMemberCount;

            /** 是否开启用户进出房间回调通知（onRoomUserUpdate），默认关闭
             */
            bool isUserStatusNotify;

        };

        struct ZegoVideoConfig
        {
            /** 
             */
            ZegoVideoConfig(ZegoResolution resolution = ZEGO_RESOLUTION_640x360)
            {
             switch (resolution) {
             case ZEGO_RESOLUTION_320x180:
              captureWidth = 320;
              captureHeight = 180;
              encodeWidth = 320;
              encodeHeight = 180;
              bitrate = 300 * 1000;
              fps = 15;
              break;
             case ZEGO_RESOLUTION_480x270:
              captureWidth = 480;
              captureHeight = 270;
              encodeWidth = 480;
              encodeHeight = 270;
              bitrate = 400 * 1000;
              fps = 15;
              break;
             case ZEGO_RESOLUTION_640x360:
              captureWidth = 640;
              captureHeight = 360;
              encodeWidth = 640;
              encodeHeight = 360;
              bitrate = 600 * 1000;
              fps = 15;
              break;
             case ZEGO_RESOLUTION_960x540:
              captureWidth = 960;
              captureHeight = 540;
              encodeWidth = 960;
              encodeHeight = 540;
              bitrate = 1200 * 1000;
              fps = 15;
              break;
             case ZEGO_RESOLUTION_1280x720:
              captureWidth = 1280;
              captureHeight = 720;
              encodeWidth = 1280;
              encodeHeight = 720;
              bitrate = 1500 * 1000;
              fps = 15;
              break;
             case ZEGO_RESOLUTION_1920x1080:
              captureWidth = 1920;
              captureHeight = 1080;
              encodeWidth = 1920;
              encodeHeight = 1080;
              bitrate = 3000 * 1000;
              fps = 15;
              break;
             }
            }

            /** 采集分辨率宽度
             */
            int captureWidth;

            /** 采集分辨率高度
             */
            int captureHeight;

            /** 编码分辨率宽度
             */
            int encodeWidth;

            /** 编码分辨率高度
             */
            int encodeHeight;

            /** 码率，单位为 bps
             */
            int bitrate;

            /** 帧率
             */
            int fps;

        };

        struct ZegoUser
        {
            /** 
             */
            ZegoUser() {}
            ZegoUser(std::string userID, std::string userName):userID(userID),userName(userName){}

            /** 用户 ID，最大长度不超过128字节的字符串
             */
            std::string userID;

            /** 用户名，最大长度不超过128字节的字符串
             */
            std::string userName;

        };

        struct ZegoStream
        {
            /** 用户对象实例
             */
            ZegoUser user;

            /** 流ID
             */
            std::string streamID;

            /** 流附加信息
             */
            std::string extraInfo;

        };

        struct ZegoRect
        {
            /** 
             */
            ZegoRect() : x(0), y(0), width(0), height(0) {}
            ZegoRect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}

            /** 矩形左上角在坐标系X轴上的值
             */
            int x;

            /** 矩形左上角在坐标系Y轴上的值
             */
            int y;

            /** 矩形宽度
             */
            int width;

            /** 矩形高度
             */
            int height;

        };

        struct ZegoCanvas
        {
            /** 
             */
            ZegoCanvas(void *view, ZegoViewMode viewMode=ZEGO_VIEW_MODE_ASPECT_FIT, int backgroundColor=0x000000)
            :view(view),viewMode(viewMode),backgroundColor(backgroundColor){}

            /** View 对象
             */
            void * view;

            /** 视图模式，默认为 ZEGO_VIEW_MODE_ASPECT_FIT
             */
            ZegoViewMode viewMode;

            /** 背景颜色，格式为 0xRRGGBB，默认为黑色即 0x000000
             */
            int backgroundColor;

        };

        struct ZegoPublishStreamQuality
        {
            /** 视频采集帧率
             */
            double videoCaptureFPS;

            /** 视频编码帧率
             */
            double videoEncodeFPS;

            /** 视频发送帧率
             */
            double videoSendFPS;

            /** 视频码率
             */
            double videoKBPS;

            /** 音频采集帧率
             */
            double audioCaptureFPS;

            /** 音频发送帧率
             */
            double audioSendFPS;

            /** 音频码率
             */
            double audioKBPS;

            /** 本端至服务端的延迟，单位为毫秒
             */
            int rtt;

            /** 丢包率，单位为百分比，0.0 ~ 1.0
             */
            double packetLostRate;

            /** 推流质量级别
             */
            ZegoStreamQualityLevel level;

            /** 是否开启硬件编码
             */
            bool isHardwareEncode;

        };

        struct ZegoStreamRelayCDNInfo
        {
            /** 转推至 CDN 的 URL
             */
            std::string url;

            /** 转推状态
             */
            ZegoStreamRelayCDNState state;

            /** 转推状态变更的原因
             */
            ZegoStreamRelayCDNReason reason;

            /** 状态发生的时间戳，单位为毫秒
             */
            unsigned long long time;

        };

        struct ZegoPlayStreamQuality
        {
            /** 视频接收帧率
             */
            double videoRecvFPS;

            /** 视频解码帧率
             */
            double videoDecodeFPS;

            /** 视频渲染帧率
             */
            double videoRenderFPS;

            /** 视频码率
             */
            double videoKBPS;

            /** 音频接收帧率
             */
            double audioRecvFPS;

            /** 音频解码帧率
             */
            double audioDecodeFPS;

            /** 音频渲染帧率
             */
            double audioRenderFPS;

            /** 音频码率
             */
            double audioKBPS;

            /** 服务端至本端的延迟，单位为毫秒
             */
            int rtt;

            /** 丢包率，单位为百分比，0.0 ~ 1.0
             */
            double packetLostRate;

            /** 拉流质量等级
             */
            ZegoStreamQualityLevel level;

            /** 本端接收到数据后到播放的延迟，单位为毫秒
             */
            int delay;

            /** 是否开启硬件解码
             */
            bool isHardwareDecode;

        };

        struct ZegoDeviceInfo
        {
            /** 设备 ID
             */
            std::string deviceID;

            /** 设备名称
             */
            std::string deviceName;

        };

        struct ZegoBeautifyOption
        {
            /** 美颜磨皮的采样步长，取值范围[0,1]。默认 0.2
             */
            double polishStep;

            /** 美颜美白的亮度参数，取值范围[0,1]，值越大亮度越亮。默认 0.5
             */
            double whitenFactor;

            /** 美颜锐化参数，取值范围[0,1]，值越大锐化越强。默认 0.1
             */
            double sharpenFactor;

        };

        struct ZegoMixerAudioConfig
        {
            /** 创建默认混流音频配置，音频码率为 48*1000 bit/s
             */
            ZegoMixerAudioConfig()
            :bitrate(48 * 1000){
            }

            /** 音频码率，默认为 48*1000 bit/s，开始混流任务后不能修改
             */
            int bitrate;

        };

        struct ZegoMixerVideoConfig
        {
            /** 通过预设枚举值创建视频配置
             */
            ZegoMixerVideoConfig(ZegoResolution resolution=ZEGO_RESOLUTION_640x360)
            {
                switch (resolution) {
                    case ZEGO_RESOLUTION_320x180:
                        bitrate = 300*1000;
                        fps = 15;
                        width = 320;
                        height = 180;
                        break;
                    case ZEGO_RESOLUTION_480x270:
                        bitrate = 400*1000;
                        fps = 15;
                        width = 480;
                        height = 270;
                        break;
                    case ZEGO_RESOLUTION_640x360:
                        bitrate = 600*1000;
                        fps = 15;
                        width = 640;
                        height = 360;
                        break;
                    case ZEGO_RESOLUTION_960x540:
                        bitrate = 1200*1000;
                        fps = 15;
                        width = 960;
                        height = 540;
                        break;
                    case ZEGO_RESOLUTION_1280x720:
                        bitrate = 1500*1000;
                        fps = 15;
                        width = 1280;
                        height = 720;
                        break;
                    case ZEGO_RESOLUTION_1920x1080:
                        bitrate = 3000*1000;
                        fps = 15;
                        width = 1920;
                        height = 1080;
                        break;
                }
            }

            /** 视频帧率，开始混流任务后不能修改
             */
            int fps;

            /** 视频码率
             */
            int bitrate;

            /** 视频分辨率宽
             */
            int width;

            /** 视频分辨率高
             */
            int height;

        };

        struct ZegoMixerInput
        {
            /** 
             */
            ZegoMixerInput() : contentType(ZegoMixerInputContentTypeVideo){}
            
            ZegoMixerInput(std::string streamID, ZegoRect layout, ZegoMixerInputContentType contentType)
             : streamID(streamID), contentType(contentType), layout(layout) {}

            /** 混流内容类型
             */
            ZegoMixerInputContentType contentType;

            /** 流 ID
             */
            std::string streamID;

            /** 流的布局
             */
            ZegoRect layout;

        };

        struct ZegoMixerOutput
        {
            /** 
             */
            ZegoMixerOutput() {}
            ZegoMixerOutput(std::string target) :target(target) {}

            /** 混流输出目标，URL 或者流 ID
             */
            std::string target;

        };

        struct ZegoWatermark
        {
            /** 
             */
            ZegoWatermark() {}
            ZegoWatermark(std::string imageURL, ZegoRect layout) :imageURL(imageURL), layout(layout) {}

            /** 水印图片 URL
             */
            std::string imageURL;

            /** 水印图片的大小方位
             */
            ZegoRect layout;

        };

        struct ZegoMixerTask
        {
            /** 
             */
            ZegoMixerTask(std::string taskID): taskID(taskID), watermark(nullptr) {}

            /** 混流任务ID
             */
            std::string taskID;

            /** 混流任务对象的音频配置
             */
            ZegoMixerAudioConfig audioConfig;

            /** 混流任务对象的视频配置
             */
            ZegoMixerVideoConfig videoConfig;

            /** 混流任务对象的输入流列表
             */
            std::vector<ZegoMixerInput> inputList;

            /** 混流任务对象的输出列表
             */
            std::vector<ZegoMixerOutput> outputList;

            /** 混流任务对象的水印
             */
            ZegoWatermark* watermark;

            /** 混流任务对象的背景图片
             */
            std::string backgroundImageURL;

        };

        struct ZegoMixerCDNInfo
        {
            /** rtmp地址列表
             */
            std::vector<std::string> rtmpList;

            /** flv地址列表
             */
            std::vector<std::string> flvList;

            /** hls地址列表
             */
            std::vector<std::string> hlsList;

        };

        struct ZegoMixerStartResult
        {
            /** 错误码
             */
            int errorCode;

        };

        struct ZegoMessageInfo
        {
            /** 消息内容
             */
            std::string message;

            /** 消息的发送时间
             */
            unsigned long long sendTime;

            /** 消息的发送者
             */
            ZegoUser fromUser;

        };


        using ZegoAudioSpectrum = std::vector<double>;
        using ZegoPublisherSetStreamExtraInfoCallback = std::function<void(int errorCode)>;
        using ZegoPublisherUpdateCDNURLCallback = std::function<void(int errorCode)>;
        using ZegoMixerStartCallback = std::function<void(ZegoMixerStartResult result)>;
        using ZegoIMSendBroadcastMessageCallback = std::function<void(int errorCode)>;
        using ZegoIMSendCustomCommandCallback = std::function<void(int errorCode)>;
        const int ZEGO_MORE_THEN_ONE_INSTANCE = 1001028;
        }
}

#endif /* __ZEGOEXPRESSDEFINE_H__ */
    
