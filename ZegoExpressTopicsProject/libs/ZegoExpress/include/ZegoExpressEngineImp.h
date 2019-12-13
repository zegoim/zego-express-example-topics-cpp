
#ifndef __ZEGOEXPRESSENGINEIMP_H__
#define __ZEGOEXPRESSENGINEIMP_H__

#include <string>
#include <unordered_map>
#include <mutex>
#include <assert.h>
#include "ZegoExpressDefine.h"
#include "ZegoExpressEventHandler.h"
#include "ZegoExpressEngineInterface.h"
#include "ZegoExpressEngineBridge.h"

namespace ZEGO {
	namespace EXPRESS {

		class ZegoExpressEngineImp : public IZegoExpressEngine
		{
		public:
			void addEventHandler(std::shared_ptr<IZegoEventHandler> handler) override
			{
				if (handler == nullptr) {
					return;
				}

				std::lock_guard<std::mutex> lock(eventHandlerMutex);
				if (mEventHandlers.size() > ZEGO_MAX_EVENT_HANDLER_COUNT) {
					return;
				}

				auto iter = std::find(mEventHandlers.begin(), mEventHandlers.end(), handler);
				if (iter == mEventHandlers.end())
				{
					mEventHandlers.emplace_back(handler);
				}
			}

			void removeEventHandler(std::shared_ptr<IZegoEventHandler> handler) override
			{
				if (handler == nullptr) {
					return;
				}

				std::lock_guard<std::mutex> lock(eventHandlerMutex);
				auto iter = std::find(mEventHandlers.begin(), mEventHandlers.end(), handler);
				if (iter != mEventHandlers.end())
				{
					mEventHandlers.erase(iter);
				}
			}

			void uploadLog() override
			{
				mEngineBridge->uploadLog();
			}

			void setDebugVerbose(bool enable, ZegoLanguage language) override
			{
				mEngineBridge->setDebugVerbose(enable, zego_language(language));
			}

			///===================================================================================================
			void loginRoom(std::string roomID, ZegoUser user, ZegoRoomConfig* config) override
			{
				zego_user _user = ZegoExpressEngineImp::ZegoConvertO2IUser(user);
				const char * _roomId = roomID.c_str();

				if (config != nullptr) {
					zego_room_config _config = ZegoExpressEngineImp::ZegoConvertO2IRoomConfig(*config);
					mEngineBridge->loginRoom(_user, _roomId, &_config);
				}
				else {
					mEngineBridge->loginRoom(_user, _roomId, nullptr);
				}
			}

			void loginRoom(std::string roomID, ZegoUser user, ZegoRoomConfig* config, std::string token) override
			{
				zego_user _user = ZegoExpressEngineImp::ZegoConvertO2IUser(user);
				const char * _roomId = roomID.c_str();
				const char * _token = token.c_str();

				if (config != nullptr) {
					zego_room_config _config = ZegoExpressEngineImp::ZegoConvertO2IRoomConfig(*config);
					mEngineBridge->loginRoom(_user, _roomId, &_config, _token);
				}
				else {
					mEngineBridge->loginRoom(_user, _roomId, nullptr, _token);
				}
			}

			void logoutRoom(std::string roomID) override
			{
				const char * _roomId = roomID.c_str();
				mEngineBridge->logoutRoom(_roomId);
			}

			///===================================================================================================
			void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode) override
			{
				mEngineBridge->setVideoMirrorMode(zego_video_mirror_mode(mirrorMode));
			}

			void startPreview(ZegoCanvas *canvas) override
			{
				if (canvas == nullptr) {
					mEngineBridge->startPreview(nullptr);
				}
				else
				{
					zego_canvas _canvas;
					_canvas.view = canvas->view;
					_canvas.view_mode = zego_view_mode(canvas->viewMode);
					_canvas.background_color = canvas->backgroundColor;
					mEngineBridge->startPreview(&_canvas);
				}
			}

			void stopPreview() override
			{
				mEngineBridge->stopPreview();
			}

			void setLatancyMode(ZegoLatencyMode latencyMode) override
			{
				mEngineBridge->setLatencyMode(zego_latency_mode(latencyMode));
			}

			void setAudioBitrate(int bitrate) override
			{
				mEngineBridge->setAudioBitrate(bitrate);
			}

			void setVideoConfig(ZegoVideoConfig videoConfig) override
			{
				zego_video_config _config = ZegoExpressEngineImp::ZegoConvertO2IVideoConfig(videoConfig);
				mEngineBridge->setVideoConfig(_config);
			}

			void startPublishing(std::string streamID) override
			{
				const char * stream_id = streamID.c_str();
				mEngineBridge->startPublishing(stream_id);
			}

			void stopPublishing() override
			{
				mEngineBridge->stopPublishing();
			}

			void setStreamExtraInfo(std::string extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback) override
			{
				const char * _extraInfo = extraInfo.c_str();
				int seq = mEngineBridge->setStreamExtraInfo(_extraInfo);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(callbackMutex);
					StreamExtraInfoCallbacks.insert({seq, callback });
				}
			}

			void mutePublishStreamAudio(bool mute) override
			{
				mEngineBridge->mutePublishStreamAudio(mute);
			}

			void mutePublishStreamVideo(bool mute) override
			{
				mEngineBridge->mutePublishStreamVideo(mute);
			}

			void setCaptureVolume(int volume) override
			{
				mEngineBridge->setCaptureVolume(volume);
			}

			void addPublishCDNURL(std::string streamID, std::string targetURL, ZegoPublisherUpdateCDNURLCallback callback) override
			{
				const char * stream_id = streamID.c_str();
				const char * target_url = targetURL.c_str();

				int seq = mEngineBridge->addPublishCNDURL(stream_id, target_url);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(callbackMutex);
					CDNCallbacks.insert({seq, callback });
				}
			}

			void removePublishCDNURL(std::string streamID, std::string targetURL, ZegoPublisherUpdateCDNURLCallback callback) override
			{
				const char * stream_id = streamID.c_str();
				const char * target_url = targetURL.c_str();

				int seq = mEngineBridge->removePublishCNDURL(stream_id, target_url);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(callbackMutex);
					CDNCallbacks.insert({seq, callback});
				}
			}

			void setPublishWatermark(ZegoWatermark *watermark, bool isPreviewVisible) override
			{
				if (watermark != nullptr) {
					zego_watermark _watermark;
					{
						_watermark.layout.left = watermark->layout.x;
						_watermark.layout.right = watermark->layout.x + watermark->layout.width;
						_watermark.layout.top = watermark->layout.y;
						_watermark.layout.bottom = watermark->layout.y + watermark->layout.height;
						memset(_watermark.image, 0, sizeof(_watermark.image));
						strncpy_s(_watermark.image, watermark->imageURL.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
					}
					mEngineBridge->setPublishWatermark(&_watermark, isPreviewVisible);
				}
				else {
					mEngineBridge->setPublishWatermark(nullptr, isPreviewVisible);
				}
			}

			void enableHardwareEncoder(bool enable) override
			{
				mEngineBridge->enableHardwareEncoder(enable);
			}

			void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) override
			{
				zego_capture_pipeline_scale_mode _mode = zego_capture_pipeline_scale_mode(mode);
				mEngineBridge->setCapturePipelineScaleMode(_mode);
			}

			void enableCheckPoc(bool enable) override
			{
				mEngineBridge->enableCheckPoc(enable);
			}

			///===================================================================================================
			void startPlayingStream(std::string streamID, ZegoCanvas *canvas) override
			{
				const char * stream_id = streamID.c_str();
				if (canvas == nullptr) {
					mEngineBridge->startPlayingStream(stream_id, nullptr);
				}
				else {
					zego_canvas _canvas;
					_canvas.view = canvas->view;
					_canvas.view_mode = zego_view_mode(canvas->viewMode);
					_canvas.background_color = canvas->backgroundColor;
					mEngineBridge->startPlayingStream(stream_id, &_canvas);
				}
			}

			void stopPlayingStream(std::string streamID) override
			{
				const char * stream_id = streamID.c_str();
				mEngineBridge->stopPlayingStream(stream_id);
			}

			void setPlayVolume(std::string streamID, int volume) override
			{
				const char * stream_id = streamID.c_str();
				mEngineBridge->setPlayVolume(stream_id, volume);
			}

			void mutePlayStreamAudio(std::string streamID, bool mute) override
			{
				const char * stream_id = streamID.c_str();
				mEngineBridge->mutePlayStreamAudio(stream_id, mute);
			}

			void mutePlayStreamVideo(std::string streamID, bool mute) override
			{
				const char * stream_id = streamID.c_str();
				mEngineBridge->mutePlayStreamVideo(stream_id, mute);
			}

			void enableHarewareDecoder(bool enable) override
			{
				mEngineBridge->enableHardwareDecoder(enable);
			}

			//===================================================================================================

			void muteMicrophone(bool enable) override
			{
				mEngineBridge->muteMicrophone(enable);
			}

			void muteAudioOutput(bool mute) override
			{
				mEngineBridge->muteAudioOutput(mute);
			}

			void enableCamera(bool enable) override
			{
				mEngineBridge->enableCamera(enable);
			}

			void enableAudioCaptureDevice(bool enable) override
			{
				mEngineBridge->enableAudioCaptureDevice(enable);
			}

			void startSoundLevelMonitor() override
			{
				mEngineBridge->startSoundLevelMonitor();
			}

			void stopSoundLevelMonitor() override
			{
				mEngineBridge->stopSoundLevelMonitor();
			}
			void startAudioSpectrumMonitor() override
			{
				mEngineBridge->startAudioSpectrumMonitor();
			}

			void stopAudioSpectrumMonitor() override
			{
				mEngineBridge->stopAudioSpectrumMonitor();
			}

			void useAudioDevice(ZegoAudioDeviceType deviceType, std::string deviceID) override
			{
				const char * device_id = deviceID.c_str();
				mEngineBridge->useAudioDevice(zego_audio_device_type(deviceType), device_id);
			}

			std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) override
			{
				int count = 0;
				zego_device_info* _deviceInfoList = mEngineBridge->getAudioDeviceList(zego_audio_device_type(deviceType), &count);

				std::vector<ZegoDeviceInfo> result;
				for (int i = 0; i < count; i++) {
					ZegoDeviceInfo deviceInfo = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_deviceInfoList[i]);
					result.push_back(deviceInfo);
				}

				mEngineBridge->freeAudioDeviceList(_deviceInfoList);
				return result;
			}

			void useVideoDevice(std::string deviceID) override
			{
				const char * device_id = deviceID.c_str();
				mEngineBridge->useVideoDevice(device_id);
			}

			std::vector<ZegoDeviceInfo> getVideoDeviceList() override
			{
				int count = 0;
				zego_device_info* _deviceInfoList = mEngineBridge->getVideoDeviceList(&count);

				std::vector<ZegoDeviceInfo> result;
				for (int i = 0; i < count; i++) {
					ZegoDeviceInfo deviceInfo = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_deviceInfoList[i]);
					result.push_back(deviceInfo);
				}

				mEngineBridge->freeVideoDeviceList(_deviceInfoList);
				return result;
			}

			//===================================================================================================

			void enableAEC(bool enable) override
			{
				mEngineBridge->enableAEC(enable);
			}

			void setAECMode(ZegoAECMode mode) override
			{
				mEngineBridge->setAECMode(zego_aec_mode(mode));
			}

			void enableAGC(bool enable) override
			{
				mEngineBridge->enableAGC(enable);
			}

			void enableANS(bool enable) override
			{
				mEngineBridge->enableANS(enable);
			}

			//===================================================================================================
			void sendBroadcastMassage(std::string roomID, std::string message, ZegoIMSendBroadcastMessageCallback callback) override
			{
				const char * _roomID = roomID.c_str();
				const char * _message = message.c_str();

				int seq = mEngineBridge->sendBroadcastMessage(_roomID, _message);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(callbackMutex);
					IMSendBroadcastMessageCallbacks.insert({seq, callback});
				}
			}
			void sendCustomCommand(std::string roomID, std::vector<ZegoUser> toUserList, std::string command, ZegoIMSendCustomCommandCallback callback) override
			{
				std::vector<zego_user> _userList;
				for (auto user : toUserList) {
					zego_user _user = ZegoExpressEngineImp::ZegoConvertO2IUser(user);
					_userList.push_back(_user);
				}
				zego_user *users = _userList.data();
				const char * _roomID = roomID.c_str();
				const char * _command = command.c_str();

				int seq = mEngineBridge->sendCustomCommand(_roomID, users, (unsigned int)(_userList.size()), _command);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(callbackMutex);
					IMSendCustomCommandCallbacks.insert({seq, callback});
				}
			}

			//===================================================================================================
			void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) override {

				const char * task_id = task.taskID.c_str();

				std::vector<zego_mixer_input> input_list;
				{
					for (auto & input : task.inputList) {
						zego_mixer_input _input;
						_input.content_type = zego_mixer_input_content_type(input.contentType);
						_input.layout.left = input.layout.x;
						_input.layout.right = input.layout.x + input.layout.width;
						_input.layout.top = input.layout.y;
						_input.layout.bottom = input.layout.y + input.layout.height;
						memset(_input.stream_id, 0, sizeof(_input.stream_id));
						strncpy_s(_input.stream_id, input.streamID.c_str(), ZEGO_EXPRESS_MAX_STREAM_LEN);
						input_list.push_back(_input);
					}
				}
				mEngineBridge->setMixerInputList(task_id, input_list.data(), (unsigned int)(input_list.size()));


				std::vector<zego_mixer_output> output_list;
				{
					for (auto & output : task.outputList) {
						zego_mixer_output _output;
						memset(_output.target, 0, sizeof(_output.target));
						strncpy_s(_output.target, output.target.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
						output_list.push_back(_output);
					}
				}
				mEngineBridge->setMixerOutputList(task_id, output_list.data(), (unsigned int)(output_list.size()));

				zego_mixer_audio_config audio_config;
				{
					audio_config.bitrate = task.audioConfig.bitrate;
					audio_config.audio_encode_type = 0;
				}
				mEngineBridge->setMixerOutputAudioConfig(task_id, audio_config);


				zego_mixer_video_config video_config;
				{
					video_config.bitrate = task.videoConfig.bitrate;
					video_config.fps = task.videoConfig.fps;
					video_config.resolution_width = task.videoConfig.width;
					video_config.resolution_height = task.videoConfig.height;
				}
				mEngineBridge->setMixerOutputVideoConfig(task_id, video_config);

				if (task.watermark != nullptr) {
					zego_watermark _watermark;
					{
						_watermark.layout.left = task.watermark->layout.x;
						_watermark.layout.right = task.watermark->layout.x + task.watermark->layout.width;
						_watermark.layout.top = task.watermark->layout.y;
						_watermark.layout.bottom = task.watermark->layout.y + task.watermark->layout.height;
						memset(_watermark.image, 0, sizeof(_watermark.image));
						strncpy_s(_watermark.image, task.watermark->imageURL.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
					}
					mEngineBridge->setMixerOutputWatermark(task_id, &_watermark);
				}
				else {
					mEngineBridge->setMixerOutputWatermark(task_id, nullptr);
				}

				if (!task.backgroundImageURL.empty())
				{
					const char * _backgroundImageURL = task.backgroundImageURL.c_str();
					mEngineBridge->setMixerOutputBackgroundImage(task_id, _backgroundImageURL);
				}

				int seq = mEngineBridge->startMixerTask(task_id);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(callbackMutex);
					mixerStartCallbacks.insert({seq, callback});
				}
			}

			void stopMixerTask(std::string taskID) override {
				const char *task_id = taskID.c_str();
				mEngineBridge->stopMixerTask(task_id);
			}


		public:
			// DEBUG CALLBACK
			static void zego_on_debug_error(int error_code, const char* func_name, const char* _info, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				if (0 != error_code) {
					pthis->mEngineBridge->showDebugMessageBox(error_code, func_name, _info);
				}

				if (0 != error_code) {
					std::string funcName = func_name;
					std::string info = _info;

					std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
					for (auto handler : pthis->mEventHandlers)
					{
						ZEGO_SWITCH_THREAD_PRE
							handler->onDebugError(error_code, funcName, info);
						ZEGO_SWITCH_THREAD_ING
					}
				}
			}

			// ROOM CALLBACK
			static void zego_on_room_state_update(const char* room_id, zego_room_state state, zego_error error_code, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string roomID = room_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomStateUpdate(roomID, ZegoRoomState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_stream_update(const char* room_id, zego_update_type update_type, const struct zego_stream *stream_info_list, unsigned int stream_info_count, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string roomID = room_id;
				std::vector<ZegoStream> streamList;
				for (unsigned int i = 0; i < stream_info_count; i++) {
					ZegoStream stream = ZegoExpressEngineImp::ZegoConvertI2OStream(stream_info_list[i]);
					streamList.push_back(stream);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomStreamUpdate(roomID, ZegoUpdateType(update_type), streamList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_room_stream_extra_info_update(const char* room_id, const struct zego_stream* stream_info_list, unsigned int stream_info_count, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string roomID = room_id;
				std::vector<ZegoStream> streamList;
				for (unsigned int i = 0; i < stream_info_count; i++) {
					ZegoStream stream = ZegoExpressEngineImp::ZegoConvertI2OStream(stream_info_list[i]);
					streamList.push_back(stream);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomStreamExtraInfoUpdate(roomID, streamList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_user_update(const char* room_id, zego_update_type update_type, const struct zego_user *user_list, unsigned int user_count, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string roomID = room_id;
				std::vector<ZegoUser> userList;
				for (unsigned int i = 0; i < user_count; i++) {
					ZegoUser user = ZegoExpressEngineImp::ZegoConvertI2OUser(user_list[i]);
					userList.push_back(user);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomUserUpdate(roomID, ZegoUpdateType(update_type), userList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			// PUBLISHER CALLBACK
			static void zego_on_publisher_state_update(const char* stream_id, zego_publisher_state state, zego_error error_code, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherStateUpdate(streamID, ZegoPublisherState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_quality_update(const char* stream_id, zego_publish_stream_quality quality, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				ZegoPublishStreamQuality pushlishQuality = ZegoExpressEngineImp::ZegoConvertI2OPushlishQuality(quality);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherQualityUpdate(streamID, pushlishQuality);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_recv_audio_captured_first_frame(void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherRecvFirstFrameEvent(ZEGO_PUBLISHER_FIRST_FRAME_EVENT_AUDIO_CAPTURED);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_recv_video_captured_first_frame(void *user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherRecvFirstFrameEvent(ZEGO_PUBLISHER_FIRST_FRAME_EVENT_VIDEO_CAPTURED);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_video_size_changed(int width, int height, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherVideoSizeChanged(width, height);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_update_cdn_url_result(const char* stream_id, zego_error error_code, zego_seq seq, void* user_context)
			{
				ZEGO_UNUSED_VARIABLE(stream_id);
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->callbackMutex);
				if (pthis->CDNCallbacks.count(seq) <= 0) {
					return;
				}
				else {
					auto cb = pthis->CDNCallbacks[seq];
					pthis->CDNCallbacks.erase(seq);
					ZEGO_SWITCH_THREAD_PRE
						cb(error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_relay_cdn_state_update(const char* stream_id, const zego_stream_relay_cdn_info* state_info_list, unsigned int info_count, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;
				std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

				for (unsigned int i = 0; i < info_count; i++) {
					zego_stream_relay_cdn_info _info = state_info_list[i];
					ZegoStreamRelayCDNInfo info;
					info.url = _info.url;
					info.time = _info.state_time;
					info.state = ZegoStreamRelayCDNState(_info.cdn_state);
					info.reason = ZegoStreamRelayCDNReason(_info.update_reason);
					streamInfoList.push_back(info);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_update_stream_extra_info_result(zego_error error_code, zego_seq seq, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->callbackMutex);
				if (pthis->StreamExtraInfoCallbacks.count(seq) <= 0) {
					return;
				}
				else {
					auto cb = pthis->StreamExtraInfoCallbacks[seq];
					pthis->StreamExtraInfoCallbacks.erase(seq);
					ZEGO_SWITCH_THREAD_PRE
						cb(error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			// PLAYER CALLBACK
			static void zego_on_player_state_update(const char* stream_id, zego_player_state state, zego_error error_code, void *user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerStateUpdate(streamID, ZegoPlayerState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_quality_update(const char* stream_id, zego_play_stream_quality quality, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;
				ZegoPlayStreamQuality playQuality = ZegoExpressEngineImp::ZegoConvertI2OPlayQuality(quality);

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerQualityUpdate(streamID, playQuality);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_media_event(const char* stream_id, zego_player_media_event media_event, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerMediaEvent(streamID, ZegoPlayerMediaEvent(media_event));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_recv_audio_first_frame(const char* stream_id, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvFirstFrameEvent(streamID, ZEGO_PLAYER_FIRST_FRAME_EVENT_AUDIO_RCV);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_recv_video_first_frame(const char* stream_id, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvFirstFrameEvent(streamID, ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RCV);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_render_video_first_frame(const char* stream_id, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvFirstFrameEvent(streamID, ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDER);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_video_size_changed(const char* stream_id, int width, int height, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerVideoSizeChanged(streamID, width, height);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			// DEVICE CALLBACK
			static void zego_on_audio_device_state_changed(enum zego_update_type update_type, enum zego_audio_device_type device_type, struct zego_device_info _device, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				ZegoDeviceInfo device = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_device);

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onAudioDeviceStateChanged(ZegoUpdateType(update_type), ZegoAudioDeviceType(device_type), device);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_video_device_state_changed(enum zego_update_type update_type, struct zego_device_info _device, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				ZegoDeviceInfo device = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_device);

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onVideoDeviceStateChanged(ZegoUpdateType(update_type), device);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_device_error(zego_error error_code, const char* device_name, void* user_context)
			{
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string deviceName = device_name;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onDeviceError(error_code, deviceName);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_mic_state_update(const char* stream_id, zego_remote_device_state state, void *user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteMicStateUpdate(streamID, ZegoRemoteDeviceState(state));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_camera_state_update(const char* stream_id, zego_remote_device_state state, void *user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteCameraStateUpdate(streamID, ZegoRemoteDeviceState(state));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_captured_sound_level_update(const zego_sound_level_info* sound_level_info, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				double soundLevel = sound_level_info->sound_level;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onCapturedSoundLevelUpdate(soundLevel);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_sound_level_update(const zego_sound_level_info* sound_level_info_list, unsigned int info_count, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::map<std::string, double> soundLevels;
				for (unsigned int i = 0; i < info_count; i++) {
					zego_sound_level_info sound_level_info = sound_level_info_list[i];
					std::string streamID = sound_level_info.stream_id;
					double soundLevel = sound_level_info.sound_level;
					soundLevels.insert({ streamID, soundLevel });
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteSoundLevelUpdate(soundLevels);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_captured_audio_spectrum_update(const zego_audio_spectrum_info* audio_spectrum_info, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				ZegoAudioSpectrum audioSpectrum;
				for (unsigned int i = 0; i < audio_spectrum_info->spectrum_count; i++)
				{
					audioSpectrum.push_back(audio_spectrum_info->spectrum_list[i]);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onCapturedAudioSpectrumUpdate(audioSpectrum);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_audio_spectrum_update(const zego_audio_spectrum_info* audio_spectrum_info_list, unsigned int info_count, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::map<std::string, ZegoAudioSpectrum> audioSpectrums;
				for (unsigned int j = 0; j < info_count; j++) {
					zego_audio_spectrum_info frequency_spectrum_info = audio_spectrum_info_list[j];

					std::string streamID = frequency_spectrum_info.stream_id;
					ZegoAudioSpectrum frequencySpectrum;
					for (unsigned int i = 0; i < frequency_spectrum_info.spectrum_count; i++)
					{
						frequencySpectrum.push_back(frequency_spectrum_info.spectrum_list[i]);
					}
					audioSpectrums.insert({ streamID, frequencySpectrum });
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteAudioSpectrumUpdate(audioSpectrums);
					ZEGO_SWITCH_THREAD_ING
				}
			}


			// IM CALLBACK
			static void zego_on_im_send_broadcast_message_result(const char* room_id, zego_error error_code, zego_seq seq, void* user_context) {
				ZEGO_UNUSED_VARIABLE(room_id);
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->callbackMutex);
				if (pthis->IMSendBroadcastMessageCallbacks.count(seq) <= 0) {
					return;
				}
				else {
					auto cb = pthis->IMSendBroadcastMessageCallbacks[seq];
					pthis->IMSendBroadcastMessageCallbacks.erase(seq);
					ZEGO_SWITCH_THREAD_PRE
						cb(error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_im_recv_broadcast_message(const char* room_id, const struct zego_message_info* message_info_list, unsigned int message_count, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::string roomID = room_id;
				std::vector<ZegoMessageInfo> messageInfoList;
				for (unsigned int i = 0; i < message_count; i++) {
					zego_message_info message_info = message_info_list[i];
					ZegoMessageInfo messageInfo;
					messageInfo.sendTime = message_info.send_time;
					messageInfo.message = message_info.message;
					messageInfo.fromUser = ZegoExpressEngineImp::ZegoConvertI2OUser(message_info.from_user);
					messageInfoList.push_back(messageInfo);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onIMRecvBroadcastMessage(roomID, messageInfoList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_im_send_custom_command_result(const char* room_id, zego_error error_code, zego_seq seq, void* user_context) {
				ZEGO_UNUSED_VARIABLE(room_id);
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->callbackMutex);
				if (pthis->IMSendCustomCommandCallbacks.count(seq) <= 0) {
					return;
				}
				else {
					auto cb = pthis->IMSendCustomCommandCallbacks[seq];
					pthis->IMSendCustomCommandCallbacks.erase(seq);
					ZEGO_SWITCH_THREAD_PRE
						cb(error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_im_recv_custom_command(const char* room_id, struct zego_user from_user, const char* _content, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::string roomID = room_id;
				ZegoUser user = ZegoExpressEngineImp::ZegoConvertI2OUser(from_user);
				std::string content = _content;

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onIMRecvCustomCommand(roomID, user, content);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			// MIXER CALLBACK
			static void zego_on_mixer_start_result(const char* task_id, const struct zego_mixer_start_result _result, zego_seq seq, void* user_context) {
				ZEGO_UNUSED_VARIABLE(task_id);
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;

				std::lock_guard<std::mutex> lock(pthis->callbackMutex);
				if (pthis->mixerStartCallbacks.count(seq) <= 0) {
					return;
				}
				else {
					ZegoMixerStartResult result;
					result.errorCode = _result.error_code;
					auto cb = pthis->mixerStartCallbacks[seq];
					pthis->mixerStartCallbacks.erase(seq);
					ZEGO_SWITCH_THREAD_PRE
						cb(result);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_mixer_relay_cdn_state_update(const char* task_id, const struct zego_stream_relay_cdn_info* cdn_info_list, unsigned int info_count, void* user_context) {
				ZegoExpressEngineImp * pthis = (ZegoExpressEngineImp*)user_context;
				std::string taskID = task_id;
				std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

				for (unsigned int i = 0; i < info_count; i++) {
					zego_stream_relay_cdn_info _info = cdn_info_list[i];
					ZegoStreamRelayCDNInfo info;
					info.url = _info.url;
					info.time = _info.state_time;
					info.state = ZegoStreamRelayCDNState(_info.cdn_state);
					info.reason = ZegoStreamRelayCDNReason(_info.update_reason);
					streamInfoList.push_back(info);
				}

				std::lock_guard<std::mutex> lock(pthis->eventHandlerMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onMixerRelayCDNStateUpdate(taskID, streamInfoList);
					ZEGO_SWITCH_THREAD_ING
				}
			}


#ifdef WIN32
			static LRESULT CALLBACK ZegoCommuExchangeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				if (uMsg == WM_ZEGO_SWITCH_THREAD)
				{
					std::function<void(void)>* pFunc = (std::function<void(void)>*)wParam;
					(*pFunc)();
					delete pFunc;
				}

				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}

			void LoadThreadTools()
			{
				static std::wstring wstrWndClassName;
				wstrWndClassName = L"SignalWndClass" + std::to_wstring(ZegoExpressEngineBridge::GetCurTimeStampMs());
				ZegoCommWndClassName = wstrWndClassName.c_str();

				static std::wstring wstrWndName;
				wstrWndName = L"SignalWnd" + std::to_wstring(ZegoExpressEngineBridge::GetCurTimeStampMs());
				ZegoCommWndName = wstrWndName.c_str();

				WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
				wcex.hInstance = GetModuleHandle(0);
				wcex.lpszClassName = ZegoCommWndClassName;
				wcex.lpfnWndProc = &ZegoCommuExchangeWndProc;
				wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
				RegisterClassEx(&wcex);
				m_hCommuWnd = CreateWindowEx(WS_EX_TOOLWINDOW, wcex.lpszClassName, ZegoCommWndName, WS_POPUP, 0, 0, 100, 100,
					NULL, NULL, wcex.hInstance, NULL);
				::ShowWindow(m_hCommuWnd, SW_HIDE);
			}

			void UnloadThreadTools()
			{
				::DestroyWindow(m_hCommuWnd);
				::CloseWindow(m_hCommuWnd);
			}

		private:
			HWND m_hCommuWnd;
			LPCWSTR ZegoCommWndClassName;
			LPCWSTR  ZegoCommWndName;
#else

			void LoadThreadTools()
			{
			}

			void UnloadThreadTools()
			{

			}
#endif // WIN32

		private:
			std::unordered_map< zego_seq, ZegoPublisherSetStreamExtraInfoCallback> StreamExtraInfoCallbacks;
			std::unordered_map< zego_seq, ZegoPublisherUpdateCDNURLCallback> CDNCallbacks;
			std::unordered_map< zego_seq, ZegoIMSendBroadcastMessageCallback> IMSendBroadcastMessageCallbacks;
			std::unordered_map< zego_seq, ZegoIMSendCustomCommandCallback> IMSendCustomCommandCallbacks;
			std::unordered_map< zego_seq, ZegoMixerStartCallback> mixerStartCallbacks;
			std::vector<std::shared_ptr<IZegoEventHandler>> mEventHandlers;
			ZegoExpressEngineBridge *mEngineBridge = nullptr;
			std::mutex eventHandlerMutex;
			std::mutex callbackMutex;

		public:
			///===================================================================================================

			ZegoExpressEngineImp()
			{
				LoadThreadTools();
				mEngineBridge = new ZegoExpressEngineBridge();
			}

			~ZegoExpressEngineImp() override
			{
				UnloadThreadTools();
				delete mEngineBridge;
				mEngineBridge = nullptr;
			}

			static std::string getVersion() {
				std::string version = ZegoExpressEngineBridge::getVersion();
				return version;
			}

		private:
			static ZegoUser ZegoConvertI2OUser(const zego_user &_user)
			{
				ZegoUser user;
				user.userID = _user.userId;
				user.userName = _user.userName;
				return user;
			}

			static zego_user ZegoConvertO2IUser(const ZegoUser &user)
			{
				zego_user _user;
				memset(_user.userId, 0, sizeof(_user.userId));
				memset(_user.userName, 0, sizeof(_user.userName));
				strncpy_s(_user.userId, user.userID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
				strncpy_s(_user.userName, user.userName.c_str(), ZEGO_EXPRESS_MAX_USERNAME_LEN);
				return _user;
			}

			static zego_room_config ZegoConvertO2IRoomConfig(const ZegoRoomConfig &roomConfig)
			{
				zego_room_config _config;
				_config.max_member_count = roomConfig.maxMemberCount;
				_config.is_user_status_notify = roomConfig.isUserStatusNotify;
				return _config;
			}

			static ZegoStream ZegoConvertI2OStream(const zego_stream &_stream)
			{
				ZegoStream stream;
				stream.user.userID = _stream.user.userId;
				stream.user.userName = _stream.user.userName;
				stream.streamID = _stream.streamId;
				stream.extraInfo = _stream.extraInfo;
				return stream;
			}

			static ZegoPublishStreamQuality ZegoConvertI2OPushlishQuality(const zego_publish_stream_quality &_quality)
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

			static ZegoPlayStreamQuality ZegoConvertI2OPlayQuality(const zego_play_stream_quality &_quality)
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

			static ZegoDeviceInfo ZegoConvertI2ODeviceInfo(const zego_device_info &_deviceInfo)
			{
				ZegoDeviceInfo deviceInfo;
				deviceInfo.deviceID = _deviceInfo.device_id;
				deviceInfo.deviceName = _deviceInfo.device_name;
				return deviceInfo;
			}

			static zego_beautify_option ZegoConvertO2IBeautifyOption(const ZegoBeautifyOption &option)
			{
				zego_beautify_option _option;
				_option.polish_step = option.polishStep;
				_option.sharpen_factor = option.sharpenFactor;
				_option.whiten_factor = option.whitenFactor;
				return _option;
			}

			static zego_video_config ZegoConvertO2IVideoConfig(const ZegoVideoConfig &config)
			{
				zego_video_config _config;
				_config.bitrate = config.bitrate;
				_config.fps = config.fps;

				_config.capture_resolution_height = config.captureHeight;
				_config.capture_resolution_width = config.captureWidth;

				_config.encode_resolution_height = config.encodeHeight;
				_config.encode_resolution_width = config.encodeWidth;

				return _config;
			}

		private:
			int initEngine(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario) {
				if (mEngineBridge->createdBefore) {
					return ZEGO_MORE_THEN_ONE_INSTANCE;
				}

				mEngineBridge->registerDebugErrorCallback((void*)&ZegoExpressEngineImp::zego_on_debug_error, this);

				mEngineBridge->registerRoomStreamUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_stream_update, this);
				mEngineBridge->registerRoomStreamExtraInfoUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_room_stream_extra_info_update, this);
				mEngineBridge->registerRoomStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_room_state_update, this);
				mEngineBridge->registerRoomUserStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_user_update, this);

				mEngineBridge->registerPublisherStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_state_update, this);
				mEngineBridge->registerPublisherQualityUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_quality_update, this);
				mEngineBridge->registerPublisherRecvAudioCapturedFirstFrameCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_recv_audio_captured_first_frame, this);
				mEngineBridge->registerPublisherRecvVideoCapturedFirstFrameCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_recv_video_captured_first_frame, this);
				mEngineBridge->registerPublisherVideoSizeChangedCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_video_size_changed, this);
				mEngineBridge->registerPublisherUpdatePublishCDNUrlCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_update_cdn_url_result, this);
				mEngineBridge->registerPublisherRelayCDNStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_relay_cdn_state_update, this);
				mEngineBridge->registerPublisherUpdateStreamExtraInfoResultCallback((void*)&ZegoExpressEngineImp::zego_on_publisher_update_stream_extra_info_result, this);

				mEngineBridge->registerPlayerStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_player_state_update, this);
				mEngineBridge->registerPlayerQualityUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_player_quality_update, this);
				mEngineBridge->registerPlayerMediaEventCallback((void*)&ZegoExpressEngineImp::zego_on_player_media_event, this);
				mEngineBridge->registerPlayerRecvAudioFirstFrameCallback((void*)&ZegoExpressEngineImp::zego_on_player_recv_audio_first_frame, this);
				mEngineBridge->registerPlayerRecvVideoFirstFrameCallback((void*)&ZegoExpressEngineImp::zego_on_player_recv_video_first_frame, this);
				mEngineBridge->registerPlayerRenderVideoFirstFrameCallback((void*)&ZegoExpressEngineImp::zego_on_player_render_video_first_frame, this);
				mEngineBridge->registerPlayerVideoSizeChangedCallback((void*)&ZegoExpressEngineImp::zego_on_player_video_size_changed, this);

				mEngineBridge->registerAudioDeviceStateChangedCallback((void*)&ZegoExpressEngineImp::zego_on_audio_device_state_changed, this);
				mEngineBridge->registerVideoDeviceStateChangedCallback((void*)&ZegoExpressEngineImp::zego_on_video_device_state_changed, this);
				mEngineBridge->registerDeviceErrorCallback((void*)&ZegoExpressEngineImp::zego_on_device_error, this);
				mEngineBridge->registerRemoteCameraStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_remote_camera_state_update, this);
				mEngineBridge->registerRemoteMicStateUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_remote_mic_state_update, this);
				mEngineBridge->registerCapturedSoundLevelUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_captured_sound_level_update, this);
				mEngineBridge->registerRemoteSoundLevelUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_remote_sound_level_update, this);
				mEngineBridge->registerCapturedAudioSpectrumUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_captured_audio_spectrum_update, this);
				mEngineBridge->registerRemoteAudioSpectrumUpdateCallback((void*)&ZegoExpressEngineImp::zego_on_remote_audio_spectrum_update, this);

				mEngineBridge->registerIMSendBroadcastMessageResultCallback((void*)&ZegoExpressEngineImp::zego_on_im_send_broadcast_message_result, this);
				mEngineBridge->registerIMRecvBroadcasMessageCallback((void*)&ZegoExpressEngineImp::zego_on_im_recv_broadcast_message, this);
				mEngineBridge->registerIMSendCustomCommandResultCallback((void*)&ZegoExpressEngineImp::zego_on_im_send_custom_command_result, this);
				mEngineBridge->registerIMRecvCustomCommandCallback((void*)&ZegoExpressEngineImp::zego_on_im_recv_custom_command, this);

				mEngineBridge->registerMixerStartResultCallback((void *)&ZegoExpressEngineImp::zego_on_mixer_start_result, this);
				mEngineBridge->registerMixerRelayCDNStateUpdateCallback((void *)&ZegoExpressEngineImp::zego_on_mixer_relay_cdn_state_update, this);

				int result = mEngineBridge->initSDK(appID, appSign.c_str(), isTestEnvironment, zego_scenario(scenario));
				if (0 == result) {
					mEngineBridge->createdBefore = true;
				}
				return result;
			}

			void uninitEngine() {
				mEngineBridge->uninitSDK();
				mEngineBridge->createdBefore = false;
			}

			friend class ZegoExpressEngineInternal;
		};

		class ZegoExpressEngineInternal {
		public:

			static IZegoExpressEngine* createEngine(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler)
			{
				auto engine = new ZegoExpressEngineImp();

				int initResult = engine->initEngine(appID, appSign, isTestEnvironment, scenario);
				bool initSucceed = 0 == initResult;
				assert(initSucceed);
				if (initSucceed) {
					engine->addEventHandler(eventHandler);
				}
				else {
					delete engine;
					engine = nullptr;
				}

				return engine;
			}

			static void destroyEngine(IZegoExpressEngine* engine)
			{
				if (engine != nullptr) {
					ZegoExpressEngineImp *imp = (ZegoExpressEngineImp*)engine;
					imp->uninitEngine();
					delete imp;
					imp = nullptr;
				}
			}

			static std::string getVersion() {
				return ZegoExpressEngineImp::getVersion();
			}
		};


	} //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSENGINEIMP_H__
