#ifndef __ZEGOEXPRESSENGINEBRIDGE_H__
#define __ZEGOEXPRESSENGINEBRIDGE_H__

#include "ZegoExpressDefineInternal.h"

namespace ZEGO {
	namespace EXPRESS {

		class ZegoExpressRoom;
		class ZegoExpressPublisher;
		class ZegoExpressPlayer;
		class ZegoExpressDevice;
		class ZegoExpressPreprocess;
		class ZegoExpressIM;
		class ZegoExpressMixer;

		class ZEGO_EXPRESS ZegoExpressEngineBridge
		{
		public:
			ZegoExpressEngineBridge();
			~ZegoExpressEngineBridge();

			static const char* getVersion();
			
			static long long GetCurTimeStampMs();

			void showDebugMessageBox(int error_code, const char* func_name, const char* _info);

			void printDebugInfo(int level, int module, const char * function_name, int error_code, int info_id = 0);

			int initSDK(unsigned int appID, const char * appSign, bool isTestEnvironment, zego_scenario scenario);
			void uninitSDK();
			void uploadLog();
			void setDebugVerbose(bool enable, zego_language language);


			// room
			int loginRoom(zego_user user, const char* room_id, zego_room_config*  room_config);
			int loginRoom(zego_user user, const char* room_id, zego_room_config* room_config, const char* token);
			void logoutRoom(const char* room_id);

			//  publisher
			void setVideoMirrorMode(zego_video_mirror_mode mirrorMode);
			void startPreview(zego_canvas *canvas);
			void stopPreview();
			void updatePreviewView(zego_canvas *canvas);
			void setLatencyMode(zego_latency_mode latencyMode);
			void setAudioBitrate(int bitrate);
			void setVideoConfig(zego_video_config videoConfig);
			int startPublishing(const char* streamID);
			void stopPublishing();
			int setStreamExtraInfo(const char * extraInfo);
			void mutePublishStreamAudio(bool mute);
			void mutePublishStreamVideo(bool mute);
			void setCaptureVolume(int volume);
			int addPublishCNDURL(const char * streamID, const char *  targetURL);
			int removePublishCNDURL(const char * streamID, const char *  targetURL);
			void setPublishWatermark(zego_watermark* watermark, bool isPreviewVisible);
			void enableHardwareEncoder(bool enable);
			void setCapturePipelineScaleMode(zego_capture_pipeline_scale_mode mode);
			void enableCheckPoc(bool enable);

			// play
			int startPlayingStream(const char * streamID, zego_canvas *canvas);
			void stopPlayingStream(const char * streamID);
			void setPlayVolume(const char * streamID, int volume);
			void mutePlayStreamAudio(const char * streamID, bool mute);
			void mutePlayStreamVideo(const char * streamID, bool mute);
			void enableHardwareDecoder(bool enable);

			// device
			void muteMicrophone(bool enable);
			void muteAudioOutput(bool mute);
			void enableCamera(bool enable);
			void enableAudioCaptureDevice(bool enable);
			void startSoundLevelMonitor();
			void stopSoundLevelMonitor();
			void startAudioSpectrumMonitor();
			void stopAudioSpectrumMonitor();

			void useAudioDevice(zego_audio_device_type deviceType, const char * deviceID);
			zego_device_info* getAudioDeviceList(zego_audio_device_type device_type, int * device_count);
			void freeAudioDeviceList(zego_device_info* device_list);
			void useVideoDevice(const char * deviceID);
			zego_device_info* getVideoDeviceList(int * device_count);
			void freeVideoDeviceList(zego_device_info* device_list);

			// preprocess
			void enableAEC(bool enable);
			void setAECMode(zego_aec_mode mode);
			void enableAGC(bool enable);
			void enableANS(bool enable);
			void enableBeautify(int feature);
			void setBeautifyOption(zego_beautify_option option);

			// IM
			int sendBroadcastMessage(const char* room_id, const char* content);
			int sendCustomCommand(const char* room_id, struct zego_user*  to_user_list, unsigned int to_user_count, const char* content);

			// Mixer
			int startMixerTask(const char* task_id);
			int stopMixerTask(const char* task_id);
			void setMixerInputList(const char* task_id, zego_mixer_input* input_list, unsigned int input_count);
			void setMixerOutputList(const char* task_id, zego_mixer_output* output_list, unsigned int output_count);
			void setMixerOutputAudioConfig(const char* task_id, zego_mixer_audio_config audio_config);
			void setMixerOutputVideoConfig(const char* task_id, zego_mixer_video_config video_config);
			void setMixerOutputBackgroundImage(const char* task_id, const char* image_url);
			void setMixerOutputWatermark(const char* task_id, zego_watermark* watermark);

			// eventHandler
			void registerDebugErrorCallback(void* callback_func, void* user_context);

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
			void registerPublisherUpdatePublishCDNUrlCallback(void* callback_func, void* user_context);
			void registerPublisherUpdateStreamExtraInfoResultCallback(void* callback_func, void* user_context);

			void registerPlayerStateUpdateCallback(void* callback_func, void* user_context);
			void registerPlayerQualityUpdateCallback(void* callback_func, void* user_context);
			void registerPlayerMediaEventCallback(void* callback_func, void* user_context);
			void registerPlayerRecvAudioFirstFrameCallback(void* callback_func, void* user_context);
			void registerPlayerRecvVideoFirstFrameCallback(void* callback_func, void* user_context);
			void registerPlayerRenderVideoFirstFrameCallback(void* callback_func, void* user_context);
			void registerPlayerVideoSizeChangedCallback(void* callback_func, void* user_context);

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
			void registerIMSendCustomCommandResultCallback(void* callback_func, void* user_context);
			void registerIMRecvCustomCommandCallback(void* callback_func, void* user_context);

			void registerMixerStartResultCallback(void* callback_func, void* user_context);
			void registerMixerRelayCDNStateUpdateCallback(void* callback_func, void* user_context);

			// only one instance allowed
			static bool createdBefore;

		private:
			ZegoExpressRoom *mRoom = nullptr;
			ZegoExpressPublisher *mPublisher = nullptr;
			ZegoExpressPlayer *mPlayer = nullptr;
			ZegoExpressDevice *mDevice = nullptr;
			ZegoExpressPreprocess *mPreprocess = nullptr;
			ZegoExpressIM *mIM = nullptr;
			ZegoExpressMixer *mMixer = nullptr;

			zego_event_handler *mInternalEventHandlerForCallback = nullptr;
			zego_event_handler *mInternalEventHandlerForEventHandler = nullptr;

			zego_language debugVerboseLanguage = zego_language_english;
			bool debugVerboseEnabled = true;
			bool isTestEnv = false;
		};

	}
}

#endif /* __ZEGOEXPRESSENGINEBRIDGE_H__ */
