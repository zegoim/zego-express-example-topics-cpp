#ifndef __IZEGOEXPRESSCUSTOMIO_H__
#define __IZEGOEXPRESSCUSTOMIO_H__

#include "ZegoExpressDefine.h"

namespace ZEGO {
    namespace EXPRESS {


        class IZegoCustomVideoCaptureHandler
        {
        protected:

            virtual ~IZegoCustomVideoCaptureHandler() {}

        public:

            /**
             * SDK 通知将要开始采集视频帧，收到该回调后向 SDK 发送的视频帧数据才有效
             *
             * @param channel 推流通道
             */
            virtual void onStart(ZegoPublishChannel channel) = 0;

            /**
             * SDK 通知将要停止采集视频帧
             *
             * @param channel 推流通道
             */
            virtual void onStop(ZegoPublishChannel channel) = 0;

        };


        class IZegoCustomVideoRenderHandler
        {
        protected:

            virtual ~IZegoCustomVideoRenderHandler() {}

        public:

            /**
             * 本地预览视频帧裸数据回调
             *
             * @param data 视频帧的裸数据（例：RGBA 只需考虑 data[0]，I420 需考虑 data[0,1,2]）
             * @param dataLength 数据的长度（例：RGBA 只需考虑 dataLength[0]，I420 需考虑 dataLength[0,1,2]）
             * @param param 视频帧参数
             * @param flipMode 视频帧翻转模式
             * @param channel 推流通道
             */
            virtual void onCapturedVideoFrameRawData(unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/, ZegoVideoFlipMode /*flipMode*/, ZegoPublishChannel /*channel*/) {

            }

            /**
             * 远端拉流视频帧裸数据回调，通过 streamID 区分不同的流
             *
             * @param data 视频帧的裸数据（例：RGBA 只需考虑 data[0]，I420 需考虑 data[0,1,2]）
             * @param dataLength 数据的长度（例：RGBA 只需考虑 dataLength[0]，I420 需考虑 dataLength[0,1,2]）
             * @param param 视频帧参数
             * @param streamID 拉流的流 ID
             */
            virtual void onRemoteVideoFrameRawData(unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/, const std::string& /*streamID*/) {

            }

        };

    }
}

#endif // __IZEGOEXPRESSCUSTOMIO_H__
