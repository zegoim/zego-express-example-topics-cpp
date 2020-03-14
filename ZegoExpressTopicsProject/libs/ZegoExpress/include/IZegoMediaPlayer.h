
#ifndef __IZEGOEXPRESSMEDIAPLAYER_H__
#define __IZEGOEXPRESSMEDIAPLAYER_H__

#include "ZegoExpressDefine.h"

namespace ZEGO {
    namespace EXPRESS {

        class IZegoMediaPlayer;


        class IZegoMediaPlayerEventHandler
        {
        protected:

            virtual ~IZegoMediaPlayerEventHandler() {}

        public:

            /**
             * 播放器播放状态回调
             *
             * @param mediaPlayer 回调的播放器实例
             * @param state 播放器状态
             * @param errorCode 错误码，详情请参考常用错误码文档
             */
            virtual void onMediaPlayerStateUpdate(IZegoMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerState /*state*/, int /*errorCode*/) {

            }

            /**
             * 播放器网络状态事件回调
             *
             * @param mediaPlayer 回调的播放器实例
             * @param networkEvent 网络状态事件
             */
            virtual void onMediaPlayerNetworkEvent(IZegoMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerNetworkEvent /*networkEvent*/) {

            }

            /**
             * 播放器播放进度回调
             *
             * @param mediaPlayer 回调的播放器实例
             * @param millisecond 进度，单位为毫秒
             */
            virtual void onMediaPlayerPlayingProgress(IZegoMediaPlayer* /*mediaPlayer*/, unsigned long long /*millisecond*/) {

            }

        };


        class IZegoMediaPlayerVideoHandler
        {
        protected:

            virtual ~IZegoMediaPlayerVideoHandler() {}

        public:

            /**
             * 播放器抛视频数据的回调
             *
             * @param mediaPlayer 回调的播放器实例
             * @param data 视频帧的裸数据（例：RGBA 只需考虑 data[0]，I420 需考虑 data[0,1,2]）
             * @param dataLength 数据的长度（例：RGBA 只需考虑 dataLength[0]，I420 需考虑 dataLength[0,1,2]）
             * @param param 视频帧参数
             */
            virtual void onVideoFrame(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/) {

            }

        };


        class IZegoMediaPlayerAudioHandler
        {
        protected:

            virtual ~IZegoMediaPlayerAudioHandler() {}

        public:

            /**
             * 播放器抛音频数据的回调
             *
             * @param mediaPlayer 回调的播放器实例
             * @param data 音频频帧的裸数据
             * @param dataLength 数据的长度
             * @param param 音频帧参数
             */
            virtual void onAudioFrame(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/) {

            }

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
             * 播放前需要先加载资源
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
             * @param enable 重复播放标记
             */
            virtual void enableRepeat(bool enable) = 0;

            /**
             * 是否将播放器的声音混入正在推的流中
             *
             * @param enable 是否混音标记
             */
            virtual void enableAux(bool enable) = 0;

            /**
             * 是否静默本地播放
             *
             * 若开启了混音入流则推的流中仍然有声音
             * @param mute 本地静音标记
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
             * @param volume 范围为 0 ~ 100
             */
            virtual void setVolume(int volume) = 0;

            /**
             * 设置播放进度回调间隔
             *
             * 可通过此接口控制 [onMediaPlayerPlayingProgress] 的回调频率
             * @param millisecond 播放进度回调间隔时间，单位为毫秒
             */
            virtual void setProgressInterval(unsigned long long millisecond) = 0;

            /**
             * 获取当前音量
             *
             * 范围为 0 ~ 100
             */
            virtual int getVolume() = 0;

            /**
             * 获取媒体资源的总进度
             *
             * @return 单位为毫秒
             */
            virtual unsigned long long getTotalDuration() = 0;

            /**
             * 获取当前播放进度
             *
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

#endif /* __IZEGOEXPRESSMEDIAPLAYER_H__ */
