#ifndef __IZEGOEXPRESSMEDIAPLAYER_H__
#define __IZEGOEXPRESSMEDIAPLAYER_H__

#include "ZegoExpressDefine.h"

namespace ZEGO {
	namespace EXPRESS {
		class IZegoExpressMediaPlayer;

		/// 播放器状态
		enum ZegoMediaPlayerState {
			ZegoMediaPlayerStateNoPlay = 0,				// 不在播放
			ZegoMediaPlayerStateWillPlay = 1,			// 准备播放
			ZegoMediaPlayerStatePlaying = 2,			// 播放中
			ZegoMediaPlayerStatePausing = 3,			// 暂停播放
			ZegoMediaPlayerStatePlayEnded = 4,			// 播放结束
		};

		/// 播放器网络事件
		enum ZegoMediaPlayerNetworkEvent {
			ZegoMediaPlayerNetworkEventBufferBegin = 0, // 网络资源播放不畅，开始尝试缓存数据
			ZegoMediaPlayerNetworkEventBufferEnded = 1, // 网络资源可以顺畅播放
		};

		/// 加载资源完成回调
		using ZegoExpressMediaPlayerLoadResourceCallback = std::function<void(int errorCode)>;

		/// 指定播放进度回调
		using ZegoExpressMediaPlayerSeekToTimeCallback = std::function<void(int errorCode)>;

		/// 播放器事件回调
		class IZegoExpressMediaPlayerEventHandler {
		protected:
			virtual ~IZegoExpressMediaPlayerEventHandler() {}
		
		public:
			/// 播放器播放状态变更及事件回调
			virtual void onMediaPlayerStateUpdate(IZegoExpressMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerState /*state*/, int /*errorCode*/) {

			}

			/// 播放器网络状态事件回调
			virtual void onMediaPlayerNetWorkEvent(IZegoExpressMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerNetworkEvent /*networkEvent*/) {

			}

			/// 播放器播放进度回调
			virtual void onMediaPlayerPlayingProcess(IZegoExpressMediaPlayer* /*mediaPlayer*/, unsigned long long /*process*/) {

			}
		};

		/// 播放器抛视频数据的回调
		class IZegoExpressMediaPlayerVideoHandler {
		protected:
			virtual ~IZegoExpressMediaPlayerVideoHandler() {}

		public:
			virtual void onVideoFrame(IZegoExpressMediaPlayer* /*mediaPlayer*/, const char** /* buffer */, ZegoVideoFrameParam /*param*/) {
				
			}
		};

		/// 播放器抛音频数据的回调
		class IZegoExpressMediaPlayerAudioHandler {
		protected:
			virtual ~IZegoExpressMediaPlayerAudioHandler() {}

		public:
			virtual void onAudioFrame(IZegoExpressMediaPlayer* /*mediaPlayer*/, const unsigned char* /* buffer */, ZegoAudioFrameParam /*param*/) {
				
			}
		};


		/// 媒体播放器
		class IZegoExpressMediaPlayer {

		protected:
			virtual ~IZegoExpressMediaPlayer() {};

		public:
			/// 设置事件回调handler
			virtual void setEventHandler(std::shared_ptr<IZegoExpressMediaPlayerEventHandler> handler) = 0;

			/// 设置视频回调handler
			virtual void setVideoHandler(ZegoVideoFrameFormat format, std::shared_ptr<IZegoExpressMediaPlayerVideoHandler> handler) = 0;

			/// 设置音频回调handler
			virtual void setAudioHandler(std::shared_ptr<IZegoExpressMediaPlayerAudioHandler> handler) = 0;

			/// 加载资源
			virtual void loadResource(std::string sourcePath , ZegoExpressMediaPlayerLoadResourceCallback completion) = 0;

			/// 开始播放（播放前需要先加载资源）
			virtual void start() = 0;

			/// 停止播放
			virtual void stop() = 0;

			/// 暂停播放
			virtual void pause() = 0;

			/// 恢复播放
			virtual void resume() = 0;

			/// 设置指定播放进度
			virtual void seekToTime(unsigned long long millisecond, ZegoExpressMediaPlayerSeekToTimeCallback completion) = 0;

			/// 设置是否重复播放
			virtual void enableRepeat(bool enable) = 0;

			/// 设置是否将声音混入正在推的流中
			virtual void enableAux(bool enable) = 0;

			/// 是否静默本地播放（若开启了混音入流则推的流中仍然有声音）
			virtual void muteLocal(bool mute) = 0;

			/// 设置播放器视图
			virtual void setPlayerCanvas(ZegoCanvas *canvas) = 0;

			/// 设置音量，0~100
			virtual void setVolume(int volume) = 0;

			/// 设置播放进度回调间隔
			virtual void setProcessInterval(unsigned long long millisecond) = 0;

			/// 获取整个文件的播放时长，毫秒
			virtual unsigned long long getTotalDuration() = 0;

			/// 获取当前播放进度，毫秒
			virtual unsigned long long getCurrentProcess() = 0;

			/// 获取播放音量
			virtual int getVolume() = 0;

			/// 获取当前播放状态
			virtual ZegoMediaPlayerState getCurrentState() = 0;
		};
	}
}

#endif /* __IZEGOEXPRESSMEDIAPLAYER_H__ */
