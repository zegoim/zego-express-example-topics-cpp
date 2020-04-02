#ifndef __ZEGO_EXPRESS_DEFINE_H__
#define __ZEGO_EXPRESS_DEFINE_H__

#ifndef ZEGO_EXPRESS
	#ifdef WIN32
		#ifdef ZEGO_EXPORTS
			#define ZEGO_EXPRESS __declspec(dllexport)
		#else
			#define ZEGO_EXPRESS __declspec(dllimport)
		#endif
	#else
		#define ZEGO_EXPRESS __attribute__((visibility("default")))
	#endif
#endif

#ifndef ZEGOEXP_API
    #ifdef ZEGO_C_INTERFACE
        #ifdef WIN32
            #ifdef ZEGO_EXPORTS
                #define ZEGOEXP_API __declspec(dllexport)
                #define EXP_CALL __cdecl
            #else
                #define ZEGOEXP_API __declspec(dllimport)
                #define EXP_CALL __cdecl
            #endif
        #else
            #define ZEGOEXP_API __attribute__((visibility("default")))
        #endif

    #else
        #define ZEGOEXP_API 
    #endif

#endif

#ifdef __cplusplus
#define ZEGO_BEGIN_DECLS    extern "C" {
#define ZEGO_END_DECLS      }
#else
#define ZEGO_BEGIN_DECLS
#define ZEGO_END_DECLS
#endif

#ifndef __cplusplus
#define bool _Bool
#define false 0
#define true 1
#endif

#if defined(__APPLE_OS__) || defined(__APPLE__)
#include "TargetConditionals.h"
#endif

#define ZEGO_EXPRESS_DEFAULT_LOG_SIZE       (5242880)       
#define ZEGO_EXPRESS_MAX_LOG_SIZE           (104857600)     



#define DEBUG_LEVEL_DEBUG		0x01
#define DEBUG_LEVEL_INFO        0x02
#define DEBUG_LEVEL_WARNING     0x03
#define DEBUG_LEVEL_ERROR       0x04


#define DEF_ML(module, level, subno, idx)  ((module<<27)+(level<<24)+(subno<<20)+(idx))

#define ML_SYSTEM_INFO_INIT_SDK DEF_ML(ZEGO_EXPRESS_MODULE_SYSTEM, DEBUG_LEVEL_INFO, 0x01, 0x0);

#define zego_error int
#define zego_seq int

#define ZEGO_EXPRESS_MAX_COMMON_LEN                         (512)
#define ZEGO_EXPRESS_MAX_APPSIGN_LEN                        (64)
#define ZEGO_EXPRESS_MAX_USERID_LEN                         (64)
#define ZEGO_EXPRESS_MAX_USERNAME_LEN                       (256)
#define ZEGO_EXPRESS_MAX_ROOMID_LEN                         (128)
#define ZEGO_EXPRESS_MAX_TOKEN_LEN                          (512)
#define ZEGO_EXPRESS_MAX_MIX_INPUT_COUNT                    (12)
#define ZEGO_EXPRESS_MAX_STREAM_LEN                         (256)
#define ZEGO_EXPRESS_MAX_MIXER_TASK_LEN                     (256)
#define ZEGO_EXPRESS_MAX_EXTRA_INFO_LEN                     (1024)
#define ZEGO_EXPRESS_MAX_DEVICE_ID_LEN                      (256)
#define ZEGO_EXPRESS_MAX_URL_COUNT                          (10)
#define ZEGO_EXPRESS_MAX_URL_LEN                            (1024)
#define ZEGO_EXPRESS_MAX_IMAGE_PATH                         (512)
#define ZEGO_EXPRESS_MAX_MESSAGE_LEN                        (512)
#define ZEGO_EXPRESS_MAX_CUSTOM_CMD_LEN                     (1024)
#define ZEGO_EXPRESS_MAX_MEDIAPLAYER_INSTANCE_COUNT         (4)


struct zego_user
{
    char userId[ZEGO_EXPRESS_MAX_USERID_LEN];
    char userName[ZEGO_EXPRESS_MAX_USERNAME_LEN];
};

struct zego_stream
{
    struct zego_user user;
    char streamId[ZEGO_EXPRESS_MAX_STREAM_LEN];
    char extraInfo[ZEGO_EXPRESS_MAX_EXTRA_INFO_LEN];
};


enum zego_scenario {
    
    zego_scenario_general,
    
    zego_scenario_communication,
    
    zego_scenario_live
};

enum zego_language {
    zego_language_english,
    zego_language_chinese
};

enum zego_engine_state {
    zego_engine_state_uninitialized,
    zego_engine_state_initialized,
    zego_engine_state_start,
    zego_engine_state_stop,
};

struct zego_log_config
{
    char log_path[ZEGO_EXPRESS_MAX_COMMON_LEN];
    unsigned long long log_size;
};

enum zego_video_buffer_type
{
    zego_video_buffer_type_unknown = 0,
    zego_video_buffer_type_raw_data = 1,
    zego_video_buffer_type_encoded_data = 2,
    zego_video_buffer_type_gl_texture_2d = 3,
    zego_video_buffer_type_cv_pixel_buffer = 4,
    zego_video_buffer_type_surface_texture = 5
};

enum zego_custom_video_render_series
{
    zego_custom_video_render_series_rgb,
    zego_custom_video_render_series_yuv
};

struct zego_custom_video_capture_config
{
    enum zego_video_buffer_type type;
};

struct zego_custom_video_render_config
{
    enum zego_video_buffer_type type;
    enum zego_custom_video_render_series series;
    bool is_internal_render;
};

struct zego_engine_config {
    
    struct zego_log_config* log_config;
    
    
    struct zego_custom_video_capture_config* custom_video_capture_config;
    
    
    struct zego_custom_video_capture_config* custom_video_capture_aux_config;
    
    
    struct zego_custom_video_render_config* custom_video_render_config;
    
    
    char advanced_config[ZEGO_EXPRESS_MAX_COMMON_LEN];
};


enum zego_room_state
{
    zego_room_state_disconnected,
    zego_room_state_connecting,
    zego_room_state_connected
};

enum zego_update_type
{
    zego_update_type_add,
    zego_update_type_delete
};

struct zego_room_config
{
    unsigned int max_member_count;
    bool is_user_status_notify;
    
    char thrid_token[ZEGO_EXPRESS_MAX_COMMON_LEN];
};

enum zego_stream_quality
{
    zego_stream_quality_excellent,
    zego_stream_quality_good,
    zego_stream_quality_medium,
    zego_stream_quality_bad,
    zego_stream_quality_die
};

struct zego_rect
{
    int left;
    int top;
    int right;
    int bottom;
};


enum zego_publisher_state
{
    zego_publisher_state_no_publish,
    zego_publisher_state_publish_requesting,
    zego_publisher_state_publishing
};

struct zego_publish_stream_quality
{
    double video_capture_fps;
    double video_encode_fps;
    double video_send_fps;
    double video_kbps;
    
    double audio_capture_fps;
    double audio_send_fps;
    double audio_kbps;
    
    int rtt;
    double packet_lost_rate;
    enum zego_stream_quality quality;
    
    bool is_hardware_encode;
    
    int video_width;
    int video_height;
};

enum zego_view_mode
{
    zego_view_mode_aspect_fit,
    zego_view_mode_aspect_fill,
    zego_view_mode_scale_to_fill
};

struct zego_canvas
{
    void* view;
    zego_view_mode view_mode;
    
    int background_color;
};

enum zego_rotation
{
    zego_rotation_0,
    zego_rotation_90,
    zego_rotation_180,
    zego_rotation_270
};

enum zego_orientation
{
    zego_orientation_0,
    zego_orientation_90,
    zego_orientation_180,
    zego_orientation_270
};

enum zego_ios_orientation
{
    zego_ios_orientation_unknown,
    zego_ios_orientation_portrait,
    zego_ios_orientation_portrait_upsidedown,
    zego_ios_orientation_landscape_right,
    zego_ios_orientation_landscape_left
};

enum zego_audio_channel
{
    
    zego_audio_channel_mono,
    
    zego_audio_channel_stereo
};

enum zego_audio_codec_id
{
    
    zego_audio_codec_id_default,
    zego_audio_codec_id_normal,
    zego_audio_codec_id_normal2,
    zego_audio_codec_id_normal3,
    zego_audio_codec_id_low,
    zego_audio_codec_id_low2,
    zego_audio_codec_id_low3
};

enum zego_video_codec_id
{
    zego_video_codec_id_default,
    zego_video_codec_id_svc,
    zego_video_codec_id_vp8
};

struct zego_audio_config
{
    int bitrate;
    zego_audio_channel channel;
    zego_audio_codec_id audio_codec_id;
};

struct zego_video_config
{
    int capture_resolution_width;
    int capture_resolution_height;
    
    int encode_resolution_width;
    int encode_resolution_height;
    
    int bitrate;
    int fps;
    
    zego_video_codec_id video_codec_id;
};

enum zego_video_mirror_mode
{
    zego_video_mirror_mode_only_preview_mirror,
    zego_video_mirror_mode_both_mirror,
    zego_video_mirror_mode_no_mirror,
    zego_video_mirror_mode_only_publish_mirror
};

enum zego_capture_pipeline_scale_mode
{
    zego_capture_pipeline_scale_mode_pre_scale,
    zego_capture_pipeline_scale_mode_post_scale
};

enum zego_stream_relay_cdn_state
{
    zego_stream_relay_cdn_state_no_relay,
    zego_stream_relay_cdn_state_relay_requesting,
    zego_stream_relay_cdn_state_relaying
};

enum zego_stream_relay_cdn_update_reason
{
    zego_stream_relay_cdn_update_reason_none,
    zego_stream_relay_cdn_update_reason_server_error,
    zego_stream_relay_cdn_update_reason_handshake_failed,
    zego_stream_relay_cdn_update_reason_access_point_error,
    zego_stream_relay_cdn_update_reason_create_stream_failed,
    zego_stream_relay_cdn_update_reason_bad_name,
    zego_stream_relay_cdn_update_reason_cdn_server_disconnected,
    zego_stream_relay_cdn_update_reason_disconnected
};

struct zego_stream_relay_cdn_info
{
    char url[ZEGO_EXPRESS_MAX_URL_LEN];
    enum zego_stream_relay_cdn_state cdn_state;
    enum zego_stream_relay_cdn_update_reason update_reason;
    unsigned int state_time;
};

struct zego_cdn_config
{
    char url[ZEGO_EXPRESS_MAX_URL_LEN];
    char auth_param[ZEGO_EXPRESS_MAX_COMMON_LEN];
};

enum zego_publish_channel
{
    zego_publish_channel_main,
    zego_publish_channel_aux
};

enum zego_traffic_control_property
{
    zego_traffic_control_basic,
    zego_traffic_control_adaptive_fps = 1,
    zego_traffic_control_adaptive_resolution = 1 << 1,
    zego_traffic_control_adaptive_audio_bitrate = 1 << 2
};

enum zego_traffic_control_min_video_bitrate_mode
{
    
    zego_traffic_control_min_video_bitrate_mode_no_video,
    
    zego_traffic_control_min_video_bitrate_mode_ultra_low_fps
};


enum zego_player_state
{
    zego_player_state_no_play,
    zego_player_state_play_requesting,
    zego_player_state_playing
};

enum zego_player_video_layer
{
    zego_player_video_layer_auto,            
    zego_player_video_layer_base,             
    zego_player_video_layer_extend       
};

struct zego_player_config
{
    
    zego_cdn_config* cdn_config;
    
    zego_player_video_layer video_layer;
};

struct zego_play_stream_quality
{
    double video_recv_fps;
    double video_dejitter_fps;
    double video_decode_fps;
    double video_render_fps;
    double video_kbps;
    
    double audio_recv_fps;
    double audio_dejitter_fps;
    double audio_decode_fps;
    double audio_render_fps;
    double audio_kbps;
    
    double audio_break_rate;
    double video_break_rate;
    
    int rtt;
    double packet_lost_rate;
    enum zego_stream_quality quality;
    int delay;
    
    bool is_hardware_decode;
    
    int video_width;
    int video_height;
};

enum zego_player_media_event
{
    
    zego_player_media_event_audio_break_occur,
    
    zego_player_media_event_audio_break_resume,
    
    zego_player_media_event_video_break_occur,
    
    zego_player_media_event_video_break_resume
};

enum zego_remote_device_state
{
    
    zego_remote_device_state_open,
    
    zego_remote_device_state_generic_error,
    
    zego_remote_device_state_invalid_id,
    
    zego_remote_device_state_no_authorization,
    
    zego_remote_device_state_zero_fps,
    
    zego_remote_device_state_in_use_by_other,
    
    zego_remote_device_state_unplugged,
    
    zego_remote_device_state_reboot_required,
    
    zego_remote_device_state_system_media_services_lost,
    
    zego_remote_device_state_disable,
    
    zego_remote_device_state_mute,
    
    zego_remote_device_state_interruption,
    
    zego_remote_device_state_in_background,
    
    zego_remote_device_state_multi_foreground_app,
    
    zego_remote_device_state_by_system_pressure
};


enum zego_mixer_input_content_type
{
    zego_mixer_input_content_type_audio,
    zego_mixer_input_content_type_video
};

struct zego_mixer_audio_config
{
    
    int bitrate;
    
    
    zego_audio_channel channel;
    
    
    zego_audio_codec_id audio_codec_id;
};

struct zego_mixer_video_config
{
    int resolution_width;
    int resolution_height;
    
    int bitrate;
    int fps;
};

struct zego_mixer_input
{
    enum zego_mixer_input_content_type content_type;
    char stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN];
    struct zego_rect layout;
    unsigned int sound_level_id;
};

struct zego_mixer_output
{
    
    char target[ZEGO_EXPRESS_MAX_URL_LEN];
    
    
    
    
    
    
};

struct zego_watermark
{
    
    char image[ZEGO_EXPRESS_MAX_COMMON_LEN];
    
    struct zego_rect layout;
    
    
};

struct zego_mixer_sound_level_info
{
    unsigned int sound_level_id;
    double sound_level;
};

struct zego_mixer_task
{
    
    char task_id[ZEGO_EXPRESS_MAX_MIXER_TASK_LEN];
    
    
    struct zego_mixer_input* input_list;
    unsigned int input_list_count;
    
    
    struct zego_mixer_output* output_list;
    unsigned int output_list_count;

    struct zego_mixer_video_config video_config;

    struct zego_mixer_audio_config audio_config;
    
    
    struct zego_watermark* watermark;
    
    
    char background_image_url[ZEGO_EXPRESS_MAX_URL_LEN];
    
    
    
    
    bool enable_sound_level;
};

struct zego_auto_mixer_task
{
    
    char task_id[ZEGO_EXPRESS_MAX_MIXER_TASK_LEN];
    
    
    char room_id[ZEGO_EXPRESS_MAX_ROOMID_LEN];
    
    
    struct zego_mixer_output* output_list;
    unsigned int output_list_count;
    
    

    
    
    bool enable_sound_level;
    
};


struct zego_beautify_option
{
    double polish_step;
    
    double whiten_factor;
    double sharpen_factor;
};

enum zego_beautify_feature
{
    zego_beautify_feature_none = 0,
    zego_beautify_feature_polish = 1,
    zego_beautify_feature_whiten = 1 << 1,
    zego_beautify_feature_skin_whiten = 1 << 2,
    zego_beautify_feature_sharpen = 1 << 3
};

enum zego_aec_mode
{
    zego_aec_mode_aggressive,
    zego_aec_mode_medium,
    zego_aec_mode_soft
};


struct zego_device_info
{
    char device_id[ZEGO_EXPRESS_MAX_COMMON_LEN];
    char device_name[ZEGO_EXPRESS_MAX_COMMON_LEN];
};

struct zego_sound_level_info
{
    char stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN];
    double sound_level;
};

struct zego_audio_spectrum_info
{
    char stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN];
    float* spectrum_list;
    unsigned int spectrum_count;
};

enum zego_audio_device_type
{
    zego_audio_device_type_input,
    zego_audio_device_type_output
};


enum zego_audio_capture_channel_type
{
    zego_audio_capture_channel_type_mono,
    zego_audio_capture_channel_type_stereo,
    zego_audio_capture_channel_type_stereo_when_publish_and_play
};


struct zego_broadcast_message_info
{
    char message[ZEGO_EXPRESS_MAX_MESSAGE_LEN];
    unsigned long long message_id;
    unsigned long long send_time;
    struct zego_user from_user;
};

struct zego_barrage_message_info
{
    char message[ZEGO_EXPRESS_MAX_MESSAGE_LEN];
    char message_id[64];
    unsigned long long send_time;
    struct zego_user from_user;
};

enum zego_video_frame_format
{
    zego_video_frame_format_unknown,
    zego_video_frame_format_i420,
    zego_video_frame_format_nv12,
    zego_video_frame_format_nv21,
    zego_video_frame_format_bgra32,
    zego_video_frame_format_rgba32,
    zego_video_frame_format_argb32,
    zego_video_frame_format_abgr32,
    zego_video_frame_format_i422
};

enum zego_video_encoded_frame_format
{
    zego_video_encoded_frame_format_avc_avcc,
    zego_video_encoded_frame_format_avc_annexb
};

struct zego_audio_frame_param
{
    zego_audio_channel channel;
    int samples_rate;
};

struct zego_video_frame_param
{
    enum zego_video_frame_format format;
    int strides[4];
    int width;
    int height;
    
    int rotation;
};

struct zego_video_encoded_frame_param
{
    enum zego_video_encoded_frame_format format;
    bool is_key_frame;
    int width;
    int height;
    const unsigned char* sei_data;
    unsigned int sei_data_length;
};

enum zego_video_flip_mode
{
    zego_video_flip_mode_none,
    zego_video_flip_mode_x,
    zego_video_flip_mode_y,
    zego_video_flip_mode_x_y,
};

enum zego_mediaplayer_instance_index
{
    zego_mediaplayer_instance_index_null = -1,
    zego_mediaplayer_instance_index_first = 0,
    zego_mediaplayer_instance_index_second = 1,
    zego_mediaplayer_instance_index_third = 2,
    zego_mediaplayer_instance_index_forth = 3
};

enum zego_mediaplayer_state
{
    zego_mediaplayer_state_no_play,
    zego_mediaplayer_state_playing,
    zego_mediaplayer_state_pausing,
    zego_mediaplayer_state_play_end
};

enum zego_mediaplayer_network_event
{
    zego_mediaplayer_network_event_buffer_begin,
    zego_mediaplayer_network_event_buffer_end
};

#endif 

#ifndef __ZEGOEXPRESSBRIDGE_H__
#define __ZEGOEXPRESSBRIDGE_H__


#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#include <sstream>
typedef void(*ZegoCommuExchangeWndFunc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
ZEGO_EXPRESS void setZegoCommuExchangeWndFunc(ZegoCommuExchangeWndFunc func);

class ZEGO_EXPRESS ZegoMainThreadTool {
public:
    ZegoMainThreadTool();
    static LRESULT CALLBACK ZegoCommuExchangeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LPCWSTR ZegoCommWndClassName;
    static LPCWSTR  ZegoCommWndName;
};
#endif

namespace ZEGO {
    namespace EXPRESS {

        class ZegoExpressRoom;
        class ZegoExpressPublisher;
        class ZegoExpressPlayer;
        class ZegoExpressDevice;
        class ZegoExpressPreprocess;
        class ZegoExpressIM;
        class ZegoExpressMixer;
        class ZegoExpressMediaPlayer;
        class ZegoExpressCustomIO;

        class ZEGO_EXPRESS ZegoExpressEngineBridge
        {
        public:
            static ZegoExpressEngineBridge *GetInstance();
            
            // engine
            static const char* getVersion();
            void setEngineConfig(zego_engine_config config);
            int initSDK(unsigned int appID, const char * appSign, bool isTestEnvironment, zego_scenario scenario);
            void uninitSDKAsync();
            void uploadLog();
            void setDebugVerbose(bool enable, zego_language language);

            void showDebugMessageBox(int error_code, const char* func_name, const char* _info);
            void printDebugInfoAndPopupWhenError(int module, const char * function_name, int error_code);
            void printDebugInfo(int module, const char * function_name, int error_code);

            // room
            void loginRoom(const char* room_id, zego_user user, zego_room_config* room_config);
            void logoutRoom(const char* room_id);

            //  publisher
            void setVideoMirrorMode(zego_video_mirror_mode mirrorMode, enum zego_publish_channel channel = zego_publish_channel_main);
            void startPreview(zego_canvas *canvas, enum zego_publish_channel channel = zego_publish_channel_main);
            void stopPreview(enum zego_publish_channel channel = zego_publish_channel_main);
            void setAudioConfig(zego_audio_config audioConfig);
            void setVideoConfig(zego_video_config videoConfig, enum zego_publish_channel channel = zego_publish_channel_main);
            void startPublishingStream(const char* streamID, enum zego_publish_channel channel = zego_publish_channel_main);
            void stopPublishingStream(enum zego_publish_channel channel = zego_publish_channel_main);
            int setStreamExtraInfo(const char * extraInfo, enum zego_publish_channel channel = zego_publish_channel_main);
            void mutePublishStreamAudio(bool mute, enum zego_publish_channel channel = zego_publish_channel_main);
            void mutePublishStreamVideo(bool mute, enum zego_publish_channel channel = zego_publish_channel_main);
            void setCaptureVolume(int volume);
            int addPublishCdnUrl(const char * streamID, const char *  targetURL);
            int removePublishCdnUrl(const char * streamID, const char *  targetURL);
            void enablePublishDirectToCDN(bool enable, zego_cdn_config* config, enum zego_publish_channel channel = zego_publish_channel_main);
            void setPublishWatermark(zego_watermark* watermark, bool isPreviewVisible, enum zego_publish_channel channel =  zego_publish_channel_main);
            void enableHardwareEncoder(bool enable);
            void setCapturePipelineScaleMode(zego_capture_pipeline_scale_mode mode);
            void sendSEI(const unsigned char* buffer, unsigned int buffer_length, enum zego_publish_channel channel = zego_publish_channel_main);
            void enableTrafficControl(bool enable, int property);
            void setMinVideoBitrateForTrafficControl(int bitrate, enum zego_traffic_control_min_video_bitrate_mode mode);

            // play
            void startPlayingStream(const char * streamID, zego_canvas *canvas);
            void startPlayingStreamWithConfig(const char* streamID, zego_canvas* canvas, zego_player_config config);
            void stopPlayingStream(const char * streamID);
            void setPlayVolume(const char * streamID, int volume);
            void mutePlayStreamAudio(const char * streamID, bool mute);
            void mutePlayStreamVideo(const char * streamID, bool mute);
            void enableHardwareDecoder(bool enable);
            void enableCheckPoc(bool enable);

            // device
            void muteMicrophone(bool mute);
            void muteSpeaker(bool mute);
            void muteAudioOutput(bool mute);
            void enableCamera(bool enable, enum zego_publish_channel channel = zego_publish_channel_main);
            void enableAudioCaptureDevice(bool enable);
            void startSoundLevelMonitor();
            void stopSoundLevelMonitor();
            void startAudioSpectrumMonitor();
            void stopAudioSpectrumMonitor();

            void useAudioDevice(zego_audio_device_type deviceType, const char * deviceID);
            zego_device_info* getAudioDeviceList(zego_audio_device_type device_type, int * device_count);
            void freeAudioDeviceList(zego_device_info* device_list);
            void useVideoDevice(const char * deviceID, enum zego_publish_channel channel = zego_publish_channel_main);
            zego_device_info* getVideoDeviceList(int * device_count);
            void freeVideoDeviceList(zego_device_info* device_list);

            // preprocess
            void enableAEC(bool enable);
            void setAECMode(zego_aec_mode mode);
            void enableAGC(bool enable);
            void enableANS(bool enable);
            void enableBeautify(int feature, enum zego_publish_channel channel = zego_publish_channel_main);
            void setBeautifyOption(zego_beautify_option option, enum zego_publish_channel channel = zego_publish_channel_main);

            // IM
            int sendBroadcastMessage(const char* room_id, const char* content);
            int sendBarrageMessage(const char* room_id, const char* content);
            int sendCustomCommand(const char* room_id, const char* content, struct zego_user*  to_user_list, unsigned int to_user_count);

            // Mixer
            int startMixerTask(zego_mixer_task task);
            int stopMixerTask(zego_mixer_task task);
            int startAutoMixerTask(zego_auto_mixer_task task);
            int stopAutoMixerTask(zego_auto_mixer_task task);

            // MediaPlayer
            zego_mediaplayer_instance_index mpCreate();
            int mpRelease(zego_mediaplayer_instance_index instance_index);
            int mpStart(zego_mediaplayer_instance_index instance_index);
            int mpStop(zego_mediaplayer_instance_index instance_index);
            int mpPause(zego_mediaplayer_instance_index instance_index);
            int mpResume(zego_mediaplayer_instance_index instance_index);
            int mpPreload(const char* path, zego_mediaplayer_instance_index instance_index);
            int mpSeekTo(unsigned long long millisecond, zego_mediaplayer_instance_index instance_index);
            int mpSetVolume(int volume, zego_mediaplayer_instance_index instance_index);
            unsigned long long mpGetTotalDuration(zego_mediaplayer_instance_index instance_index);
            unsigned long long mpGetCurrentProgress(zego_mediaplayer_instance_index instance_index);
            int mpGetVolume(zego_mediaplayer_instance_index instance_index);
            zego_mediaplayer_state mpGetCurrentState(zego_mediaplayer_instance_index instance_index);
            int mpMuteLocal(bool mute, zego_mediaplayer_instance_index instance_index);
            int mpSetPlayerCanvas(struct zego_canvas *canvas, zego_mediaplayer_instance_index instance_index);
            int mpEnableAUX(bool enable, zego_mediaplayer_instance_index instance_index);
            int mpEnableRepeat(bool enable, zego_mediaplayer_instance_index instance_index);
            int mpSetProgressInterval(unsigned long long millisecond, zego_mediaplayer_instance_index instance_index);
            int mpEnableAudioData(bool enable, zego_mediaplayer_instance_index instance_index);
            int mpEnableVideoData(bool enable, zego_video_frame_format format, zego_mediaplayer_instance_index instance_index);

            // CustomlIO
            void customVideoCaptureSetFillMode(zego_view_mode mode, enum zego_publish_channel channel = zego_publish_channel_main);
            void customVideoCaptureSendRawData(const unsigned char* data, unsigned int data_length, const struct zego_video_frame_param param, unsigned long long reference_time_millisecond, enum zego_publish_channel channel = zego_publish_channel_main);
            void customVideoCaptureSendEncodedFrameData(const unsigned char* data, unsigned int data_length, const struct zego_video_encoded_frame_param param, double reference_time_millisecond, enum zego_publish_channel channel = zego_publish_channel_main);

            // eventHandler
            void unregisterEngineCallback();

            void registerEngineStateCallback(void* callback_func, void* user_context);

            void registerRoomStreamUpdateCallback(void* callback_func, void* user_context);
            void registerRoomStreamExtraInfoUpdateCallback(void* callback_func, void* user_context);
            void registerRoomStateUpdateCallback(void* callback_func, void* user_context);
            void registerRoomUserStateUpdateCallback(void* callback_func, void* user_context);

            void registerPublisherStateUpdateCallback(void* callback_func, void* user_context);
            void registerPublisherQualityUpdateCallback(void* callback_func, void* user_context);
            void registerPublisherRecvAudioCapturedFirstFrameCallback(void* callback_func, void* user_context);
            void registerPublisherRecvVideoCapturedFirstFrameCallback(void* callback_func, void* user_context);
            void registerPublisherVideoSizeChangedCallback(void* callback_func, void* user_context);
            void registerPublisherRelayCDNStateUpdateCallback(void *callback_func, void *user_context);
            void registerPublisherUpdatePublishCdnUrlCallback(void* callback_func, void* user_context);
            void registerPublisherUpdateStreamExtraInfoResultCallback(void* callback_func, void* user_context);

            void registerPlayerStateUpdateCallback(void* callback_func, void* user_context);
            void registerPlayerQualityUpdateCallback(void* callback_func, void* user_context);
            void registerPlayerMediaEventCallback(void* callback_func, void* user_context);
            void registerPlayerRecvAudioFirstFrameCallback(void* callback_func, void* user_context);
            void registerPlayerRecvVideoFirstFrameCallback(void* callback_func, void* user_context);
            void registerPlayerRenderVideoFirstFrameCallback(void* callback_func, void* user_context);
            void registerPlayerVideoSizeChangedCallback(void* callback_func, void* user_context);
            void registerPlayerRecvSEICallback(void* callback_func, void* user_context);

            void registerCapturedSoundLevelUpdateCallback(void* callback_func, void* user_context);
            void registerRemoteSoundLevelUpdateCallback(void* callback_func, void* user_context);
            void registerCapturedAudioSpectrumUpdateCallback(void* callback_func, void* user_context);
            void registerRemoteAudioSpectrumUpdateCallback(void* callback_func, void* user_context);

            void registerAudioDeviceStateChangedCallback(void* callback_func, void* user_context);
            void registerVideoDeviceStateChangedCallback(void* callback_func, void* user_context);
            void registerDeviceErrorCallback(void* callback_func, void* user_context);

            void registerRemoteCameraStateUpdateCallback(void* callback_func, void* user_context);
            void registerRemoteMicStateUpdateCallback(void* callback_func, void* user_context);	

            void registerIMSendBroadcastMessageResultCallback(void* callback_func, void* user_context);
            void registerIMRecvBroadcasMessageCallback(void* callback_func, void* user_context);
            void registerIMSendBarrageMessageResultCallback(void* callback_func, void* user_context);
            void registerIMRecvBarrageMessageResultCallback(void* callback_func, void* user_context);
            void registerIMSendCustomCommandResultCallback(void* callback_func, void* user_context);
            void registerIMRecvCustomCommandCallback(void* callback_func, void* user_context);

            void registerMixerStartResultCallback(void* callback_func, void* user_context);
            void registerMixerStopResultCallback(void* callback_func, void* user_context);
            void registerMixerSoundLevelUpdateCallback(void* callback_func, void* user_context);
            void registerAutoMixerSoundLevelUpdateCallback(void* callback_func, void* user_context);
            void registerMixerRelayCDNStateUpdateCallback(void* callback_func, void* user_context);

            void registerMediaPlayerStateUpdateCallback(void* callback_func, void* user_context);
            void registerMediaPlayerNetworkEventCallback(void* callback_func, void* user_context);
            void registerMediaPlayerPlayingProgressCallback(void* callback_func, void* user_context);
            void registerMediaPlayerSeekToResult(void* callback_func, void* user_context);
            void registerMediaPlayerLoadFileResult(void* callback_func, void* user_context);
            void registerMediaPlayerAudioDataCallback(void* callback_func, void* user_context);
            void registerMediaPlayerVideoDataCallback(void* callback_func,  void* user_context);

            void registerCustomVideoRenderLocalFrameDataCallback(void* callback_func, void* user_context);
            void registerCustomVideoRenderRemoteFrameDataCallback(void* callback_func, void* user_context);
            
            void registerCustomVideoCaptureStartCallback(void* callback_func, void* user_context);
            void registerCustomVideoCaptureStopCallback(void* callback_func, void* user_context);

        public:
            typedef void(*CBridgeOnCallError)(int error_code, const char* func_name, const char* info, void* user_context);
            void registerOnCallErrorCallback(void* callback, void* context);
        private:
            CBridgeOnCallError mOnCallError = nullptr;
            void* mOnCallErrorContext = nullptr;
            
        private:
            ZegoExpressEngineBridge();
            ~ZegoExpressEngineBridge();

            ZegoExpressRoom *mRoom = nullptr;
            ZegoExpressPublisher *mPublisher = nullptr;
            ZegoExpressPlayer *mPlayer = nullptr;
            ZegoExpressDevice *mDevice = nullptr;
            ZegoExpressPreprocess *mPreprocess = nullptr;
            ZegoExpressIM *mIM = nullptr;
            ZegoExpressMixer *mMixer = nullptr;
            ZegoExpressMediaPlayer *mMediaPlayer = nullptr;
            ZegoExpressCustomIO *mCustomIO = nullptr;

            zego_language debugVerboseLanguage = zego_language_english;
            bool debugVerboseEnabled = true;
            bool isTestEnv = false;
            bool enableErrorMessageBox = true;
        };

    }
}

#endif /* __ZEGOEXPRESSBRIDGE_H__ */