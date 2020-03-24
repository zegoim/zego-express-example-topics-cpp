
#ifndef __ZEGOEXPRESSENGINEIMP_H__
#define __ZEGOEXPRESSENGINEIMP_H__

#include "ZegoExpressInterfaces.h"
#include "ZegoInternalBridge.h"

namespace ZEGO
{
namespace EXPRESS
{
template <typename T>
class ZegoSingleton
{
public:
    template <typename... Args>
    static T *CreateInstance(Args &&... args)
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new T(std::forward<Args>(args)...);
        }
        return m_pInstance;
    }

    static T *GetInstance()
    {
        return m_pInstance;
    }

    static void DestroyInstance()
    {
        if (m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

private:
    ZegoSingleton(void) = default;
    virtual ~ZegoSingleton(void) = default;

private:
    static T *m_pInstance;
};
template <class T>
T *ZegoSingleton<T>::m_pInstance = nullptr;

class ZegoExpressEngineImp;
class ZegoExpressGlobalData
{
public:
    std::recursive_mutex EngineMutex;
    std::shared_ptr<ZegoExpressEngineImp> EngineInstance = nullptr;
    ZegoDestroyCompletionCallback afterDestoyed = nullptr;
};

class ZegoExpressConvert
{
public:
    static ZegoUser I2OUser(const zego_user &_user)
    {
        ZegoUser user;
        user.userID = _user.userId;
        user.userName = _user.userName;
        return user;
    }

    static zego_user O2IUser(const ZegoUser &user)
    {
        zego_user _user;
        memset(_user.userId, 0, sizeof(_user.userId));
        memset(_user.userName, 0, sizeof(_user.userName));
        strncpy(_user.userId, user.userID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
        strncpy(_user.userName, user.userName.c_str(), ZEGO_EXPRESS_MAX_USERNAME_LEN);
        return _user;
    }

    static zego_room_config O2IRoomConfig(const ZegoRoomConfig &config)
    {
        zego_room_config _config;
        _config.max_member_count = config.maxMemberCount;
        _config.is_user_status_notify = config.isUserStatusNotify;
        memset(_config.thrid_token, 0, sizeof(_config.thrid_token));
        strncpy(_config.thrid_token, config.token.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        return _config;
    }

    static ZegoStream I2OStream(const zego_stream &_stream)
    {
        ZegoStream stream;
        stream.user.userID = _stream.user.userId;
        stream.user.userName = _stream.user.userName;
        stream.streamID = _stream.streamId;
        stream.extraInfo = _stream.extraInfo;
        return stream;
    }

    static zego_canvas O2ICanvas(const ZegoCanvas &canvas)
    {
        zego_canvas _canvas;
        _canvas.view = canvas.view;
        _canvas.view_mode = zego_view_mode(canvas.viewMode);
        _canvas.background_color = canvas.backgroundColor;
        return _canvas;
    }

    static ZegoPublishStreamQuality I2OPushlishQuality(const zego_publish_stream_quality &_quality)
    {
        ZegoPublishStreamQuality quality;

        quality.videoCaptureFPS = _quality.video_capture_fps;
        quality.videoEncodeFPS = _quality.video_encode_fps;
        quality.videoSendFPS = _quality.video_send_fps;
        quality.videoKBPS = _quality.video_kbps;

        quality.audioCaptureFPS = _quality.audio_capture_fps;
        quality.audioSendFPS = _quality.audio_send_fps;
        quality.audioKBPS = _quality.audio_kbps;

        quality.rtt = _quality.rtt;
        quality.packetLostRate = _quality.packet_lost_rate;
        quality.level = ZegoStreamQualityLevel(_quality.quality);
        quality.isHardwareEncode = _quality.is_hardware_encode;

        return quality;
    }

    static ZegoPlayStreamQuality I2OPlayQuality(const zego_play_stream_quality &_quality)
    {
        ZegoPlayStreamQuality quality;
        quality.videoRecvFPS = _quality.video_recv_fps;
        quality.videoDecodeFPS = _quality.video_decode_fps;
        quality.videoRenderFPS = _quality.video_render_fps;
        quality.videoKBPS = _quality.video_kbps;

        quality.audioRecvFPS = _quality.audio_recv_fps;
        quality.audioDecodeFPS = _quality.audio_decode_fps;
        quality.audioRenderFPS = _quality.audio_render_fps;
        quality.audioKBPS = _quality.audio_kbps;

        quality.rtt = _quality.rtt;
        quality.packetLostRate = _quality.packet_lost_rate;
        quality.level = ZegoStreamQualityLevel(_quality.quality);
        quality.delay = _quality.delay;

        quality.isHardwareDecode = _quality.is_hardware_decode;

        return quality;
    }

    static ZegoDeviceInfo I2ODeviceInfo(const zego_device_info &_deviceInfo)
    {
        ZegoDeviceInfo deviceInfo;
        deviceInfo.deviceID = _deviceInfo.device_id;
        deviceInfo.deviceName = _deviceInfo.device_name;
        return deviceInfo;
    }

    static zego_video_config O2IVideoConfig(const ZegoVideoConfig &config)
    {
        zego_video_config _config;
        _config.bitrate = config.bitrate;
        _config.fps = config.fps;
        _config.capture_resolution_height = config.captureHeight;
        _config.capture_resolution_width = config.captureWidth;
        _config.encode_resolution_height = config.encodeHeight;
        _config.encode_resolution_width = config.encodeWidth;
        _config.video_codec_id = zego_video_codec_id(config.codecID);
        return _config;
    }

    static zego_video_frame_param O2IVideoFrameParam(const ZegoVideoFrameParam &param)
    {
        zego_video_frame_param _param;
        _param.format = zego_video_frame_format(param.format);
        memcpy(_param.strides, param.strides, sizeof(_param.strides));
        _param.height = param.height;
        _param.width = param.width;
        return _param;
    }

    static ZegoVideoFrameParam I2OVideoFrameParam(const zego_video_frame_param &_param)
    {
        ZegoVideoFrameParam param;
        param.format = ZegoVideoFrameFormat(_param.format);
        memcpy(param.strides, _param.strides, sizeof(param.strides));
        param.height = _param.height;
        param.width = _param.width;
        return param;
    }

    static zego_cdn_config O2ICDNConfig(const ZegoCDNConfig &config)
    {
        zego_cdn_config _config;
        memset(_config.url, 0, sizeof(_config.url));
        memset(_config.auth_param, 0, sizeof(_config.auth_param));
        strncpy(_config.url, config.URL.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        strncpy(_config.auth_param, config.authParam.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        return _config;
    }

    static zego_mixer_input O2IMixerInput(const ZegoMixerInput &input)
    {
        zego_mixer_input _input;
        _input.content_type = zego_mixer_input_content_type(input.contentType);
        _input.layout.left = input.layout.x;
        _input.layout.right = input.layout.x + input.layout.width;
        _input.layout.top = input.layout.y;
        _input.layout.bottom = input.layout.y + input.layout.height;
        _input.sound_level_id = input.soundLevelID;
        memset(_input.stream_id, 0, sizeof(_input.stream_id));
        strncpy(_input.stream_id, input.streamID.c_str(), ZEGO_EXPRESS_MAX_STREAM_LEN);
        return _input;
    }

    static zego_mixer_output O2IMixerOutput(const ZegoMixerOutput &output)
    {
        zego_mixer_output _output;
        memset(_output.target, 0, sizeof(_output.target));
        strncpy(_output.target, output.target.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);

        _output.audio_config = O2IMixerAudioConfig(output.audioConfig);
        _output.video_config = O2IMixerVideoConfig(output.videoConfig);
        return _output;
    }

    static zego_mixer_video_config O2IMixerVideoConfig(const ZegoMixerVideoConfig &config)
    {
        zego_mixer_video_config _config;
        _config.bitrate = config.bitrate;
        _config.fps = config.fps;
        _config.resolution_width = config.width;
        _config.resolution_height = config.height;
        return _config;
    }

    static zego_mixer_audio_config O2IMixerAudioConfig(const ZegoMixerAudioConfig &config)
    {
        zego_mixer_audio_config _config;
        _config.bitrate = config.bitrate;
        _config.audio_codec_id = zego_audio_codec_id(config.codecID);
        _config.channel = zego_audio_channel(config.channel);
        return _config;
    }

    static zego_watermark O2IWatermark(const ZegoWatermark &watermark)
    {
        zego_watermark _watermark;
        _watermark.layout.left = watermark.layout.x;
        _watermark.layout.right = watermark.layout.x + watermark.layout.width;
        _watermark.layout.top = watermark.layout.y;
        _watermark.layout.bottom = watermark.layout.y + watermark.layout.height;
        memset(_watermark.image, 0, sizeof(_watermark.image));
        strncpy(_watermark.image, watermark.imageURL.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        return _watermark;
    }
};

#ifdef WIN32
inline void ZegoCommuExchangeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ZEGO_UNUSED_VARIABLE(hWnd);
    ZEGO_UNUSED_VARIABLE(lParam);
    if (uMsg == (WM_APP + 10086))
    {
        std::function<void(void)> *pFunc = (std::function<void(void)> *)wParam;
        (*pFunc)();
        delete pFunc;
    }
}
#endif

class ZegoExpressMediaPlayerImp : public IZegoMediaPlayer
{
public:
    ZegoExpressMediaPlayerImp(int index)
        : instanceIndex(index)
    {
    }

    int getInstanceIndex()
    {
        return instanceIndex;
    }

    void setEventHandler(std::shared_ptr<IZegoMediaPlayerEventHandler> handler) override
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        mpEventHandler = handler;
    }

    void setVideoHandler(std::shared_ptr<IZegoMediaPlayerVideoHandler> handler, ZegoVideoFrameFormat format) override
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (handler)
        {
            ZegoCBridge->mpEnableVideoData(true, zego_video_frame_format(format), zego_mediaplayer_instance_index(instanceIndex));
        }
        else
        {
            ZegoCBridge->mpEnableVideoData(false, zego_video_frame_format(format), zego_mediaplayer_instance_index(instanceIndex));
        }
        mpVideoHandler = handler;
    }

    void setAudioHandler(std::shared_ptr<IZegoMediaPlayerAudioHandler> handler) override
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (handler)
        {
            ZegoCBridge->mpEnableAudioData(true, zego_mediaplayer_instance_index(instanceIndex));
        }
        else
        {
            ZegoCBridge->mpEnableAudioData(false, zego_mediaplayer_instance_index(instanceIndex));
        }
        mpAudioHandler = handler;
    }

    void loadResource(std::string sourcePath, ZegoMediaPlayerLoadResourceCallback callback) override
    {
        int seq = ZegoCBridge->mpPreload(sourcePath.c_str(), zego_mediaplayer_instance_index(instanceIndex));
        ZEGO_UNUSED_VARIABLE(seq);
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpLoadResourceCallbacks = callback;
        }
    }

    void start() override
    {
        ZegoCBridge->mpStart(zego_mediaplayer_instance_index(instanceIndex));
    }

    void stop() override
    {
        ZegoCBridge->mpStop(zego_mediaplayer_instance_index(instanceIndex));
    }

    void pause() override
    {
        ZegoCBridge->mpPause(zego_mediaplayer_instance_index(instanceIndex));
    }

    void resume() override
    {
        ZegoCBridge->mpResume(zego_mediaplayer_instance_index(instanceIndex));
    }

    void seekTo(unsigned long long millisecond, ZegoMediaPlayerSeekToCallback callback) override
    {
        int seq = ZegoCBridge->mpSeekTo(millisecond, zego_mediaplayer_instance_index(instanceIndex));
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpSeekToCallbacks.insert({seq, callback});
        }
    }

    void enableRepeat(bool enable) override
    {
        ZegoCBridge->mpEnableRepeat(enable, zego_mediaplayer_instance_index(instanceIndex));
    }

    void enableAux(bool enable) override
    {
        ZegoCBridge->mpEnableAUX(enable, zego_mediaplayer_instance_index(instanceIndex));
    }

    void muteLocal(bool mute) override
    {
        ZegoCBridge->mpMuteLocal(mute, zego_mediaplayer_instance_index(instanceIndex));
    }

    void setPlayerCanvas(ZegoCanvas *canvas) override
    {
        if (canvas == nullptr)
        {
            ZegoCBridge->mpSetPlayerCanvas(nullptr, zego_mediaplayer_instance_index(instanceIndex));
        }
        else
        {
            zego_canvas _canvas;
            _canvas.view = canvas->view;
            _canvas.view_mode = zego_view_mode(canvas->viewMode);
            _canvas.background_color = canvas->backgroundColor;
            ZegoCBridge->mpSetPlayerCanvas(&_canvas, zego_mediaplayer_instance_index(instanceIndex));
        }
    }

    void setVolume(int volume) override
    {
        ZegoCBridge->mpSetVolume(volume, zego_mediaplayer_instance_index(instanceIndex));
    }

    void setProgressInterval(unsigned long long millisecond) override
    {
        ZegoCBridge->mpSetProgressInterval(millisecond, zego_mediaplayer_instance_index(instanceIndex));
    }

    unsigned long long getTotalDuration() override
    {
        return ZegoCBridge->mpGetTotalDuration(zego_mediaplayer_instance_index(instanceIndex));
    }

    unsigned long long getCurrentProgress() override
    {
        return ZegoCBridge->mpGetCurrentProgress(zego_mediaplayer_instance_index(instanceIndex));
    }

    int getVolume() override
    {
        return ZegoCBridge->mpGetVolume(zego_mediaplayer_instance_index(instanceIndex));
    }

    ZegoMediaPlayerState getCurrentState() override
    {
        zego_mediaplayer_state state = ZegoCBridge->mpGetCurrentState(zego_mediaplayer_instance_index(instanceIndex));
        return ZegoMediaPlayerState(state);
    }

    void zego_on_mediaplayer_state_update(zego_mediaplayer_state state, zego_error error_code)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpEventHandler)
        {
            auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
            ZEGO_SWITCH_THREAD_PRE
            auto mpEventHandler = weakMPEventHandler.lock();
            if (mpEventHandler)
            {
                mpEventHandler->onMediaPlayerStateUpdate(this, ZegoMediaPlayerState(state), error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_mediaplayer_network_event(zego_mediaplayer_network_event event)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        ZEGO_SWITCH_THREAD_PRE
        auto mpEventHandler = weakMPEventHandler.lock();
        if (mpEventHandler)
        {
            mpEventHandler->onMediaPlayerNetworkEvent(this, ZegoMediaPlayerNetworkEvent(event));
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_mediaplayer_playing_progress(unsigned long long millisecond)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        ZEGO_SWITCH_THREAD_PRE
        auto mpEventHandler = weakMPEventHandler.lock();
        if (mpEventHandler)
        {
            mpEventHandler->onMediaPlayerPlayingProgress(this, millisecond);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_mediaplayer_seek_to_result(zego_seq seq, zego_error error_code)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpSeekToCallbacks.count(seq) > 0)
        {
            auto cb = mpSeekToCallbacks[seq];
            mpSeekToCallbacks.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_mediaplayer_load_file_result(zego_error error_code)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpLoadResourceCallbacks)
        {
            ZEGO_SWITCH_THREAD_PRE
            mpLoadResourceCallbacks(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_mediaplayer_video_data(const unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpVideoHandler)
        {
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            mpVideoHandler->onVideoFrame(this, data, data_length, param);
        }
    }

    void zego_on_mediaplayer_audio_data(const unsigned char *data, unsigned int data_length, const struct zego_audio_frame_param _param)
    {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpAudioHandler)
        {
            ZegoAudioFrameParam param;
            param.channel = ZegoAudioChannel(_param.channel);
            param.sampleRate = _param.samples_rate;
            mpAudioHandler->onAudioFrame(this, data, data_length, param);
        }
    }

private:
    int instanceIndex = -1;
    friend class ZegoExpressEngineImp;

    std::shared_ptr<IZegoMediaPlayerEventHandler> mpEventHandler;
    std::shared_ptr<IZegoMediaPlayerVideoHandler> mpVideoHandler;
    std::shared_ptr<IZegoMediaPlayerAudioHandler> mpAudioHandler;
    ZegoMediaPlayerLoadResourceCallback mpLoadResourceCallbacks;
    std::unordered_map<zego_seq, ZegoMediaPlayerSeekToCallback> mpSeekToCallbacks;
    std::mutex mediaEventMutex;
};

using ZegoVoidPtr = void*; 
class ZegoExpressEngineImp : public IZegoExpressEngine
{
public:
    using ZegoExpressEngineImpPtr = ZegoExpressEngineImp *;
    void setEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) override
    {
        std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
        mEventHandler = eventHandler;
    }

    void uploadLog() override
    {
        ZegoCBridge->uploadLog();
    }

    void setDebugVerbose(bool enable, ZegoLanguage language) override
    {
        ZegoCBridge->setDebugVerbose(enable, zego_language(language));
    }

    ///===================================================================================================
    void loginRoom(const std::string &roomID, ZegoUser user) override
    {
        const char *_roomId = roomID.c_str();
        zego_user _user = ZegoExpressConvert::O2IUser(user);
        ZegoCBridge->loginRoom(_roomId, _user, nullptr);
    }

    void loginRoom(const std::string &roomID, ZegoUser user, ZegoRoomConfig config) override
    {
        const char *_roomId = roomID.c_str();
        zego_user _user = ZegoExpressConvert::O2IUser(user);
        zego_room_config _config = ZegoExpressConvert::O2IRoomConfig(config);
        ZegoCBridge->loginRoom(_roomId, _user, &_config);
    }

    void logoutRoom(const std::string &roomID) override
    {
        const char *_roomId = roomID.c_str();
        ZegoCBridge->logoutRoom(_roomId);
    }

    ///===================================================================================================
    void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode) override
    {
        setVideoMirrorMode(mirrorMode, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, ZegoPublishChannel channel) override
    {
        ZegoCBridge->setVideoMirrorMode(zego_video_mirror_mode(mirrorMode), zego_publish_channel(channel));
    }

    void startPreview(ZegoCanvas *canvas) override
    {
        startPreview(canvas, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void startPreview(ZegoCanvas *canvas, ZegoPublishChannel channel) override
    {
        if (canvas == nullptr)
        {
            ZegoCBridge->startPreview(nullptr, zego_publish_channel(channel));
        }
        else
        {
            zego_canvas _canvas;
            _canvas.view = canvas->view;
            _canvas.view_mode = zego_view_mode(canvas->viewMode);
            _canvas.background_color = canvas->backgroundColor;
            ZegoCBridge->startPreview(&_canvas, zego_publish_channel(channel));
        }
    }

    void startPreview()
    {
        ZegoCBridge->startPreview(nullptr, zego_publish_channel_main);
    }

    void stopPreview() override
    {
        stopPreview(ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void stopPreview(ZegoPublishChannel channel) override
    {
        ZegoCBridge->stopPreview(zego_publish_channel(channel));
    }

    void setAudioConfig(ZegoAudioConfig audioConfig) override
    {
        zego_audio_config config;
        config.bitrate = audioConfig.bitrate;
        config.channel = zego_audio_channel(audioConfig.channel);
        config.audio_codec_id = zego_audio_codec_id(audioConfig.codecID);
        ZegoCBridge->setAudioConfig(config);
    }

    void setVideoConfig(ZegoVideoConfig videoConfig) override
    {
        setVideoConfig(videoConfig, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void setVideoConfig(ZegoVideoConfig videoConfig, ZegoPublishChannel channel) override
    {
        zego_video_config _config = ZegoExpressConvert::O2IVideoConfig(videoConfig);
        ZegoCBridge->setVideoConfig(_config, zego_publish_channel(channel));
    }

    void startPublishingStream(const std::string &streamID) override
    {
        startPublishingStream(streamID, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void startPublishingStream(const std::string &streamID, ZegoPublishChannel channel) override
    {
        const char *stream_id = streamID.c_str();
        ZegoCBridge->startPublishingStream(stream_id, zego_publish_channel(channel));
    }

    void stopPublishingStream() override
    {
        stopPublishingStream(ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void stopPublishingStream(ZegoPublishChannel channel) override
    {
        ZegoCBridge->stopPublishingStream(zego_publish_channel(channel));
    }

    void setStreamExtraInfo(const std::string &extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback) override
    {
        setStreamExtraInfo(extraInfo, callback, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void setStreamExtraInfo(const std::string &extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback, ZegoPublishChannel channel) override
    {
        const char *_extraInfo = extraInfo.c_str();
        int seq = ZegoCBridge->setStreamExtraInfo(_extraInfo, zego_publish_channel(channel));
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
            StreamExtraInfoCallbacks.insert({seq, callback});
        }
    }

    void mutePublishStreamAudio(bool mute) override
    {
        mutePublishStreamAudio(mute, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void mutePublishStreamAudio(bool mute, ZegoPublishChannel channel) override
    {
        ZegoCBridge->mutePublishStreamAudio(mute, zego_publish_channel(channel));
    }

    void mutePublishStreamVideo(bool mute) override
    {
        mutePublishStreamVideo(mute, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void mutePublishStreamVideo(bool mute, ZegoPublishChannel channel) override
    {
        ZegoCBridge->mutePublishStreamVideo(mute, zego_publish_channel(channel));
    }

    void setCaptureVolume(int volume) override
    {
        ZegoCBridge->setCaptureVolume(volume);
    }

    void addPublishCDNURL(const std::string &streamID, const std::string &targetURL, ZegoPublisherUpdateCDNURLCallback callback) override
    {
        const char *stream_id = streamID.c_str();
        const char *target_url = targetURL.c_str();

        int seq = ZegoCBridge->addPublishCNDURL(stream_id, target_url);
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
            CDNCallbacks.insert({seq, callback});
        }
    }

    void removePublishCDNURL(const std::string &streamID, const std::string &targetURL, ZegoPublisherUpdateCDNURLCallback callback) override
    {
        const char *stream_id = streamID.c_str();
        const char *target_url = targetURL.c_str();

        int seq = ZegoCBridge->removePublishCNDURL(stream_id, target_url);
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
            CDNCallbacks.insert({seq, callback});
        }
    }

    void enablePublishDirectToCDN(bool enable, ZegoCDNConfig *config) override
    {
        enablePublishDirectToCDN(enable, config, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void enablePublishDirectToCDN(bool enable, ZegoCDNConfig *config, ZegoPublishChannel channel) override
    {
        if (config == nullptr)
        {
            ZegoCBridge->enablePublishDirectToCDN(enable, nullptr, zego_publish_channel(channel));
        }
        else
        {
            zego_cdn_config _config = ZegoExpressConvert::O2ICDNConfig(*config);
            ZegoCBridge->enablePublishDirectToCDN(enable, &_config, zego_publish_channel(channel));
        }
    }

    void setPublishWatermark(ZegoWatermark *watermark, bool isPreviewVisible) override
    {
        setPublishWatermark(watermark, isPreviewVisible, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void setPublishWatermark(ZegoWatermark *watermark, bool isPreviewVisible, ZegoPublishChannel channel) override
    {
        if (watermark != nullptr)
        {
            zego_watermark _watermark = ZegoExpressConvert::O2IWatermark(*watermark);
            ZegoCBridge->setPublishWatermark(&_watermark, isPreviewVisible, zego_publish_channel(channel));
        }
        else
        {
            ZegoCBridge->setPublishWatermark(nullptr, isPreviewVisible, zego_publish_channel(channel));
        }
    }

    void enableHardwareEncoder(bool enable) override
    {
        ZegoCBridge->enableHardwareEncoder(enable);
    }

    void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) override
    {
        zego_capture_pipeline_scale_mode _mode = zego_capture_pipeline_scale_mode(mode);
        ZegoCBridge->setCapturePipelineScaleMode(_mode);
    }

    void sendSEI(const unsigned char *buffer, unsigned int buffer_length) override
    {
        sendSEI(buffer, buffer_length, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void sendSEI(const unsigned char *buffer, unsigned int buffer_length, ZegoPublishChannel channel) override
    {
        ZegoCBridge->sendSEI(buffer, buffer_length, zego_publish_channel(channel));
    }

    void enableTrafficControl(bool enable, int property) override
    {
        ZegoCBridge->enableTrafficControl(enable, property);
    }

    void setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode) override
    {
        ZegoCBridge->setMinVideoBitrateForTrafficControl(bitrate, zego_traffic_control_min_video_bitrate_mode(mode));
    }

    ///===================================================================================================
    void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas) override
    {
        const char *stream_id = streamID.c_str();
        if (canvas == nullptr)
        {
            ZegoCBridge->startPlayingStream(stream_id, nullptr);
        }
        else
        {
            zego_canvas _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
            ZegoCBridge->startPlayingStream(stream_id, &_canvas);
        }
    }

    void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas, ZegoPlayerConfig config) override
    {
        const char *stream_id = streamID.c_str();
        zego_canvas _canvas;
        if (canvas)
        {
            _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
        }
        zego_player_config _config;
        zego_cdn_config cdn_cofig;
        if (config.CDNConfig)
        {
            cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.CDNConfig);
            _config.cdn_config = &cdn_cofig;
        }
        else
        {
            _config.cdn_config = nullptr;
        }
        _config.video_layer = zego_player_video_layer(config.videoLayer);
        ZegoCBridge->startPlayingStreamWithConfig(stream_id, canvas ? &_canvas : nullptr, _config);
    }

    void startPlayingStream(const std::string &streamID)
    {
        const char *stream_id = streamID.c_str();
        ZegoCBridge->startPlayingStream(stream_id, nullptr);
    }

    void startPlayingStream(const std::string &streamID, ZegoPlayerConfig config)
    {
        const char *stream_id = streamID.c_str();
        zego_player_config _config;
        zego_cdn_config cdn_cofig;
        if (config.CDNConfig)
        {
            cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.CDNConfig);
            _config.cdn_config = &cdn_cofig;
        }
        else
        {
            _config.cdn_config = nullptr;
        }
        _config.video_layer = zego_player_video_layer(config.videoLayer);
        ZegoCBridge->startPlayingStreamWithConfig(stream_id, nullptr, _config);
    }

    void stopPlayingStream(const std::string &streamID) override
    {
        const char *stream_id = streamID.c_str();
        ZegoCBridge->stopPlayingStream(stream_id);
    }

    void setPlayVolume(const std::string &streamID, int volume) override
    {
        const char *stream_id = streamID.c_str();
        ZegoCBridge->setPlayVolume(stream_id, volume);
    }

    void mutePlayStreamAudio(const std::string &streamID, bool mute) override
    {
        const char *stream_id = streamID.c_str();
        ZegoCBridge->mutePlayStreamAudio(stream_id, mute);
    }

    void mutePlayStreamVideo(const std::string &streamID, bool mute) override
    {
        const char *stream_id = streamID.c_str();
        ZegoCBridge->mutePlayStreamVideo(stream_id, mute);
    }

    void enableHardwareDecoder(bool enable) override
    {
        ZegoCBridge->enableHardwareDecoder(enable);
    }

    void enableCheckPoc(bool enable) override
    {
        ZegoCBridge->enableCheckPoc(enable);
    }

    //===================================================================================================

    void muteMicrophone(bool enable) override
    {
        ZegoCBridge->muteMicrophone(enable);
    }

    void muteAudioOutput(bool mute) override
    {
        ZegoCBridge->muteAudioOutput(mute);
    }

    void enableCamera(bool enable) override
    {
        enableCamera(enable, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void enableCamera(bool enable, ZegoPublishChannel channel) override
    {
        ZegoCBridge->enableCamera(enable, zego_publish_channel(channel));
    }

    void enableAudioCaptureDevice(bool enable) override
    {
        ZegoCBridge->enableAudioCaptureDevice(enable);
    }

    void startSoundLevelMonitor() override
    {
        ZegoCBridge->startSoundLevelMonitor();
    }

    void stopSoundLevelMonitor() override
    {
        ZegoCBridge->stopSoundLevelMonitor();
    }
    void startAudioSpectrumMonitor() override
    {
        ZegoCBridge->startAudioSpectrumMonitor();
    }

    void stopAudioSpectrumMonitor() override
    {
        ZegoCBridge->stopAudioSpectrumMonitor();
    }

    void useAudioDevice(ZegoAudioDeviceType deviceType, const std::string &deviceID) override
    {
        const char *device_id = deviceID.c_str();
        ZegoCBridge->useAudioDevice(zego_audio_device_type(deviceType), device_id);
    }

    std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) override
    {
        int count = 0;
        zego_device_info *_deviceInfoList = ZegoCBridge->getAudioDeviceList(zego_audio_device_type(deviceType), &count);

        std::vector<ZegoDeviceInfo> result;
        for (int i = 0; i < count; i++)
        {
            ZegoDeviceInfo deviceInfo = ZegoExpressConvert::I2ODeviceInfo(_deviceInfoList[i]);
            result.push_back(deviceInfo);
        }

        ZegoCBridge->freeAudioDeviceList(_deviceInfoList);
        return result;
    }

    void useVideoDevice(const std::string &deviceID) override
    {
        useVideoDevice(deviceID, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void useVideoDevice(const std::string &deviceID, ZegoPublishChannel channel) override
    {
        const char *device_id = deviceID.c_str();
        ZegoCBridge->useVideoDevice(device_id, zego_publish_channel(channel));
    }

    std::vector<ZegoDeviceInfo> getVideoDeviceList() override
    {
        int count = 0;
        zego_device_info *_deviceInfoList = ZegoCBridge->getVideoDeviceList(&count);

        std::vector<ZegoDeviceInfo> result;
        for (int i = 0; i < count; i++)
        {
            ZegoDeviceInfo deviceInfo = ZegoExpressConvert::I2ODeviceInfo(_deviceInfoList[i]);
            result.push_back(deviceInfo);
        }

        ZegoCBridge->freeVideoDeviceList(_deviceInfoList);
        return result;
    }

    //===================================================================================================

    void enableAEC(bool enable) override
    {
        ZegoCBridge->enableAEC(enable);
    }

    void setAECMode(ZegoAECMode mode) override
    {
        ZegoCBridge->setAECMode(zego_aec_mode(mode));
    }

    void enableAGC(bool enable) override
    {
        ZegoCBridge->enableAGC(enable);
    }

    void enableANS(bool enable) override
    {
        ZegoCBridge->enableANS(enable);
    }

    //===================================================================================================
    void sendBroadcastMessage(const std::string &roomID, const std::string &message, ZegoIMSendBroadcastMessageCallback callback) override
    {
        const char *_roomID = roomID.c_str();
        const char *_message = message.c_str();

        int seq = ZegoCBridge->sendBroadcastMessage(_roomID, _message);
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
            IMSendBroadcastMessageCallbacks.insert({seq, callback});
        }
    }

    void sendBarrageMessage(const std::string &roomID, const std::string &message, ZegoIMSendBarrageMessageCallback callback) override
    {
        const char *_roomID = roomID.c_str();
        const char *_message = message.c_str();

        int seq = ZegoCBridge->sendBarrageMessage(_roomID, _message);
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
            IMSendBarrageMessageCallbacks.insert({seq, callback});
        }
    }

    void sendCustomCommand(const std::string &roomID, const std::string &command, std::vector<ZegoUser> toUserList, ZegoIMSendCustomCommandCallback callback) override
    {
        std::vector<zego_user> _userList;
        for (auto user : toUserList)
        {
            zego_user _user = ZegoExpressConvert::O2IUser(user);
            _userList.push_back(_user);
        }
        zego_user *users = _userList.data();
        const char *_roomID = roomID.c_str();
        const char *_command = command.c_str();

        int seq = ZegoCBridge->sendCustomCommand(_roomID, _command, users, u_int(_userList.size()));
        if (callback != nullptr)
        {
            std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
            IMSendCustomCommandCallbacks.insert({seq, callback});
        }
    }

    //===================================================================================================
    void StartOrStopMixerTask(bool isStart, ZegoMixerTask task, ZegoMixerStartCallback startCallback, ZegoMixerStopCallback stopCallback)
    {
        zego_mixer_task _task;

        memset(_task.task_id, 0, sizeof(_task.task_id));
        strncpy(_task.task_id, task.taskID.c_str(), ZEGO_EXPRESS_MAX_MIXER_TASK_LEN);

        std::vector<zego_mixer_input> input_list;
        {
            for (auto &input : task.inputList)
            {
                zego_mixer_input _input = ZegoExpressConvert::O2IMixerInput(input);
                input_list.push_back(_input);
            }
        }
        _task.input_list = input_list.data();
        _task.input_list_count = u_int(input_list.size());

        std::vector<zego_mixer_output> output_list;
        {
            for (auto &output : task.outputList)
            {
                zego_mixer_output _output = ZegoExpressConvert::O2IMixerOutput(output);
                output_list.push_back(_output);
            }
        }
        _task.output_list = output_list.data();
        _task.output_list_count = u_int(output_list.size());

        zego_watermark _watermark;
        if (task.watermark != nullptr)
        {
            _watermark = ZegoExpressConvert::O2IWatermark(*task.watermark);
            _task.watermark = &_watermark;
        }
        else
        {
            _task.watermark = nullptr;
        }

        memset(_task.background_image_url, 0, sizeof(_task.background_image_url));
        strncpy(_task.background_image_url, task.backgroundImageURL.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);

        _task.enable_sound_level = task.enableSoundLevel;

        if (isStart)
        {
            int seq = ZegoCBridge->startMixerTask(_task);
            if (startCallback != nullptr)
            {
                std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
                mixerStartCallbacks.insert({seq, startCallback});
            }
        }
        else
        {
            int seq = ZegoCBridge->stopMixerTask(_task);
            if (stopCallback != nullptr)
            {
                std::lock_guard<std::mutex> lock(mEngineEventHandlerMutex);
                mixerStopCallbacks.insert({seq, stopCallback});
            }
        }
    }

    void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) override
    {
        StartOrStopMixerTask(true, task, callback, nullptr);
    }

    void stopMixerTask(ZegoMixerTask task, ZegoMixerStopCallback callback) override
    {
        StartOrStopMixerTask(false, task, nullptr, callback);
    }

    //===================================================================================================
    IZegoMediaPlayer *createMediaPlayer() override
    {
        auto index = ZegoCBridge->mpCreate();
        if (index == zego_mediaplayer_instance_index_null)
        {
            return nullptr;
        }
        std::lock_guard<std::mutex> lock(mMediaPlayerEventMutex);
        auto mediaPlayer = std::make_shared<ZegoExpressMediaPlayerImp>(index);
        mMediaPlayers.insert({index, mediaPlayer});
        return mediaPlayer.get();
    }

    void destroyMediaPlayer(IZegoMediaPlayer *&IMediaPlayer) override
    {
        std::lock_guard<std::mutex> lock(mMediaPlayerEventMutex);
        for (auto iter = mMediaPlayers.begin(); iter != mMediaPlayers.end(); iter++)
        {
            if (iter->second.get() == IMediaPlayer)
            {
                ZegoCBridge->mpRelease(zego_mediaplayer_instance_index(iter->second->getInstanceIndex()));
                mMediaPlayers.erase(iter);
                IMediaPlayer = nullptr;
                break;
            }
        }
    }

    //===================================================================================================
    void setCustomVideoCaptureHandler(std::shared_ptr<IZegoCustomVideoCaptureHandler> videoCaptureHandler) override
    {
        std::lock_guard<std::mutex> locker(mEngineCaptureHandlerMutex);
        mCustomVideoCaptureHandler = videoCaptureHandler;
    }

    void setCustomVideoCaptureFillMode(ZegoViewMode mode) override
    {
        setCustomVideoCaptureFillMode(mode, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void setCustomVideoCaptureFillMode(ZegoViewMode mode, ZegoPublishChannel channel) override
    {
        ZegoCBridge->customVideoCaptureSetFillMode(zego_view_mode(mode), zego_publish_channel(channel));
    }

    void sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength, ZegoVideoFrameParam param, unsigned long long referenceTimeMillisecond) override
    {
        sendCustomVideoCaptureRawData(data, dataLength, param, referenceTimeMillisecond, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    void sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength, ZegoVideoFrameParam param, unsigned long long referenceTimeMillisecond, ZegoPublishChannel channel) override
    {
        auto _param = ZegoExpressConvert::O2IVideoFrameParam(param);
        ZegoCBridge->customVideoCaptureSendRawData(data, dataLength, _param, referenceTimeMillisecond, zego_publish_channel(channel));
    }

    //===================================================================================================
    void setCustomVideoRenderHandler(std::shared_ptr<IZegoCustomVideoRenderHandler> videoRenderHandler) override
    {
        std::lock_guard<std::mutex> locker(mEngineRenderHandlerMutex);
        mCustomVideoRenderHandler = videoRenderHandler;
    }

public:
    static std::shared_ptr<IZegoEventHandler> eventHandlerFromContext(void *context)
    {
        std::shared_ptr<IZegoEventHandler> handler = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                handler = pthis->mEventHandler;
            }
        }
        return handler;
    }

    static std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayerFromContext(void *context, int instance_index)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mMediaPlayerEventMutex);
                if (pthis->mMediaPlayers.count(instance_index) > 0)
                {
                    mediaPlayer = pthis->mMediaPlayers[instance_index];
                }
            }
        }
        return mediaPlayer;
    }

    // DEBUG CALLBACK
    static void zego_on_debug_error(int error_code, const char *func_name, const char *_info, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string funcName = func_name;
        std::string info = _info;
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onDebugError(error_code, funcName, info);
        ZEGO_SWITCH_THREAD_ING
    }

    // ROOM CALLBACK
    static void zego_on_room_state_update(const char *room_id, zego_room_state state, zego_error error_code, const char *extend_data, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string roomID = room_id;
        std::string extendData = extend_data;
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRoomStateUpdate(roomID, ZegoRoomState(state), error_code, extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_stream_update(const char *room_id, zego_update_type update_type, const struct zego_stream *stream_info_list, unsigned int stream_info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string roomID = room_id;
        std::vector<ZegoStream> streamList;
        for (unsigned int i = 0; i < stream_info_count; i++)
        {
            ZegoStream stream = ZegoExpressConvert::I2OStream(stream_info_list[i]);
            streamList.push_back(stream);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRoomStreamUpdate(roomID, ZegoUpdateType(update_type), streamList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_room_stream_extra_info_update(const char *room_id, const struct zego_stream *stream_info_list, unsigned int stream_info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string roomID = room_id;
        std::vector<ZegoStream> streamList;
        for (unsigned int i = 0; i < stream_info_count; i++)
        {
            ZegoStream stream = ZegoExpressConvert::I2OStream(stream_info_list[i]);
            streamList.push_back(stream);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRoomStreamExtraInfoUpdate(roomID, streamList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_user_update(const char *room_id, zego_update_type update_type, const struct zego_user *user_list, unsigned int user_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string roomID = room_id;
        std::vector<ZegoUser> userList;
        for (unsigned int i = 0; i < user_count; i++)
        {
            ZegoUser user = ZegoExpressConvert::I2OUser(user_list[i]);
            userList.push_back(user);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRoomUserUpdate(roomID, ZegoUpdateType(update_type), userList);
        ZEGO_SWITCH_THREAD_ING
    }

    // PUBLISHER CALLBACK
    static void zego_on_publisher_state_update(const char *stream_id, zego_publisher_state state, zego_error error_code, const char *extend_data, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;
        std::string extendData = extend_data;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPublisherStateUpdate(streamID, ZegoPublisherState(state), error_code, extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_quality_update(const char *stream_id, zego_publish_stream_quality quality, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        ZegoPublishStreamQuality pushlishQuality = ZegoExpressConvert::I2OPushlishQuality(quality);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPublisherQualityUpdate(streamID, pushlishQuality);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_recv_audio_captured_first_frame(void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPublisherCapturedAudioFirstFrame();
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_recv_video_captured_first_frame(enum zego_publish_channel channel, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPublisherCapturedVideoFirstFrame(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_video_size_changed(int width, int height, enum zego_publish_channel channel, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPublisherVideoSizeChanged(width, height, ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_update_cdn_url_result(const char *stream_id, zego_error error_code, zego_seq seq, void *user_context)
    {
        ZEGO_UNUSED_VARIABLE(stream_id);
        ZegoPublisherUpdateCDNURLCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->CDNCallbacks.count(seq) > 0)
                {
                    callback = pthis->CDNCallbacks[seq];
                    pthis->CDNCallbacks.erase(seq);
                }
            }
        }
        if(callback){
            ZEGO_SWITCH_THREAD_PRE
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_publisher_relay_cdn_state_update(const char *stream_id, const zego_stream_relay_cdn_info *state_info_list, unsigned int info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;
        std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

        for (unsigned int i = 0; i < info_count; i++)
        {
            zego_stream_relay_cdn_info _info = state_info_list[i];
            ZegoStreamRelayCDNInfo info;
            info.URL = _info.url;
            info.stateTime = _info.state_time;
            info.state = ZegoStreamRelayCDNState(_info.cdn_state);
            info.updateReason = ZegoStreamRelayCDNUpdateReason(_info.update_reason);
            streamInfoList.push_back(info);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_update_stream_extra_info_result(zego_error error_code, zego_seq seq, void *user_context)
    {
        ZegoPublisherSetStreamExtraInfoCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->StreamExtraInfoCallbacks.count(seq) > 0)
                {
                    callback = pthis->StreamExtraInfoCallbacks[seq];
                    pthis->StreamExtraInfoCallbacks.erase(seq);
                }
            }
        }
        if(callback){
            ZEGO_SWITCH_THREAD_PRE
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    // PLAYER CALLBACK
    static void zego_on_player_state_update(const char *stream_id, zego_player_state state, zego_error error_code, const char *extend_data, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;
        std::string extendData = extend_data;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerStateUpdate(streamID, ZegoPlayerState(state), error_code, extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_quality_update(const char *stream_id, zego_play_stream_quality quality, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;
        ZegoPlayStreamQuality playQuality = ZegoExpressConvert::I2OPlayQuality(quality);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerQualityUpdate(streamID, playQuality);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_media_event(const char *stream_id, zego_player_media_event media_event, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerMediaEvent(streamID, ZegoPlayerMediaEvent(media_event));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_audio_first_frame(const char *stream_id, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerRecvAudioFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_video_first_frame(const char *stream_id, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerRecvVideoFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_render_video_first_frame(const char *stream_id, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerRenderVideoFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_video_size_changed(const char *stream_id, int width, int height, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerVideoSizeChanged(streamID, width, height);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_sei(const char *stream_id, const unsigned char *data, unsigned int data_length, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;
        std::vector<unsigned char> seiData;
        seiData.reserve(data_length);
        for (unsigned int i = 0; i < data_length; i++)
        {
            seiData.push_back(data[i]);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onPlayerRecvSEI(streamID, seiData.data(), u_int(seiData.size()));
        ZEGO_SWITCH_THREAD_ING
    }

    // DEVICE CALLBACK
    static void zego_on_audio_device_state_changed(enum zego_update_type update_type, enum zego_audio_device_type device_type, struct zego_device_info _device, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        ZegoDeviceInfo device = ZegoExpressConvert::I2ODeviceInfo(_device);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onAudioDeviceStateChanged(ZegoUpdateType(update_type), ZegoAudioDeviceType(device_type), device);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_video_device_state_changed(enum zego_update_type update_type, struct zego_device_info _device, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        ZegoDeviceInfo device = ZegoExpressConvert::I2ODeviceInfo(_device);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onVideoDeviceStateChanged(ZegoUpdateType(update_type), device);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_device_error(zego_error error_code, const char *device_name, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string deviceName = device_name;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onDeviceError(error_code, deviceName);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_remote_mic_state_update(const char *stream_id, zego_remote_device_state state, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRemoteMicStateUpdate(streamID, ZegoRemoteDeviceState(state));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_remote_camera_state_update(const char *stream_id, zego_remote_device_state state, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRemoteCameraStateUpdate(streamID, ZegoRemoteDeviceState(state));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_captured_sound_level_update(const zego_sound_level_info *sound_level_info, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        double soundLevel = sound_level_info->sound_level;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onCapturedSoundLevelUpdate(soundLevel);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_remote_sound_level_update(const zego_sound_level_info *sound_level_info_list, unsigned int info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        std::map<std::string, double> soundLevels;
        for (unsigned int i = 0; i < info_count; i++)
        {
            zego_sound_level_info sound_level_info = sound_level_info_list[i];
            std::string streamID = sound_level_info.stream_id;
            double soundLevel = sound_level_info.sound_level;
            soundLevels.insert({streamID, soundLevel});
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRemoteSoundLevelUpdate(soundLevels);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_captured_audio_spectrum_update(const zego_audio_spectrum_info *audio_spectrum_info, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        ZegoAudioSpectrum audioSpectrum;
        for (unsigned int i = 0; i < audio_spectrum_info->spectrum_count; i++)
        {
            audioSpectrum.push_back(audio_spectrum_info->spectrum_list[i]);
        }
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onCapturedAudioSpectrumUpdate(audioSpectrum);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_remote_audio_spectrum_update(const zego_audio_spectrum_info *audio_spectrum_info_list, unsigned int info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::map<std::string, ZegoAudioSpectrum> audioSpectrums;
        for (unsigned int j = 0; j < info_count; j++)
        {
            zego_audio_spectrum_info frequency_spectrum_info = audio_spectrum_info_list[j];

            std::string streamID = frequency_spectrum_info.stream_id;
            ZegoAudioSpectrum frequencySpectrum;
            for (unsigned int i = 0; i < frequency_spectrum_info.spectrum_count; i++)
            {
                frequencySpectrum.push_back(frequency_spectrum_info.spectrum_list[i]);
            }
            audioSpectrums.insert({streamID, frequencySpectrum});
        }
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRemoteAudioSpectrumUpdate(audioSpectrums);
        ZEGO_SWITCH_THREAD_ING
    }

    // IM CALLBACK
    static void zego_on_im_send_broadcast_message_result(const char *room_id, unsigned long long message_id, zego_error error_code, zego_seq seq, void *user_context)
    {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZegoIMSendBroadcastMessageCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->IMSendBroadcastMessageCallbacks.count(seq) > 0)
                {
                    callback = pthis->IMSendBroadcastMessageCallbacks[seq];
                    pthis->IMSendBroadcastMessageCallbacks.erase(seq);
                }
            }
        }
        if(callback){
            unsigned long long messageID = message_id;
            ZEGO_SWITCH_THREAD_PRE
            callback(error_code, messageID);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_im_recv_broadcast_message(const char *room_id, const struct zego_broadcast_message_info *message_info_list, unsigned int message_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string roomID = room_id;
        std::vector<ZegoBroadcastMessageInfo> messageInfoList;
        for (unsigned int i = 0; i < message_count; i++)
        {
            zego_broadcast_message_info message_info = message_info_list[i];
            ZegoBroadcastMessageInfo messageInfo;
            messageInfo.messageID = message_info.message_id;
            messageInfo.sendTime = message_info.send_time;
            messageInfo.message = message_info.message;
            messageInfo.fromUser = ZegoExpressConvert::I2OUser(message_info.from_user);
            messageInfoList.push_back(messageInfo);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onIMRecvBroadcastMessage(roomID, messageInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_im_send_barrage_message_result(const char *room_id, const char *message_id, zego_error error_code, zego_seq seq, void *user_context)
    {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZegoIMSendBarrageMessageCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->IMSendBarrageMessageCallbacks.count(seq) > 0)
                {
                    callback = pthis->IMSendBarrageMessageCallbacks[seq];
                    pthis->IMSendBarrageMessageCallbacks.erase(seq);
                }
            }
        }
        if(callback){
            std::string messageID = message_id;
            ZEGO_SWITCH_THREAD_PRE
            callback(error_code, messageID);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_im_recv_barrage_message(const char *room_id, const struct zego_barrage_message_info *message_info_list, unsigned int message_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        std::string roomID = room_id;
        std::vector<ZegoBarrageMessageInfo> messageInfoList;
        for (unsigned int i = 0; i < message_count; i++)
        {
            zego_barrage_message_info message_info = message_info_list[i];
            ZegoBarrageMessageInfo messageInfo;
            messageInfo.messageID = message_info.message_id;
            messageInfo.sendTime = message_info.send_time;
            messageInfo.message = message_info.message;
            messageInfo.fromUser = ZegoExpressConvert::I2OUser(message_info.from_user);
            messageInfoList.push_back(messageInfo);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onIMRecvBarrageMessage(roomID, messageInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_im_send_custom_command_result(const char *room_id, zego_error error_code, zego_seq seq, void *user_context)
    {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZegoIMSendCustomCommandCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->IMSendCustomCommandCallbacks.count(seq) > 0)
                {
                    callback = pthis->IMSendCustomCommandCallbacks[seq];
                    pthis->IMSendCustomCommandCallbacks.erase(seq);
                }
            }
        }
        if(callback){
            ZEGO_SWITCH_THREAD_PRE
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_im_recv_custom_command(const char *room_id, struct zego_user from_user, const char *_content, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        std::string roomID = room_id;
        ZegoUser user = ZegoExpressConvert::I2OUser(from_user);
        std::string content = _content;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onIMRecvCustomCommand(roomID, user, content);
        ZEGO_SWITCH_THREAD_ING
    }

    // MIXER CALLBACK
    static void zego_on_mixer_start_result(zego_error error_code, zego_seq seq, const char *extended_data, void *user_context)
    {
        ZegoMixerStartCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->mixerStartCallbacks.count(seq) > 0)
                {
                    callback = pthis->mixerStartCallbacks[seq];
                    pthis->mixerStartCallbacks.erase(seq);
                }
            }
        }
        if(callback){
            int errorCode = error_code;
            std::string extendData = extended_data;
            ZEGO_SWITCH_THREAD_PRE
            callback(errorCode, extendData);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_mixer_stop_result(zego_error error_code, zego_seq seq, void *user_context)
    {
        ZegoMixerStopCallback callback = nullptr;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineEventHandlerMutex);
                if (pthis->mixerStopCallbacks.count(seq) > 0)
                {
                    callback = pthis->mixerStopCallbacks[seq];
                    pthis->mixerStopCallbacks.erase(seq);
                }
            }
        }
        if(callback){
        int errorCode = error_code;
        ZEGO_SWITCH_THREAD_PRE
        callback(errorCode);
        ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_mixer_sound_level_update(const struct zego_mixer_sound_level_info *sound_level_info_list, unsigned int info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);

        std::map<unsigned int, double> soundLevels;
        for (unsigned int i = 0; i < info_count; i++)
        {
            zego_mixer_sound_level_info sound_level_info = sound_level_info_list[i];
            unsigned int soundLevelID = sound_level_info.sound_level_id;
            double soundLevel = sound_level_info.sound_level;
            soundLevels.insert({soundLevelID, soundLevel});
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onMixerSoundLevelUpdate(soundLevels);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_mixer_relay_cdn_state_update(const char *task_id, const struct zego_stream_relay_cdn_info *cdn_info_list, unsigned int info_count, void *user_context)
    {
        auto handler = eventHandlerFromContext(user_context);
        std::string taskID = task_id;
        std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

        for (unsigned int i = 0; i < info_count; i++)
        {
            zego_stream_relay_cdn_info _info = cdn_info_list[i];
            ZegoStreamRelayCDNInfo info;
            info.URL = _info.url;
            info.stateTime = _info.state_time;
            info.state = ZegoStreamRelayCDNState(_info.cdn_state);
            info.updateReason = ZegoStreamRelayCDNUpdateReason(_info.update_reason);
            streamInfoList.push_back(info);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onMixerRelayCDNStateUpdate(streamInfoList, taskID);
        ZEGO_SWITCH_THREAD_ING
    }

    // MediaPlayer Callback
    static void zego_on_mediaplayer_state_update(zego_mediaplayer_state state, zego_error error_code, zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_state_update(state, error_code);
        }
    }

    static void zego_on_mediaplayer_network_event(zego_mediaplayer_network_event event, zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_network_event(event);
        }
    }

    static void zego_on_mediaplayer_playing_progress(unsigned long long millisecond, zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_playing_progress(millisecond);
        }
    }

    static void zego_on_mediaplayer_seek_to_time_result(zego_seq seq, zego_error error_code, enum zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_seek_to_result(seq, error_code);
        }
    }

    static void zego_on_mediaplayer_load_file_result(zego_error error_code, zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_load_file_result(error_code);
        }
    }

    static void zego_on_mediaplayer_audio_data(const unsigned char *data, unsigned int data_length, const struct zego_audio_frame_param param, enum zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_audio_data(data, data_length, param);
        }
    }

    static void zego_on_mediaplayer_video_data(const unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param param, enum zego_mediaplayer_instance_index instance_index, void *user_context)
    {
        std::shared_ptr<ZegoExpressMediaPlayerImp> mediaPlayer = mediaPlayerFromContext(user_context, instance_index);
        if(mediaPlayer)
        {
            mediaPlayer->zego_on_mediaplayer_video_data(data, data_length, param);
        }
    }

    // CustomVideoRender
    static void zego_on_custom_video_render_captured_frame_data(unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param, enum zego_video_flip_mode flip_mode, enum zego_publish_channel channel, void *user_context)
    {
        std::shared_ptr<IZegoCustomVideoRenderHandler> handler;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineRenderHandlerMutex);
                handler = pthis->mCustomVideoRenderHandler;
            }
        }
        if (handler)
        {
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            ZegoVideoFlipMode flipMode = ZegoVideoFlipMode(flip_mode);
            handler->onCapturedVideoFrameRawData(data, data_length, param, flipMode, ZegoPublishChannel(channel));
        }
    }

    static void zego_on_custom_video_render_remote_frame_data(const char *stream_id, unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param, void *user_context)
    {
        std::shared_ptr<IZegoCustomVideoRenderHandler> handler;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineRenderHandlerMutex);
                handler = pthis->mCustomVideoRenderHandler;
            }
        }
        if (handler)
        {
            std::string streamID = stream_id;
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            handler->onRemoteVideoFrameRawData(data, data_length, param, streamID);
        }
    }

    // CustomVideoCapture
    static void zego_on_custom_video_capture_start(enum zego_publish_channel channel, void *user_context)
    {
        std::shared_ptr<IZegoCustomVideoCaptureHandler> handler;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineCaptureHandlerMutex);
                handler = pthis->mCustomVideoCaptureHandler;
            }
        }
        auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onStart(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_custom_video_capture_stop(enum zego_publish_channel channel, void *user_context)
    {
        std::shared_ptr<IZegoCustomVideoCaptureHandler> handler;
        {
            ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
            std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
            if (pthis == ZegoExpressGD->EngineInstance.get())
            {
                std::lock_guard<std::mutex> lock(pthis->mEngineCaptureHandlerMutex);
                handler = pthis->mCustomVideoCaptureHandler;
            }
        }
        auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE
        auto handler = weakHandler.lock();
        if (handler)
            handler->onStop(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

private:
    int initEngine()
    {
        return ZegoCBridge->initSDK(appID, appSign.c_str(), isTestEnvironment, zego_scenario(scenario));
    }

    void uninitEngine()
    {
        ZegoCBridge->uninitSDKAsync();
    }

    void clearResources()
    {
        mEngineEventHandlerMutex.lock();
        mEventHandler=nullptr;
        StreamExtraInfoCallbacks.clear();
        CDNCallbacks.clear();
        IMSendBroadcastMessageCallbacks.clear();
        IMSendBarrageMessageCallbacks.clear();
        IMSendCustomCommandCallbacks.clear();
        mixerStartCallbacks.clear();
        mixerStopCallbacks.clear();
        mEngineEventHandlerMutex.unlock();

        mMediaPlayerEventMutex.lock();
        mMediaPlayers.clear();
        mMediaPlayerEventMutex.unlock();

        // Spectial resouece
        mCustomVideoCaptureHandler.reset();
        mCustomVideoRenderHandler.reset();

        ZegoCBridge->unregisterEngineCallback();
    }

private:
    unsigned int appID = 0;
    std::string appSign = "";
    bool isTestEnvironment = false;
    ZegoScenario scenario = ZEGO_SCENARIO_GENERAL;

    std::mutex mEngineEventHandlerMutex;
    std::shared_ptr<IZegoEventHandler> mEventHandler;

    std::unordered_map<zego_seq, ZegoPublisherSetStreamExtraInfoCallback> StreamExtraInfoCallbacks;
    std::unordered_map<zego_seq, ZegoPublisherUpdateCDNURLCallback> CDNCallbacks;
    std::unordered_map<zego_seq, ZegoIMSendBroadcastMessageCallback> IMSendBroadcastMessageCallbacks;
    std::unordered_map<zego_seq, ZegoIMSendBarrageMessageCallback> IMSendBarrageMessageCallbacks;
    std::unordered_map<zego_seq, ZegoIMSendCustomCommandCallback> IMSendCustomCommandCallbacks;
    std::unordered_map<zego_seq, ZegoMixerStartCallback> mixerStartCallbacks;
    std::unordered_map<zego_seq, ZegoMixerStopCallback> mixerStopCallbacks;

    std::mutex mMediaPlayerEventMutex;
    std::unordered_map<int, std::shared_ptr<ZegoExpressMediaPlayerImp>> mMediaPlayers;

    std::mutex mEngineRenderHandlerMutex;
    std::shared_ptr<IZegoCustomVideoRenderHandler> mCustomVideoRenderHandler;

    std::mutex mEngineCaptureHandlerMutex;
    std::shared_ptr<IZegoCustomVideoCaptureHandler> mCustomVideoCaptureHandler;
public:
    ///===================================================================================================
    static std::string getVersion()
    {
        std::string version = ZegoExpressEngineBridge::getVersion();
        return version;
    }

    static void setEngineConfig(ZegoEngineConfig engineConfig)
    {
        zego_engine_config _engineConfig;

        zego_log_config _logConfig;
        if (engineConfig.logConfig == nullptr)
        {
            _engineConfig.log_config = nullptr;
        }
        else
        {
            memset(_logConfig.log_path, 0, sizeof(_logConfig.log_path));
            strncpy(_logConfig.log_path, engineConfig.logConfig->logPath.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
            _logConfig.log_size = engineConfig.logConfig->logSize;
            _engineConfig.log_config = &_logConfig;
        }

        zego_custom_video_capture_config _custom_video_capture_config_main;
        if (engineConfig.customVideoCaptureMainConfig == nullptr)
        {
            _engineConfig.custom_video_capture_config = nullptr;
        }
        else
        {
            _custom_video_capture_config_main.type = zego_video_buffer_type(engineConfig.customVideoCaptureMainConfig->bufferType);
            _engineConfig.custom_video_capture_config = &_custom_video_capture_config_main;
        }

        zego_custom_video_capture_config _custom_video_capture_config_aux;
        if (engineConfig.customVideoCaptureAuxConfig == nullptr)
        {
            _engineConfig.custom_video_capture_aux_config = nullptr;
        }
        else
        {
            _custom_video_capture_config_aux.type = zego_video_buffer_type(engineConfig.customVideoCaptureAuxConfig->bufferType);
            _engineConfig.custom_video_capture_aux_config = &_custom_video_capture_config_aux;
        }

        zego_custom_video_render_config _custom_video_render_config;
        if (engineConfig.customVideoRenderConfig == nullptr)
        {
            _engineConfig.custom_video_render_config = nullptr;
        }
        else
        {
            _custom_video_render_config.type = zego_video_buffer_type(engineConfig.customVideoRenderConfig->bufferType);
            _custom_video_render_config.series = zego_custom_video_render_series(engineConfig.customVideoRenderConfig->frameFormatSeries);
            _custom_video_render_config.is_internal_render = engineConfig.customVideoRenderConfig->enableEngineRender;
            _engineConfig.custom_video_render_config = &_custom_video_render_config;
        }

        memset(_engineConfig.advanced_config, 0, sizeof(_engineConfig.advanced_config));
        strncpy(_engineConfig.advanced_config, engineConfig.advancedConfig.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);

        ZegoCBridge->setEngineConfig(_engineConfig);
    }

    ZegoExpressEngineImp(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler)
        :appID(appID), appSign(appSign), isTestEnvironment(isTestEnvironment), scenario(scenario), mEventHandler(eventHandler)
    {
#ifdef WIN32
        setZegoCommuExchangeWndFunc(ZegoCommuExchangeWndProc);
#endif // WIN32
        ZegoCBridge->registerOnCallErrorCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_debug_error), ZegoVoidPtr(this));

        ZegoCBridge->registerRoomStreamUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_stream_update), ZegoVoidPtr(this));
        ZegoCBridge->registerRoomStreamExtraInfoUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_room_stream_extra_info_update), ZegoVoidPtr(this));
        ZegoCBridge->registerRoomStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_room_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerRoomUserStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_user_update), ZegoVoidPtr(this));

        ZegoCBridge->registerPublisherStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherQualityUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_quality_update), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherRecvAudioCapturedFirstFrameCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_recv_audio_captured_first_frame), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherRecvVideoCapturedFirstFrameCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_recv_video_captured_first_frame), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherVideoSizeChangedCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_video_size_changed), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherUpdatePublishCDNUrlCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_update_cdn_url_result), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherRelayCDNStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_relay_cdn_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerPublisherUpdateStreamExtraInfoResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_publisher_update_stream_extra_info_result), ZegoVoidPtr(this));

        ZegoCBridge->registerPlayerStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerQualityUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_quality_update), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerMediaEventCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_media_event), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerRecvAudioFirstFrameCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_recv_audio_first_frame), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerRecvVideoFirstFrameCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_recv_video_first_frame), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerRenderVideoFirstFrameCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_render_video_first_frame), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerVideoSizeChangedCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_video_size_changed), ZegoVoidPtr(this));
        ZegoCBridge->registerPlayerRecvSEICallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_player_recv_sei), ZegoVoidPtr(this));

        ZegoCBridge->registerAudioDeviceStateChangedCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_audio_device_state_changed), ZegoVoidPtr(this));
        ZegoCBridge->registerVideoDeviceStateChangedCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_video_device_state_changed), ZegoVoidPtr(this));
        ZegoCBridge->registerDeviceErrorCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_device_error), ZegoVoidPtr(this));
        ZegoCBridge->registerRemoteCameraStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_remote_camera_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerRemoteMicStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_remote_mic_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerCapturedSoundLevelUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_captured_sound_level_update), ZegoVoidPtr(this));
        ZegoCBridge->registerRemoteSoundLevelUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_remote_sound_level_update), ZegoVoidPtr(this));
        ZegoCBridge->registerCapturedAudioSpectrumUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_captured_audio_spectrum_update), ZegoVoidPtr(this));
        ZegoCBridge->registerRemoteAudioSpectrumUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_remote_audio_spectrum_update), ZegoVoidPtr(this));

        ZegoCBridge->registerIMSendBroadcastMessageResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_im_send_broadcast_message_result), ZegoVoidPtr(this));
        ZegoCBridge->registerIMRecvBroadcasMessageCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_im_recv_broadcast_message), ZegoVoidPtr(this));
        ZegoCBridge->registerIMSendBarrageMessageResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_im_send_barrage_message_result), ZegoVoidPtr(this));
        ZegoCBridge->registerIMRecvBarrageMessageResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_im_recv_barrage_message), ZegoVoidPtr(this));
        ZegoCBridge->registerIMSendCustomCommandResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_im_send_custom_command_result), ZegoVoidPtr(this));
        ZegoCBridge->registerIMRecvCustomCommandCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_im_recv_custom_command), ZegoVoidPtr(this));

        ZegoCBridge->registerMixerStartResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mixer_start_result), ZegoVoidPtr(this));
        ZegoCBridge->registerMixerStopResultCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mixer_stop_result), ZegoVoidPtr(this));
        ZegoCBridge->registerMixerSoundLevelUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mixer_sound_level_update), ZegoVoidPtr(this));
        ZegoCBridge->registerMixerRelayCDNStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mixer_relay_cdn_state_update), ZegoVoidPtr(this));

        ZegoCBridge->registerMediaPlayerStateUpdateCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_state_update), ZegoVoidPtr(this));
        ZegoCBridge->registerMediaPlayerNetworkEventCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_network_event), ZegoVoidPtr(this));
        ZegoCBridge->registerMediaPlayerPlayingProgressCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_playing_progress), ZegoVoidPtr(this));
        ZegoCBridge->registerMediaPlayerSeekToResult(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_seek_to_time_result), ZegoVoidPtr(this));
        ZegoCBridge->registerMediaPlayerLoadFileResult(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_load_file_result), ZegoVoidPtr(this));
        ZegoCBridge->registerMediaPlayerAudioDataCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_audio_data), ZegoVoidPtr(this));
        ZegoCBridge->registerMediaPlayerVideoDataCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_mediaplayer_video_data), ZegoVoidPtr(this));

        ZegoCBridge->registerCustomVideoRenderLocalFrameDataCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_custom_video_render_captured_frame_data), ZegoVoidPtr(this));
        ZegoCBridge->registerCustomVideoRenderRemoteFrameDataCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_custom_video_render_remote_frame_data), ZegoVoidPtr(this));

        ZegoCBridge->registerCustomVideoCaptureStartCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_custom_video_capture_start), ZegoVoidPtr(this));
        ZegoCBridge->registerCustomVideoCaptureStopCallback(ZegoVoidPtr(&ZegoExpressEngineImp::zego_on_custom_video_capture_stop), ZegoVoidPtr(this));
    }

    ~ZegoExpressEngineImp() override
    {
        this->clearResources();
        this->uninitEngine();
    }
    friend class ZegoExpressSDKInternal;
};

class ZegoExpressSDKInternal
{
public:
    static void setEngineConfig(ZegoEngineConfig engineConfig)
    {
        std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
        ZegoExpressEngineImp::setEngineConfig(engineConfig);
    }

    static IZegoExpressEngine *createEngine(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler)
    {
        ZegoCBridge->registerEngineStateCallback(ZegoVoidPtr(&ZegoExpressSDKInternal::onEngineStateUpdate), nullptr);
        std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
        if(ZegoExpressGD->EngineInstance == nullptr)
        {
            auto newEngineInstance = std::make_shared<ZegoExpressEngineImp>(appID, appSign, isTestEnvironment, scenario, eventHandler);
            if (0 == newEngineInstance->initEngine())
            {
                ZegoExpressGD->EngineInstance = newEngineInstance;
            }
        }
        return ZegoExpressGD->EngineInstance.get();
    }

    static void destroyEngine(IZegoExpressEngine *&iEngine, ZegoDestroyCompletionCallback afterDestroyed)
    {
        std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
        if (iEngine != nullptr && iEngine == ZegoExpressGD->EngineInstance.get())
        {
            ZegoExpressGD->afterDestoyed = afterDestroyed;
            ZegoExpressGD->EngineInstance = nullptr;
            iEngine = nullptr;
        }
    }

    static void onEngineStateUpdate(enum zego_engine_state state, void *user_context)
    {
        ZEGO_UNUSED_VARIABLE(user_context);
        if (state == zego_engine_state_uninitialized && ZegoExpressGD->afterDestoyed)
        {
            ZegoExpressGD->afterDestoyed();
        }
    }

    static IZegoExpressEngine *getEngine()
    {
        std::lock_guard<std::recursive_mutex> locker(ZegoExpressGD->EngineMutex);
        return ZegoExpressGD->EngineInstance.get();
    }

    static std::string getVersion()
    {
        return ZegoExpressEngineImp::getVersion();
    }
};

} //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSENGINEIMP_H__
