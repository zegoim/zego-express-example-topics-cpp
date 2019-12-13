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
            #else
                #define ZEGOEXP_API __declspec(dllimport)
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

#define ZEGO_EXPRESS_MAX_COMMON_LEN         (512)
#define ZEGO_EXPRESS_MAX_APPSIGN_LEN        (64)
#define ZEGO_EXPRESS_MAX_USERID_LEN         (64)
#define ZEGO_EXPRESS_MAX_USERNAME_LEN       (256)
#define ZEGO_EXPRESS_MAX_ROOMID_LEN         (128)
#define ZEGO_EXPRESS_MAX_TOKEN_LEN          (512)
#define ZEGO_EXPRESS_MAX_MIX_INPUT_COUNT    (12)
#define ZEGO_EXPRESS_MAX_STREAM_LEN         (256)
#define ZEGO_EXPRESS_MAX_MIXER_TASK_LEN     (256)
#define ZEGO_EXPRESS_MAX_EXTRA_INFO_LEN     (1024)
#define ZEGO_EXPRESS_MAX_DEVICE_ID_LEN      (256)
#define ZEGO_EXPRESS_MAX_URL_COUNT          (10)
#define ZEGO_EXPRESS_MAX_URL_LEN            (1024)
#define ZEGO_EXPRESS_MAX_IMAGE_PATH         (512)
#define ZEGO_EXPRESS_MAX_MESSAGE_LEN        (512)
#define ZEGO_EXPRESS_MAX_CUSTOM_CMD_LEN     (1024)

struct zego_event_handler;


struct zego_user
{
    char userId[ZEGO_EXPRESS_MAX_USERID_LEN + 1];
    char userName[ZEGO_EXPRESS_MAX_USERNAME_LEN + 1];
};

struct zego_stream
{
    struct zego_user user;
    char streamId[ZEGO_EXPRESS_MAX_STREAM_LEN + 1];
    char extraInfo[ZEGO_EXPRESS_MAX_EXTRA_INFO_LEN + 1];
};


enum zego_scenario {
    zego_scenario_general,
    zego_scenario_live,
    zego_scenario_chatroom,
    zego_scenario_education
};

enum zego_language {
    zego_language_english,
    zego_language_chinese
};

struct zego_init_config {
    char log_path[ZEGO_EXPRESS_MAX_COMMON_LEN + 1];
    unsigned long long log_size;
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

enum zego_latency_mode
{
    zego_latency_mode_normal,
    zego_latency_mode_low,
    zego_latency_mode_normal2,
    zego_latency_mode_low2,
    zego_latency_mode_low3,
    zego_latency_mode_normal3
};

struct zego_video_config
{
    int capture_resolution_width;
    int capture_resolution_height;
    
    int encode_resolution_width;
    int encode_resolution_height;
    
    int bitrate;
    int fps;
    
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
    zego_stream_relay_cdn_state_stop,
    zego_stream_relay_cdn_state_start,
    zego_stream_relay_cdn_state_retry
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
    char url[ZEGO_EXPRESS_MAX_URL_LEN + 1];
    enum zego_stream_relay_cdn_state cdn_state;
    enum zego_stream_relay_cdn_update_reason update_reason;
    unsigned int state_time;
};

struct zego_publish_stream_info
{
    char* arrRtmpURLs[ZEGO_EXPRESS_MAX_URL_COUNT];      
    unsigned int uiRtmpURLCount;                
    
    char* arrFlvRULs[ZEGO_EXPRESS_MAX_URL_COUNT];
    unsigned int uiFlvURLCount;
    
    char* arrHlsURLs[ZEGO_EXPRESS_MAX_URL_COUNT];
    unsigned int uiHlsURLCount;
};


enum zego_player_state
{
    zego_player_state_no_play,
    zego_player_state_play_requesting,
    zego_player_state_playing
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

struct zego_mixer_input
{
    enum zego_mixer_input_content_type content_type;
    char stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN + 1];
    struct zego_rect layout;
};

struct zego_mixer_output
{
    
    char target[ZEGO_EXPRESS_MAX_URL_LEN + 1];
};

struct zego_watermark
{
    
    char image[ZEGO_EXPRESS_MAX_COMMON_LEN + 1];
    
    struct zego_rect layout;
    
    
};

struct zego_mixer_cdn_info
{
    char output_stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN + 1];
    
    char* rtmp_url_list[ZEGO_EXPRESS_MAX_URL_COUNT];        
    unsigned int rtmp_url_count;                            
    
    char* flv_url_list[ZEGO_EXPRESS_MAX_URL_COUNT];
    unsigned int flv_url_count;
    
    char* hls_url_list[ZEGO_EXPRESS_MAX_URL_COUNT];
    unsigned int hls_url_count;
};

struct zego_mixer_start_result
{
    zego_error error_code;
};

struct zego_mixer_audio_config
{
    
    int bitrate;
    
    int audio_encode_type;
};

struct zego_mixer_video_config
{
    int resolution_width;
    int resolution_height;
    int fps;
    int bitrate;
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
    char device_id[ZEGO_EXPRESS_MAX_COMMON_LEN + 1];
    char device_name[ZEGO_EXPRESS_MAX_COMMON_LEN + 1];
};

struct zego_sound_level_info
{
    char stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN + 1];
    double sound_level;
};

struct zego_audio_spectrum_info
{
    char stream_id[ZEGO_EXPRESS_MAX_STREAM_LEN + 1];
    double *spectrum_list;
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


struct zego_message_info
{
    char message[ZEGO_EXPRESS_MAX_MESSAGE_LEN + 1];
    unsigned long long send_time;
    struct zego_user from_user;
};


#endif 
