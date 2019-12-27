
#ifndef __ZEGOEXPRESSENGINEIMP_H__
#define __ZEGOEXPRESSENGINEIMP_H__


#include "ZegoExpressDefine.h"
#include "IZegoEventHandler.h"
#include "IZegoExpressEngine.h"
#include "IZegoMediaPlayer.h"
#include "ZegoInternalBridge.h"

namespace ZEGO {
	namespace EXPRESS {
		template<typename T>
		class ZegoSingleton {
		public:
			template<typename... Args>
			static T* CreateInstance(Args&&... args) {
				if (m_pInstance == nullptr) {
					m_pInstance = new T(std::forward<Args>(args)...);
				}
				return  m_pInstance;
			}

			static T* GetInstance() {
				return m_pInstance;
			}

			static void DestroyInstance() {
				if (m_pInstance) {
					delete m_pInstance;
					m_pInstance = nullptr;
				}
			}
		private:
			ZegoSingleton(void) = default;
			virtual ~ZegoSingleton(void) = default;

		private:
			static T*m_pInstance;
		};
		template <class T> T* ZegoSingleton<T>::m_pInstance = nullptr;

		class ZegoMainThreadTool {
		public:
			ZegoMainThreadTool() {
				LoadThreadResource();
			}
			~ZegoMainThreadTool() {
				UnloadThreadResource();
			}
#ifdef WIN32
			static LRESULT CALLBACK ZegoCommuExchangeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				if (uMsg == (WM_APP + 10086))
				{
					std::function<void(void)>* pFunc = (std::function<void(void)>*)wParam;
					(*pFunc)();
					delete pFunc;
				}

				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}

			void LoadThreadResource()
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
				wcex.hbrBackground = HBRUSH(GetStockObject(NULL_BRUSH));
				RegisterClassEx(&wcex);
				m_hCommuWnd = CreateWindowEx(WS_EX_TOOLWINDOW, wcex.lpszClassName, ZegoCommWndName, WS_POPUP, 0, 0, 100, 100,
					NULL, NULL, wcex.hInstance, NULL);
				::ShowWindow(m_hCommuWnd, SW_HIDE);
			}

			void UnloadThreadResource()
			{
				::DestroyWindow(m_hCommuWnd);
				::CloseWindow(m_hCommuWnd);
			}

			HWND m_hCommuWnd;
			LPCWSTR ZegoCommWndClassName;
			LPCWSTR  ZegoCommWndName;
#else

			void LoadThreadResource()
			{
			}

			void UnloadThreadResource()
			{

			}
#endif // WIN32
		};

		class ZegoExpressMediaPlayerImp : public IZegoExpressMediaPlayer {
		public:
			ZegoExpressMediaPlayerImp(int index)
				: instanceIndex(index) {
				mInterBridge = ZegoSingleton<ZegoExpressEngineBridge>::GetInstance();
			}

			int getInstanceIndex() {
				return instanceIndex;
			}

			void setEventHandler(std::shared_ptr<IZegoExpressMediaPlayerEventHandler> handler) override
			{
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				mpEventHandler = handler;
			}

			void setVideoHandler(ZegoVideoFrameFormat format, std::shared_ptr<IZegoExpressMediaPlayerVideoHandler> handler) override
			{
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (handler)
				{
					mInterBridge->mpEnableVideoData(true, zego_video_frame_format(format), zego_mediaplayer_instance_index(instanceIndex));
				}
				else {
					mInterBridge->mpEnableVideoData(false, zego_video_frame_format(format), zego_mediaplayer_instance_index(instanceIndex));
				}
				mpVideoHandler = handler;
			}

			void setAudioHandler(std::shared_ptr<IZegoExpressMediaPlayerAudioHandler> handler) override
			{
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (handler)
				{
					mInterBridge->mpEnableAudioData(true, zego_mediaplayer_instance_index(instanceIndex));
				}
				else {
					mInterBridge->mpEnableAudioData(false, zego_mediaplayer_instance_index(instanceIndex));
				}
				mpAudioHandler = handler;
			}

			void loadResource(std::string sourcePath, ZegoExpressMediaPlayerLoadResourceCallback callback) override
			{
				int seq = mInterBridge->mpPreload(sourcePath.c_str(), zego_mediaplayer_instance_index(instanceIndex));
				ZEGO_UNUSED_VARIABLE(seq);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(mediaEventMutex);
					mpLoadResourceCallbacks = callback;
				}
			}

			void start() override
			{
				mInterBridge->mpStart(zego_mediaplayer_instance_index(instanceIndex));
			}

			void stop() override
			{
				mInterBridge->mpStop(zego_mediaplayer_instance_index(instanceIndex));
			}

			void pause() override
			{
				mInterBridge->mpPause(zego_mediaplayer_instance_index(instanceIndex));
			}

			void resume() override
			{
				mInterBridge->mpResume(zego_mediaplayer_instance_index(instanceIndex));
			}

			void seekToTime(unsigned long long millisecond, ZegoExpressMediaPlayerSeekToTimeCallback callback) override
			{
				int seq = mInterBridge->mpSeekTo(millisecond, zego_mediaplayer_instance_index(instanceIndex));
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(mediaEventMutex);
					mpSeekToTimeCallbacks.insert({ seq, callback });
				}
			}

			void enableRepeat(bool enable) override
			{
				mInterBridge->mpEnableRepeat(enable, zego_mediaplayer_instance_index(instanceIndex));
			}

			void enableAux(bool enable) override
			{
				mInterBridge->mpEnableAUX(enable, zego_mediaplayer_instance_index(instanceIndex));
			}

			void muteLocal(bool mute) override
			{
				mInterBridge->mpMuteLocalAudio(mute, zego_mediaplayer_instance_index(instanceIndex));
			}

			void setPlayerCanvas(ZegoCanvas *canvas) override
			{
				if (canvas == nullptr) {
					mInterBridge->mpSetPlayerCanvas(nullptr, zego_mediaplayer_instance_index(instanceIndex));
				}
				else
				{
					zego_canvas _canvas;
					_canvas.view = canvas->view;
					_canvas.view_mode = zego_view_mode(canvas->viewMode);
					_canvas.background_color = canvas->backgroundColor;
					mInterBridge->mpSetPlayerCanvas(&_canvas, zego_mediaplayer_instance_index(instanceIndex));
				}
			}

			void setVolume(int volume) override
			{
				mInterBridge->mpSetVolume(volume, zego_mediaplayer_instance_index(instanceIndex));
			}

			void setProcessInterval(unsigned long long millisecond) override
			{
				mInterBridge->mpSetProcessInterval(millisecond, zego_mediaplayer_instance_index(instanceIndex));
			}

			unsigned long long getTotalDuration() override
			{
				return mInterBridge->mpGetTotalDuration(zego_mediaplayer_instance_index(instanceIndex));
			}

			unsigned long long getCurrentProcess() override
			{
				return mInterBridge->mpGetCurrentDuration(zego_mediaplayer_instance_index(instanceIndex));
			}

			int getVolume() override
			{
				return mInterBridge->mpGetVolume(zego_mediaplayer_instance_index(instanceIndex));
			}

			ZegoMediaPlayerState getCurrentState() override
			{
				zego_mediaplayer_state state = mInterBridge->mpGetCurrentState(zego_mediaplayer_instance_index(instanceIndex));
				return ZegoMediaPlayerState(state);
			}

			void zego_on_mediaplayer_state_update(zego_mediaplayer_state state, zego_error error_code) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpEventHandler) {
					ZEGO_SWITCH_THREAD_PRE
						mpEventHandler->onMediaPlayerStateUpdate(this, ZegoMediaPlayerState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			void zego_on_mediaplayer_network_event(zego_mediaplayer_network_event event) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpEventHandler) {
					ZEGO_SWITCH_THREAD_PRE
						mpEventHandler->onMediaPlayerNetWorkEvent(this, ZegoMediaPlayerNetworkEvent(event));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			void zego_on_mediaplayer_playing_process(unsigned long long millisecond) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpEventHandler) {
					ZEGO_SWITCH_THREAD_PRE
						mpEventHandler->onMediaPlayerPlayingProcess(this, millisecond);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			void zego_on_mediaplayer_seek_to_time_result(zego_seq seq, zego_error error_code) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpSeekToTimeCallbacks.count(seq) > 0) {
					auto cb = mpSeekToTimeCallbacks[seq];
					mpSeekToTimeCallbacks.erase(seq);
					ZEGO_SWITCH_THREAD_PRE
						cb(error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			void zego_on_mediaplayer_load_file_result(zego_error error_code) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpLoadResourceCallbacks) {
					ZEGO_SWITCH_THREAD_PRE
						mpLoadResourceCallbacks(error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			void zego_on_mediaplayer_video_data(const char** buffer, const struct zego_video_frame_param _param) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpVideoHandler) {
					ZegoVideoFrameParam param;
					param.format = ZegoVideoFrameFormat(_param.format);
					param.dataLength = _param.data_length;
					param.strides = _param.strides;
					param.height = _param.height;
					param.width = _param.width;
					mpVideoHandler->onVideoFrame(this, buffer, param);
				}
			}

			void zego_on_mediaplayer_audio_data(const unsigned char* buffer, const struct zego_audio_frame_param _param) {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				if (mpAudioHandler) {
					ZegoAudioFrameParam param;
					param.bufferLength = _param.buffer_length;
					param.channels = _param.channels;
					param.sampleRate = _param.samples_rate;
					mpAudioHandler->onAudioFrame(this, buffer, param);
				}
			}

		private:
			int instanceIndex = -1;
			ZegoExpressEngineBridge *mInterBridge = nullptr;
			friend class ZegoExpressEngineImp;

			std::shared_ptr<IZegoExpressMediaPlayerEventHandler> mpEventHandler;
			std::shared_ptr<IZegoExpressMediaPlayerVideoHandler> mpVideoHandler;
			std::shared_ptr<IZegoExpressMediaPlayerAudioHandler> mpAudioHandler;
			ZegoExpressMediaPlayerLoadResourceCallback mpLoadResourceCallbacks;
			std::unordered_map< zego_seq, ZegoExpressMediaPlayerSeekToTimeCallback> mpSeekToTimeCallbacks;
			std::mutex mediaEventMutex;
		};

		class ZegoExpressEngineImp : public IZegoExpressEngine
		{
		public:
			using ZegoExpressEngineImpPtr = ZegoExpressEngineImp*;
			void addEventHandler(std::shared_ptr<IZegoEventHandler> handler) override
			{
				if (handler == nullptr) {
					return;
				}

				std::lock_guard<std::mutex> lock(engineEventMutex);
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

				std::lock_guard<std::mutex> lock(engineEventMutex);
				auto iter = std::find(mEventHandlers.begin(), mEventHandlers.end(), handler);
				if (iter != mEventHandlers.end())
				{
					mEventHandlers.erase(iter);
				}
			}

			void uploadLog() override
			{
				mInterBridge->uploadLog();
			}

			void setDebugVerbose(bool enable, ZegoLanguage language) override
			{
				mInterBridge->setDebugVerbose(enable, zego_language(language));
			}

			///===================================================================================================
			void loginRoom(std::string roomID, ZegoUser user, ZegoRoomConfig* config) override
			{
				zego_user _user = ZegoExpressEngineImp::ZegoConvertO2IUser(user);
				const char * _roomId = roomID.c_str();

				if (config != nullptr) {
					zego_room_config _config = ZegoExpressEngineImp::ZegoConvertO2IRoomConfig(*config);
					mInterBridge->loginRoom(_user, _roomId, &_config);
				}
				else {
					mInterBridge->loginRoom(_user, _roomId, nullptr);
				}
			}

			void loginRoom(std::string roomID, ZegoUser user, ZegoRoomConfig* config, std::string token) override
			{
				zego_user _user = ZegoExpressEngineImp::ZegoConvertO2IUser(user);
				const char * _roomId = roomID.c_str();
				const char * _token = token.c_str();

				if (config != nullptr) {
					zego_room_config _config = ZegoExpressEngineImp::ZegoConvertO2IRoomConfig(*config);
					mInterBridge->loginRoom(_user, _roomId, &_config, _token);
				}
				else {
					mInterBridge->loginRoom(_user, _roomId, nullptr, _token);
				}
			}

			void logoutRoom(std::string roomID) override
			{
				const char * _roomId = roomID.c_str();
				mInterBridge->logoutRoom(_roomId);
			}

			///===================================================================================================
			void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode) override
			{
				mInterBridge->setVideoMirrorMode(zego_video_mirror_mode(mirrorMode));
			}

			void startPreview(ZegoCanvas *canvas) override
			{
				if (canvas == nullptr) {
					mInterBridge->startPreview(nullptr);
				}
				else
				{
					zego_canvas _canvas;
					_canvas.view = canvas->view;
					_canvas.view_mode = zego_view_mode(canvas->viewMode);
					_canvas.background_color = canvas->backgroundColor;
					mInterBridge->startPreview(&_canvas);
				}
			}

			void stopPreview() override
			{
				mInterBridge->stopPreview();
			}

			void setAudioConfig(ZegoAudioConfig audioConfig) override
			{
				zego_audio_config config;
				config.bitrate = audioConfig.bitrate;
				config.channels = audioConfig.channels;
				config.audio_codec_id = audioConfig.codecID;
				mInterBridge->setAudioConfig(config);
			}

			void setVideoConfig(ZegoVideoConfig videoConfig) override
			{
				zego_video_config _config = ZegoExpressEngineImp::ZegoConvertO2IVideoConfig(videoConfig);
				mInterBridge->setVideoConfig(_config);
			}

			void startPublishing(std::string streamID) override
			{
				const char * stream_id = streamID.c_str();
				mInterBridge->startPublishing(stream_id);
			}

			void stopPublishing() override
			{
				mInterBridge->stopPublishing();
			}

			void setStreamExtraInfo(std::string extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback) override
			{
				const char * _extraInfo = extraInfo.c_str();
				int seq = mInterBridge->setStreamExtraInfo(_extraInfo);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(engineEventMutex);
					StreamExtraInfoCallbacks.insert({ seq, callback });
				}
			}

			void mutePublishStreamAudio(bool mute) override
			{
				mInterBridge->mutePublishStreamAudio(mute);
			}

			void mutePublishStreamVideo(bool mute) override
			{
				mInterBridge->mutePublishStreamVideo(mute);
			}

			void setCaptureVolume(int volume) override
			{
				mInterBridge->setCaptureVolume(volume);
			}

			void addPublishCDNURL(std::string streamID, std::string targetURL, ZegoPublisherUpdateCDNURLCallback callback) override
			{
				const char * stream_id = streamID.c_str();
				const char * target_url = targetURL.c_str();

				int seq = mInterBridge->addPublishCNDURL(stream_id, target_url);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(engineEventMutex);
					CDNCallbacks.insert({ seq, callback });
				}
			}

			void removePublishCDNURL(std::string streamID, std::string targetURL, ZegoPublisherUpdateCDNURLCallback callback) override
			{
				const char * stream_id = streamID.c_str();
				const char * target_url = targetURL.c_str();

				int seq = mInterBridge->removePublishCNDURL(stream_id, target_url);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(engineEventMutex);
					CDNCallbacks.insert({ seq, callback });
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
					mInterBridge->setPublishWatermark(&_watermark, isPreviewVisible);
				}
				else {
					mInterBridge->setPublishWatermark(nullptr, isPreviewVisible);
				}
			}

			void enableHardwareEncoder(bool enable) override
			{
				mInterBridge->enableHardwareEncoder(enable);
			}

			void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) override
			{
				zego_capture_pipeline_scale_mode _mode = zego_capture_pipeline_scale_mode(mode);
				mInterBridge->setCapturePipelineScaleMode(_mode);
			}

			void sendSEI(const unsigned char* buffer, unsigned int buffer_length) override
			{
				mInterBridge->sendSEI(buffer, buffer_length);
			}

			///===================================================================================================
			void startPlayingStream(std::string streamID, ZegoCanvas *canvas) override
			{
				const char * stream_id = streamID.c_str();
				if (canvas == nullptr) {
					mInterBridge->startPlayingStream(stream_id, nullptr);
				}
				else {
					zego_canvas _canvas;
					_canvas.view = canvas->view;
					_canvas.view_mode = zego_view_mode(canvas->viewMode);
					_canvas.background_color = canvas->backgroundColor;
					mInterBridge->startPlayingStream(stream_id, &_canvas);
				}
			}

			void stopPlayingStream(std::string streamID) override
			{
				const char * stream_id = streamID.c_str();
				mInterBridge->stopPlayingStream(stream_id);
			}

			void setPlayVolume(std::string streamID, int volume) override
			{
				const char * stream_id = streamID.c_str();
				mInterBridge->setPlayVolume(stream_id, volume);
			}

			void mutePlayStreamAudio(std::string streamID, bool mute) override
			{
				const char * stream_id = streamID.c_str();
				mInterBridge->mutePlayStreamAudio(stream_id, mute);
			}

			void mutePlayStreamVideo(std::string streamID, bool mute) override
			{
				const char * stream_id = streamID.c_str();
				mInterBridge->mutePlayStreamVideo(stream_id, mute);
			}

			void enableHarewareDecoder(bool enable) override
			{
				mInterBridge->enableHardwareDecoder(enable);
			}

			void enableCheckPoc(bool enable) override
			{
				mInterBridge->enableCheckPoc(enable);
			}

			//===================================================================================================

			void muteMicrophone(bool enable) override
			{
				mInterBridge->muteMicrophone(enable);
			}

			void muteAudioOutput(bool mute) override
			{
				mInterBridge->muteAudioOutput(mute);
			}

			void enableCamera(bool enable) override
			{
				mInterBridge->enableCamera(enable);
			}

			void enableAudioCaptureDevice(bool enable) override
			{
				mInterBridge->enableAudioCaptureDevice(enable);
			}

			void startSoundLevelMonitor() override
			{
				mInterBridge->startSoundLevelMonitor();
			}

			void stopSoundLevelMonitor() override
			{
				mInterBridge->stopSoundLevelMonitor();
			}
			void startAudioSpectrumMonitor() override
			{
				mInterBridge->startAudioSpectrumMonitor();
			}

			void stopAudioSpectrumMonitor() override
			{
				mInterBridge->stopAudioSpectrumMonitor();
			}

			void useAudioDevice(ZegoAudioDeviceType deviceType, std::string deviceID) override
			{
				const char * device_id = deviceID.c_str();
				mInterBridge->useAudioDevice(zego_audio_device_type(deviceType), device_id);
			}

			std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) override
			{
				int count = 0;
				zego_device_info* _deviceInfoList = mInterBridge->getAudioDeviceList(zego_audio_device_type(deviceType), &count);

				std::vector<ZegoDeviceInfo> result;
				for (int i = 0; i < count; i++) {
					ZegoDeviceInfo deviceInfo = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_deviceInfoList[i]);
					result.push_back(deviceInfo);
				}

				mInterBridge->freeAudioDeviceList(_deviceInfoList);
				return result;
			}

			void useVideoDevice(std::string deviceID) override
			{
				const char * device_id = deviceID.c_str();
				mInterBridge->useVideoDevice(device_id);
			}

			std::vector<ZegoDeviceInfo> getVideoDeviceList() override
			{
				int count = 0;
				zego_device_info* _deviceInfoList = mInterBridge->getVideoDeviceList(&count);

				std::vector<ZegoDeviceInfo> result;
				for (int i = 0; i < count; i++) {
					ZegoDeviceInfo deviceInfo = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_deviceInfoList[i]);
					result.push_back(deviceInfo);
				}

				mInterBridge->freeVideoDeviceList(_deviceInfoList);
				return result;
			}

			//===================================================================================================

			void enableAEC(bool enable) override
			{
				mInterBridge->enableAEC(enable);
			}

			void setAECMode(ZegoAECMode mode) override
			{
				mInterBridge->setAECMode(zego_aec_mode(mode));
			}

			void enableAGC(bool enable) override
			{
				mInterBridge->enableAGC(enable);
			}

			void enableANS(bool enable) override
			{
				mInterBridge->enableANS(enable);
			}

			//===================================================================================================
			void sendBroadcastMassage(std::string roomID, std::string message, ZegoIMSendBroadcastMessageCallback callback) override
			{
				const char * _roomID = roomID.c_str();
				const char * _message = message.c_str();

				int seq = mInterBridge->sendBroadcastMessage(_roomID, _message);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(engineEventMutex);
					IMSendBroadcastMessageCallbacks.insert({ seq, callback });
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

				int seq = mInterBridge->sendCustomCommand(_roomID, users, u_int(_userList.size()), _command);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(engineEventMutex);
					IMSendCustomCommandCallbacks.insert({ seq, callback });
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

				mInterBridge->setMixerInputList(task_id, input_list.data(), u_int(input_list.size()));


				std::vector<zego_mixer_output> output_list;
				{
					for (auto & output : task.outputList) {
						zego_mixer_output _output;
						memset(_output.target, 0, sizeof(_output.target));
						strncpy_s(_output.target, output.target.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
						output_list.push_back(_output);
					}
				}
				mInterBridge->setMixerOutputList(task_id, output_list.data(), u_int(output_list.size()));

				zego_mixer_audio_config audio_config;
				{
					audio_config.bitrate = task.audioConfig.bitrate;
					audio_config.audio_encode_type = 0;
				}
				mInterBridge->setMixerOutputAudioConfig(task_id, audio_config);


				zego_mixer_video_config video_config;
				{
					video_config.bitrate = task.videoConfig.bitrate;
					video_config.fps = task.videoConfig.fps;
					video_config.resolution_width = task.videoConfig.width;
					video_config.resolution_height = task.videoConfig.height;
				}
				mInterBridge->setMixerOutputVideoConfig(task_id, video_config);

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
					mInterBridge->setMixerOutputWatermark(task_id, &_watermark);
				}
				else {
					mInterBridge->setMixerOutputWatermark(task_id, nullptr);
				}

				if (!task.backgroundImageURL.empty())
				{
					const char * _backgroundImageURL = task.backgroundImageURL.c_str();
					mInterBridge->setMixerOutputBackgroundImage(task_id, _backgroundImageURL);
				}

				int seq = mInterBridge->startMixerTask(task_id);
				if (callback != nullptr) {
					std::lock_guard<std::mutex> lock(engineEventMutex);
					mixerStartCallbacks.insert({ seq, callback });
				}
			}

			void stopMixerTask(std::string taskID) override {
				const char *task_id = taskID.c_str();
				mInterBridge->stopMixerTask(task_id);
			}

			//===================================================================================================
			IZegoExpressMediaPlayer* createMediaPlayer() override {
				auto index = mInterBridge->mpCreate();
				if (index == zego_mediaplayer_instance_index_null) {
					return nullptr;
				}
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				auto mediaPlayer = new ZegoExpressMediaPlayerImp(index);
				mMediaPlayers.insert({ index, mediaPlayer });
				return mediaPlayer;
			}

			void destroyMediaPlayer(IZegoExpressMediaPlayer *& IMediaPlayer) override {
				std::lock_guard<std::mutex> lock(mediaEventMutex);
				for (auto iter = mMediaPlayers.begin(); iter != mMediaPlayers.end(); iter++) {
					if (iter->second == IMediaPlayer) {
						mInterBridge->mpRelease(zego_mediaplayer_instance_index(iter->second->getInstanceIndex()));
						delete iter->second;
						mMediaPlayers.erase(iter);
						IMediaPlayer = nullptr;
						break;
					}
				}
			}

		public:
			// DEBUG CALLBACK
			static void zego_on_debug_error(int error_code, const char* func_name, const char* _info, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				if (0 != error_code) {
					pthis->mInterBridge->showDebugMessageBox(error_code, func_name, _info);
				}

				if (0 != error_code) {
					std::string funcName = func_name;
					std::string info = _info;

					std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string roomID = room_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomStateUpdate(roomID, ZegoRoomState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_stream_update(const char* room_id, zego_update_type update_type, const struct zego_stream *stream_info_list, unsigned int stream_info_count, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string roomID = room_id;
				std::vector<ZegoStream> streamList;
				for (unsigned int i = 0; i < stream_info_count; i++) {
					ZegoStream stream = ZegoExpressEngineImp::ZegoConvertI2OStream(stream_info_list[i]);
					streamList.push_back(stream);
				}

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomStreamUpdate(roomID, ZegoUpdateType(update_type), streamList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_room_stream_extra_info_update(const char* room_id, const struct zego_stream* stream_info_list, unsigned int stream_info_count, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string roomID = room_id;
				std::vector<ZegoStream> streamList;
				for (unsigned int i = 0; i < stream_info_count; i++) {
					ZegoStream stream = ZegoExpressEngineImp::ZegoConvertI2OStream(stream_info_list[i]);
					streamList.push_back(stream);
				}

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRoomStreamExtraInfoUpdate(roomID, streamList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_user_update(const char* room_id, zego_update_type update_type, const struct zego_user *user_list, unsigned int user_count, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string roomID = room_id;
				std::vector<ZegoUser> userList;
				for (unsigned int i = 0; i < user_count; i++) {
					ZegoUser user = ZegoExpressEngineImp::ZegoConvertI2OUser(user_list[i]);
					userList.push_back(user);
				}

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherStateUpdate(streamID, ZegoPublisherState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_quality_update(const char* stream_id, zego_publish_stream_quality quality, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				ZegoPublishStreamQuality pushlishQuality = ZegoExpressEngineImp::ZegoConvertI2OPushlishQuality(quality);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherQualityUpdate(streamID, pushlishQuality);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_recv_audio_captured_first_frame(void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherRecvFirstFrameEvent(ZEGO_PUBLISHER_FIRST_FRAME_EVENT_AUDIO_CAPTURED);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_recv_video_captured_first_frame(void *user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherRecvFirstFrameEvent(ZEGO_PUBLISHER_FIRST_FRAME_EVENT_VIDEO_CAPTURED);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_video_size_changed(int width, int height, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
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

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_publisher_update_stream_extra_info_result(zego_error error_code, zego_seq seq, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerStateUpdate(streamID, ZegoPlayerState(state), error_code);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_quality_update(const char* stream_id, zego_play_stream_quality quality, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;
				ZegoPlayStreamQuality playQuality = ZegoExpressEngineImp::ZegoConvertI2OPlayQuality(quality);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerQualityUpdate(streamID, playQuality);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_media_event(const char* stream_id, zego_player_media_event media_event, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerMediaEvent(streamID, ZegoPlayerMediaEvent(media_event));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_recv_audio_first_frame(const char* stream_id, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvFirstFrameEvent(streamID, ZEGO_PLAYER_FIRST_FRAME_EVENT_AUDIO_RCV);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_recv_video_first_frame(const char* stream_id, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvFirstFrameEvent(streamID, ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RCV);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_render_video_first_frame(const char* stream_id, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvFirstFrameEvent(streamID, ZEGO_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDER);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_video_size_changed(const char* stream_id, int width, int height, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerVideoSizeChanged(streamID, width, height);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_player_recv_sei(const char* stream_id, const unsigned char* data, unsigned int data_length, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;
				std::vector<unsigned char> seiData;
				seiData.reserve(data_length);
				for (unsigned int i = 0; i < data_length; i++) {
					seiData.push_back(data[i]);
				}

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onPlayerRecvSEI(streamID, seiData.data(), u_int(seiData.size()));
					ZEGO_SWITCH_THREAD_ING
				}
			}


			// DEVICE CALLBACK
			static void zego_on_audio_device_state_changed(enum zego_update_type update_type, enum zego_audio_device_type device_type, struct zego_device_info _device, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				ZegoDeviceInfo device = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_device);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onAudioDeviceStateChanged(ZegoUpdateType(update_type), ZegoAudioDeviceType(device_type), device);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_video_device_state_changed(enum zego_update_type update_type, struct zego_device_info _device, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				ZegoDeviceInfo device = ZegoExpressEngineImp::ZegoConvertI2ODeviceInfo(_device);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onVideoDeviceStateChanged(ZegoUpdateType(update_type), device);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_device_error(zego_error error_code, const char* device_name, void* user_context)
			{
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string deviceName = device_name;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onDeviceError(error_code, deviceName);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_mic_state_update(const char* stream_id, zego_remote_device_state state, void *user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteMicStateUpdate(streamID, ZegoRemoteDeviceState(state));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_camera_state_update(const char* stream_id, zego_remote_device_state state, void *user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::string streamID = stream_id;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteCameraStateUpdate(streamID, ZegoRemoteDeviceState(state));
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_captured_sound_level_update(const zego_sound_level_info* sound_level_info, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				double soundLevel = sound_level_info->sound_level;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onCapturedSoundLevelUpdate(soundLevel);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_sound_level_update(const zego_sound_level_info* sound_level_info_list, unsigned int info_count, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::map<std::string, double> soundLevels;
				for (unsigned int i = 0; i < info_count; i++) {
					zego_sound_level_info sound_level_info = sound_level_info_list[i];
					std::string streamID = sound_level_info.stream_id;
					double soundLevel = sound_level_info.sound_level;
					soundLevels.insert({ streamID, soundLevel });
				}

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onRemoteSoundLevelUpdate(soundLevels);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_captured_audio_spectrum_update(const zego_audio_spectrum_info* audio_spectrum_info, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				ZegoAudioSpectrum audioSpectrum;
				for (unsigned int i = 0; i < audio_spectrum_info->spectrum_count; i++)
				{
					audioSpectrum.push_back(audio_spectrum_info->spectrum_list[i]);
				}

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onCapturedAudioSpectrumUpdate(audioSpectrum);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_remote_audio_spectrum_update(const zego_audio_spectrum_info* audio_spectrum_info_list, unsigned int info_count, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

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

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

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

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onIMRecvBroadcastMessage(roomID, messageInfoList);
					ZEGO_SWITCH_THREAD_ING
				}
			}

			static void zego_on_im_send_custom_command_result(const char* room_id, zego_error error_code, zego_seq seq, void* user_context) {
				ZEGO_UNUSED_VARIABLE(room_id);
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::string roomID = room_id;
				ZegoUser user = ZegoExpressEngineImp::ZegoConvertI2OUser(from_user);
				std::string content = _content;

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
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
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
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

				std::lock_guard<std::mutex> lock(pthis->engineEventMutex);
				for (auto handler : pthis->mEventHandlers)
				{
					ZEGO_SWITCH_THREAD_PRE
						handler->onMixerRelayCDNStateUpdate(taskID, streamInfoList);
					ZEGO_SWITCH_THREAD_ING
				}
			}


			// MediaPlayer Callback
			static void zego_on_mediaplayer_state_update(zego_mediaplayer_state state, zego_error error_code, zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_state_update(state, error_code);
				}
			}

			static  void zego_on_mediaplayer_network_event(zego_mediaplayer_network_event event, zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_network_event(event);
				}
			}

			static void zego_on_mediaplayer_playing_process(unsigned long long millisecond, zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_playing_process(millisecond);
				}
			}

			static void zego_on_mediaplayer_seek_to_time_result(zego_seq seq, zego_error error_code, enum zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_seek_to_time_result(seq, error_code);
				}
			}

			static void zego_on_mediaplayer_load_file_result(zego_error error_code, zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_load_file_result(error_code);
				}
			}

			static void zego_on_mediaplayer_audio_data(const unsigned char* buffer, const struct zego_audio_frame_param param, enum zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_audio_data(buffer, param);
				}
			}

			static void zego_on_mediaplayer_video_data(const char** buffer, const struct zego_video_frame_param param, enum zego_mediaplayer_instance_index instance_index, void* user_context) {
				ZegoExpressEngineImpPtr pthis = ZegoExpressEngineImpPtr(user_context);
				std::lock_guard<std::mutex> lock(pthis->mediaEventMutex);
				if (pthis->mMediaPlayers.count(instance_index) > 0) {
					auto mediaPlayer = pthis->mMediaPlayers[instance_index];
					mediaPlayer->zego_on_mediaplayer_video_data(buffer, param);
				}
			}

		private:
			ZegoExpressEngineBridge *mInterBridge = nullptr;

			std::unordered_map< zego_seq, ZegoPublisherSetStreamExtraInfoCallback> StreamExtraInfoCallbacks;
			std::unordered_map< zego_seq, ZegoPublisherUpdateCDNURLCallback> CDNCallbacks;
			std::unordered_map< zego_seq, ZegoIMSendBroadcastMessageCallback> IMSendBroadcastMessageCallbacks;
			std::unordered_map< zego_seq, ZegoIMSendCustomCommandCallback> IMSendCustomCommandCallbacks;
			std::unordered_map< zego_seq, ZegoMixerStartCallback> mixerStartCallbacks;
			std::vector<std::shared_ptr<IZegoEventHandler>> mEventHandlers;
			std::mutex engineEventMutex;

			std::unordered_map<int, ZegoExpressMediaPlayerImp*> mMediaPlayers;
			std::mutex mediaEventMutex;

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
		public:
			///===================================================================================================
			ZegoExpressEngineImp()
			{
				ZegoSingleton<ZegoMainThreadTool>::CreateInstance();
				mInterBridge = ZegoSingleton<ZegoExpressEngineBridge>::CreateInstance();

				mInterBridge->registerDebugErrorCallback(&ZegoExpressEngineImp::zego_on_debug_error, this);

				mInterBridge->registerRoomStreamUpdateCallback(&ZegoExpressEngineImp::zego_on_stream_update, this);
				mInterBridge->registerRoomStreamExtraInfoUpdateCallback(&ZegoExpressEngineImp::zego_on_room_stream_extra_info_update, this);
				mInterBridge->registerRoomStateUpdateCallback(&ZegoExpressEngineImp::zego_on_room_state_update, this);
				mInterBridge->registerRoomUserStateUpdateCallback(&ZegoExpressEngineImp::zego_on_user_update, this);

				mInterBridge->registerPublisherStateUpdateCallback(&ZegoExpressEngineImp::zego_on_publisher_state_update, this);
				mInterBridge->registerPublisherQualityUpdateCallback(&ZegoExpressEngineImp::zego_on_publisher_quality_update, this);
				mInterBridge->registerPublisherRecvAudioCapturedFirstFrameCallback(&ZegoExpressEngineImp::zego_on_publisher_recv_audio_captured_first_frame, this);
				mInterBridge->registerPublisherRecvVideoCapturedFirstFrameCallback(&ZegoExpressEngineImp::zego_on_publisher_recv_video_captured_first_frame, this);
				mInterBridge->registerPublisherVideoSizeChangedCallback(&ZegoExpressEngineImp::zego_on_publisher_video_size_changed, this);
				mInterBridge->registerPublisherUpdatePublishCDNUrlCallback(&ZegoExpressEngineImp::zego_on_publisher_update_cdn_url_result, this);
				mInterBridge->registerPublisherRelayCDNStateUpdateCallback(&ZegoExpressEngineImp::zego_on_publisher_relay_cdn_state_update, this);
				mInterBridge->registerPublisherUpdateStreamExtraInfoResultCallback(&ZegoExpressEngineImp::zego_on_publisher_update_stream_extra_info_result, this);

				mInterBridge->registerPlayerStateUpdateCallback(&ZegoExpressEngineImp::zego_on_player_state_update, this);
				mInterBridge->registerPlayerQualityUpdateCallback(&ZegoExpressEngineImp::zego_on_player_quality_update, this);
				mInterBridge->registerPlayerMediaEventCallback(&ZegoExpressEngineImp::zego_on_player_media_event, this);
				mInterBridge->registerPlayerRecvAudioFirstFrameCallback(&ZegoExpressEngineImp::zego_on_player_recv_audio_first_frame, this);
				mInterBridge->registerPlayerRecvVideoFirstFrameCallback(&ZegoExpressEngineImp::zego_on_player_recv_video_first_frame, this);
				mInterBridge->registerPlayerRenderVideoFirstFrameCallback(&ZegoExpressEngineImp::zego_on_player_render_video_first_frame, this);
				mInterBridge->registerPlayerVideoSizeChangedCallback(&ZegoExpressEngineImp::zego_on_player_video_size_changed, this);
				mInterBridge->registerPlayerRecvSEICallback(&ZegoExpressEngineImp::zego_on_player_recv_sei, this);

				mInterBridge->registerAudioDeviceStateChangedCallback(&ZegoExpressEngineImp::zego_on_audio_device_state_changed, this);
				mInterBridge->registerVideoDeviceStateChangedCallback(&ZegoExpressEngineImp::zego_on_video_device_state_changed, this);
				mInterBridge->registerDeviceErrorCallback(&ZegoExpressEngineImp::zego_on_device_error, this);
				mInterBridge->registerRemoteCameraStateUpdateCallback(&ZegoExpressEngineImp::zego_on_remote_camera_state_update, this);
				mInterBridge->registerRemoteMicStateUpdateCallback(&ZegoExpressEngineImp::zego_on_remote_mic_state_update, this);
				mInterBridge->registerCapturedSoundLevelUpdateCallback(&ZegoExpressEngineImp::zego_on_captured_sound_level_update, this);
				mInterBridge->registerRemoteSoundLevelUpdateCallback(&ZegoExpressEngineImp::zego_on_remote_sound_level_update, this);
				mInterBridge->registerCapturedAudioSpectrumUpdateCallback(&ZegoExpressEngineImp::zego_on_captured_audio_spectrum_update, this);
				mInterBridge->registerRemoteAudioSpectrumUpdateCallback(&ZegoExpressEngineImp::zego_on_remote_audio_spectrum_update, this);

				mInterBridge->registerIMSendBroadcastMessageResultCallback(&ZegoExpressEngineImp::zego_on_im_send_broadcast_message_result, this);
				mInterBridge->registerIMRecvBroadcasMessageCallback(&ZegoExpressEngineImp::zego_on_im_recv_broadcast_message, this);
				mInterBridge->registerIMSendCustomCommandResultCallback(&ZegoExpressEngineImp::zego_on_im_send_custom_command_result, this);
				mInterBridge->registerIMRecvCustomCommandCallback(&ZegoExpressEngineImp::zego_on_im_recv_custom_command, this);

				mInterBridge->registerMixerStartResultCallback(&ZegoExpressEngineImp::zego_on_mixer_start_result, this);
				mInterBridge->registerMixerRelayCDNStateUpdateCallback(&ZegoExpressEngineImp::zego_on_mixer_relay_cdn_state_update, this);

				mInterBridge->registerMediaPlayerStateUpdateCallback(&ZegoExpressEngineImp::zego_on_mediaplayer_state_update, this);
				mInterBridge->registerMediaPlayerNetworkEventCallback(&ZegoExpressEngineImp::zego_on_mediaplayer_network_event, this);
				mInterBridge->registerMediaPlayerPlayingProcessCallback(&ZegoExpressEngineImp::zego_on_mediaplayer_playing_process, this);
				mInterBridge->registerMediaPlayerSeekToTimeResult(&ZegoExpressEngineImp::zego_on_mediaplayer_seek_to_time_result, this);
				mInterBridge->registerMediaPlayerLoadFileResult(&ZegoExpressEngineImp::zego_on_mediaplayer_load_file_result, this);
				mInterBridge->registerMediaPlayerVideoDataCallback(&ZegoExpressEngineImp::zego_on_mediaplayer_video_data, this);
				mInterBridge->registerMediaPlayerAudioDataCallback(&ZegoExpressEngineImp::zego_on_mediaplayer_audio_data, this);
			}

			~ZegoExpressEngineImp() override
			{
				ZegoSingleton<ZegoMainThreadTool>::DestroyInstance();
				ZegoSingleton<ZegoExpressEngineBridge>::DestroyInstance();
			}

			static std::string getVersion() {
				std::string version = ZegoExpressEngineBridge::getVersion();
				return version;
			}

		private:
			int initEngine(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler) {
				int result = mInterBridge->initSDK(appID, appSign.c_str(), isTestEnvironment, zego_scenario(scenario));
				if (0 == result) {
					this->addEventHandler(eventHandler);
				}
				return result;
			}

			void uninitEngine() {
				mInterBridge->uninitSDK();

			}

			friend class ZegoExpressEngineInternal;
		};

		class ZegoExpressEngineInternal {
		public:
			static IZegoExpressEngine* createEngine(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler)
			{
				auto engine = ZegoSingleton<ZegoExpressEngineImp>::GetInstance();
				if (engine != nullptr) {
					return engine;
				}

				engine = ZegoSingleton<ZegoExpressEngineImp>::CreateInstance();
				int initResult = engine->initEngine(appID, appSign, isTestEnvironment, scenario, eventHandler);
				if (0 == initResult) {
					return engine;
				}
				else {
					if (isTestEnvironment) {
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
					ZegoSingleton<ZegoExpressEngineImp>::DestroyInstance();
					return nullptr;
				}
			}

			static void destroyEngine(IZegoExpressEngine *&iEngine)
			{
				auto engine = ZegoSingleton<ZegoExpressEngineImp>::GetInstance();
				if (engine != nullptr && engine == iEngine) {
					iEngine = nullptr;
					engine->uninitEngine();
					ZegoSingleton<ZegoExpressEngineImp>::DestroyInstance();
				}
			}

			static std::string getVersion() {
				return ZegoExpressEngineImp::getVersion();
			}
		};

	} //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSENGINEIMP_H__
